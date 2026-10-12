"""Offline tests: exact Enforce scalar expressions + source contracts + fault model.
These do not emulate the DayZ VM and never constitute a script-compile PASS.
"""
import ast, copy, hashlib, json, math, pathlib, random, re, tempfile, unittest
from types import SimpleNamespace
ROOT=pathlib.Path(__file__).resolve().parents[1]
MODEL='\n'.join(p.read_text() for p in sorted((ROOT/'mods/deutschz_world_memoriez/scripts/3_game').glob('*.c')))
SERVICE=(ROOT/'mods/deutschz_world_memoriez/scripts/4_world/dzwm_service.c').read_text()
MISSION=(ROOT/'mods/deutschz_world_memoriez/scripts/5_mission/dzwm_mission.c').read_text()
SCHED=(ROOT/'mods/deutschz_eventschedulerz/scripts/3_game/dzes_runtime.c').read_text()
ADAPTER=(ROOT/'mods/deutschz_eventschedulerz/scripts/4_world/dzes_adapters.c').read_text()
import sys
sys.path.insert(0,str(ROOT/'tools'))
from prepare_worldmemory import defaults

def rules_from_source():
    """Only the return expressions of pure scalar helpers; reject other syntax."""
    env={'Math':SimpleNamespace(Min=min,Max=max,Pow=pow,Floor=math.floor)}
    functions=re.findall(r'static (?:int|float) (\w+)\(([^)]*)\)\s*\{\s*return ([^;]+);\s*\}',MODEL.split('class DZWM_Settings')[0])
    assert len(functions)==6
    for name,params,expr in functions:
        args=[p.strip().split()[-1] for p in params.split(',')]
        tree=ast.parse(expr,mode='eval')
        assert all(isinstance(n,(ast.Expression,ast.Call,ast.Attribute,ast.Name,ast.Load,ast.BinOp,ast.Add,ast.Sub,ast.Mult,ast.Div,ast.Constant,ast.USub,ast.UnaryOp)) for n in ast.walk(tree))
        code=compile(tree,'Enforce:'+name,'eval')
        def run(*values,_args=args,_code=code):
            return eval(_code,{'__builtins__':{}},dict(env,**dict(zip(_args,values))))
        env[name]=run
    return SimpleNamespace(**{k:v for k,v in env.items() if k!='Math'})
R=rules_from_source()

