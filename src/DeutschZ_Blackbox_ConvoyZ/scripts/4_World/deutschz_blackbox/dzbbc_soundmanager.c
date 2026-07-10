class DZBBC_SoundManager
{
	void BroadcastHackStage(int stage, vector pos, DZBBC_MessagesConfig messages)
	{
		if (stage == 30)
		{
			DZBBC_ServerRPC.BroadcastNotification("#STR_DZBBC_TITLE_BLACKBOX", messages.AudioFragment30, 8.0);
			DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_AUDIO_FRAGMENT, pos);
		}
		else if (stage == 60)
		{
			DZBBC_ServerRPC.BroadcastNotification("#STR_DZBBC_TITLE_SIGNAL_WARNING", messages.GlobalWarning60, 8.0);
			DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_SIGNAL_WARNING, pos);
		}
		else if (stage == 90)
		{
			DZBBC_ServerRPC.BroadcastNotification("#STR_DZBBC_TITLE_RECOVERY_UNIT", messages.RecoveryUnit90, 8.0);
			DZBBC_ServerRPC.BroadcastFX(DZBBC_FX_FINAL_WAVE, pos);
		}
	}
}
