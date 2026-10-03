class DZPoints_InfectedRule
{
	string Match;
	int Value;
}

class DZPoints_Settings
{
	int Version = 1;
	bool Enabled = true;
	bool TopGamesEnabled = false;
	int PollIntervalSeconds = 600;
	ref array<int> VoteRewards = {5,10,25};
	ref array<int> MonthlyBonuses = {75,50,25};
	int MaxSimultaneousPouchCargo = 200;
	ref array<ref DZPoints_InfectedRule> InfectedRules = new array<ref DZPoints_InfectedRule>;

	void DZPoints_Settings()
	{
		AddRule("Mummy",5);
		AddRule("Soldier",3); AddRule("usSoldier",3); AddRule("eastSoldier",3); AddRule("Patrol",3);
		AddRule("Policeman",2); AddRule("PoliceWoman",2); AddRule("Hunter",2); AddRule("Hiker",2); AddRule("Hermit",2);
		AddRule("Farmer",1); AddRule("MilkMaid",1);
		AddRule("Zmb",1);
	}
	void AddRule(string match, int value)
	{
		DZPoints_InfectedRule rule = new DZPoints_InfectedRule;
		rule.Match = match; rule.Value = value; InfectedRules.Insert(rule);
	}
}

class DZPoints_Secrets
{
	string TopGamesServerToken = "";
}

class DZPoints_GlobalState
{
	int Version = 1;
	string CurrentVoteMonthBerlin;
	string LastFinalizedVoteMonthBerlin;
	ref array<string> KnownPlayerIDs = new array<string>;
}

class DZPoints_Transaction
{
	string TransactionID;
	string Source;
	string TimestampUTC;
	int Amount;
	int BalanceAfter;
}

class DZPoints_Account
{
	int Version = 1;
	string PlayerID;
	int Balance;
	int LifetimeEarned;
	int RedeemedTokens;
	int VoteCredits;
	int MonthlyBonuses;
	int Purchases;
	int AdminCorrections;
	string LastVoteDayBerlin;
	int DailyVoteCount;
	string VoteMonthBerlin;
	int MonthlyVoteCount;
	string MonthlyBonusPaidFor;
	ref array<ref DZPoints_Transaction> Journal = new array<ref DZPoints_Transaction>;
	ref array<string> ProcessedTransactionIDs = new array<string>;
}

class DZPoints_TokenValues
{
	static int Get(string className)
	{
		if (className == "DeutschZ_Token_1") return 1;
		if (className == "DeutschZ_Token_2") return 2;
		if (className == "DeutschZ_Token_3") return 3;
		if (className == "DeutschZ_Token_5") return 5;
		if (className == "DeutschZ_Token_25") return 25;
		return 0;
	}
	static string ClassFor(int value)
	{
		if (value == 2) return "DeutschZ_Token_2";
		if (value == 3) return "DeutschZ_Token_3";
		if (value == 5) return "DeutschZ_Token_5";
		if (value == 25) return "DeutschZ_Token_25";
		return "DeutschZ_Token_1";
	}
}
