class DZRMZ_RewardDefinition
{
	string ClassName;
	int MinimumCount;
	int MaximumCount;
	float Chance;

	void DZRMZ_RewardDefinition()
	{
		ClassName = "";
		MinimumCount = 1;
		MaximumCount = 1;
		Chance = 1.0;
	}

	bool IsStructurallyValid()
	{
		return ClassName != "" && MinimumCount > 0 && MaximumCount >= MinimumCount && MaximumCount <= 10 && Chance > 0.0 && Chance <= 1.0;
	}
}

class DZRMZ_Settings
{
	int SchemaVersion;
	bool Enabled;
	string WorldName;
	bool DisableOnWorldMismatch;
	float RadioFrequencyMHz;
	float FrequencyToleranceMHz;
	bool RequirePoweredReceivingRadio;
	int FirstMissionDelaySeconds;
	int MissionCooldownMinSeconds;
	int MissionCooldownMaxSeconds;
	int MissionTimeoutSeconds;
	int BroadcastStepDelaySeconds;
	int ServerTickMilliseconds;
	int RuntimeSaveIntervalSeconds;
	int OwnerReleaseAfterDisconnectSeconds;
	float ObjectiveRadiusMeters;
	float DeliveryRadiusMeters;
	bool ConsumeDeliveryItems;
	int OwnerHintIntervalSeconds;
	int AnnouncementDisplaySeconds;
	int RecentMissionMemory;
	int RecentLocationMemory;
	bool SuccessRewardsEnabled;
	ref array<ref DZRMZ_RewardDefinition> SuccessRewards;
	bool HardlineRewardsEnabled;
	int SideMissionReputation;
	int EventMissionReputation;
	int EventGateReputation;
	int FinalOperationReputation;
	bool DebugEnabled;

	void DZRMZ_Settings()
	{
		SchemaVersion = 1;
		Enabled = true;
		WorldName = "chernarusplus";
		DisableOnWorldMismatch = true;
		RadioFrequencyMHz = 89.5;
		FrequencyToleranceMHz = 0.01;
		RequirePoweredReceivingRadio = true;
		FirstMissionDelaySeconds = 300;
		MissionCooldownMinSeconds = 1800;
		MissionCooldownMaxSeconds = 3600;
		// 90 Minuten geben auch bei einer weiten Anfahrt, Kampf und anschliessender
		// Sicherungsphase genug Luft. Bereits vorhandene kuerzere Konfigurationen
		// werden in Validate() mindestens auf 60 Minuten angehoben.
		MissionTimeoutSeconds = 5400;
		BroadcastStepDelaySeconds = 8;
		ServerTickMilliseconds = 5000;
		RuntimeSaveIntervalSeconds = 30;
		OwnerReleaseAfterDisconnectSeconds = 180;
		ObjectiveRadiusMeters = 40.0;
		DeliveryRadiusMeters = 25.0;
		ConsumeDeliveryItems = true;
		OwnerHintIntervalSeconds = 60;
		AnnouncementDisplaySeconds = 12;
		RecentMissionMemory = 10;
		RecentLocationMemory = 12;
		SuccessRewardsEnabled = false;
		SuccessRewards = new array<ref DZRMZ_RewardDefinition>;
		HardlineRewardsEnabled = true;
		SideMissionReputation = 5;
		EventMissionReputation = 15;
		EventGateReputation = 30;
		FinalOperationReputation = 150;
		DebugEnabled = false;
	}

	void Validate()
	{
		if (SchemaVersion < 1)
			SchemaVersion = 1;
		if (WorldName == "")
			WorldName = "chernarusplus";
		if (RadioFrequencyMHz < 1.0)
			RadioFrequencyMHz = 89.5;
		if (FrequencyToleranceMHz < 0.001)
			FrequencyToleranceMHz = 0.01;
		// Missionsfunk und Auftragsannahme bleiben immer an ein wirklich
		// eingeschaltetes, empfangendes Funkgeraet gebunden.
		RequirePoweredReceivingRadio = true;
		if (FirstMissionDelaySeconds < 10)
			FirstMissionDelaySeconds = 10;
		if (MissionCooldownMinSeconds < 60)
			MissionCooldownMinSeconds = 60;
		if (MissionCooldownMaxSeconds < MissionCooldownMinSeconds)
			MissionCooldownMaxSeconds = MissionCooldownMinSeconds;
		if (MissionTimeoutSeconds < 3600)
			MissionTimeoutSeconds = 3600;
		if (BroadcastStepDelaySeconds < 2)
			BroadcastStepDelaySeconds = 2;
		if (ServerTickMilliseconds < 1000)
			ServerTickMilliseconds = 1000;
		if (RuntimeSaveIntervalSeconds < 10)
			RuntimeSaveIntervalSeconds = 10;
		if (OwnerReleaseAfterDisconnectSeconds < 30)
			OwnerReleaseAfterDisconnectSeconds = 30;
		if (ObjectiveRadiusMeters < 5.0)
			ObjectiveRadiusMeters = 5.0;
		if (DeliveryRadiusMeters < 5.0)
			DeliveryRadiusMeters = 5.0;
		if (OwnerHintIntervalSeconds < 15)
			OwnerHintIntervalSeconds = 15;
		if (AnnouncementDisplaySeconds < 5)
			AnnouncementDisplaySeconds = 5;
		if (RecentMissionMemory < 0)
			RecentMissionMemory = 0;
		if (RecentLocationMemory < 0)
			RecentLocationMemory = 0;
		if (!SuccessRewards)
			SuccessRewards = new array<ref DZRMZ_RewardDefinition>;
		for (int i = SuccessRewards.Count() - 1; i >= 0; i--)
		{
			DZRMZ_RewardDefinition reward = SuccessRewards[i];
			if (!reward || !reward.IsStructurallyValid())
				SuccessRewards.RemoveOrdered(i);
		}
		if (SideMissionReputation < 0)
			SideMissionReputation = 0;
		if (EventMissionReputation < 0)
			EventMissionReputation = 0;
		if (EventGateReputation < EventMissionReputation)
			EventGateReputation = EventMissionReputation;
		if (FinalOperationReputation < EventGateReputation)
			FinalOperationReputation = EventGateReputation;
	}

	protected void AddDefaultReward(string className, int minimumCount, int maximumCount, float chance)
	{
		ref DZRMZ_RewardDefinition reward = new DZRMZ_RewardDefinition;
		reward.ClassName = className;
		reward.MinimumCount = minimumCount;
		reward.MaximumCount = maximumCount;
		reward.Chance = chance;
		SuccessRewards.Insert(reward);
	}
}
