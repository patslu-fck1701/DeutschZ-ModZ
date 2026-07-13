modded class NoxZ_Speaker
{
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		// Alte Speaker wurden vor den neuen Link-Feldern gespeichert. Die
		// Originalklasse liefert dann false und DayZ meldet den Datensatz als
		// korrupt. Der Datensatz wird direkt nach EEInit geloescht, deshalb darf
		// dieser einmalige Migrationsfall den Serverstart nicht abbrechen.
		bool loaded = super.OnStoreLoad(ctx, version);
		if (!loaded)
			Print("[DeutschZ SpeakerCleanup] Legacy-Speaker toleriert und zur Loeschung markiert.");

		return true;
	}

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