def balanced(text):
    text=re.sub(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"','',text,flags=re.S)
    stack=[]
    for c in text:
        if c in '({[':stack.append(c)
        if c in ')}]':
            if not stack or '({['[')}]'.index(c)]!=stack.pop():return False
    return not stack

class ScalarTests(unittest.TestCase):
    def test_region_edges(self):
        for coord,want in [(-1000,-1),(-0.001,-1),(0,0),(999.999,0),(1000,1),(15359.99,15)]:
            self.assertEqual(R.Axis(coord,0,1000),want)
    def test_region_origin(self):
        self.assertEqual(R.Axis(-500,-500,500),0)
        self.assertEqual(R.Axis(0,-500,500),1)
    def test_half_life(self):
        self.assertAlmostEqual(R.Decay(80,0,1800,1800),40)
        self.assertAlmostEqual(R.Decay(0,50,1800,1800),25)
    def test_clock_backwards(self):
        self.assertEqual(R.Decay(80,0,-30,1800),80)
    def test_half_life_minimum(self):
        self.assertEqual(R.Decay(80,0,60,0),40)
    def test_caps(self):
        self.assertEqual(R.Add(99,20,12),100)
        self.assertEqual(R.Add(0,20,12),12)
        self.assertEqual(R.Add(10,-8,12),10)
        self.assertEqual(R.Add(10,8,-1),10)
    def test_recovery(self):
        self.assertEqual(R.Recovery(0,0,0),100)
        self.assertEqual(R.Recovery(40,80,20),20)
    def test_severity(self):
        self.assertEqual(R.Severity(20,60,40),60)
    def test_decay_partition(self):
        self.assertAlmostEqual(R.Decay(R.Decay(80,0,900,1800),0,900,1800),R.Decay(80,0,1800,1800))
    def test_fuzz_bounds(self):
        rng=random.Random(12)
        for _ in range(10000):
            value=rng.uniform(-1000,1000)
            result=R.Decay(value,50,rng.uniform(-10,1e6),rng.uniform(0,1e5))
            self.assertTrue(0<=result<=100)

class ConfigTests(unittest.TestCase):
    def test_every_numeric_has_clamp(self):
        cfg=defaults()
        validate=MODEL.split('void Validate()')[1].split('class DZWM_Region')[0]
        for key,value in cfg.items():
            if key=='schemaVersion' or isinstance(value,bool):continue
            self.assertRegex(validate,rf'\b{key} = DZWM_Rules.Clamp\({key},')
    def test_deployment_defaults_match_source(self):
        actual=json.loads((ROOT/'deployment/profiles/DeutschZ-System/deutschz_world_memoriez/config.json').read_text())
        self.assertEqual(actual,defaults())
    def test_invalid_config_preserved(self):
        self.assertIn('loaded.schemaVersion != 1',SERVICE)
        branch=SERVICE.split('CONFIG_INVALID')[1].split('config = loaded')[0]
        self.assertIn('return;',branch)
        self.assertNotIn('SaveFile',branch)
    def test_conservative_limits(self):
        c=defaults()
        self.assertLessEqual(c['maxReactionsPerTick'],1)
        self.assertGreaterEqual(c['saveSeconds'],60)
        self.assertLessEqual(c['maxRegions'],256)
        self.assertLessEqual(c['reactionLifetimeSeconds'],10)

class PersistenceFaultModelTests(unittest.TestCase):
    """Failure injection for the documented alternating-slot protocol, plus contracts."""
    def test_slot_selection_after_torn_write(self):
        with tempfile.TemporaryDirectory() as d:
            a,b=pathlib.Path(d)/'a.json',pathlib.Path(d)/'b.json'
            a.write_text('{"schemaVersion":1,"revision":1}')
            b.write_text('{"schemaVersion":1,"revision":2}')
            a.write_text('{"schemaVersion":1,"revision":')
            valid=[]
            for p in (a,b):
                try:valid.append(json.loads(p.read_text()))
                except json.JSONDecodeError:pass
            self.assertEqual(max(valid,key=lambda x:x['revision'])['revision'],2)
    def test_roundtrip_regional_data_no_player_ids(self):
        data={'schemaVersion':1,'regions':[{'id':'4:10','noise':78,'supply':55}],'seen':[{'key':'RAVEN_run:6','utc':1790000000}]}
        self.assertEqual(json.loads(json.dumps(data)),data)
        self.assertNotIn('GetPlainId',SERVICE)
        self.assertNotIn('GetIdentity',SERVICE)
    def test_unknown_schema_and_grid_fail_closed(self):
        for contract in ('candidate.schemaVersion != 1','candidate.regionSize != config.regionSize','candidate.originX != config.originX','m_ReadOnly = FileExist','second.revision > data.revision','readback.revision != data.revision'):
            self.assertIn(contract,SERVICE)
    def test_both_slots_not_overwritten(self):
        self.assertIn('if (!data || m_ReadOnly) return;',SERVICE)
        self.assertIn('data.revision % 2 == 0',SERVICE)
        self.assertNotIn('DeleteFile',SERVICE)
    def test_restart_offline_decay(self):
        self.assertAlmostEqual(R.Decay(80,0,3600,1800),20)
        self.assertIn('Math.Max(last, quietEnd)',SERVICE)
    def test_dedupe_model_load_and_saturation(self):
        seen={f'event_{i}:5' for i in range(512)}
        restored=set(json.loads(json.dumps(sorted(seen))))
        self.assertIn('event_4:5',restored)
        self.assertEqual(len(restored),512)
        self.assertIn('Seen(key) || data.seen.Count() >= config.maxDedupe',SERVICE)
        self.assertNotIn('data.seen.Remove(0)',SERVICE)

class IntegrationTests(unittest.TestCase):
    def test_no_debug_class_name_as_field(self):
        self.assertNotRegex(MODEL, r"\bbool\s+debug\b")

    def test_syntax_structure(self):
        for path in (ROOT/'mods').rglob('*.c'):
            self.assertTrue(balanced(path.read_text()),str(path))
    def test_optional_bus_only_after_save(self):
        self.assertIn('static ref ScriptInvoker WorldMemoryTransitions',SCHED)
        self.assertIn('if (transitionSaved) WorldMemoryTransitions.Invoke',SCHED)
        self.assertNotIn('DZWM_Service',SCHED)
    def test_event_ids_and_real_positions(self):
        for provider in ('KOTH','COURIER','RAVEN','CONVOY'):self.assertIn('"'+provider+'"',SERVICE)
        for position in ('m_Location.GetPosition()','GetCenter()','params.Waypoints[0]','m_Settings.CasePosition','m_Carrier.GetPosition()'):self.assertIn(position,ADAPTER)
        self.assertIn('if (eventId == "" || position == vector.Zero) return;',SERVICE)
        self.assertIn('m_RewardCommitted && m_Carrier',ADAPTER)
    def test_no_major_event_or_rewards(self):
        for forbidden in ('TryAcquire','RequestLease','FinishCleanup','CommitReward','CreateObject','CreateInInventory','Expansion_SetReputation','DZPoints_','DZToxicZ','GetInventory','GetObjectsAtPosition'):
            self.assertNotIn(forbidden,SERVICE)
    def test_lifecycle_no_duplicate_handlers(self):
        self.assertIn('if (s_Instance ||',SERVICE)
        for handler in ('OnEvent','OnExplosion'):
            self.assertEqual(SERVICE.count('.Insert('+handler+')'),1)
            self.assertEqual(SERVICE.count('.Remove('+handler+')'),2)
        self.assertEqual(SERVICE.count('.CallLater(Tick,'),1)
        self.assertEqual(SERVICE.count('.Remove(Tick)'),2)
        self.assertIn('DZWM_Service.Stop();',MISSION)
    def test_no_pvp_doublecount(self):
        self.assertNotIn('EEHitBy',SERVICE)
        self.assertIn('if (m_DZWM_Killed || !killer || killer == this) return;',SERVICE)
    def test_worldreaction_limits(self):
        for contract in ('reactions >= config.maxReactionsPerTick','config.reactionCooldownSeconds','config.maxReactionChecksPerTick','if (!nearby','AddNoiseTarget','supplyAlarmReduction'):
            self.assertIn(contract,SERVICE)
    def test_load_bound_model(self):
        c=defaults();accepted=0;value=0;used=0
        for i in range(100000):
            if accepted>=c['maxSignalsPerTick']:continue
            accepted+=1
            allowed=min(c['shotWeight'],max(0,c['perRegionTickCap']-used))
            value=R.Add(value,allowed,100);used+=allowed
        self.assertEqual(accepted,128)
        self.assertEqual(value,12)
        self.assertEqual(used,12)
        for cap in ('maxSignalsPerTick','maxPlayersPerTick','maxRegions','maxDedupe','maxReactionChecksPerTick'):self.assertIn('config.'+cap,SERVICE)
    def test_ammo_override_only_loose_20_stacks(self):
        source=(ROOT/'mods/deutschz_ammostackz/config.cpp').read_text()
        names=re.findall(r'class (Ammo_\w+): Ammunition_Base \{count=80;\};',source)
        self.assertEqual(len(names),13)
        self.assertEqual(len(set(names)),13)
        self.assertNotIn('class Mag_',source)

if __name__=='__main__':unittest.main(verbosity=2)
