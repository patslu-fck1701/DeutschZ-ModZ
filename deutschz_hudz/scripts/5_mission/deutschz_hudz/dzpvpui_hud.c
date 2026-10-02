modded class IngameHudVisibility
{
    Widget DZPVPUI_GetElementWidget(EHudElement element)
    {
        if (!m_ElementMap)
            return null;
        return m_ElementMap.Get(element);
    }
}

class DZPVPUI_HudController extends ScriptedWidgetEventHandler
{
    protected Widget m_Root;
    protected Widget m_CrosshairFrame;
    protected ImageWidget m_CrosshairShadow;
    protected ImageWidget m_CrosshairImage;
    protected TextWidget m_Restart;
    protected TextWidget m_Balance;
    protected TextWidget m_Group;
    protected TextWidget m_StatusHealth;
    protected TextWidget m_StatusBlood;
    protected TextWidget m_StatusEnergy;
    protected TextWidget m_StatusWater;
    protected TextWidget m_StatusStamina;
    protected ProgressBarWidget m_StaminaBar;
    protected TextWidget m_WeaponName;
    protected TextWidget m_WeaponAmmo;
    protected TextWidget m_WeaponMode;
    protected TextWidget m_WeaponZeroing;
    protected ref array<TextWidget> m_QuickSlots;
    protected TextWidget m_MusicText;
    protected Widget m_LeaderboardRoot;
    protected ImageWidget m_LeaderboardCrown;
    protected TextWidget m_LeaderboardWinnerText;
    protected TextWidget m_LeaderboardText;
    protected ref DZPVPUI_MusicPlayer m_Music;
    protected float m_UpdateElapsed;
    protected float m_SyncElapsed;
    protected float m_StatusSyncElapsed;
    protected bool m_LeaderboardVisible;
    protected bool m_F12Pressed;
    protected bool m_F12HoldActive;
    protected int m_F12PressedAt;
    protected int m_F12LastStep;
    protected bool m_CrosshairAdjustPressed;
    protected bool m_CrosshairAdjustHoldActive;
    protected bool m_CrosshairGrow = true;
    protected int m_CrosshairAdjustPressedAt;
    protected int m_CrosshairAdjustLastStep;
    protected bool m_MenuWasOpen;
    bool Init()
    {
        if (!GetGame() || !GetGame().GetWorkspace())
            return false;
        LoadClientPreferences();
        m_Root = GetGame().GetWorkspace().CreateWidgets(DZPVPUI_Constants.HUD_LAYOUT);
        m_LeaderboardRoot = GetGame().GetWorkspace().CreateWidgets(DZPVPUI_Constants.LEADERBOARD_LAYOUT);
        if (!m_Root || !m_LeaderboardRoot)
            return false;
        m_CrosshairFrame = m_Root.FindAnyWidget("dzpvpui_crosshair_frame");
        m_CrosshairShadow = ImageWidget.Cast(m_Root.FindAnyWidget("dzpvpui_crosshair_shadow"));
        m_CrosshairImage = ImageWidget.Cast(m_Root.FindAnyWidget("dzpvpui_crosshair_image"));
        m_Restart = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_restart"));
        m_Balance = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_balance"));
        m_Group = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_group"));
        m_StatusHealth = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_status_health"));
        m_StatusBlood = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_status_blood"));
        m_StatusEnergy = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_status_energy"));
        m_StatusWater = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_status_water"));
        m_StatusStamina = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_status_stamina"));
        m_StaminaBar = ProgressBarWidget.Cast(m_Root.FindAnyWidget("dzpvpui_stamina_bar"));
        m_WeaponName = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_weapon_name"));
        m_WeaponAmmo = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_weapon_ammo"));
        m_WeaponMode = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_weapon_mode"));
        m_WeaponZeroing = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_weapon_zeroing"));
        m_QuickSlots = new array<TextWidget>;
        for (int slot = 0; slot < 10; slot++)
            m_QuickSlots.Insert(TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_q" + slot.ToString())));
        m_MusicText = TextWidget.Cast(m_Root.FindAnyWidget("dzpvpui_music"));
        m_LeaderboardCrown = ImageWidget.Cast(m_LeaderboardRoot.FindAnyWidget("DZPVPUI_LeaderboardCrown"));
        m_LeaderboardWinnerText = TextWidget.Cast(m_LeaderboardRoot.FindAnyWidget("DZPVPUI_LeaderboardWinnerText"));
        m_LeaderboardText = TextWidget.Cast(m_LeaderboardRoot.FindAnyWidget("DZPVPUI_LeaderboardText"));
        if (!m_Restart || !m_Balance || !m_Group || !m_StatusHealth || !m_StatusBlood || !m_StatusEnergy || !m_StatusWater || !m_StatusStamina || !m_StaminaBar)
            return false;
        if (!m_WeaponName || !m_WeaponAmmo || !m_WeaponMode || !m_WeaponZeroing)
            return false;
        if (!m_CrosshairFrame || !m_CrosshairShadow || !m_CrosshairImage)
            return false;
        if (!m_MusicText)
            return false;
        if (!m_LeaderboardCrown || !m_LeaderboardWinnerText || !m_LeaderboardText)
            return false;
        m_LeaderboardRoot.Show(false);
        m_Music = new DZPVPUI_MusicPlayer;
        ApplyCrosshair();
        ApplyHudState();
        RequestServerData();
        return true;
    }

    void Destroy()
    {
        RestoreVanillaHud();
        SaveClientPreferences();
        if (m_Music)
            m_Music.Destroy();
        m_Music = null;
        if (m_LeaderboardRoot)
            delete m_LeaderboardRoot;
        if (m_Root)
            delete m_Root;
        m_LeaderboardRoot = null;
        m_Root = null;
    }

    void Update(float timeslice)
    {
        if (!m_Root || !GetGame())
            return;
        UpdateMenuState();
        HandleInput();
        m_SyncElapsed += timeslice;
        if (m_SyncElapsed >= 30.0)
        {
            m_SyncElapsed = 0;
            RequestServerData();
        }
        m_StatusSyncElapsed += timeslice;
        if (m_StatusSyncElapsed >= 1.0)
        {
            m_StatusSyncElapsed = 0;
            RequestStatusData();
        }
        m_UpdateElapsed += timeslice;
        if (m_UpdateElapsed < DZPVPUI_State.Settings.UpdateIntervalSeconds)
            return;
        m_UpdateElapsed = 0;
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        bool gameplayVisible = player && player.IsAlive() && !GetGame().GetUIManager().GetMenu();
        m_Root.Show(gameplayVisible && DZPVPUI_State.Preferences.HudVisible);
        if (gameplayVisible)
        {
            if (DZPVPUI_State.Preferences.HudVisible)
                KeepSharedHudLayerVisible();
            SetVanillaHudElementsVisible(!DZPVPUI_State.Preferences.HudVisible);
        }
        if (gameplayVisible && DZPVPUI_State.Preferences.HudVisible)
        {
            ApplyCrosshair();
            UpdateRestart();
            UpdateBalance();
            UpdateGroup(player);
            UpdateStatus(player);
            UpdateWeaponInHands(player);
            UpdateQuickbar(player);
            m_MusicText.SetText(m_Music.GetStatus());
        }
        m_LeaderboardWinnerText.SetText(DZPVPUI_State.LeaderboardWinnerText);
        m_LeaderboardCrown.Show(DZPVPUI_State.LeaderboardWinnerText != "");
        m_LeaderboardText.SetText(DZPVPUI_State.LeaderboardText);
    }

    protected void UpdateMenuState()
    {
        bool menuOpen = GetGame().GetUIManager().GetMenu() != null;
        if (m_MenuWasOpen && !menuOpen && m_Music)
            m_Music.RecoverAfterMenu();
        m_MenuWasOpen = menuOpen;
    }

    protected void HandleInput()
    {
        if (GetGame().GetUIManager().GetMenu())
            return;
        if (Pressed("UADZPVPUILeaderboard"))
        {
            m_LeaderboardVisible = !m_LeaderboardVisible;
            m_LeaderboardRoot.Show(m_LeaderboardVisible);
            if (m_LeaderboardVisible)
                RequestServerData();
        }
        if (Pressed("UADZPVPUIToggleHud"))
        {
            DZPVPUI_State.Preferences.HudVisible = !DZPVPUI_State.Preferences.HudVisible;
            ApplyHudState();
            SaveClientPreferences();
        }
        if (Pressed("UADZPVPUIToggleCrosshair"))
        {
            DZPVPUI_State.Preferences.CrosshairVisible = !DZPVPUI_State.Preferences.CrosshairVisible;
            ApplyCrosshair();
            SaveClientPreferences();
        }
        if (Pressed("UADZPVPUICrosshairSize"))
        {
            DZPVPUI_State.Preferences.CrosshairShape++;
            if (DZPVPUI_State.Preferences.CrosshairShape > 2)
                DZPVPUI_State.Preferences.CrosshairShape = 0;
            ApplyCrosshair();
            SaveClientPreferences();
        }
        HandleCrosshairAdjust();
        if (Pressed("UADZPVPUIMusicPlay"))
            m_Music.TogglePlayPause();
        if (Pressed("UADZPVPUIMusicNext"))
            m_Music.Next();
        HandleF12();
    }

    protected void HandleF12()
    {
        UAInput input = GetUApi().GetInputByName("UADZPVPUIMusicVolume");
        if (!input)
            return;
        int now = GetGame().GetTime();
        if (input.LocalPress())
        {
            m_F12Pressed = true;
            m_F12HoldActive = false;
            m_F12PressedAt = now;
            m_F12LastStep = now;
        }
        if (m_F12Pressed && input.LocalHold() && now - m_F12PressedAt >= DZPVPUI_State.Settings.F12HoldMilliseconds)
        {
            m_F12HoldActive = true;
            if (now - m_F12LastStep >= 100)
            {
                m_F12LastStep = now;
                m_Music.ChangeVolume(0.01);
            }
        }
        if (m_F12Pressed && input.LocalRelease())
        {
            if (!m_F12HoldActive)
                m_Music.ChangeVolume(-0.05);
            m_F12Pressed = false;
            m_F12HoldActive = false;
            SaveClientPreferences();
        }
    }

    protected void HandleCrosshairAdjust()
    {
        UAInput input = GetUApi().GetInputByName("UADZPVPUICrosshairColor");
        if (!input)
            return;

        int now = GetGame().GetTime();
        if (input.LocalPress())
        {
            m_CrosshairAdjustPressed = true;
            m_CrosshairAdjustHoldActive = false;
            m_CrosshairAdjustPressedAt = now;
            m_CrosshairAdjustLastStep = now;
        }

        if (m_CrosshairAdjustPressed && input.LocalHold() && now - m_CrosshairAdjustPressedAt >= 450)
        {
            m_CrosshairAdjustHoldActive = true;
            if (now - m_CrosshairAdjustLastStep >= 180)
            {
                m_CrosshairAdjustLastStep = now;
                if (m_CrosshairGrow)
                    DZPVPUI_State.Preferences.CrosshairSize++;
                else
                    DZPVPUI_State.Preferences.CrosshairSize--;

                if (DZPVPUI_State.Preferences.CrosshairSize >= 12)
                {
                    DZPVPUI_State.Preferences.CrosshairSize = 12;
                    m_CrosshairGrow = false;
                }
                else if (DZPVPUI_State.Preferences.CrosshairSize <= 0)
                {
                    DZPVPUI_State.Preferences.CrosshairSize = 0;
                    m_CrosshairGrow = true;
                }
                ApplyCrosshair();
            }
        }

        if (m_CrosshairAdjustPressed && input.LocalRelease())
        {
            if (!m_CrosshairAdjustHoldActive)
            {
                DZPVPUI_State.Preferences.CrosshairColor++;
                if (DZPVPUI_State.Preferences.CrosshairColor > 5)
                    DZPVPUI_State.Preferences.CrosshairColor = 0;
                ApplyCrosshair();
            }
            SaveClientPreferences();
            m_CrosshairAdjustPressed = false;
            m_CrosshairAdjustHoldActive = false;
        }
    }

    protected bool Pressed(string name)
    {
        UAInput input = GetUApi().GetInputByName(name);
        return input && input.LocalPress();
    }

    protected void UpdateGroup(PlayerBase player)
    {
        m_Group.SetText("SOLO");
#ifdef EXPANSIONMODGROUPS
        ExpansionPartyModule partyModule;
        if (!CF_Modules<ExpansionPartyModule>.Get(partyModule) || !partyModule.GetParty())
            return;
        ExpansionPartyData party = partyModule.GetParty();
        array<ref ExpansionPartyPlayerData> members = party.GetPlayers();
        if (!members)
            return;
        int online;
        int nearestDistance = 999999;
        string nearestName;
        array<string> cacheNames = new array<string>;
        array<int> cacheOnline = new array<int>;
        foreach (ExpansionPartyPlayerData member: members)
        {
            if (!member)
                continue;
            bool isOnline = PlayerBase.Expansion_IsOnline(member.GetID());
            cacheNames.Insert(member.GetName());
            if (isOnline)
                cacheOnline.Insert(1);
            else
                cacheOnline.Insert(0);
            if (isOnline)
                online++;
            if (!member.Player)
                continue;
            if (member.Player == player)
                continue;
            int distance = Math.Round(vector.Distance(player.GetPosition(), member.Player.GetPosition()));
            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestName = member.GetName();
            }
        }
        string groupName = party.GetPartyName();
        if (groupName == "")
            groupName = "SOLO";
        string groupText = groupName + "  " + online.ToString() + "/" + members.Count().ToString() + " ONLINE";
        if (nearestName != "")
            groupText += "  " + nearestName + " " + nearestDistance.ToString() + "M";
        m_Group.SetText(groupText);
        DZPVPUI_ClientDisplayCacheStore.SaveGroupSnapshot(groupName + "  " + online.ToString() + " / " + members.Count().ToString(), party.GetMoneyDeposited(), cacheNames, cacheOnline);
#endif
    }

