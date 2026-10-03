class DZCourierZ_SoundPlayer
{
	protected static AbstractWave s_Wave;

	static void Play(string soundSet)
	{
		if (!GetGame() || GetGame().IsServer() || soundSet == "") return;
		if (s_Wave) s_Wave.Stop();
		SoundParams soundParams = new SoundParams(soundSet);
		if (!soundParams.IsValid())
		{
			DZCourierZ_Log.Warn("Ungueltiges SoundSet auf Client: " + soundSet);
			return;
		}
		SoundObjectBuilder builder = new SoundObjectBuilder(soundParams);
		SoundObject soundObject = builder.BuildSoundObject();
		if (!soundObject) return;
		soundObject.SetKind(WaveKind.WAVEENVIRONMENT);
		s_Wave = GetGame().GetSoundScene().Play2D(soundObject, builder);
		if (!s_Wave) return;
		s_Wave.Loop(false);
		s_Wave.SetVolume(0.85);
		s_Wave.Play();
	}
}

class DZCourierZ_RewardMenu: UIScriptedMenu
{
	protected ButtonWidget m_Keep;
	protected ButtonWidget m_Item;
	protected ButtonWidget m_Money;
	protected ButtonWidget m_Hint;
	protected bool m_Submitted;

	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("deutschz_courierz/gui/layouts/courierz_reward.layout");
		m_Keep = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CourierZKeep"));
		m_Item = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CourierZItem"));
		m_Money = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CourierZMoney"));
		m_Hint = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CourierZHint"));
		GetGame().GetInput().ChangeGameFocus(1);
		return layoutRoot;
	}

	static void Open()
	{
		if (!GetGame() || GetGame().IsServer()) return;
		GetGame().GetUIManager().EnterScriptedMenu(DZCourierZ_Constants.REWARD_MENU_ID, null);
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (m_Submitted) return true;
		int choice;
		if (w == m_Keep) choice = DZCourierZ_RewardChoice.KEEP_CASE;
		else if (w == m_Item) choice = DZCourierZ_RewardChoice.SECRET_ITEM;
		else if (w == m_Money) choice = DZCourierZ_RewardChoice.MONEY;
		else if (w == m_Hint) choice = DZCourierZ_RewardChoice.SECRET_HINT;
		else return super.OnClick(w, x, y, button);

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player || !player.GetIdentity()) return true;
		m_Submitted = true;
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(DZCourierZ_RPCMessage.SELECT_REWARD);
		rpc.Write(choice);
		rpc.Send(player, DZCourierZ_Constants.RPC, true, player.GetIdentity());
		Close();
		return true;
	}

	override void OnHide()
	{
		super.OnHide();
		GetGame().GetInput().ResetGameFocus();
	}
}

class DZCourierZ_RouteMarker
{
	static const string UID = "DZ_COURIERZ_ROUTE";

	static void Set(string label, vector position)
	{
#ifdef EXPANSIONMODNAVIGATION
		ExpansionMarkerModule module = ExpansionMarkerModule.GetModuleInstance();
		if (!module) return;
		module.RemovePersonalMarkerByUID(UID);
		if (label == "") return;
		ExpansionMarkerData marker = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, UID);
		marker.SetName(label);
		marker.SetIconName("Deliver");
		marker.SetPosition(position);
		marker.SetColor(ARGB(255, 255, 210, 0));
		marker.Set3D(true);
		module.CreateMarker(marker);
#endif
	}
}

modded class PlayerBase
{
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		if (rpc_type != DZCourierZ_Constants.RPC)
		{
			super.OnRPC(sender, rpc_type, ctx);
			return;
		}

		int messageType;
		if (!ctx.Read(messageType)) return;
		if (GetGame().IsServer())
		{
			if (messageType != DZCourierZ_RPCMessage.SELECT_REWARD) return;
			int choice;
			if (ctx.Read(choice)) DZCourierZ_Manager.Get().HandleRewardSelection(this, sender, choice);
			return;
		}

		if (messageType == DZCourierZ_RPCMessage.PLAY_SOUND)
		{
			string soundSet;
			if (ctx.Read(soundSet)) DZCourierZ_SoundPlayer.Play(soundSet);
		}
		else if (messageType == DZCourierZ_RPCMessage.OPEN_REWARD)
			DZCourierZ_RewardMenu.Open();
		else if (messageType == DZCourierZ_RPCMessage.SET_ROUTE_MARKER)
		{
			string routeLabel;
			vector routePosition;
			if (ctx.Read(routeLabel) && ctx.Read(routePosition))
				DZCourierZ_RouteMarker.Set(routeLabel, routePosition);
		}
	}
}
