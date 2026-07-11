class DeutschZ_FireworksLauncher: FireworksLauncher
{
	protected ref array<Object> m_DZKOTHG_SmokeObjects;

	void DeutschZ_FireworksLauncher()
	{
		m_DZKOTHG_SmokeObjects = new array<Object>;
	}

	void ~DeutschZ_FireworksLauncher()
	{
		if (GetGame())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DZKOTHG_SpawnSmokeField);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DZKOTHG_CleanupSmokeField);
		}
	}

	override protected void OnFuseIgnitedServer()
	{
		super.OnFuseIgnitedServer();

		if (!GetGame() || !GetGame().IsServer())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DZKOTHG_SpawnSmokeField);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DZKOTHG_SpawnSmokeField, 250, false);
	}

	protected void DZKOTHG_SpawnSmokeField()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		DZKOTHG_CleanupSmokeField();

		vector center = GetPosition();
		DZKOTHG_SpawnSmokeAt(center);
		DZKOTHG_SpawnSmokeAt(center + Vector(25.0, 0.0, 0.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(-25.0, 0.0, 0.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(0.0, 0.0, 25.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(0.0, 0.0, -25.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(18.0, 0.0, 18.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(-18.0, 0.0, 18.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(18.0, 0.0, -18.0));
		DZKOTHG_SpawnSmokeAt(center + Vector(-18.0, 0.0, -18.0));

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DZKOTHG_CleanupSmokeField);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DZKOTHG_CleanupSmokeField, 90000, false);
		Print("[DZKOTHG] DeutschZ_FireworksLauncher spawned 25m smoke field.");
	}

	protected void DZKOTHG_SpawnSmokeAt(vector position)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 0.05;

		Object smokeObject = GetGame().CreateObjectEx("M18SmokeGrenade_White", position, ECE_PLACE_ON_SURFACE);
		if (!smokeObject)
		{
			Print("[DZKOTHG][WARN] Failed to spawn firework smoke at " + position.ToString());
			return;
		}

		smokeObject.SetPosition(position);
		smokeObject.SetOrientation(Vector(Math.RandomInt(0, 360), 0.0, 0.0));

		ItemBase smokeItem = ItemBase.Cast(smokeObject);
		if (smokeItem && smokeItem.GetCompEM() && smokeItem.GetCompEM().CanWork())
			smokeItem.GetCompEM().SwitchOn();

		m_DZKOTHG_SmokeObjects.Insert(smokeObject);
	}

	protected void DZKOTHG_CleanupSmokeField()
	{
		if (!m_DZKOTHG_SmokeObjects || !GetGame() || !GetGame().IsServer())
			return;

		foreach (Object smokeObject: m_DZKOTHG_SmokeObjects)
		{
			if (smokeObject)
				GetGame().ObjectDelete(smokeObject);
		}

		m_DZKOTHG_SmokeObjects.Clear();
	}
}
