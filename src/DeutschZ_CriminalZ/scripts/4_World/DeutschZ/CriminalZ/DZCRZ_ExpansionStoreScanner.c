class DZCRZ_ExpansionStoreScanner
{
	protected DZCRZ_ATMManager m_ATMManager;

	void DZCRZ_ExpansionStoreScanner(DZCRZ_ATMManager atmManager)
	{
		m_ATMManager = atmManager;
	}

	void Scan()
	{
		if (!m_ATMManager || !GetGame() || !GetGame().IsServer())
			return;
		int added;
#ifdef EXPANSIONMODMARKET
		set<ExpansionTraderNPCBase> npcTraders = ExpansionTraderNPCBase.GetAll();
		foreach (ExpansionTraderNPCBase npcTrader: npcTraders)
		{
			if (npcTrader && AddForStore(npcTrader, "Expansion Store"))
				added++;
		}
		set<ExpansionTraderStaticBase> staticTraders = ExpansionTraderStaticBase.GetAll();
		foreach (ExpansionTraderStaticBase staticTrader: staticTraders)
		{
			if (staticTrader && AddForStore(staticTrader, "Expansion Static Store"))
				added++;
		}
		DZCRZ_Log.Info("Expansion-Store-Scan abgeschlossen. Neue ATMs=" + added.ToString());
#else
		DZCRZ_Log.Warn("EXPANSIONMODMARKET fehlt; Store-Scanner bleibt inaktiv.");
#endif
	}

	protected bool AddForStore(Object store, string name)
	{
		vector position = store.ModelToWorld("2 0 1");
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		vector orientation = store.GetOrientation();
		return m_ATMManager.AddStoreATM(position, orientation, name);
	}
}
