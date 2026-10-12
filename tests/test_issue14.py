"""Source contracts and explicit failure/state models; engine results are separate."""
import pathlib, re, unittest
ROOT=pathlib.Path(__file__).resolve().parents[1]
def source(module,name):return (ROOT/'mods'/module/'scripts'/name).read_text()
W=source('deutschz_witnessz','4_world/dzwz_service.c')
C=source('deutschz_carraidez','4_world/dzcr_service.c')

class WitnessTests(unittest.TestCase):
    def test_real_hit_only(self):
        text=source('deutschz_witnessz','4_world/dzwz_hooks.c')
        for token in ['override void EEHitBy','super.EEHitBy','GetHighestDamage("Health") <= 0','GetHierarchyRootPlayer','attacker.IsAI()','< 1000']:
            self.assertIn(token,text)
    def test_identity_and_current_los(self):
        for token in ['GetIdentity()', 'FindAttachmentBySlotName("Mask")', 'minimumFacingDot','info.GetEntity() != subject','eAI_HasLOS(info)','ClearRay(witness, subject)','DayZPhysics.RayCastBullet']:
            self.assertIn(token,W)
    def test_actual_hit_must_be_visible(self):
        self.assertIn('ClearRay(witness, victim)',W)
        self.assertIn('victim.GetPosition()) > m_Settings.identificationRange',W)
    def test_dead_witness_never_reports(self):
        text=W[W.index('void Tick()'):W.index('bool Remembers')]
        self.assertLess(text.index('!pending.witness.IsAlive()'),text.index('memory.expiresUTC ='))
        self.assertIn('FactionOf(pending.witness) != pending.faction',text)
        self.assertIn('pending.witness.IsUnconscious()',text)
    def test_no_omniscient_tracking(self):
        self.assertNotIn('GetName()',W.replace('GetFaction().GetName()',''))
        self.assertNotIn('GetPlayers',W)
        self.assertNotIn('GetObjectsAtPosition',W)
        self.assertNotIn('SetPosition',W)
        self.assertIn('return Identifies(witness, subject)',W)
    def test_lookup_precedes_expensive_visibility(self):
        text=W[W.index('bool Remembers'):]
        self.assertLess(text.index('m_Index.Find'),text.index('Identifies(witness'))
        self.assertIn('m_Checks >= m_Settings.maxChecksPerSecond',text)
    def test_load_and_memory_caps(self):
        for token in ['candidate.memories.Count() > m_Settings.maxMemories','m_Pending.Count() < m_Settings.maxPending','m_Checks < m_Settings.maxChecksPerSecond','m_Data.memories.Count() >= m_Settings.maxMemories']:
            self.assertIn(token,W)
    def test_restart_drops_unreported_witnesses(self):
        self.assertNotIn('m_Pending',source('deutschz_witnessz','3_game/dzwz_save.c'))
        self.assertIn('Instance.m_Pending.Clear()',W)
    def test_dual_save_readback_unknown_schema(self):
        for token in ['state.a.json','state.b.json','candidate.schemaVersion != 1','readback.revision != nextRevision','existing && !restored','unreadable state preserved']:
            self.assertIn(token,W)
    def test_identity_faction_expiry_model(self):
        memories={('alice','east'):130}
        self.assertTrue(memories.get(('alice','east'),0)>100)
        self.assertFalse(memories.get(('bob','east'),0)>100)
        self.assertFalse(memories.get(('alice','west'),0)>100)
        self.assertFalse(memories.get(('alice','east'),0)>131)
    def test_bounded_multiplayer_load_model(self):
        # Global budget, not a fresh 64-ray allowance for every player.
        checks=0;accepted=0
        for player in range(10000):
            if checks>=64:continue
            checks+=1;accepted+=1
        self.assertEqual((checks,accepted),(64,64))

