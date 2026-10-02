class DZATM_ClientNotifications
{
	protected static ref DZATM_ClientNotifications s_Instance;
	protected Widget m_HackRoot;
	protected TextWidget m_HackText;
	protected ProgressBarWidget m_HackBar;
	protected Widget m_WarningRoot;
	protected TextWidget m_WarningText;

	static DZATM_ClientNotifications Get()
	{
		if (!s_Instance)
			s_Instance = new DZATM_ClientNotifications;
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
			m_HackRoot.Show(DZATM_ClientProgressState.Visible);
			if (DZATM_ClientProgressState.Visible)
			{
				float percent = Math.Clamp(DZATM_ClientProgressState.Progress * 100.0, 0.0, 100.0);
				float remaining = Math.Max(DZATM_ClientProgressState.Duration * (1.0 - DZATM_ClientProgressState.Progress), 0.0);
				m_HackText.SetText("ATM RAIDZ | " + DZATM_ClientProgressState.Label + " | " + Math.Ceil(remaining).ToString() + "s");
				m_HackBar.SetCurrent(percent);
			}
		}
		if (m_WarningRoot)
		{
			bool showMessage = DZATM_ClientProgressState.Message != "" && GetGame().GetTime() < DZATM_ClientProgressState.MessageUntil;
			m_WarningRoot.Show(showMessage);
			if (showMessage)
				m_WarningText.SetText(DZATM_ClientProgressState.Message);
		}
	}

	protected void EnsureWidgets()
	{
		if (m_HackRoot || !GetGame().GetWorkspace())
			return;
		m_HackRoot = GetGame().GetWorkspace().CreateWidgets("deutschz_atmraidez/gui/layouts/dzatm_hack_progress.layout");
		if (m_HackRoot)
		{
			m_HackText = TextWidget.Cast(m_HackRoot.FindAnyWidget("DZATM_HackText"));
			m_HackBar = ProgressBarWidget.Cast(m_HackRoot.FindAnyWidget("DZATM_HackBar"));
			m_HackRoot.Show(false);
		}
		m_WarningRoot = GetGame().GetWorkspace().CreateWidgets("deutschz_atmraidez/gui/layouts/dzatm_warning.layout");
		if (m_WarningRoot)
		{
			m_WarningText = TextWidget.Cast(m_WarningRoot.FindAnyWidget("DZATM_WarningText"));
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
