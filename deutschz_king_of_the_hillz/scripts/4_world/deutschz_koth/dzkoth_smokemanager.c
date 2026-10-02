class DZKOTH_SmokeManager
{
    protected DZKOTH_EventFlagpole m_Flagpole;
    protected vector m_FlagpolePosition;
	protected string m_FlagClassName;

    void Setup(vector flagPosition, vector flagOrientation, string flagClassName = "")
    {
        if (!GetGame() || m_Flagpole)
            return;
		m_FlagClassName = flagClassName;
		if (m_FlagClassName == "" || !GetGame().ConfigIsExisting("CfgVehicles " + m_FlagClassName))
		{
			DZKOTH_Utils.Warn("Invalid event flag class '" + m_FlagClassName + "'; using " + DZKOTH_Const.FLAG_CLASSNAME);
			m_FlagClassName = DZKOTH_Const.FLAG_CLASSNAME;
		}

        vector pos = DZKOTH_Utils.Grounded(flagPosition);
        m_FlagpolePosition = pos;
        RemoveStaleEventFlagpoles(pos);
        int runtimeFlags = ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD;
        m_Flagpole = TrySpawnFlagpole(pos, ECE_SETUP | ECE_CREATEPHYSICS | ECE_PLACE_ON_SURFACE | runtimeFlags);
        if (!m_Flagpole)
            m_Flagpole = TrySpawnFlagpole(pos, ECE_SETUP | ECE_PLACE_ON_SURFACE | runtimeFlags);
        if (!m_Flagpole)
            m_Flagpole = TrySpawnFlagpole(pos, runtimeFlags);
        if (!m_Flagpole)
        {
            DZKOTH_Utils.Warn("Could not spawn event flagpole at " + pos.ToString());
            return;
        }

        m_Flagpole.SetOrientation(flagOrientation);
        m_Flagpole.SetPosition(pos);
        m_Flagpole.EnsureExactlyOneEventFlag(m_FlagClassName);
		// Re-assert once after the fully-built mast has propagated its attachment
		// slots. This keeps the event flag visible for joining clients as well.
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(EnsureEventFlag, 250, false);
        SetReady();
        DZKOTH_Utils.Log("Flagpole spawned at " + pos.ToString() + " orientation " + flagOrientation.ToString() + " flagClass=" + m_FlagClassName);
    }

	protected void EnsureEventFlag()
	{
		if (m_Flagpole)
			m_Flagpole.EnsureExactlyOneEventFlag(m_FlagClassName);
	}

    protected void RemoveStaleEventFlagpoles(vector pos)
    {
        if (!GetGame() || pos == vector.Zero)
            return;

        array<Object> objects = new array<Object>;
        array<CargoBase> proxies = new array<CargoBase>;
        array<Object> staleFlags = new array<Object>;
        array<Object> stalePoles = new array<Object>;
        GetGame().GetObjectsAtPosition3D(pos, 20.0, objects, proxies);

        foreach (Object object: objects)
        {
            if (!object)
                continue;
            string typeName = object.GetType();
            if (typeName == m_FlagClassName || typeName == DZKOTH_Const.FLAG_CLASSNAME || typeName == "DZEV_KOTH_Flag")
                staleFlags.Insert(object);
            else if (typeName == DZKOTH_Const.FLAGPOLE_CLASSNAME || typeName == "DZEV_KOTH_Flagpole")
                stalePoles.Insert(object);
        }

        foreach (Object staleFlag: staleFlags)
            if (staleFlag) GetGame().ObjectDelete(staleFlag);
        foreach (Object stalePole: stalePoles)
            if (stalePole) GetGame().ObjectDelete(stalePole);
    }

    protected DZKOTH_EventFlagpole TrySpawnFlagpole(vector pos, int flags)
    {
        Object object = GetGame().CreateObjectEx(DZKOTH_Const.FLAGPOLE_CLASSNAME, pos, flags);
        DZKOTH_EventFlagpole flagpole = DZKOTH_EventFlagpole.Cast(object);
        if (!flagpole && object)
            GetGame().ObjectDelete(object);
        if (flagpole)
            DZKOTH_Utils.Log("Flagpole spawn path accepted flags=" + flags.ToString() + " at " + pos.ToString());
        return flagpole;
    }

    void SetReady() { SetSmokeState(DZKOTHF_SmokeState.WHITE, "white"); }
    void SetCapture() { SetSmokeState(DZKOTHF_SmokeState.GREEN, "green"); }
    void SetCompleted() { SetSmokeState(DZKOTHF_SmokeState.RED, "red"); }

    void StopSmokeOnly()
    {
        if (m_Flagpole)
            m_Flagpole.SetSmokeState(DZKOTHF_SmokeState.NONE);
    }

    void SetFlagRaiseProgress(float progress)
    {
        if (m_Flagpole)
            m_Flagpole.SetCaptureProgress(progress);
    }

    void Cleanup()
    {
        StopSmokeOnly();
    }

    void DeleteFlagpole()
    {
        Cleanup();
        vector oldPosition = m_FlagpolePosition;
        if (m_Flagpole && GetGame())
        {
            oldPosition = m_Flagpole.GetPosition();
            GetGame().ObjectDelete(m_Flagpole);
        }
        m_Flagpole = null;
        m_FlagpolePosition = vector.Zero;
        if (oldPosition != vector.Zero && GetGame())
        {
            RemoveStaleEventFlagpoles(oldPosition);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RemoveStaleEventFlagpoles, 250, false, oldPosition);
        }
    }

    protected void SetSmokeState(int state, string label)
    {
        if (!m_Flagpole)
            return;
        // Intentionally use the networked flagpole particle only. Never spawn a physical M18 grenade.
        m_Flagpole.SetSmokeState(state);
        DZKOTH_Utils.Log("KotHZ flagpole particle smoke active: " + label + " at " + m_Flagpole.GetPosition().ToString());
    }
}
