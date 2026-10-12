"""Issue15: exact idle predicate, source contracts and offline operator faults.
These tests cannot compile Enforce or prove AI gameplay.
"""
import ast,json,pathlib,re,sys,tempfile,unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from carraidez_new_run import prepare
from test_worldmemory import R,defaults,balanced
S=(ROOT/'mods/deutschz_eventschedulerz/scripts/3_game/dzes_runtime.c').read_text()
A=(ROOT/'mods/deutschz_eventschedulerz/scripts/4_world/dzes_adapters.c').read_text()
W=(ROOT/'mods/deutschz_world_memoriez/scripts/4_world/dzwm_service.c').read_text()
C=(ROOT/'mods/deutschz_carraidez/scripts/4_world/dzcr_service.c').read_text()
rule=(ROOT/'mods/deutschz_eventschedulerz/scripts/3_game/dzes_issue15rules.c').read_text()
expr=re.search(r'return ([^;]+);',rule).group(1).replace('&&',' and ').replace('||',' or ')
code=compile(ast.parse(expr,mode='eval'),'Enforce:IdleExpired','eval')
def expired(now,since,seconds=900,participants=0,waiting=True):
    return eval(code,{'__builtins__':{}},locals())
class SchedulerTests(unittest.TestCase):
    def test_exact_idle_boundary(self):
        self.assertFalse(expired(900000,1));self.assertTrue(expired(900001,1))
    def test_participant_capture_and_clock_protection(self):
        for args in [(999999,0,900,1,True),(999999,0,900,0,False),(100,-1,900,0,True),(0,100,900,0,True)]:self.assertFalse(expired(*args))
    def test_both_adapters_reset_new_run(self):
        self.assertIn('m_DZES_NoPlayerSince = -1;\n  m_DZES_Owned=true;',A)
        self.assertIn('m_DZES_FreeNoPlayerSince = -1;\n  m_DZES_FreeOwned=true;',A)
        self.assertEqual(A.count('DZES_Issue15Rules.IdleExpired('),2)
    def test_cleanup_before_slot_release(self):
        cleanup=A.split('override void CleanupEvent')[1].split('modded class DZKOTHF')[0]
        self.assertLess(cleanup.index('super.CleanupEvent'),cleanup.index('FinishCleanup'))
        self.assertIn('CleanupRewardCrate();',A)
    def test_restore_never_shortens_deadline(self):
        block=S.split('// A provider becoming ready')[1].split('if (s_State.ProviderId !=')[0]
        for forbidden in ['NextEventAt =','NextEventUTC =','s_NextAllowedAt =']:self.assertNotIn(forbidden,block)
        self.assertIn('now < s_NextAllowedAt || now < s_State.NextEventAt',S)
        self.assertIn('ScheduledProviderGeneration = providerGeneration',S)
    def test_rotation_no_starvation_contract(self):
        self.assertIn('KOTH',S)
        rotation=re.search(r's_MajorRotation = \{([^}]+)\}',S)
        self.assertIsNotNone(rotation)
        order=re.findall(r'"(KOTH|COURIER|RAVEN|CONVOY)"',rotation.group(1))
        self.assertEqual(order,['KOTH','COURIER','RAVEN','CONVOY'])
        self.assertEqual([order[i % len(order)] for i in range(16)],order*4)
        self.assertIn('RotationIndex',S);self.assertIn('ReleaseCurrent("CLEANUP_CONFIRMED")',S)
    def test_logging_does_not_reduce_poll(self):
        convoy=A.split('modded class AIConvoyManager')[1]
        self.assertIn('DeferredLogIntervalSeconds()',convoy)
        self.assertIn('10000',convoy)
        self.assertIn('blockedState != m_DZES_LastDeferredState',convoy)
