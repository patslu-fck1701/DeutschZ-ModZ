modded class MissionServer
{
    override void OnInit()
    {
        super.OnInit();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZATM_DeferredInit, 1500, false);
    }

    override void InvokeOnDisconnect(PlayerBase player)
    {
        DZATM_Manager.GetInstance().CancelPlayer(player, DZATM_Const.CANCEL_PLAYER_INVALID);
        super.InvokeOnDisconnect(player);
    }

    override void OnMissionFinish()
    {
        DZATM_Manager.DestroyInstance();
        super.OnMissionFinish();
    }

    protected void DZATM_DeferredInit()
    {
        if (GetGame() && GetGame().IsServer())
            DZATM_Manager.GetInstance().Init();
    }
}
