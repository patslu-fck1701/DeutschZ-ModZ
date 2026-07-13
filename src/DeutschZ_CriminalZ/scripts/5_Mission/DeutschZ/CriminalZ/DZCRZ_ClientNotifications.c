class DZCRZ_ClientNotifications
{
	protected static ref DZCRZ_ClientNotifications s_Instance;
	protected Widget m_HackRoot;
	protected TextWidget m_HackText;
	protected ProgressBarWidget m_HackBar;
	protected Widget m_WarningRoot;
	protected TextWidget m_WarningText;

	static DZCRZ_ClientNotifications Get()
	{
		if (!s_Instance)
			s_Instance = new DZCRZ_ClientNotifications;
		return s_Instance;
	}

	static void Destroy()
	{
		if (s_Instance)
			s_Instance.HideAll();
		s_Instance = null;
	}

	void Update()
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;
		EnsureWidgets();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.IsAlive() || player.IsUnconscious())
		{
			HideAll();
			return;
		}
		if (m_HackRoot)
		{
			m_HackRoot.Show(DZCRZ_ClientProgressState.Visible);
			if (DZCRZ_ClientProgressState.Visible)
			{
				float percent = Math.Clamp(DZCRZ_ClientProgressState.Progress * 100.0, 0.0, 100.0);
				float remaining = Math.Max(DZCRZ_ClientProgressState.Duration * (1.0 - DZCRZ_ClientProgressState.Progress), 0.0);
				m_HackText.SetText("CRIMINALZ | " + DZCRZ_ClientProgressState.Label + " | " + Math.Ceil(remaining).ToString() + "s");
				m_HackBar.SetCurrent(percent);
			}
		}
		if (m_WarningRoot)
		{
			bool showMessage = DZCRZ_ClientProgressState.Message != "" && GetGame().GetTime() < DZCRZ_ClientProgressState.MessageUntil;
			m_WarningRoot.Show(showMessage);
			if (showMessage)
				m_WarningText.SetText(DZCRZ_ClientProgressState.Message);
		}
	}

	protected void EnsureWidgets()
	{
		if (m_HackRoot || !GetGame().GetWorkspace())
			return;
		m_HackRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_CriminalZ/gui/layouts/dzcrz_hack_progress.layout");
		if (m_HackRoot)
		{
			m_HackText = TextWidget.Cast(m_HackRoot.FindAnyWidget("DZCRZ_HackText"));
			m_HackBar = ProgressBarWidget.Cast(m_HackRoot.FindAnyWidget("DZCRZ_HackBar"));
			m_HackRoot.Show(false);
		}
		m_WarningRoot = GetGame().GetWorkspace().CreateWidgets("DeutschZ_CriminalZ/gui/layouts/dzcrz_warning.layout");
		if (m_WarningRoot)
		{
			m_WarningText = TextWidget.Cast(m_WarningRoot.FindAnyWidget("DZCRZ_WarningText"));
			m_WarningRoot.Show(false);
		}
	}

	protected void HideAll()
	{
		if (m_HackRoot)
			m_HackRoot.Show(false);
		if (m_WarningRoot)
			m_WarningRoot.Show(false);
	}
}
