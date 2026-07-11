class DZKOTH_ClientHUD
{
	static void OnHudRPC(ParamsReadContext ctx)
	{
		int mode;
		string label;
		float current;
		float max;

		if (!ctx.Read(mode))
			return;
		if (!ctx.Read(label))
			return;
		if (!ctx.Read(current))
			return;
		if (!ctx.Read(max))
			return;

		DZEV_ClientProgressState.Set(mode, label, current, max);
	}

	static void OnFXRPC(ParamsReadContext ctx)
	{
		int fx;
		vector pos;

		if (!ctx.Read(fx))
			return;
		if (!ctx.Read(pos))
			return;

		if (fx == DZKOTH_FXIds.EVENT_START)
			DZEV_ClientProgressHud.Get().PlayEventStart(pos);
		else if (fx == DZKOTH_FXIds.FIREWORKS)
			DZEV_ClientProgressHud.Get().PlayFireworks(pos);
		else if (fx == DZKOTH_FXIds.BOSS_WARNING)
			DZEV_ClientProgressHud.Get().PlayAncientScream(pos);
		else if (fx == DZKOTH_FXIds.BOSS_SPAWN)
			DZEV_ClientProgressHud.Get().PlayBossSpawnEffect(pos);
		else if (fx == DZKOTH_FXIds.BOSS_DEATH)
		{
			DZEV_ClientProgressHud.Get().PlayFireworks(pos);
			DZEV_ClientProgressHud.Get().StopMusic();
		}
		else if (fx == DZKOTH_FXIds.KEYCARD_SIGNAL)
			DZEV_ClientProgressHud.Get().PlayBossAlarm(pos);
		else if (fx == DZKOTH_FXIds.CLEAR)
			DZEV_ClientProgressHud.Get().StopMusic();
	}
}