class WorldTests(unittest.TestCase):
    def test_default_grid_is_256_valid_unique_cells(self):
        c=defaults();ids=set()
        for x in range(R.Axis(c['worldMin'],c['originX'],c['regionSize']),R.Axis(c['worldMax']-.01,c['originX'],c['regionSize'])+1):
            for z in range(R.Axis(c['worldMin'],c['originZ'],c['regionSize']),R.Axis(c['worldMax']-.01,c['originZ'],c['regionSize'])+1):ids.add((x,z))
        self.assertEqual(len(ids),256);self.assertTrue(c['preseedGrid'])
    def test_grid_preserves_loaded_values_and_budget(self):
        seed=W.split('protected void SeedGrid()')[1].split('protected bool Valid')[0]
        self.assertEqual(seed.count('data.regions.Count() < config.maxRegions'),2)
        self.assertIn('if (m_Regions.Find(id, region)) return region;',W)
        self.assertLess(W.index('Restore();'),W.index('SeedGrid();'))
    def test_quiet_grid_cannot_starve_one_hot_region(self):
        self.assertIn('candidates.Insert(candidate)',W)
        self.assertIn('Math.Min(config.maxReactionChecksPerTick, candidates.Count())',W)
        self.assertIn('reactions >= config.maxReactionsPerTick',W)
    def test_real_combat_sensor_server_guard_cooldown(self):
        hit=(ROOT/'mods/deutschz_world_memoriez/scripts/4_world/dzwm_combat.c').read_text()
        self.assertEqual(hit.count('super.EEHitBy'),2)
        self.assertEqual(hit.count('"combat"'),2)
        self.assertIn('1000',hit);self.assertIn('IsServer()',hit);self.assertIn('GetHighestDamage("Health")',hit)
    def test_all_changed_scripts_balanced(self):
        for mod in ['deutschz_eventschedulerz','deutschz_world_memoriez','deutschz_carraidez']:
            for path in (ROOT/'mods'/mod).rglob('*.c'):self.assertTrue(balanced(path.read_text()),str(path))
class LedgerTests(unittest.TestCase):
    def setup_profile(self,root,phase='ABORTED'):
        (root/'config.json').write_text(json.dumps({'schemaVersion':1,'runID':'old','enabled':True,'position':[1,0,2]}))
        (root/'ledger.json').write_text(json.dumps({'schemaVersion':1,'runID':'old','phase':phase,'rewardCommitted':phase=='CLAIMED'}))
    def test_archive_precedes_current_and_spawn(self):
        commit=C.split('bool Commit(')[1].split('bool Initialize')[0]
        self.assertLess(commit.index('ArchiveLedger(proposed)'),commit.index('SaveFile(ROOT'))
        init=C.split('bool Initialize()')[1]
        self.assertLess(init.index('RUN_CONSUMED'),init.index('CreateObjectEx'))
        self.assertIn('previousArchive.rewardCommitted && !ledger.rewardCommitted',C)
    def test_new_run_retains_consumed_history_and_current_ledger(self):
        with tempfile.TemporaryDirectory() as d:
            root=pathlib.Path(d);self.setup_profile(root)
            before=(root/'ledger.json').read_bytes();prepare(root,'new')
            self.assertEqual((root/'ledger.json').read_bytes(),before)
            self.assertEqual(json.loads((root/'runs/old.json').read_text())['phase'],'ABORTED')
            with self.assertRaises(ValueError):prepare(root,'old')
    def test_active_requires_explicit_cancel(self):
        with tempfile.TemporaryDirectory() as d:
            root=pathlib.Path(d);self.setup_profile(root,'ACTIVE')
            with self.assertRaises(ValueError):prepare(root,'next')
            prepare(root,'next',True)
            self.assertEqual(json.loads((root/'ledger.json').read_text())['phase'],'ABORTED')
    def test_claimed_never_reset(self):
        with tempfile.TemporaryDirectory() as d:
            root=pathlib.Path(d);self.setup_profile(root,'CLAIMED');prepare(root,'next',True)
            self.assertTrue(json.loads((root/'runs/old.json').read_text())['rewardCommitted'])
    def test_invalid_ids_and_corruption_fail_closed(self):
        for run in ['../old','a/b','', 'x'*65]:
            with tempfile.TemporaryDirectory() as d:
                root=pathlib.Path(d);self.setup_profile(root)
                with self.assertRaises(ValueError):prepare(root,run)
        with tempfile.TemporaryDirectory() as d:
            root=pathlib.Path(d);self.setup_profile(root);(root/'ledger.json').write_text('{broken')
            with self.assertRaises(ValueError):prepare(root,'new')
