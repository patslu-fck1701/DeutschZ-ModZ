modded class PlayerBase
{
	protected EffectSound m_DZRMZ_TransmissionSound;
	protected string m_DZRMZ_CurrentSoundSet;
	protected int m_DZRMZ_LastSoundStart;

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type == DZRMZ_RPC_PLAY_TRANSMISSION)
		{
			Param1<string> data;
			if (!ctx.Read(data) || !GetGame() || !GetGame().IsClient()) return;
			if (data.param1.IndexOf("DZRMZ_") != 0) return;
			if (m_DZRMZ_CurrentSoundSet == data.param1 && GetGame().GetTime() - m_DZRMZ_LastSoundStart < 120000) return;
			if (m_DZRMZ_TransmissionSound) m_DZRMZ_TransmissionSound.SoundStop();
			m_DZRMZ_TransmissionSound = SEffectManager.PlaySound(data.param1, GetPosition());
			if (m_DZRMZ_TransmissionSound) m_DZRMZ_TransmissionSound.SetSoundAutodestroy(true);
			m_DZRMZ_CurrentSoundSet = data.param1;
			m_DZRMZ_LastSoundStart = GetGame().GetTime();
			return;
		}
		super.OnRPC(sender, rpc_type, ctx);
	}
}
