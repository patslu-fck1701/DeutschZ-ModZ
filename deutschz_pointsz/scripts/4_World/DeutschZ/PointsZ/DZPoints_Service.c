class DZPoints_Service
{
	static const string ROOT = "$profile:DeutschZ-System/PointsZ";
	static const string ACCOUNTS = ROOT + "/Accounts";
	protected static ref DZPoints_Settings s_Settings;
	protected static ref DZPoints_Secrets s_Secrets;
	protected static ref DZPoints_GlobalState s_State;

	static void Initialize()
	{
		if (s_Settings) return;
		MakeDirectory("$profile:DeutschZ-System"); MakeDirectory(ROOT); MakeDirectory(ACCOUNTS);
		s_Settings = new DZPoints_Settings;
		string error;
		if (FileExist(ROOT + "/settings.json")) JsonFileLoader<DZPoints_Settings>.LoadFile(ROOT + "/settings.json",s_Settings,error);
		else JsonFileLoader<DZPoints_Settings>.SaveFile(ROOT + "/settings.json",s_Settings,error);
		if (error != "") Print("[DeutschZ PointsZ] SETTINGS_ERROR " + error);
		s_Secrets = new DZPoints_Secrets;
		error = "";
		if (FileExist(ROOT + "/secrets.json")) JsonFileLoader<DZPoints_Secrets>.LoadFile(ROOT + "/secrets.json",s_Secrets,error);
		if (error != "") Print("[DeutschZ PointsZ] SECRETS_ERROR");
		s_State = new DZPoints_GlobalState;
		error = "";
		if (FileExist(ROOT + "/state.json")) JsonFileLoader<DZPoints_GlobalState>.LoadFile(ROOT + "/state.json",s_State,error);
		if (error != "") Print("[DeutschZ PointsZ] STATE_ERROR " + error);
	}

	static DZPoints_Settings Settings()
	{
		Initialize();
		return s_Settings;
	}

	static string TopGamesToken()
	{
		Initialize();
		if (!s_Secrets) return "";
		return s_Secrets.TopGamesServerToken;
	}

	static int InfectedValue(string className)
	{
		Initialize();
		if (!s_Settings || !s_Settings.Enabled) return 0;
		foreach (DZPoints_InfectedRule rule : s_Settings.InfectedRules)
			if (rule && className.Contains(rule.Match)) return rule.Value;
		return 0;
	}

	static bool Credit(string playerID, int amount, string source, string transactionID)
	{
		if (playerID == "" || amount == 0 || transactionID == "") return false;
		DZPoints_Account account = Load(playerID);
		RegisterPlayer(playerID);
		if (account.ProcessedTransactionIDs.Find(transactionID) >= 0) return false;
		account.Balance += amount;
		if (amount > 0) account.LifetimeEarned += amount;
		if (source == "TOKEN") account.RedeemedTokens += amount;
		else if (source == "VOTE") account.VoteCredits += amount;
		else if (source == "MONTHLY") account.MonthlyBonuses += amount;
		else if (source == "PURCHASE") account.Purchases += Math.AbsInt(amount);
		else if (source == "ADMIN") account.AdminCorrections += amount;
		DZPoints_Transaction tx = new DZPoints_Transaction;
		tx.TransactionID = transactionID; tx.Source = source; tx.Amount = amount; tx.BalanceAfter = account.Balance;
		tx.TimestampUTC = ExpansionStatic.GetTimestamp(true).ToString();
		account.Journal.Insert(tx); account.ProcessedTransactionIDs.Insert(transactionID);
		return Save(account);
	}

	static bool CreditVote(string playerID)
	{
		if (playerID == "") return false;
		Initialize();
		DZPoints_Account account = Load(playerID);
		RegisterPlayer(playerID);
		int year, month, day;
		GetYearMonthDay(year,month,day);
		string dayKey = year.ToString() + "-" + month.ToStringLen(2) + "-" + day.ToStringLen(2);
		string monthKey = year.ToString() + "-" + month.ToStringLen(2);
		EnsureVoteMonth(monthKey);
		if (account.LastVoteDayBerlin != dayKey)
		{
			account.LastVoteDayBerlin = dayKey;
			account.DailyVoteCount = 0;
		}
		if (account.VoteMonthBerlin != monthKey)
		{
			account.VoteMonthBerlin = monthKey;
			account.MonthlyVoteCount = 0;
		}
		account.DailyVoteCount++;
		account.MonthlyVoteCount++;
		int amount = 0;
		if (account.DailyVoteCount <= s_Settings.VoteRewards.Count()) amount = s_Settings.VoteRewards[account.DailyVoteCount - 1];
		string transactionID = "TOPGAMES-" + playerID + "-" + dayKey + "-" + account.DailyVoteCount.ToString() + "-" + ExpansionStatic.GetTimestamp(true).ToString();
		if (amount > 0)
		{
			account.Balance += amount;
			account.LifetimeEarned += amount;
			account.VoteCredits += amount;
		}
		DZPoints_Transaction tx = new DZPoints_Transaction;
		tx.TransactionID = transactionID;
		tx.Source = "VOTE";
		tx.Amount = amount;
		tx.BalanceAfter = account.Balance;
		tx.TimestampUTC = ExpansionStatic.GetTimestamp(true).ToString();
		account.Journal.Insert(tx);
		account.ProcessedTransactionIDs.Insert(transactionID);
		if (!Save(account)) return false;
		Print("[DeutschZ PointsZ] TOPGAMES_CLAIM_OK player=" + playerID + " dailyVote=" + account.DailyVoteCount.ToString() + " monthlyVotes=" + account.MonthlyVoteCount.ToString() + " points=" + amount.ToString());
		return true;
	}

	protected static void RegisterPlayer(string playerID)
	{
		if (!s_State || playerID == "" || s_State.KnownPlayerIDs.Find(playerID) >= 0) return;
		s_State.KnownPlayerIDs.Insert(playerID);
		SaveState();
	}

	protected static void EnsureVoteMonth(string monthKey)
	{
		if (!s_State) return;
		if (s_State.CurrentVoteMonthBerlin == "")
		{
			s_State.CurrentVoteMonthBerlin = monthKey;
			SaveState();
			return;
		}
		if (s_State.CurrentVoteMonthBerlin == monthKey) return;
		FinalizeMonth(s_State.CurrentVoteMonthBerlin);
		s_State.LastFinalizedVoteMonthBerlin = s_State.CurrentVoteMonthBerlin;
		s_State.CurrentVoteMonthBerlin = monthKey;
		SaveState();
	}

	protected static void FinalizeMonth(string monthKey)
	{
		if (monthKey == "" || !s_State || s_State.LastFinalizedVoteMonthBerlin == monthKey) return;
		ref array<ref DZPoints_Account> ranking = new array<ref DZPoints_Account>;
		foreach (string playerID : s_State.KnownPlayerIDs)
		{
			DZPoints_Account account = Load(playerID);
			if (account.VoteMonthBerlin == monthKey && account.MonthlyVoteCount > 0) ranking.Insert(account);
		}
		for (int i = 0; i < ranking.Count(); i++)
		{
			for (int j = i + 1; j < ranking.Count(); j++)
			{
				if (ranking[j].MonthlyVoteCount > ranking[i].MonthlyVoteCount)
				{
					DZPoints_Account swap = ranking[i];
					ranking[i] = ranking[j];
					ranking[j] = swap;
				}
			}
		}
		int count = Math.Min(ranking.Count(),s_Settings.MonthlyBonuses.Count());
		for (int rank = 0; rank < count; rank++)
		{
			DZPoints_Account winner = ranking[rank];
			if (winner.MonthlyBonusPaidFor == monthKey) continue;
			int bonus = s_Settings.MonthlyBonuses[rank];
			winner.Balance += bonus;
			winner.LifetimeEarned += bonus;
			winner.MonthlyBonuses += bonus;
			winner.MonthlyBonusPaidFor = monthKey;
			DZPoints_Transaction tx = new DZPoints_Transaction;
			tx.TransactionID = "MONTHLY-" + monthKey + "-" + (rank + 1).ToString() + "-" + winner.PlayerID;
			tx.Source = "MONTHLY";
			tx.Amount = bonus;
			tx.BalanceAfter = winner.Balance;
			tx.TimestampUTC = ExpansionStatic.GetTimestamp(true).ToString();
			winner.Journal.Insert(tx);
			winner.ProcessedTransactionIDs.Insert(tx.TransactionID);
			Save(winner);
			Print("[DeutschZ PointsZ] MONTHLY_BONUS_OK month=" + monthKey + " rank=" + (rank + 1).ToString() + " player=" + winner.PlayerID + " votes=" + winner.MonthlyVoteCount.ToString() + " points=" + bonus.ToString());
		}
	}

	protected static void SaveState()
	{
		string error;
		if (!JsonFileLoader<DZPoints_GlobalState>.SaveFile(ROOT + "/state.json",s_State,error)) Print("[DeutschZ PointsZ] STATE_SAVE_ERROR " + error);
	}

	static DZPoints_Account Load(string playerID)
	{
		Initialize(); DZPoints_Account account = new DZPoints_Account; account.PlayerID = playerID;
		string error; string path = ACCOUNTS + "/" + playerID + ".json";
		if (FileExist(path) && !JsonFileLoader<DZPoints_Account>.LoadFile(path,account,error)) Print("[DeutschZ PointsZ] ACCOUNT_LOAD_ERROR player=" + playerID + " error=" + error);
		return account;
	}

	static bool Save(DZPoints_Account account)
	{
		string error; bool ok = JsonFileLoader<DZPoints_Account>.SaveFile(ACCOUNTS + "/" + account.PlayerID + ".json",account,error);
		if (!ok) Print("[DeutschZ PointsZ] ACCOUNT_SAVE_ERROR player=" + account.PlayerID + " error=" + error);
		return ok;
	}
}

modded class ZombieBase
{
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (!GetGame() || !GetGame().IsServer()) return;
		int value = DZPoints_Service.InfectedValue(GetType());
		if (value <= 0 || value == 25) return;
		string tokenClass = DZPoints_TokenValues.ClassFor(value);
		EntityAI token = GetInventory().CreateInInventory(tokenClass);
		if (!token) token = EntityAI.Cast(GetGame().CreateObjectEx(tokenClass,GetPosition(),ECE_PLACE_ON_SURFACE));
		if (token) Print("[DeutschZ PointsZ] TOKEN_DROPPED infected=" + GetType() + " token=" + tokenClass + " value=" + value.ToString());
		else Print("[DeutschZ PointsZ] TOKEN_DROP_FAILED infected=" + GetType() + " token=" + tokenClass);
	}
}