    protected void UpdateStatus(PlayerBase player)
    {
        int stamina;
        if (player.GetStaminaHandler())
            stamina = Math.Round(Math.Clamp(player.GetStaminaHandler().GetSyncedStaminaNormalized(), 0.0, 1.0) * 100.0);
        m_StatusHealth.SetText("HP " + StatusValue(DZPVPUI_State.HealthPercent));
        m_StatusBlood.SetText("BLUT " + StatusValue(DZPVPUI_State.BloodPercent));
        m_StatusEnergy.SetText("ESSEN " + StatusValue(DZPVPUI_State.EnergyPercent));
        m_StatusWater.SetText("WASSER " + StatusValue(DZPVPUI_State.WaterPercent));
        m_StatusStamina.SetText("AUS " + stamina.ToString() + "%");
        m_StaminaBar.SetCurrent(stamina);
    }

    protected void UpdateWeaponInHands(PlayerBase player)
    {
        EntityAI hands = player.GetHumanInventory().GetEntityInHands();
        if (!hands)
        {
            m_WeaponName.SetText("KEINE WAFFE");
            m_WeaponAmmo.SetText("[--/--]");
            m_WeaponMode.SetText("-");
            m_WeaponZeroing.SetText("-- M");
            return;
        }

        m_WeaponName.SetText(hands.GetDisplayName());
        Weapon_Base weapon = Weapon_Base.Cast(hands);
        if (!weapon)
        {
            m_WeaponAmmo.SetText("IN HAND");
            m_WeaponMode.SetText("");
            m_WeaponZeroing.SetText("");
            return;
        }

        int muzzle = weapon.GetCurrentMuzzle();
        int chamber = 0;
        if (!weapon.IsChamberEmpty(muzzle) && !weapon.IsChamberFiredOut(muzzle))
            chamber = 1;
        int reserve = weapon.GetInternalMagazineCartridgeCount(muzzle);
        Magazine magazine = weapon.GetMagazine(muzzle);
        if (magazine)
            reserve = magazine.GetAmmoCount();

        m_WeaponAmmo.SetText("[" + chamber.ToString() + "/" + reserve.ToString() + "]");
        m_WeaponMode.SetText(player.GetWeaponManager().GetCurrentModeName());
        m_WeaponZeroing.SetText(weapon.GetCurrentZeroing(muzzle).ToString() + " M");
    }

