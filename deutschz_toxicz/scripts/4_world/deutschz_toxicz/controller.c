class DZToxicZController
{
    protected const string SETTINGS_DIR="$profile:DeutschZ-System/deutschz_toxicz";
    protected const string SETTINGS_FILE="$profile:DeutschZ-System/deutschz_toxicz/ToxicZSettings.json";
    protected const string SAVE_FILE="$profile:DeutschZ-System/deutschz_toxicz/ToxicZState.json";
    protected const string LEGACY_SETTINGS_FILE="$profile:DeutschZ/ToxicZ/ToxicZSettings.json";
    protected const string LEGACY_SAVE_FILE="$profile:DeutschZ/ToxicZ/ToxicZState.json";
    protected ref DZToxicZSettings m_Settings;
    protected ref DZToxicZSave m_Save;
    protected ref array<Object> m_Objects=new array<Object>;
    protected ref array<eAIBase> m_AI=new array<eAIBase>;
    protected eAIGroup m_AIGroup;
    protected ExpansionMarkerModule m_MarkerModule;
    protected ExpansionMarkerData m_Marker;
    protected bool m_StageSpawned;

    void Start()
    {
        MakeDirectory("$profile:DeutschZ-System"); MakeDirectory(SETTINGS_DIR);
        if (!FileExist(SETTINGS_FILE) && FileExist(LEGACY_SETTINGS_FILE))
        {
            if (CopyFile(LEGACY_SETTINGS_FILE,SETTINGS_FILE)) Print("[DeutschZ ToxicZ] Legacy settings migrated; source retained as backup.");
            else Print("[DeutschZ ToxicZ] ERROR legacy settings migration failed.");
        }
        if (!FileExist(SAVE_FILE) && FileExist(LEGACY_SAVE_FILE))
        {
            if (CopyFile(LEGACY_SAVE_FILE,SAVE_FILE)) Print("[DeutschZ ToxicZ] Legacy state migrated; source retained as backup.");
            else Print("[DeutschZ ToxicZ] ERROR legacy state migration failed.");
        }
        LoadSettings(); LoadState();
        m_MarkerModule=ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
        RecoverAfterRestart();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick,2000,true);
        Print("[DeutschZ ToxicZ] Controller started state="+m_Save.State.ToString());
    }
    void Stop() { GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick); SaveState(); CleanupRuntime(); }

    protected void LoadSettings()
    {
        m_Settings=DZToxicZSettings.Defaults();
        string error;
        if (FileExist(SETTINGS_FILE))
        {
            DZToxicZSettings loaded;
            if (JsonFileLoader<DZToxicZSettings>.LoadFile(SETTINGS_FILE,loaded,error) && loaded) m_Settings=loaded;
            else Print("[DeutschZ ToxicZ] ERROR settings load failed: "+error);
        }
        else if (!JsonFileLoader<DZToxicZSettings>.SaveFile(SETTINGS_FILE,m_Settings,error)) Print("[DeutschZ ToxicZ] ERROR settings save failed: "+error);
    }
    protected void LoadState()
    {
        m_Save=new DZToxicZSave;
        string error;
        if (FileExist(SAVE_FILE) && !JsonFileLoader<DZToxicZSave>.LoadFile(SAVE_FILE,m_Save,error)) Print("[DeutschZ ToxicZ] ERROR state load failed: "+error);
        if (m_Save.Version != 1) { Print("[DeutschZ ToxicZ] ERROR unsupported state version"); m_Save=new DZToxicZSave; }
        if (!m_Save.ParticipantIds) m_Save.ParticipantIds=new array<string>;
        if (m_Save.OwnerId!="" && m_Save.ParticipantIds.Find(m_Save.OwnerId)==-1) m_Save.ParticipantIds.Insert(m_Save.OwnerId);
    }
    protected void SaveState() { string error; if (m_Save && !JsonFileLoader<DZToxicZSave>.SaveFile(SAVE_FILE,m_Save,error)) Print("[DeutschZ ToxicZ] ERROR state save failed: "+error); }
    protected void RecoverAfterRestart()
    {
        CleanupRuntime(); m_StageSpawned=false;
        if (m_Save.State == DZToxicZState.RIFFY_COMBAT) m_Save.State=DZToxicZState.RIFFY_APPROACH;
        if (m_Save.State == DZToxicZState.COOLDOWN) m_Save=new DZToxicZSave;
        RestoreMarkerForState();
        SaveState();
    }

    protected void RestoreMarkerForState()
    {
        switch (m_Save.State)
        {
            case DZToxicZState.START_SCENE: SetMarker(m_Save.StartPosition,"TOXICZ: Signalquelle"); break;
            case DZToxicZState.HOSPITAL_ONE: SetMarker(m_Save.HospitalOne,"T-17: Bergungsort I"); break;
            case DZToxicZState.HOSPITAL_TWO: SetMarker(m_Save.HospitalTwo,"T-17: Bergungsort II"); break;
            case DZToxicZState.RIFFY_APPROACH: SetMarker(m_Settings.RiffyPosition,"TOXICZ: Ursprung Riffy"); break;
            case DZToxicZState.RIFFY_COMBAT: SetMarker(m_Settings.RiffyPosition,"TOXICZ: Ursprung Riffy"); break;
            case DZToxicZState.BLACKBOX: SetMarker(m_Settings.RiffyPosition,"TOXICZ: Blackbox"); break;
            case DZToxicZState.DECODER: SetMarker(m_Settings.DecoderPosition,"T-17: Decoderstation"); break;
        }
        if (m_Marker) Print("[DeutschZ ToxicZ] Marker restored for state="+m_Save.State.ToString());
    }

    protected void Tick()
    {
        if (!m_Settings || !m_Settings.Enabled || !m_Save) return;
        if (m_Save.State == DZToxicZState.WAITING) return;
        if (m_Save.State == DZToxicZState.COOLDOWN)
        {
            if (GetGame().GetTime() >= m_Save.CooldownUntil) ResetEvent();
            return;
        }
        bool activePlayer=false;
        foreach(string uid:m_Save.ParticipantIds)
        {
            PlayerBase participant=FindPlayer(uid);
            if (!participant) continue;
            activePlayer=true; TickParticipant(participant);
        }
        if (!activePlayer) return;
    }
    protected void TickParticipant(PlayerBase owner)
    {
        switch (m_Save.State)
        {
            case DZToxicZState.START_SCENE: TickStart(owner); break;
            case DZToxicZState.HOSPITAL_ONE: TickHospitalOne(owner); break;
            case DZToxicZState.HOSPITAL_TWO: TickHospitalTwo(owner); break;
            case DZToxicZState.RIFFY_APPROACH: TickRiffyApproach(owner); break;
            case DZToxicZState.RIFFY_COMBAT: TickRiffyCombat(owner); break;
            case DZToxicZState.BLACKBOX: TickBlackbox(owner); break;
            case DZToxicZState.DECODER: TickDecoder(owner); break;
            case DZToxicZState.T17: if (GetGame().GetTime() >= m_Save.CliffhangerAt) Cliffhanger(m_Save.OwnerId); break;
        }
    }

    bool ActivateSignalMarker(PlayerBase player, Object marker)
    {
        if (!GetGame() || !GetGame().IsServer() || !m_Settings || !m_Settings.Enabled || !m_Save) return false;
        if (!Valid(player) || !marker || marker.GetType()!=m_Settings.RequiredChainItem) return false;
        string uid=player.GetIdentity().GetPlainId();
        if (m_Save.State == DZToxicZState.COOLDOWN) return false;
        if (m_Save.State == DZToxicZState.WAITING)
        {
            GetGame().ObjectDelete(marker);
            m_Save.OwnerId=uid; m_Save.StartPosition=Ground(player.GetPosition()+"2 0 2");
            if (m_Save.ParticipantIds.Find(uid)==-1) m_Save.ParticipantIds.Insert(uid);
            RecordStoryCompletion(player,"toxic_started");
            SelectHospitals(player.GetPosition()); SetState(DZToxicZState.START_SCENE);
            SpawnStartScene(); SetMarker(m_Save.StartPosition,"TOXICZ: Signalquelle"); Notify(player,"TOXICZ","Signal aktiviert. Eine auffaellige Flare und ein toter Soldat liegen vor Ihnen.");
            return true;
        }
        if (m_Save.ParticipantIds.Find(uid)!=-1)
        {
            NotifyCurrentObjective(player);
            return false;
        }
        GetGame().ObjectDelete(marker);
        m_Save.ParticipantIds.Insert(uid); SaveState();
        RecordStoryCompletion(player,"toxic_started");
        Notify(player,"TOXICZ","Signal aktiviert und mit der laufenden T-17-Operation gekoppelt.");
        NotifyCurrentObjective(player);
        if (!m_Marker) RestoreMarkerForState();
        Print("[DeutschZ ToxicZ] Participant joined by signal activation "+uid+" state="+m_Save.State.ToString());
        return true;
    }
    protected void NotifyCurrentObjective(PlayerBase player)
    {
        switch (m_Save.State)
        {
            case DZToxicZState.START_SCENE: Notify(player,"TOXICZ","Folgen Sie der aktiven Spur zur Flare und zum gefallenen Soldaten."); break;
            case DZToxicZState.HOSPITAL_ONE: Notify(player,"Protokoll T-17","Suchen Sie den markierten Bergungsort I und sichern Sie Akte und Schutzteile."); break;
            case DZToxicZState.HOSPITAL_TWO: Notify(player,"Protokoll T-17","Suchen Sie Bergungsort II und vervollstaendigen Sie den Schutzanzug."); break;
            case DZToxicZState.RIFFY_APPROACH: Notify(player,"CHERNARUS - RIFFY","Die beteiligten Parteien werden am Ursprung des Signals zusammengefuehrt."); break;
            case DZToxicZState.RIFFY_COMBAT: Notify(player,"TOXICZ","Die T-17-Operation in Riffy laeuft bereits. Unterstuetzen Sie die anderen Parteien."); break;
            case DZToxicZState.BLACKBOX: Notify(player,"Transport Sieben","Sichern Sie gemeinsam die Blackbox in Riffy."); break;
            case DZToxicZState.DECODER: Notify(player,"Blackbox verschluesselt","Treffen Sie die anderen Parteien an der Decoderstation."); break;
        }
    }
    protected void TickStart(PlayerBase player)
    {
        if (!m_StageSpawned) SpawnStartScene();
        if (!HasItem(player,"DZToxicZ_DamagedDocument") || vector.Distance(player.GetPosition(),m_Save.StartPosition)>30) return;
        CleanupStage(); SetState(DZToxicZState.HOSPITAL_ONE); SpawnHospitalOne();
        NotifyParticipants("Protokoll T-17","Morozov weiss, was in Riffy liegt. Suchen Sie den markierten Bergungsort I."); SetMarker(m_Save.HospitalOne,"T-17: Bergungsort I");
    }
    protected void TickHospitalOne(PlayerBase player)
    {
        if (!m_StageSpawned) SpawnHospitalOne();
        bool collected=HasParticipantItem("DZToxicZ_MorozovFile") && HasParticipantItem("DZToxicZ_NBCHood") && HasParticipantItem("DZToxicZ_NBCGloves") && HasParticipantItem("DZToxicZ_NBCBoots");
        if (vector.Distance(player.GetPosition(),m_Save.HospitalOne) < 80 && m_AI.Count()==0 && !m_Save.FirstSquadDefeated) { SendAudio(player,"DZToxicZ_AI_SoundSet"); SpawnSquad(m_Save.HospitalOne,m_Settings.FirstSquadCount,"T-17 Bergung I"); }
        if (m_AI.Count()>0 && CountAliveAI()==0) { m_Save.FirstSquadDefeated=true; ClearAI(); SaveState(); }
        if (!collected || !m_Save.FirstSquadDefeated || vector.Distance(player.GetPosition(),m_Save.HospitalOne)>80) return;
        CleanupStage(); SetState(DZToxicZState.HOSPITAL_TWO); SpawnHospitalTwo(); SetMarker(m_Save.HospitalTwo,"T-17: Bergungsort II");
        NotifyParticipants("Akte Morozov","Status: VERSTORBEN. Todesdatum: 14 Tage vor Transportbeginn.");
    }
    protected void TickHospitalTwo(PlayerBase player)
    {
        if (!m_StageSpawned) SpawnHospitalTwo();
        if (vector.Distance(player.GetPosition(),m_Save.HospitalTwo) < 100 && m_AI.Count()==0 && !m_Save.SecondSquadDefeated) { SendAudio(player,"DZToxicZ_AI_SoundSet"); SpawnSquad(m_Save.HospitalTwo,m_Settings.SecondSquadCount,"T-17 Bergung II"); }
        if (m_AI.Count()>0 && CountAliveAI()==0) { m_Save.SecondSquadDefeated=true; ClearAI(); SaveState(); }
        if (!m_Save.SecondSquadDefeated || !HasFullSuitAcrossParticipants() || !HasParticipantItem("DZToxicZ_AudioRecorder") || vector.Distance(player.GetPosition(),m_Save.HospitalTwo)>100) return;
        CleanupStage(); SetState(DZToxicZState.RIFFY_APPROACH); RemoveMarker();
        NotifyParticipants("CHERNARUS - RIFFY","TOXICZ-PROTOKOLL AKTIV. Schutzkleidung erforderlich.");
    }
    protected void TickRiffyApproach(PlayerBase player)
    {
        float distance=vector.Distance(player.GetPosition(),m_Settings.RiffyPosition);
        if (distance <= m_Settings.RiffyRevealDistance && !m_Marker) SetMarker(m_Settings.RiffyPosition,"TOXICZ: Ursprung Riffy");
        if (distance > m_Settings.CombatActivationDistance) return;
        if (!SpawnSquad(m_Settings.RiffyPosition,m_Settings.RiffySquadCount,"T-17 Riffy")) return;
        SendAudio(player,"DZToxicZ_Reception_SoundSet"); SetState(DZToxicZState.RIFFY_COMBAT);
    }
    protected void TickRiffyCombat(PlayerBase player)
    {
        if (CountAliveAI()>0) return;
        m_Save.RiffySquadDefeated=true; ClearAI(); CleanupStage();
        EntityAI box=EntityAI.Cast(Spawn("DZToxicZ_Blackbox",m_Settings.RiffyPosition+"6 0 3"));
        SetState(DZToxicZState.BLACKBOX); NotifyParticipants("Transport Sieben","Ladung versiegeln. Schiff isolieren. Keine Ueberlebenden an Land lassen.");
    }
    protected void TickBlackbox(PlayerBase player)
    {
        if (!m_StageSpawned && !HasItem(player,"DZToxicZ_Blackbox")) { Spawn("DZToxicZ_Blackbox",m_Settings.RiffyPosition+"6 0 3"); m_StageSpawned=true; }
        if (!HasItem(player,"DZToxicZ_Blackbox")) return;
        CleanupStage(); SetState(DZToxicZState.DECODER); Spawn("DZToxicZ_DocumentDecoder",m_Settings.DecoderPosition); m_StageSpawned=true; SetMarker(m_Settings.DecoderPosition,"T-17: Decoderstation");
        NotifyParticipants("Blackbox verschluesselt","Bringen Sie die Blackbox zum DeutschZ Document Decoder.");
    }
    protected void TickDecoder(PlayerBase player)
    {
        if (!m_StageSpawned && !HasItem(player,"DZToxicZ_DocumentDecoder")) { Spawn("DZToxicZ_DocumentDecoder",m_Settings.DecoderPosition); m_StageSpawned=true; }
        if (vector.Distance(player.GetPosition(),m_Settings.DecoderPosition)>m_Settings.ObjectiveRadius) return;
        if (!HasItem(player,"DZToxicZ_Blackbox") || !HasItem(player,"DZToxicZ_DocumentDecoder")) return;
        if (m_Save.RewardGranted) return;
        if (!SpawnReward(player.GetPosition())) { Print("[DeutschZ ToxicZ] ERROR reward transaction failed; state remains DECODER"); return; }
        m_Save.RewardGranted=true; m_Save.CliffhangerAt=GetGame().GetTime()+(m_Settings.CliffhangerDelaySeconds*1000); SaveState(); SetState(DZToxicZState.T17); RemoveMarker();
        RecordParticipantCompletions("toxic");
        NotifyParticipants("T-17 entschluesselt","Transport Sieben an Zentrale. Kuehlung ausgefallen. Morozov, was haben Sie uns da gegeben?");
    }
    protected void Cliffhanger(string ownerId)
    {
        NotifyParticipants("Unbekannte Funkquelle","Interessant. Sie sind weiter gekommen als die anderen. Aber Sie haben das Falsche auf dem Schiff gesucht.");
        m_Save.State=DZToxicZState.COOLDOWN; m_Save.CooldownUntil=GetGame().GetTime()+(m_Settings.CooldownMinutes*60000); SaveState();
    }

    protected void RecordParticipantCompletions(string eventId)
    {
        foreach(string uid:m_Save.ParticipantIds) { PlayerBase participant=FindPlayer(uid); if(participant)RecordStoryCompletion(participant,eventId); }
    }

    protected void RecordStoryCompletion(PlayerBase player,string eventId)
    {
        if(!player||!player.GetIdentity())return; string root="$profile:DeutschZ-System/deutschz_radiomissionz/event_completions"; string dir=root+"/"+eventId;
        MakeDirectory("$profile:DeutschZ-System"); MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz"); MakeDirectory(root); MakeDirectory(dir);
        FileHandle file=OpenFile(dir+"/"+player.GetIdentity().GetPlainId()+".done",FileMode.WRITE); if(file!=0){FPrintln(file,eventId);CloseFile(file);}
    }

    protected void SpawnStartScene()
    {
        CleanupStage(); Spawn("DZToxicZ_Flare",m_Save.StartPosition); Spawn("DZToxicZ_DamagedDocument",m_Save.StartPosition+"1 0 0");
        EntityAI corpse=EntityAI.Cast(Spawn("ZmbM_SoldierNormal",m_Save.StartPosition+"0 0 1"));
        if (corpse) corpse.SetHealth("","Health",0); else Print("[DeutschZ ToxicZ] ERROR start-scene corpse could not be spawned");
        m_StageSpawned=true;
    }
    protected void SpawnHospitalOne()
    {
        CleanupStage();
        SpawnStationItem("DZToxicZ_MorozovFile",m_Save.HospitalOne,0);
        SpawnStationItem("DZToxicZ_NBCHood",m_Save.HospitalOne,1);
        SpawnStationItem("DZToxicZ_NBCGloves",m_Save.HospitalOne,2);
        SpawnStationItem("DZToxicZ_NBCBoots",m_Save.HospitalOne,3);
        m_StageSpawned=true;
        Print("[DeutschZ ToxicZ] Bergungsort I loot spawned at "+m_Save.HospitalOne.ToString());
    }
    protected void SpawnHospitalTwo()
    {
        CleanupStage();
        SpawnStationItem("DZToxicZ_NBCJacket",m_Save.HospitalTwo,0);
        SpawnStationItem("DZToxicZ_NBCPants",m_Save.HospitalTwo,1);
        SpawnStationItem("DZToxicZ_GP5Mask",m_Save.HospitalTwo,2);
        SpawnStationItem("DZToxicZ_Filter",m_Save.HospitalTwo,3);
        SpawnStationItem("DZToxicZ_AudioRecorder",m_Save.HospitalTwo,4);
        m_StageSpawned=true;
        Print("[DeutschZ ToxicZ] Bergungsort II loot spawned at "+m_Save.HospitalTwo.ToString());
    }
    protected void SpawnStationItem(string type,vector center,int index)
    {
        vector pos=Ground(center+Vector(1.5+index*1.5,0,1.5));
        if (!Spawn(type,pos)) Print("[DeutschZ ToxicZ] ERROR station loot failed class="+type+" at "+pos.ToString());
    }
    protected bool SpawnReward(vector position)
    {
        EntityAI crate=EntityAI.Cast(GetGame().CreateObjectEx("DZToxicZ_RewardCrate",Ground(position+"2 0 2"),ECE_PLACE_ON_SURFACE));
        if (!crate) return false;
        foreach (DZToxicZLoot loot:m_Settings.RewardLoot) for (int i=0;i<loot.Count;i++) if (Math.RandomFloatInclusive(0,100)<=loot.Chance) AddCargo(crate,loot.ClassName);
        if (!SpawnCurrency(position,m_Settings.RewardCurrencyClass,m_Settings.RewardValueDMarkZ,m_Settings.RewardCurrencyUnitValue)) { GetGame().ObjectDelete(crate); return false; }
        Print("[DeutschZ ToxicZ] Reward transaction complete; D-MarkZ="+m_Settings.RewardValueDMarkZ.ToString()+" class="+m_Settings.RewardCurrencyClass+" unit="+m_Settings.RewardCurrencyUnitValue.ToString());
        return true;
    }
    protected bool SpawnCurrency(vector position,string className,int totalValue,int unitValue)
    {
        if(className=="" || unitValue<=0 || totalValue<=0 || !GetGame().ConfigIsExisting("CfgVehicles "+className)) return false;
        if((totalValue/unitValue)*unitValue!=totalValue) return false;
        int remaining=totalValue/unitValue; vector drop=Ground(position+"3 0 2"); ref array<ItemBase> created=new array<ItemBase>;
        while(remaining>0)
        {
            ItemBase note=ItemBase.Cast(GetGame().CreateObjectEx(className,drop,ECE_PLACE_ON_SURFACE));
            if(!note){foreach(ItemBase rollback:created)if(rollback)GetGame().ObjectDelete(rollback);return false;}
            int stack=1; if(note.HasQuantity()&&note.GetQuantityMax()>0)stack=Math.Min(remaining,note.GetQuantityMax());
            if(note.HasQuantity())note.SetQuantity(stack); note.SetSynchDirty(); created.Insert(note); remaining-=stack; drop[0]=drop[0]+0.15;
        }
        return true;
    }
    protected bool SpawnSquad(vector center,int count,string groupName)
    {
        ClearAI();
        for (int i=0;i<count;i++)
        {
            vector pos=Ground(center+Vector(Math.RandomFloatInclusive(-35,35),0,Math.RandomFloatInclusive(-35,35)));
            pos[1]=pos[1]+0.7;
            eAIBase ai=eAIBase.Cast(GetGame().CreateObject(eAISurvivor.GetQuasiRandom(),pos)); if (!ai) continue;
            ExpansionHumanLoadout.Apply(ai,m_Settings.AILoadout,false);
            if (!m_AIGroup) { m_AIGroup=ai.GetGroup(); m_AIGroup.SetName(groupName); m_AIGroup.SetFaction(eAIFaction.Create(m_Settings.AIFaction)); m_AIGroup.SetWaypointBehaviour(eAIWaypointBehavior.HALT); }
            else ai.SetGroup(m_AIGroup);
            m_AI.Insert(ai);
        }
        Print("[DeutschZ ToxicZ] AI squad spawned "+m_AI.Count().ToString()+"/"+count.ToString());
        return m_AI.Count()>0;
    }
    protected int CountAliveAI()
    {
        int alive=0;
        foreach(eAIBase ai:m_AI) { if(ai && ai.IsAlive()) alive++; }
        return alive;
    }
    protected void ClearAI() { if(m_AIGroup){m_AIGroup.ClearAI(true);m_AIGroup=null;} m_AI.Clear(); }
    protected Object Spawn(string type,vector pos) { Object o=GetGame().CreateObjectEx(type,Ground(pos),ECE_PLACE_ON_SURFACE|ECE_NOPERSISTENCY_WORLD); if(o)m_Objects.Insert(o); return o; }
    protected void AddCargo(EntityAI parent,string type) { if(parent && type!="") parent.GetInventory().CreateInInventory(type); }
    protected vector Ground(vector p) { p[1]=GetGame().SurfaceY(p[0],p[2]); return p; }
    protected bool Valid(PlayerBase p) { return p && p.GetIdentity() && p.IsAlive() && !p.IsUnconscious(); }
    protected PlayerBase FindOwner() { return FindPlayer(m_Save.OwnerId); }
    protected PlayerBase FindActiveParticipant()
    {
        PlayerBase owner=FindOwner(); if (owner) return owner;
        foreach(string uid:m_Save.ParticipantIds) { PlayerBase participant=FindPlayer(uid); if(participant)return participant; }
        return null;
    }
    protected PlayerBase FindPlayer(string id) { array<Man> ps=new array<Man>; GetGame().GetPlayers(ps); foreach(Man m:ps){PlayerBase p=PlayerBase.Cast(m);if(Valid(p)&&p.GetIdentity().GetPlainId()==id)return p;} return null; }
    protected bool HasItem(PlayerBase p,string type)
    {
        array<EntityAI> items=new array<EntityAI>;
        p.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER,items);
        foreach(EntityAI item:items) { if(item && item.GetType()==type) return true; }
        return false;
    }
    protected bool RemoveOne(PlayerBase p,string type)
    {
        array<EntityAI> items=new array<EntityAI>;
        p.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER,items);
        foreach(EntityAI item:items) { if(item && item.GetType()==type) { GetGame().ObjectDelete(item); return true; } }
        return false;
    }
    protected bool HasFullSuit(PlayerBase p) { return HasItem(p,"DZToxicZ_NBCJacket")&&HasItem(p,"DZToxicZ_NBCPants")&&HasItem(p,"DZToxicZ_NBCHood")&&HasItem(p,"DZToxicZ_NBCGloves")&&HasItem(p,"DZToxicZ_NBCBoots")&&HasItem(p,"DZToxicZ_GP5Mask")&&HasItem(p,"DZToxicZ_Filter"); }
    protected bool HasParticipantItem(string type)
    {
        foreach(string uid:m_Save.ParticipantIds) { PlayerBase participant=FindPlayer(uid); if(participant && HasItem(participant,type)) return true; }
        return false;
    }
    protected bool HasFullSuitAcrossParticipants()
    {
        return HasParticipantItem("DZToxicZ_NBCJacket") && HasParticipantItem("DZToxicZ_NBCPants") && HasParticipantItem("DZToxicZ_NBCHood") && HasParticipantItem("DZToxicZ_NBCGloves") && HasParticipantItem("DZToxicZ_NBCBoots") && HasParticipantItem("DZToxicZ_GP5Mask") && HasParticipantItem("DZToxicZ_Filter");
    }
    protected void SelectHospitals(vector start)
    {
        float best=float.MAX; float second=float.MAX; DZToxicZLocation a; DZToxicZLocation b;
        foreach(DZToxicZLocation h:m_Settings.Hospitals) if(h && h.Enabled){float score=vector.Distance(start,h.Position)+vector.Distance(h.Position,m_Settings.RiffyPosition);if(score<best){second=best;b=a;best=score;a=h;}else if(score<second){second=score;b=h;}}
        if(a)m_Save.HospitalOne=Ground(a.Position); if(b)m_Save.HospitalTwo=Ground(b.Position); else m_Save.HospitalTwo=m_Save.HospitalOne;
        if(vector.Distance(m_Save.HospitalOne,m_Settings.RiffyPosition)<vector.Distance(m_Save.HospitalTwo,m_Settings.RiffyPosition)){vector swap=m_Save.HospitalOne;m_Save.HospitalOne=m_Save.HospitalTwo;m_Save.HospitalTwo=swap;}
    }
    protected void SetState(int state) { m_Save.State=state; m_StageSpawned=false; SaveState(); Print("[DeutschZ ToxicZ] State="+state.ToString()); }
    protected void Notify(PlayerBase p,string title,string text) { if(p&&p.GetIdentity())NotificationSystem.SendNotificationToPlayerIdentityExtended(p.GetIdentity(),10,title,text); }
    protected void NotifyParticipants(string title,string text) { foreach(string uid:m_Save.ParticipantIds){PlayerBase participant=FindPlayer(uid);if(participant)Notify(participant,title,text);} }
    protected void SendAudio(PlayerBase p,string setName) { if(p&&p.GetIdentity())GetRPCManager().SendRPC(DZToxicZRPC.NS,DZToxicZRPC.AUDIO,new Param1<string>(setName),true,p.GetIdentity()); }
    protected void SetMarker(vector p,string name) { RemoveMarker(); if(m_MarkerModule)m_Marker=m_MarkerModule.CreateServerMarker(name,"Deliver",p,ARGB(255,100,255,80),false,"deutschz_toxicz"); }
    protected void RemoveMarker() { if(m_MarkerModule&&m_Marker)m_MarkerModule.RemoveServerMarker(m_Marker.GetUID()); m_Marker=null; }
    protected void CleanupStage() { foreach(Object o:m_Objects)if(o)GetGame().ObjectDelete(o);m_Objects.Clear();m_StageSpawned=false; }
    protected void CleanupRuntime() { CleanupStage(); ClearAI(); RemoveMarker(); }
    protected void ResetEvent() { CleanupRuntime(); m_Save=new DZToxicZSave; SaveState(); }
}

ref DZToxicZController g_DZToxicZ;
