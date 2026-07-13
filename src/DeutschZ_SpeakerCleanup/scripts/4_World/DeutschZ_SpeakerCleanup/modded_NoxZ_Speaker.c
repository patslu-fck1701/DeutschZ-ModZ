modded class NoxZ_Speaker
{
	override void EEInit()
	{
		super.EEInit();

		if (GetGame() && GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZSPK_DeleteDisabledSpeaker, 250, false);
	}

	protected void DZSPK_DeleteDisabledSpeaker()
	{
		if (GetGame() && GetGame().IsServer())
			GetGame().ObjectDelete(this);
	}
}
