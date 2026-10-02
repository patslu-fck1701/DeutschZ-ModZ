class DZATM_Utils
{
    static string CancelReasonLabel(int reason)
    {
        if (reason == DZATM_Const.CANCEL_ACTION_ENDED) return "Aktion vorzeitig beendet";
        if (reason == DZATM_Const.CANCEL_PLAYER_INVALID) return "Spieler nicht handlungsfaehig";
        if (reason == DZATM_Const.CANCEL_TOO_FAR) return "Zu weit vom ATM entfernt";
        if (reason == DZATM_Const.CANCEL_TOOL_MISSING) return "Werkzeug fehlt oder ist ruiniert";
        if (reason == DZATM_Const.CANCEL_TARGET_INVALID) return "ATM nicht mehr gueltig";
        if (reason == DZATM_Const.CANCEL_SERVER_SHUTDOWN) return "Server wird beendet";
        if (reason == DZATM_Const.CANCEL_ADMIN) return "Durch Administration beendet";
        return "Unbekannter Grund";
    }
}