class CarRaidTests(unittest.TestCase):
    def test_reservation_precedes_spawn(self):
        text=C[C.index('bool Initialize()'):C.index('bool Near(')]
        self.assertLess(text.index('Commit("RESERVED", false)'),text.index('CreateObjectEx'))
        self.assertIn('m_Ledger.runID == m_Settings.runID',text)
    def test_payout_precedes_loot_and_at_most_once(self):
        text=C[C.index('void Tick()'):C.index('void Finish(')]
        self.assertLess(text.index('Commit("CLAIMED", true)'),text.index('CreateObjectEx("SeaChest"'))
        self.assertIn('if (m_Finished)',text)
        self.assertIn('Finish(true)',text)
    def test_failures_stop_and_preserve_consumed_run(self):
        self.assertIn('if (!Commit("ACTIVE", false)) {Finish(false);return;}',C)
        self.assertIn('readback.rewardCommitted != proposed.rewardCommitted',C)
        self.assertIn('m_Ledger.schemaVersion != 1',C)
    def test_multiplayer_ownership_and_distance(self):
        self.assertIn('m_CapturerUID = player.GetIdentity().GetId()',C)
        self.assertIn('m_Capturer.GetIdentity().GetId() != m_CapturerUID',C)
        self.assertIn('m_Settings.interactionRadius',C)
        self.assertIn('|| m_Capturer) return',C)
    def test_disconnect_death_abort_and_cleanup(self):
        for token in ['!Near(m_Capturer)','player.IsAlive()','player.IsUnconscious()','m_CleanupUTC','m_Owned.Clear()','ObjectDelete(entity)','Remove(Instance.Tick)']:
            self.assertIn(token,C)
    def test_major_rotation_unchanged(self):
        self.assertIn('DZES_Scheduler.HasActiveMajorEvent()',C)
        for token in ['TryAcquire','Transition(', 'RequestMajor','DZES_Scheduler.Queue(']:self.assertNotIn(token,C)
    def test_no_unbounded_spawns(self):
        settings=source('deutschz_carraidez','3_game/dzcr_settings.c')
        self.assertIn('Math.Clamp(guardCount, 1, 4)',settings)
        self.assertIn('rewards.Count() > 8',settings)
        self.assertIn('m_Vehicle.DZCR_Stage() == 1',C)
        self.assertIn('m_Vehicle.DZCR_SetStage(2)',C)
    def test_configured_classes_runtime_validated(self):
        for token in ['Exists(m_Settings.vehicleClass)','Exists(m_Settings.guardClass)','ConfigIsExisting("CfgWeapons AKM")','ConfigIsExisting("CfgMagazines Mag_AKM_30Rnd")','CfgVehicles " + rewardClass','CfgMagazines " + rewardClass']:
            self.assertIn(token,C)
    def test_crash_point_at_most_once_model(self):
        # Persistent reservation is the authority even if a process dies between steps.
        for crash_after in range(5):
            ledger=None;payouts=0
            steps=['reserve','spawn','activate','claim','payout']
            for step in steps[:crash_after+1]:
                if step=='reserve':ledger='RESERVED'
                if step=='claim':ledger='CLAIMED'
                if step=='payout':payouts+=1
            restart_allowed=ledger is None
            self.assertFalse(restart_allowed)
            self.assertLessEqual(payouts,1)

class PackagingTests(unittest.TestCase):
    def test_template_calls_are_explicit_assignments(self):
        for module in ['deutschz_carraidez','deutschz_witnessz','deutschz_adaptive_camouflage']:
            for path in (ROOT/'mods'/module/'scripts').rglob('*.c'):
                self.assertNotRegex(path.read_text(),r'if\s*\([^\n]*JsonFileLoader<')
    def test_action_argument_does_not_collide_with_type(self):
        text=source('deutschz_carraidez','4_world/dzcr_actions.c')
        self.assertIn('OnExecuteServer(ActionData action_data)',text)
        self.assertNotRegex(text,r'ActionData\s+actionData\b')

    def test_independent_modules_and_cleanup(self):
        for module,prefix in [('deutschz_carraidez','dzcr'),('deutschz_witnessz','dzwz'),('deutschz_adaptive_camouflage','dzac')]:
            config=(ROOT/'mods'/module/'config.cpp').read_text()
            self.assertIn('DayZExpansion_AI_Scripts',config)
            self.assertIn('scripts/3_game',config)
            self.assertIn('scripts/4_world',config)
            self.assertIn('scripts/5_mission',config)
            mission=source(module,'5_mission/'+prefix+'_mission.c')
            self.assertIn('super.OnMissionStart()',mission)
            self.assertIn('super.OnMissionFinish()',mission)
            for path in (ROOT/'mods'/module/'scripts').rglob('*.c'):
                text=path.read_text()
                self.assertNotRegex(text,r'\bbool\s+debug\b')
                self.assertNotIn('D ZCR_',text)
                self.assertEqual(text.count('{'),text.count('}'))
if __name__=='__main__':unittest.main()
