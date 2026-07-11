modded class RespawnDialogue
{
	static const int DZSPAWN_ID_RANDOM = 201;
	static const int DZSPAWN_ID_COAST = 202;
	static const int DZSPAWN_ID_INLAND = 203;
	static const int DZSPAWN_ID_TRADER = 204;
	static const int DZSPAWN_ID_SAFE = 205;
	static const int DZSPAWN_ID_LOADOUT = 206;

	protected string m_DZSPAWN_SelectedMode;
	protected bool m_DZSPAWN_RequestPending;
	protected float m_DZSPAWN_CooldownRemaining;
	protected TextWidget m_DZSPAWN_ModeValue;
	protected TextWidget m_DZSPAWN_CooldownValue;
	protected TextWidget m_DZSPAWN_StatusValue;
	protected TextWidget m_DZSPAWN_InfoName;
	protected TextWidget m_DZSPAWN_InfoMode;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_MenuMusic/gui/dzkothg_respawn.layout");
		if (!layoutRoot)
			return super.Init();

		m_DetailsRoot = layoutRoot.FindAnyWidget("menu_details_tooltip");
		if (m_DetailsRoot)
		{
			m_DetailsLabel = TextWidget.Cast(m_DetailsRoot.FindAnyWidget("menu_details_label"));
			m_DetailsText = RichTextWidget.Cast(m_DetailsRoot.FindAnyWidget("menu_details_tooltip_content"));
		}

		m_CustomRespawn = layoutRoot.FindAnyWidget("respawn_button_custom");
		m_DZSPAWN_ModeValue = TextWidget.Cast(layoutRoot.FindAnyWidget("DZSPAWN_ModeValue"));
		m_DZSPAWN_CooldownValue = TextWidget.Cast(layoutRoot.FindAnyWidget("DZSPAWN_CooldownValue"));
		m_DZSPAWN_StatusValue = TextWidget.Cast(layoutRoot.FindAnyWidget("DZSPAWN_StatusValue"));
		m_DZSPAWN_InfoName = TextWidget.Cast(layoutRoot.FindAnyWidget("DZSPAWN_InfoName"));
		m_DZSPAWN_InfoMode = TextWidget.Cast(layoutRoot.FindAnyWidget("DZSPAWN_InfoMode"));
		m_DZSPAWN_SelectedMode = DZSPAWN_Mode.SAFE_RANDOM;
		SetFocus(m_CustomRespawn);
		Print("[DeutschZ SpawnSystem][CLIENT] Spawn selection active");
		return layoutRoot;
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		string acceptedMode;
		if (DZSPAWN_ClientState.ConsumeAccepted(acceptedMode))
			DZSPAWN_OnAccepted(acceptedMode);

		string deniedReason;
		int deniedRemaining;
		if (DZSPAWN_ClientState.ConsumeDenied(deniedReason, deniedRemaining))
			DZSPAWN_OnDenied(deniedReason, deniedRemaining);

		if (m_DZSPAWN_CooldownRemaining > 0.0)
		{
			m_DZSPAWN_CooldownRemaining = m_DZSPAWN_CooldownRemaining - timeslice;
			if (m_DZSPAWN_CooldownRemaining < 0.0)
				m_DZSPAWN_CooldownRemaining = 0.0;
			if (m_DZSPAWN_CooldownValue)
				m_DZSPAWN_CooldownValue.SetText(Math.Ceil(m_DZSPAWN_CooldownRemaining).ToString() + " SEK");
		}
		else if (m_DZSPAWN_CooldownValue)
		{
			m_DZSPAWN_CooldownValue.SetText("BEREIT");
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (!w || button != MouseState.LEFT)
			return false;

		int id = w.GetUserID();
		if (id == IDC_CANCEL)
		{
			Close();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().DisconnectSessionForce);
			return true;
		}

		if (id == DZSPAWN_ID_RANDOM)
			return DZSPAWN_SelectMode(DZSPAWN_Mode.RANDOM, "Expansion Zufall");
		if (id == DZSPAWN_ID_COAST)
			return DZSPAWN_SelectMode(DZSPAWN_Mode.COAST, "Expansion Kueste");
		if (id == DZSPAWN_ID_INLAND)
			return DZSPAWN_ShowDisabled("INLAND ist nicht in Expansion konfiguriert");
		if (id == DZSPAWN_ID_TRADER)
			return DZSPAWN_ShowDisabled("TRADER ist serverseitig deaktiviert");
		if (id == DZSPAWN_ID_SAFE)
			return DZSPAWN_SelectMode(DZSPAWN_Mode.SAFE_RANDOM, "Sicherer Expansion Spawn");
		if (id == DZSPAWN_ID_LOADOUT)
		{
			DZSPAWN_SetStatus("LOADOUT: SURVIVOR BASIC");
			return true;
		}
		if (id == ID_RESPAWN_CUSTOM)
		{
			if (m_DZSPAWN_RequestPending || m_DZSPAWN_CooldownRemaining > 0.0)
				return true;
			m_DZSPAWN_RequestPending = true;
			DZSPAWN_SetStatus("SERVER PRUEFT...");
			DZSPAWN_ClientRPC.RequestMode(m_DZSPAWN_SelectedMode);
			return true;
		}

		return false;
	}

	protected bool DZSPAWN_SelectMode(string mode, string displayName)
	{
		m_DZSPAWN_SelectedMode = mode;
		if (m_DZSPAWN_ModeValue)
			m_DZSPAWN_ModeValue.SetText(mode);
		if (m_DZSPAWN_InfoName)
			m_DZSPAWN_InfoName.SetText(displayName);
		if (m_DZSPAWN_InfoMode)
			m_DZSPAWN_InfoMode.SetText("ZONE                         " + mode);
		DZSPAWN_SetStatus("BEREIT");
		return true;
	}

	protected bool DZSPAWN_ShowDisabled(string reason)
	{
		DZSPAWN_SetStatus(reason);
		return true;
	}

	protected void DZSPAWN_SetStatus(string status)
	{
		if (m_DZSPAWN_StatusValue)
			m_DZSPAWN_StatusValue.SetText(status);
	}

	void DZSPAWN_OnAccepted(string mode)
	{
		m_DZSPAWN_RequestPending = false;
		DZSPAWN_SetStatus("AKZEPTIERT");
		RequestRespawn(true);
	}

	void DZSPAWN_OnDenied(string reason, int remainingSeconds)
	{
		m_DZSPAWN_RequestPending = false;
		m_DZSPAWN_CooldownRemaining = remainingSeconds;
		DZSPAWN_SetStatus("ABGELEHNT: " + reason);
	}

	protected void DZSPAWN_SetButtonImage(Widget button, string imagePath)
	{
		if (!button)
			return;
		ImageWidget image = ImageWidget.Cast(button.FindAnyWidget(button.GetName() + "_image"));
		if (image)
		{
			image.LoadImageFile(0, imagePath);
			image.SetImage(0);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!w)
			return false;
		int id = w.GetUserID();
		if (id == DZSPAWN_ID_RANDOM || id == DZSPAWN_ID_COAST || id == DZSPAWN_ID_SAFE || id == DZSPAWN_ID_LOADOUT || id == ID_RESPAWN_CUSTOM)
			DZSPAWN_SetButtonImage(w, "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/button_wide_empty_hover.paa");
		else if (id == IDC_CANCEL)
			DZSPAWN_SetButtonImage(w, "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/button_small_empty_hover.paa");
		return true;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!w)
			return false;
		int id = w.GetUserID();
		if (id == DZSPAWN_ID_RANDOM || id == DZSPAWN_ID_COAST || id == DZSPAWN_ID_SAFE || id == DZSPAWN_ID_LOADOUT || id == ID_RESPAWN_CUSTOM)
			DZSPAWN_SetButtonImage(w, "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/button_wide_empty_normal.paa");
		else if (id == IDC_CANCEL)
			DZSPAWN_SetButtonImage(w, "DeutschZ_MenuMusic/gui/darkglass_v1/02_buttons_text/button_small_empty_normal.paa");
		return true;
	}

	override bool IsFocusable(Widget w)
	{
		if (!w)
			return false;
		int id = w.GetUserID();
		return id == IDC_CANCEL || id == ID_RESPAWN_CUSTOM || id == DZSPAWN_ID_RANDOM || id == DZSPAWN_ID_COAST || id == DZSPAWN_ID_SAFE || id == DZSPAWN_ID_LOADOUT;
	}
}
