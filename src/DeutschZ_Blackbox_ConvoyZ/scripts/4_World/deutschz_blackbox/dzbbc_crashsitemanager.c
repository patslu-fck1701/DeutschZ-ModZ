class DZBBC_CrashSiteManager
{
	protected ref array<Object> m_Objects;

	void DZBBC_CrashSiteManager()
	{
		m_Objects = new array<Object>;
	}

	void Spawn(DZBBC_CrashSiteConfig site)
	{
		if (!site)
			return;

		if (site.SceneObjects && site.SceneObjects.Count() > 0)
			SpawnScene(site);
		else
		{
			SpawnList(site.WreckObjects, site.GetPosition(), 7.0, 22.0);
			SpawnList(site.EffectObjects, site.GetPosition(), 5.0, 18.0);
		}

		Object marker = DZBBC_Utils.CreateObjectSafe(DZBBC_MARKER_OBJECT_CLASSNAME, site.GetPosition());
		if (marker)
			m_Objects.Insert(marker);

		DZBBC_Utils.Log("Crashsite spawned " + m_Objects.Count().ToString() + " object(s) at " + site.GetPosition().ToString());
	}

	void Cleanup()
	{
		foreach (Object object: m_Objects)
		{
			DZBBC_Utils.DeleteObjectSafe(object);
		}

		m_Objects.Clear();
	}

	protected void SpawnList(array<string> types, vector center, float minDistance, float maxDistance)
	{
		if (!types)
			return;

		foreach (string typeName: types)
		{
			if (ShouldSkipSceneClass(typeName))
				continue;

			vector pos = DZBBC_Utils.RandomPointAround(center, minDistance, maxDistance);
			Object object = DZBBC_Utils.CreateObjectSafe(typeName, pos);
			if (object)
				m_Objects.Insert(object);
		}
	}

	protected void SpawnScene(DZBBC_CrashSiteConfig site)
	{
		vector center = site.GetPosition();
		int spawned = 0;

		foreach (DZBBC_SceneObjectConfig sceneObject: site.SceneObjects)
		{
			if (!sceneObject || sceneObject.ClassName == "")
				continue;

			if (ShouldSkipSceneClass(sceneObject.ClassName))
				continue;

			Object object = SpawnSceneObject(sceneObject, center);
			if (!object)
			{
				if (sceneObject.Required)
					DZBBC_Utils.Warn("Required crash scene object failed: " + sceneObject.ClassName);
				continue;
			}

			m_Objects.Insert(object);
			spawned++;
		}

		DZBBC_Utils.Log("Crash scene layout spawned " + spawned.ToString() + "/" + site.SceneObjects.Count().ToString() + " configured object(s).");
	}

	protected Object SpawnSceneObject(DZBBC_SceneObjectConfig sceneObject, vector center)
	{
		vector offset = sceneObject.GetOffset();
		vector pos = center + offset;
		bool placeOnSurface = sceneObject.AlignToGround;
		if (sceneObject.AlignToGround)
		{
			pos = DZBBC_Utils.Grounded(pos);
			float lift = Math.Max(offset[1], GetClassLift(sceneObject.ClassName));
			if (lift != 0.0)
			{
				pos[1] = pos[1] + lift;
				placeOnSurface = false;
			}
		}
		else
		{
			placeOnSurface = false;
		}

		Object object = DZBBC_Utils.CreateObjectSafe(sceneObject.ClassName, pos, placeOnSurface);
		if (!object)
			return null;

		object.SetOrientation(sceneObject.GetOrientation());
		object.SetPosition(pos);
		DZBBC_Utils.Log("Crash scene object placed " + sceneObject.ClassName + " pos=" + pos.ToString() + " ori=" + sceneObject.GetOrientation().ToString());
		TryActivateEffectObject(object);
		return object;
	}

	protected float GetClassLift(string typeName)
	{
		if (typeName.IndexOf("T72_Chassis") >= 0)
			return 2.05;
		if (typeName.IndexOf("HMMWV") >= 0)
			return 1.65;
		if (typeName.IndexOf("BRDM") >= 0)
			return 1.15;
		if (typeName.IndexOf("BMP1") >= 0)
			return 1.35;
		if (typeName.IndexOf("Ural") >= 0)
			return 0.8;
		if (typeName.IndexOf("Uaz") >= 0)
			return 0.65;
		if (typeName.IndexOf("Trailer") >= 0)
			return 0.45;

		return 0.0;
	}

	protected bool ShouldSkipSceneClass(string typeName)
	{
		if (typeName == "")
			return true;

		if (typeName.IndexOf("Barrel") >= 0 || typeName == DZBBC_WRECK_PROXY_CLASSNAME || typeName == "OffroadHatchback" || typeName == "CivilianSedan")
		{
			DZBBC_Utils.Warn("Blocked legacy crash scene fallback object: " + typeName);
			return true;
		}

		if (!DZBBC_Utils.IsCfgVehicle(typeName))
		{
			DZBBC_Utils.Warn("Crash scene class missing in CfgVehicles: " + typeName);
			return true;
		}

		return false;
	}

	protected void TryActivateEffectObject(Object object)
	{
		ItemBase item = ItemBase.Cast(object);
		if (!item)
			return;

		if (!item.GetCompEM())
			return;

		if (!item.GetCompEM().CanWork())
			return;

		item.GetCompEM().SwitchOn();
		DZBBC_Utils.Log("Activated scene effect object " + item.GetType());
	}
}