class DependencyContracts(unittest.TestCase):
    def test_magazine_patch_removes_only_proven_ids(self):
        proof=json.loads((ROOT/'tests/fixtures/issue15_magazine_audit.json').read_text())
        patch=(ROOT/'mods/deutschz_weapon_compat/config.cpp').read_text()
        covered=set()
        for weapon in proof['weapons']:
            self.assertTrue(weapon['preserved'])
            self.assertFalse(set(weapon['removed']) & set(weapon['preserved']))
            self.assertLessEqual(set(weapon['removed']),set(proof['rejectedMagazineIDs']))
            self.assertIn('class '+weapon['weapon']+': '+weapon['parent'],patch)
            self.assertIn('"'+weapon['requiredAddon']+'"',patch)
            covered.update(weapon['removed'])
        self.assertEqual(covered,set(proof['rejectedMagazineIDs']))
        self.assertEqual(patch.count('magazines[] -='),len(proof['weapons']))
    def test_radio_needs_real_working_transmitter_frequency(self):
        radio=(ROOT/'audit/issue15/radio.c').read_text()
        for value in ['Class.CastTo(transmitter, entity)','IsRuined','IsWorking','GetTunedFrequency','GetIdentity']:
            self.assertIn(value,radio)
        self.assertIn('Math.AbsFloat',radio)
    def test_rbm_recovery_requires_stationary_windows_and_real_arrival(self):
        rbm=(ROOT/'audit/issue15/rbm.c').read_text()
        recovery=rbm.split('void UpdateStuck()')[1].split('void SyncMarker')[0]
        for token in ['m_StationaryChecks < 3','m_StuckStrikes == 2','m_StuckStrikes >= 3','OpenStop(recoveryStop)','m_RecoveryCooldownUntil = now + 60000']:
            self.assertIn(token,recovery)
        last=recovery.split('vector recovered = target;')[1]
        self.assertLess(last.index('SetPosition(recovered)'),last.index('AdvanceWaypoint()'))
    def test_rbm_stop_closes_and_removes_marker(self):
        rbm=(ROOT/'audit/issue15/rbm.c').read_text()
        self.assertIn('void CloseStop()',rbm)
        self.assertIn('DZRB_ExpansionTraderService.Close()',rbm)
        self.assertIn('DZRB_MarkerService.RemoveCurrent',rbm)
class EconomyTests(unittest.TestCase):
    def test_precise_removal_preserves_valid_nominals(self):
        from issue15_economy import remove_types
        text='<types><type name="bad"><nominal>10</nominal></type><type name="good"><nominal>7</nominal><min>3</min></type></types>'
        changed,names=remove_types(text,{'bad'})
        self.assertEqual(names,['bad']);self.assertIn('<nominal>7</nominal>',changed);self.assertIn('<min>3</min>',changed)
        self.assertEqual(remove_types(changed,{'bad'}),(changed,[]))
    def test_testserver_economy_scope(self):
        changes=json.loads((ROOT/'tests/fixtures/issue15_economy_removed.json').read_text())
        removed=set(sum(changes.values(),[]))
        self.assertEqual(removed,{'ChristmasTree','OvenIndoor','Ammo_Anzio20mmTracer_Green'})
class LogVerifierTests(unittest.TestCase):
    def test_compile_failure_overrides_ready(self):
        from check_issue15_logs import inspect
        for failure in ["Can't compile \"World\" script module!",'SCRIPT (E): failure','Multiple declaration of variable']:
            self.assertTrue(inspect('[WorldMemoriez] READY\n'+failure)['compileOrRuntimeError'])
    def test_warning_is_not_claimed_gameplay_failure_or_pass(self):
        from check_issue15_logs import inspect
        report=inspect('[WorldMemoriez] GRID sectors=256 tracked=256 max=256\n[WorldMemoriez] READY\nunable to create magazine Bad\nType Bad will be ignored')
        self.assertTrue(report['worldMemoryReady']);self.assertEqual(report['magazineErrors'],1)
        self.assertEqual(report['ignoredCETypes'],1);self.assertFalse(report['gameplayTested'])
    def test_future_deadline_not_capped_by_changed_gap_config(self):
        restore=S.split('else if (s_State.NextEvent != "")')[1].split('if (!loadedState)')[0]
        self.assertNotIn('MaxMajorEventGapSeconds',restore)
        self.assertIn('s_State.NextEventAtUTCSeconds - nowUTC',restore)
if __name__=='__main__':unittest.main()