    protected string StatusValue(int value)
    {
        if (value < 0)
            return "--%";
        return value.ToString() + "%";
    }

    protected int Percent(float value, float maximum)
    {
        if (maximum <= 0)
            return 0;
        return Math.Round(Math.Clamp(value / maximum, 0.0, 1.0) * 100.0);
    }

    protected void UpdateQuickbar(PlayerBase player)
    {
        EntityAI hands = player.GetHumanInventory().GetEntityInHands();
        int count = Math.Min(player.GetQuickBarSize(), m_QuickSlots.Count());
        for (int i = 0; i < m_QuickSlots.Count(); i++)
        {
            TextWidget widget = m_QuickSlots[i];
            if (!widget)
                continue;
            string key = (i + 1).ToString();
            if (i == 9)
                key = "0";
            EntityAI item;
            if (i < count)
                item = player.GetQuickBarEntity(i);
            if (item)
            {
                string name = item.GetDisplayName();
                if (name.Length() > 12)
                    name = name.Substring(0, 12);
                widget.SetText(key + "  " + name);
                if (item == hands)
                    widget.SetColor(ARGB(255, 180, 255, 35));
                else
                    widget.SetColor(ARGB(255, 50, 255, 70));
            }
            else
            {
                widget.SetText(key + "  --");
                widget.SetColor(ARGB(150, 50, 255, 70));
            }
        }
    }

