class DZEV_ClientProgressRenderer
{
	protected static ref DZEV_ClientProgressRenderer s_Instance;
	protected Widget m_Root;
	protected TextWidget m_Title;
	protected TextWidget m_Value;
	protected ProgressBarWidget m_Progress;
	protected Widget m_RedFlash;
	protected int m_LastRevision;

	static DZEV_ClientProgressRenderer Get()
	{
		if (!s_Instance)
			s_Instance = new DZEV_ClientProgressRenderer;

		return s_Instance;
	}

	void Update(float timeslice)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		if (!DZEV_ClientProgressState.IsVisible() || DZEV_ClientProgressState.IsStale(8.0))
		{
			if (DZEV_ClientProgressState.IsStale(8.0))
				DZEV_ClientProgressState.Hide();
			Hide();
			return;
		}

		EnsureWidgets();
		if (!m_Root)
			return;

		int revision = DZEV_ClientProgressState.GetRevision();
		if (revision != m_LastRevision)
		{
			ApplyState();
			m_LastRevision = revision;
		}

		m_Root.Show(true);
	}

	void FlashRed()
	{
		EnsureWidgets();
		if (!m_RedFlash || !GetGame())
			return;

		m_RedFlash.Show(true);
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(HideFlash, 650, false);
	}

	protected void ApplyState()
	{
		float current = DZEV_ClientProgressState.GetCurrent();
		float max = DZEV_ClientProgressState.GetMax();
		if (max <= 0.0)
			max = 1.0;

		float percent = Math.Clamp((current / max) * 100.0, 0.0, 100.0);
		string valueText;
		if (DZEV_ClientProgressState.GetMode() == DZEV_ProgressModes.BOSS)
			valueText = Math.Round(current).ToString() + " / " + Math.Round(max).ToString() + " HP";
		else
			valueText = Math.Round(percent).ToString() + " %";

		if (m_Title)
			m_Title.SetText(DZEV_ClientProgressState.GetLabel());
		if (m_Value)
			m_Value.SetText(valueText);
		if (m_Progress)
		{
			m_Progress.SetColor(ARGB(255, 46, 184, 46));
			m_Progress.SetCurrent(percent);
		}
	}

	protected void EnsureWidgets()
	{
		if (m_Root || !GetGame() || !GetGame().GetWorkspace())
			return;

		m_Root = GetGame().GetWorkspace().CreateWidgets("DeutschZ_KOTH_InfectedSiege/gui/layouts/dzevent/dzev_progress.layout");
		if (!m_Root)
			m_Root = GetGame().GetWorkspace().CreateWidgets("deutschz_koth_infectedsiedge_v2/gui/layouts/dzevent/dzev_progress.layout");

		if (!m_Root)
		{
			Print("[DZKOTH] HUD layout could not be created.");
			return;
		}

		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("DZEV_Title"));
		m_Value = TextWidget.Cast(m_Root.FindAnyWidget("DZEV_Value"));
		m_Progress = ProgressBarWidget.Cast(m_Root.FindAnyWidget("DZEV_ProgressBar"));
		m_RedFlash = m_Root.FindAnyWidget("DZEV_RedFlash");
		m_Root.Show(false);
		Print("[DZKOTH] HUD layout created.");
	}

	protected void Hide()
	{
		if (m_Title)
			m_Title.SetText("");
		if (m_Value)
			m_Value.SetText("");
		if (m_Root)
			m_Root.Show(false);
	}

	protected void HideFlash()
	{
		if (m_RedFlash)
			m_RedFlash.Show(false);
	}
}
