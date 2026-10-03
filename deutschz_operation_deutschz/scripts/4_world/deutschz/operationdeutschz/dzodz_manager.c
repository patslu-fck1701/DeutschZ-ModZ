class DZODZ_Manager
{
    protected static ref DZODZ_Manager s_Instance;
    protected ref DZODZ_Settings m_Settings;
    protected Object m_Terminal;

    static DZODZ_Manager GetInstance()
    {
        if (!s_Instance)
            s_Instance = new DZODZ_Manager();
        return s_Instance;
    }

    void Initialize()
    {
        if (!GetGame().IsServer() || m_Terminal)
            return;
        m_Settings = DZODZ_SettingsLoader.Load();
        if (!m_Settings || !m_Settings.Enabled)
            return;
        vector terminalPosition = m_Settings.TerminalPosition.ToVector();
		terminalPosition[1] = GetGame().SurfaceY(terminalPosition[0], terminalPosition[2]);
        vector terminalOrientation = m_Settings.TerminalOrientation.ToVector();
        m_Terminal = GetGame().CreateObjectEx("DeutschZ_OperationDeutschZ_Terminal", terminalPosition, ECE_PLACE_ON_SURFACE);
        if (m_Terminal)
            m_Terminal.SetOrientation(terminalOrientation);
        if (m_Terminal)
            Print("[DeutschZ Operation DeutschZ] Initialisiert. Terminal=" + m_Terminal.GetPosition().ToString());
		else
			Print("[DeutschZ Operation DeutschZ] FEHLER: Terminal konnte nicht gespawnt werden.");
    }

    bool CreateMasterReader(PlayerBase player)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.IsAlive() || player.IsUnconscious() || !m_Settings || !m_Settings.Enabled || !m_Terminal)
            return false;
        if (vector.Distance(player.GetPosition(), m_Terminal.GetPosition()) > 4.0)
            return false;
        EntityAI registeredReader = DZODZ_PlayerItems.Find(player, "DeutschZ_BattlegroundZ_RegisteredCardReader");
        EntityAI operationKey = DZODZ_PlayerItems.Find(player, "DeutschZ_BattlegroundZ_OperationKeyCard");
        if (!registeredReader || !operationKey)
            return false;
        if (registeredReader.IsRuined() || operationKey.IsRuined())
            return false;
        EntityAI master = player.GetInventory().CreateInInventory("DeutschZ_OperationDeutschZ_MasterCardReader");
        if (!master)
            return false;
        GetGame().ObjectDelete(registeredReader);
        GetGame().ObjectDelete(operationKey);
        Print("[DeutschZ Operation DeutschZ] MasterCardReader erzeugt; Operation autorisiert, aber noch nicht abgeschlossen.");
        return true;
    }

    // This is the only reward/completion entry point. The real operation finale
    // must call it after its final objective has succeeded.
    bool CompleteOperation(PlayerBase player)
    {
        if (!GetGame() || !GetGame().IsServer() || !player || !player.GetIdentity())
            return false;
        string completionPath = "$profile:DeutschZ-System/deutschz_radiomissionz/event_completions/operation/" + player.GetIdentity().GetPlainId() + ".done";
        if (FileExist(completionPath))
            return true;
        if (!DZODZ_PlayerItems.Find(player, "DeutschZ_OperationDeutschZ_MasterCardReader"))
            return false;
        RecordRadioMissionCompletion(player);
        if (!FileExist(completionPath))
            return false;
        CreateRewardChest(player);
        Print("[DeutschZ Operation DeutschZ] OPERATION_COMPLETED player=" + player.GetIdentity().GetPlainId());
        return true;
    }

	protected void RecordRadioMissionCompletion(PlayerBase player)
	{
		string root="$profile:DeutschZ-System/deutschz_radiomissionz/event_completions"; string dir=root+"/operation";
		MakeDirectory("$profile:DeutschZ-System"); MakeDirectory("$profile:DeutschZ-System/deutschz_radiomissionz"); MakeDirectory(root); MakeDirectory(dir);
		FileHandle file=OpenFile(dir+"/"+player.GetIdentity().GetPlainId()+".done",FileMode.WRITE); if(file!=0){FPrintln(file,"operation");CloseFile(file);}
	}

    protected void CreateRewardChest(PlayerBase player)
    {
        if (!player || !m_Settings || !m_Settings.EventLootEnabled)
            return;
        vector position = player.GetPosition() + "1.5 0 0";
		position[1] = GetGame().SurfaceY(position[0], position[2]);
        EntityAI chest = EntityAI.Cast(GetGame().CreateObjectEx("DeutschZ_OperationDeutschZ_RewardChest", position, ECE_PLACE_ON_SURFACE));
        if (!chest || !m_Settings.EventLoot)
            return;
        foreach (DZODZ_RewardEntry reward: m_Settings.EventLoot)
        {
            if (!reward || reward.ClassName == "" || Math.RandomFloat01() > reward.Chance)
                continue;
			if (!GetGame().ConfigIsExisting("CfgVehicles " + reward.ClassName))
			{
				Print("[DeutschZ Operation DeutschZ] Ungueltige Reward-Klasse: " + reward.ClassName);
				continue;
			}
            int minimum = Math.Max(reward.MinimumCount, 1);
            int maximum = Math.Max(reward.MaximumCount, minimum);
            int count = Math.RandomIntInclusive(minimum, maximum);
            for (int i = 0; i < count; i++)
                chest.GetInventory().CreateInInventory(reward.ClassName);
        }
    }
}

// String-dispatch bridge: RadioMissionZ can signal the real finale without a
// hard addon dependency back to this PBO.
bool DZODZ_CompleteOperation(PlayerBase player)
{
    return DZODZ_Manager.GetInstance().CompleteOperation(player);
}
