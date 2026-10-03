modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type != DZRB_AudioData.RPC_GREETING)
		{
			super.OnRPC(sender, rpc_type, ctx);
			return;
		}
		if (GetGame().IsServer()) return;
		int variant;
		if (!ctx.Read(variant)) return;
		string soundSet = "DZRB_BlackMart_SoundSet";
		if (variant == 1) soundSet = "DZRB_BlackMart1_SoundSet";
		if (variant == 2) soundSet = "DZRB_BlackMart2_SoundSet";
		SEffectManager.PlaySound(soundSet, GetPosition());
	}
}