    protected void UpdateRestart()
    {
        int remaining = DZPVPUI_State.RestartSeconds;
        if (remaining >= 0)
            remaining = Math.Max(0, remaining - ((GetGame().GetTime() - DZPVPUI_State.RestartSyncTime) / 1000));
        if (remaining < 0)
        {
            m_Restart.SetText("NAECHSTER RESTART IN --:-- STUNDEN");
            return;
        }
        int hours = remaining / 3600;
        int minutes = (remaining % 3600) / 60;
        m_Restart.SetText("NAECHSTER RESTART IN " + Two(hours) + ":" + Two(minutes) + " STUNDEN");
    }

    protected void UpdateBalance()
    {
        string privateBalance = "--";
        if (DZPVPUI_State.AccountBalance >= 0)
            privateBalance = DZPVPUI_State.AccountBalance.ToString();
        string groupBalance = "--";
#ifdef EXPANSIONMODGROUPS
        ExpansionPartyModule partyModule;
        if (CF_Modules<ExpansionPartyModule>.Get(partyModule) && partyModule.GetParty())
            groupBalance = partyModule.GetParty().GetMoneyDeposited().ToString();
#endif
        m_Balance.SetText("GRUPPE: " + groupBalance + "   PRIVAT: " + privateBalance);
    }

