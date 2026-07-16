class DZKOTHF_ProgressRenderer
{
	protected static ref DZKOTHF_ProgressRenderer s_Instance;
	protected Widget m_Root;
	protected TextWidget m_Title;
	protected TextWidget m_Status;
	protected TextWidget m_Percent;
	protected ProgressBarWidget m_Bar;
	protected int m_LastRevision = -1;

	static DZKOTHF_ProgressRenderer Get()
	{
		if (!s_Instance)
			s_Instance = new DZKOTHF_ProgressRenderer;
		return s_Instance;
	}

	void Update()
	{
		if (!DZKOTHF_ClientState.ProgressVisible)
		{
			if (m_Root)
				m_Root.Show(false);
			return;
		}

		EnsureWidgets();
		if (!m_Root)
			return;

		if (m_LastRevision != DZKOTHF_ClientState.ProgressRevision)
		{
			int percent = Math.Round(DZKOTHF_ClientState.ProgressValue * 100.0);
			m_Title.SetText(DZKOTHF_ClientState.ProgressEventName);
			m_Status.SetText(DZKOTHF_ClientState.ProgressStatus);
			m_Percent.SetText(percent.ToString() + " %");
			m_Bar.SetCurrent(percent);
			m_LastRevision = DZKOTHF_ClientState.ProgressRevision;
		}
		m_Root.Show(true);
	}

	void Cleanup()
	{
		if (m_Root)
			m_Root.Unlink();
		m_Root = null;
		m_Title = null;
		m_Status = null;
		m_Percent = null;
		m_Bar = null;
		m_LastRevision = -1;
	}

	protected void EnsureWidgets()
	{
		if (m_Root || !GetGame() || !GetGame().GetWorkspace())
			return;
		m_Root = GetGame().GetWorkspace().CreateWidgets("DeutschZ_KotHZ_Free/gui/layouts/dzkothf_progress.layout");
		if (!m_Root)
			return;
		m_Title = TextWidget.Cast(m_Root.FindAnyWidget("DZKOTHF_Title"));
		m_Status = TextWidget.Cast(m_Root.FindAnyWidget("DZKOTHF_Status"));
		m_Percent = TextWidget.Cast(m_Root.FindAnyWidget("DZKOTHF_Percent"));
		m_Bar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("DZKOTHF_Bar"));
	}
}
