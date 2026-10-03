class DZRMZ_StoryDirector
{
	protected static int s_NextTick;
	protected static const string ROOT = "$profile:DeutschZ-System/deutschz_radiomissionz";

	static void Tick(DZRMZ_Settings settings)
	{
		if (!GetGame() || !GetGame().IsServer() || !settings || GetGame().GetTime() < s_NextTick) return;
		s_NextTick = GetGame().GetTime() + 30000;
		array<Man> players = new array<Man>; GetGame().GetPlayers(players);
		foreach (Man man : players)
		{
			PlayerBase player = PlayerBase.Cast(man);
			if (!player || !player.GetIdentity() || !player.IsAlive() || !DZRMZ_RadioService.HasQualifiedRadio(player, settings)) continue;
			PresentNextChapter(player, settings);
		}
	}

	protected static void PresentNextChapter(PlayerBase player, DZRMZ_Settings settings)
	{
		string uid = player.GetIdentity().GetPlainId(); string stage; string title; string text;
		if (!Heard("00_origin", uid)) { stage="00_origin"; title="NEUER AUFTRAG: WOHER KOMMST DU?"; text="Krrrrch... Wenn du wissen willst, warum du hier bist, beginne mit der einzigen Frage, die noch niemand ehrlich beantwortet hat: Woher kommst du? Hoere weiter auf 89,5 MHz und bewahre jedes ungewoehnliche Dokument auf."; }
		else if (!Done("koth", uid)) { stage="01_koth_v2"; title="NEUER AUFTRAG: KOTH-EVENT"; text="Krrrrch... Dein naechster Storyauftrag ist eindeutig: Nimm am aktiven King-of-the-Hill-Event teil, schliesse das KOTH-Event erfolgreich ab und sichere das Dokument aus dem geschuetzten Behaelter. Erst danach wird die Geschichte auf 89,5 MHz fortgesetzt. Die Position wird ueber den KOTH-Kanal uebertragen."; }
		else if (!Done("convoy", uid)) { stage="02_convoy"; title="89,5 MHz - Transport Sieben"; text="Krrrrch... Du hast es gefunden. Oben muss T-17 stehen. Ohne Decoder bekommst du daraus nichts. Transport Sieben. Finde den Konvoi und seine Blackbox."; }
		else if (!Done("combined", uid)) { stage="03_combination"; title="89,5 MHz - Der Decoder"; text="Der Decoder und das geheime ToxicZ-Dokument stammen aus derselben Operation. Verbinde Toxicz_Doc_Decoder mit ToxicZ_Secret_Document. Aber tu das nicht an einem Ort, an dem du lange bleiben willst."; }
		else if (!Done("toxic_started", uid)) { stage="04_signal"; title="89,5 MHz - Signal aktiviert"; text="Scheisse. Du hast es geoeffnet. Der Decoder hat nicht nur gelesen, er hat ToxicZ_Signal_Marker aktiviert. Dieses Signal war jahrelang tot. Jetzt weiss noch etwas, dass du existierst."; }
		else if (!Done("toxic", uid)) { stage="05_toxic"; title="89,5 MHz - Protokoll T-17"; text="Folge dem echten ToxicZ-Ablauf bis Riffy. Morozovs Akte ist der Schluessel: Seine spaeteren Befehle tragen ein Datum nach seinem offiziellen Tod."; }
		else if (!Done("atm", uid)) { stage="06_atm"; title="89,5 MHz - Die Geldspur"; text="Tote Maenner bekommen normalerweise kein Geld. Morozov offenbar schon. Drei Buchungen liefen ueber manipulierte Automaten. Wenn du wissen willst, wer bezahlt wurde, folge dem echten ATMRaideZ-Raid."; }
		else if (!Done("courier", uid)) { stage="07_courier"; title="89,5 MHz - Die letzte Meile"; text="Die Automaten waren tote Briefkaesten. Das Geld ging an einen wissenschaftlichen Kurier. Koffer und Schluessel wurden getrennt transportiert. Finde den CourierZ-Transport."; }
		else if (!Done("battleground", uid)) { stage="08_battleground"; title="89,5 MHz - Der rote Sektor"; text="Der Kurierdatensatz fuehrt in einen gesicherten T-17-Sektor. Nutze den vorhandenen BattlegroundZ-CardReader, raeume das echte Event und sichere den Operationscode."; }
		else if (!Done("operation", uid)) { stage="09_operation"; title="89,5 MHz - Operation DeutschZ"; text="Reader und Operations-KeyCard passen zusammen. Das bestehende Operation-DeutschZ-Terminal ist der letzte Zugang zum Archiv."; }
		else { stage="10_finale"; title="VERBINDUNG VERLOREN"; text="Du hast die Akte gelesen. Morozov starb vor Transport Sieben. Also frag dich: Wer benutzt seitdem seine Kennung? Und wer glaubst du... spricht gerade mit dir?"; }
		if (Heard(stage, uid)) return;
		string audioId = DZRMZ_AudioService.StoryId(stage);
		if (audioId != "")
		{
			if (!DZRMZ_AudioService.Play(player, settings, audioId)) return;
			DZRMZ_RadioService.SendToPlayer(settings, player, title, text);
		}
		else DZRMZ_RadioService.SendToPlayer(settings, player, title, text);
		MarkHeard(stage, uid);
	}

	protected static bool Done(string eventId, string uid) { return FileExist(ROOT + "/event_completions/" + eventId + "/" + uid + ".done"); }
	protected static bool Heard(string stage, string uid) { return FileExist(ROOT + "/story_heard/" + uid + "/" + stage + ".heard"); }
	protected static void MarkHeard(string stage, string uid)
	{
		MakeDirectory(ROOT); MakeDirectory(ROOT + "/story_heard"); MakeDirectory(ROOT + "/story_heard/" + uid);
		FileHandle file = OpenFile(ROOT + "/story_heard/" + uid + "/" + stage + ".heard", FileMode.WRITE); if (file != 0) { FPrintln(file, "story_schema=1"); CloseFile(file); }
	}
}