    protected string Two(int value)
    {
        if (value < 10)
            return "0" + value.ToString();
        return value.ToString();
    }

    protected void ApplyCrosshair()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        bool show = DZPVPUI_State.Preferences.CrosshairVisible && player && player.IsAlive();
        if (show && (player.IsInIronsights() || player.IsInOptics() || GetGame().GetUIManager().GetMenu()))
            show = false;
        m_CrosshairFrame.Show(show);
        if (!show)
            return;

        string crosshairPath = "deutschz_hudz/gui/icons/crosshair/crosshair_dot.paa";
        float shapeScale = 0.78;
        if (DZPVPUI_State.Preferences.CrosshairShape == 1)
        {
            crosshairPath = "deutschz_hudz/gui/icons/crosshair/crosshair_cross.paa";
            shapeScale = 1.0;
        }
        else if (DZPVPUI_State.Preferences.CrosshairShape == 2)
        {
            crosshairPath = "deutschz_hudz/gui/icons/crosshair/crosshair_holo.paa";
            shapeScale = 1.25;
        }

        m_CrosshairShadow.LoadImageFile(0, crosshairPath);
        m_CrosshairShadow.SetImage(0);
        m_CrosshairShadow.SetColor(ARGB(245, 0, 0, 0));
        m_CrosshairImage.LoadImageFile(0, crosshairPath);
        m_CrosshairImage.SetImage(0);

