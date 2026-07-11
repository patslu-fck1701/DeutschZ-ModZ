class DZBBC_ClientAdminCommands
{
	static void TrySendAdminCommand(ChatMessageEventParams chatParams)
	{
		if (!chatParams || !GetGame() || GetGame().IsServer())
			return;

		string text = chatParams.param3;
		if (text == "" || text.IndexOf(DZBBC_ADMIN_COMMAND_PREFIX) != 0)
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.GetIdentity())
			return;

		if (chatParams.param2 != "" && chatParams.param2 != player.GetIdentity().GetName())
			return;

		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(text);
		rpc.Send(player, DZBBC_RPC_ADMIN_COMMAND, true, player.GetIdentity());
	}
}

class DZBBC_ClientHudRenderer
{
	protected static ref DZBBC_ClientHudRenderer s_Instance;
	protected Widget m_Root;
	protected TextWidget m_Text;
	protected ProgressBarWidget m_Bar;
	protected int m_LastRevision;

	static DZBBC_ClientHudRenderer Get()
	{
		if (!s_Instance)
			s_Instance = new DZBBC_ClientHudRenderer;

		return s_Instance;
	}

	void Update(float timeslice)
	{
		if (!GetGame() || GetGame().IsDedicatedServer())
			return;

		PlayerBase localPlayer = PlayerBase.Cast(GetGame().GetPlayer());
		if (!localPlayer || !localPlayer.IsAlive() || localPlayer.IsUnconscious())
		{
			Hide();
			return;
		}

		if (!DZBBC_ClientHudState.IsVisible())
		{
			Hide();
			return;
		}

		EnsureWidgets();
		if (!m_Root)
			return;

		int revision = DZBBC_ClientHudState.GetRevision();
		if (revision != m_LastRevision)
		{
			ApplyState();
			m_LastRevision = revision;
		}

		m_Root.Show(true);
	}

	protected void ApplyState()
	{
		float current = DZBBC_ClientHudState.GetCurrent();
		float max = DZBBC_ClientHudState.GetMax();
		bool showValue = max > 0.0;

		if (max <= 0.0)
			max = current;
		if (max <= 0.0)
			max = 1.0;

		float percent = Math.Clamp(current / max, 0.0, 1.0);

		string text = "CONVOYZ | " + DZBBC_Utils.Localize(DZBBC_ClientHudState.GetLabel());
		if (showValue)
			text = text + ": " + Math.Round(current).ToString();

		if (m_Text)
			m_Text.SetText(text);
		if (m_Bar)
			m_Bar.SetCurrent(percent * 100.0);
	}

	protected void EnsureWidgets()
	{
		if (m_Root || !GetGame() || !GetGame().GetWorkspace())
			return;

		m_Root = GetGame().GetWorkspace().CreateWidgets("deutschz_blackbox_convoyz/gui/layouts/dzbbc_enemy_counter.layout");
		if (!m_Root)
			return;

		m_Text = TextWidget.Cast(m_Root.FindAnyWidget("DZBBC_EnemyCounterText"));
		m_Bar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("DZBBC_EnemyCounterBar"));
		m_Root.Show(false);
	}

	protected void Hide()
	{
		if (m_Root)
			m_Root.Show(false);
	}
}

modded class MissionGameplay
{
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);

		if (eventTypeId != ChatMessageEventTypeID)
			return;

		ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
		DZBBC_ClientAdminCommands.TrySendAdminCommand(chatParams);
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		DZBBC_ClientHudRenderer.Get().Update(timeslice);
	}
}
