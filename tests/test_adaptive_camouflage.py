"""Offline scalar and integration contracts; not an Enforce VM compiler."""
import pathlib, re, unittest, ast
from types import SimpleNamespace
ROOT=pathlib.Path(__file__).resolve().parents[1]/'mods/deutschz_adaptive_camouflage'
def clamp(v,lo,hi):return max(lo,min(v,hi))
RUNTIME=(ROOT/'scripts/4_world/dzac_runtime.c').read_text()
EXPRESSION=re.search(r'static float Combine\([^)]*\)\s*\{\s*return ([^;]+);',RUNTIME)[1]
TREE=ast.parse(EXPRESSION,mode='eval')
assert all(isinstance(n,(ast.Expression,ast.Call,ast.Attribute,ast.Name,ast.Load,ast.BinOp,ast.Add,ast.Sub,ast.Mult,ast.Constant)) for n in ast.walk(TREE))
CODE=compile(TREE,'Enforce:DZAC_Combine','eval')
def factor(clothing,posture,speed,light,lo=.65,hi=1.2):
    options=SimpleNamespace(movementPenalty=.1,darknessBonus=.08,minimumFactor=lo,maximumFactor=hi)
    return eval(CODE,{'__builtins__':{}},dict(Math=SimpleNamespace(Clamp=clamp),options=options,clothing=clothing,posture=posture,speed=speed,light=light))
class CamouflageTests(unittest.TestCase):
    def test_neutral_daylight(self):self.assertEqual(factor(0,0,0,1),1)
    def test_matching_environment(self):self.assertLess(factor(-.12,0,0,1),1)
    def test_wrong_environment(self):self.assertGreater(factor(.08,0,0,1),1)
    def test_posture(self):self.assertLess(factor(0,-.1,0,1),factor(0,-.05,0,1))
    def test_motion_exposes(self):self.assertGreater(factor(-.12,-.1,1,1),factor(-.12,-.1,0,1))
    def test_darkness(self):self.assertLess(factor(0,0,0,0),factor(0,0,0,1))
    def test_extremes_never_invisible(self):
        for cloth in [-10,0,10]:
            for posture in [-10,0,10]:
                for speed in [0,1]:
                    for light in [-10,0,1,10]:self.assertTrue(.65<=factor(cloth,posture,speed,light)<=1.2)
    def test_actual_installed_hook_and_combat_bypass(self):
        text=(ROOT/'scripts/4_world/dzac_detection.c').read_text()
        for token in ['override float CalculateThreat','super.CalculateThreat(ai, state)','m_SearchOnLOSLost','IsRaised()','IsFighting()','m_eAI_AttackCooldown','m_eAI_LastAggressionTimeout','m_eAI_LastAggressionTime','nativeThreat <= 0','nativeThreat >= 0.4']:
            self.assertIn(token,text)
        self.assertNotIn('SetVisibility',text)
    def test_per_player_cache_and_no_global_scan(self):
        text=(ROOT/'scripts/4_world/dzac_player.c').read_text()
        self.assertIn('m_DZAC_CacheUntil',text)
        self.assertIn('SurfaceGetType',text)
        self.assertIn('GetMovementState',text)
        self.assertNotIn('GetObjectsAtPosition',text)
        self.assertNotIn('GetPlayers',text)
    def test_config_and_lifecycle_fail_closed(self):
        text=(ROOT/'scripts/4_world/dzac_runtime.c').read_text()
        self.assertIn('loaded.schemaVersion != 1',text)
        self.assertIn('invalid configuration preserved',text)
        self.assertIn('s_Settings = null',(ROOT/'scripts/5_mission/dzac_mission.c').read_text())
if __name__=='__main__':unittest.main()