        float width = (0.0075 + (DZPVPUI_State.Preferences.CrosshairSize * 0.00075)) * shapeScale;
        float height = width * 1.78;
        m_CrosshairFrame.SetSize(width, height);
        m_CrosshairFrame.SetPos(0.5 - (width * 0.5), 0.5 - (height * 0.5));
        ref array<int> colors = {ARGB(255,255,18,10), ARGB(255,20,255,70), ARGB(255,20,135,255), ARGB(255,255,235,0), ARGB(255,255,20,230), ARGB(255,255,255,255)};
        m_CrosshairImage.SetColor(colors.Get(DZPVPUI_State.Preferences.CrosshairColor));
    }

    protected void ApplyHudState()
    {
        if (!DZPVPUI_State.Preferences.HudVisible)
        {
            RestoreVanillaHud();
            return;
        }
        KeepSharedHudLayerVisible();
        SetVanillaHudElementsVisible(false);
    }

    protected void KeepSharedHudLayerVisible()
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission || !mission.GetHud())
            return;
        mission.GetHud().ShowHudUI(true);
        // Expansion evaluates its own marker 2D/3D settings inside this shared HUD layer.
        // Keep the layer active, but never mutate marker visibility or marker dimensions here.
        // Keep DayZ's HUD context active: action prompts such as "F" depend on it.
        mission.GetHud().ShowHudPlayer(true);
        mission.GetHud().ShowQuickbarPlayer(false);
    }

    protected void SetVanillaHudElementsVisible(bool visible)
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission || !mission.GetHud())
            return;
        IngameHud ingameHud = IngameHud.Cast(mission.GetHud());
        if (!ingameHud || !ingameHud.GetHudVisibility())
            return;

        IngameHudVisibility hudVisibility = ingameHud.GetHudVisibility();
        // Presence/stance feed the native action-target and interaction display.
        // Only the player/in-hands block and quickbar are replaced by HUDZ.
        SetVanillaHudElementVisible(hudVisibility, EHudElement.LHUD_PRESENCE, true);
        SetVanillaHudElementVisible(hudVisibility, EHudElement.LHUD_STANCE, true);
        SetVanillaHudElementVisible(hudVisibility, EHudElement.LHUD_PLAYER, visible);
        SetVanillaHudElementVisible(hudVisibility, EHudElement.RHUD_BADGES, true);
        SetVanillaHudElementVisible(hudVisibility, EHudElement.RHUD_DIVIDER, true);
        SetVanillaHudElementVisible(hudVisibility, EHudElement.RHUD_NOTIFIERS, true);
    }

    protected void SetVanillaHudElementVisible(IngameHudVisibility hudVisibility, EHudElement element, bool visible)
    {
        Widget widget = hudVisibility.DZPVPUI_GetElementWidget(element);
        if (widget)
            widget.Show(visible);
    }

    protected void RestoreVanillaHud()
    {
        MissionGameplay mission = MissionGameplay.Cast(GetGame().GetMission());
        if (!mission || !mission.GetHud())
            return;
        mission.GetHud().ShowHudUI(true);
        mission.GetHud().ShowHudPlayer(true);
        mission.GetHud().ShowQuickbarPlayer(true);
        SetVanillaHudElementsVisible(true);
    }

    protected void RequestServerData()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
            player.DZPVPUI_RequestLeaderboard();
    }

    protected void RequestStatusData()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
            player.DZPVPUI_RequestStatus();
    }

    protected void LoadClientPreferences()
    {
        DZPVPUI_ClientDisplayCacheStore.Load();
        MakeDirectory("$profile:DeutschZ-System");
        MakeDirectory(DZPVPUI_Constants.PROFILE_DIR);
        string error;
        if (FileExist(DZPVPUI_Constants.CLIENT_PATH))
            JsonFileLoader<DZPVPUI_ClientPreferences>.LoadFile(DZPVPUI_Constants.CLIENT_PATH, DZPVPUI_State.Preferences, error);
        DZPVPUI_State.Preferences.Validate(DZPVPUI_State.Settings.MusicMaximumVolume);
    }

    protected void SaveClientPreferences()
    {
        string error;
        JsonFileLoader<DZPVPUI_ClientPreferences>.SaveFile(DZPVPUI_Constants.CLIENT_PATH, DZPVPUI_State.Preferences, error);
    }
}

modded class MissionGameplay
{
    protected ref DZPVPUI_HudController m_DZPVPUI_Hud;
    protected float m_DZPVPUI_InitDelay;

    override void OnMissionStart()
    {
        super.OnMissionStart();
        m_DZPVPUI_InitDelay = 1.0;
    }

    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        if (!m_DZPVPUI_Hud)
        {
            m_DZPVPUI_InitDelay -= timeslice;
            if (m_DZPVPUI_InitDelay <= 0 && GetGame().GetPlayer())
            {
                m_DZPVPUI_Hud = new DZPVPUI_HudController;
                if (!m_DZPVPUI_Hud.Init())
                    m_DZPVPUI_Hud = null;
            }
        }
        if (m_DZPVPUI_Hud)
            m_DZPVPUI_Hud.Update(timeslice);
    }

    override void OnMissionFinish()
    {
        if (m_DZPVPUI_Hud)
            m_DZPVPUI_Hud.Destroy();
        m_DZPVPUI_Hud = null;
        super.OnMissionFinish();
    }
}

modded class MissionServer
{
    override void OnMissionStart()
    {
        super.OnMissionStart();
        DZPVPUI_ServerRepository.Get();
    }

    override void OnMissionFinish()
    {
        DZPVPUI_ServerRepository.Reset();
        super.OnMissionFinish();
    }
}
