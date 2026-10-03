class DZP_PropertyDoorController : Fence
{
    protected string m_DZP_PropertyId;
    protected bool m_DZP_LocalPinAuthorized;

    void ~DZP_PropertyDoorController()
    {
        if (g_Game) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DZP_HideAttachedCodeLock);
    }

    bool DZP_IsPinAuthorized(PlayerBase player)
    {
        if (!player || !player.GetIdentity() || IsRuined()) return false;
        CodeLock codeLock = CodeLock.Cast(GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock")));
        if (!codeLock || codeLock.IsRuined()) return false;
        if (!g_Game.IsServer()) return m_DZP_LocalPinAuthorized;
        string uid = player.GetIdentity().GetId();
        return codeLock.IsOwner(uid) || codeLock.IsGuest(uid);
    }

    void DZP_SetLocalPinAuthorized(bool authorized)
    {
        m_DZP_LocalPinAuthorized = authorized;
    }

    void DZP_UpdateControllerVisual()
    {
        if (!GetInventory()) return;
        // Only the in-hand variant is the controller body. The attached
        // variant in this same P3D is a second, rotated model.
        SetAnimationPhase("Combination_Lock_Item", 0);
        SetAnimationPhase("Lock_Item_1", 0);
        SetAnimationPhase("Lock_Item_2", 0);
        SetAnimationPhase("Combination_Lock_Attached", 1);
        SetAnimationPhase("Lock_Attached_1", 1);
        SetAnimationPhase("Lock_Attached_2", 1);
        EntityAI lockItem = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock"));
        if (lockItem && lockItem.IsKindOf("CodeLock") && !lockItem.IsRuined())
        {
            // Hide the placeholder. The real attached CodeLock is aligned to
            // this controller and supplies the visible model.
            SetAnimationPhase("Combination_Lock_Item", 1);
            SetAnimationPhase("Lock_Item_1", 1);
            SetAnimationPhase("Lock_Item_2", 1);
        }
        else
        {
            SetObjectTexture(0, "#(argb,8,8,3)color(0.65,0.8,0.9,0.22,ca)");
            SetObjectMaterial(0, "deutschz_propertyz/Data/controller_empty.rvmat");
        }
    }

    protected int m_DZP_OwnerHash;
    protected int m_DZP_OwnerPartyId = -1;

    void DZP_PropertyDoorController()
    {
        RegisterNetSyncVariableInt("m_DZP_OwnerHash");
        RegisterNetSyncVariableInt("m_DZP_OwnerPartyId", -1);
        DZP_PrepareAsGate();
        DZP_RefreshPersistence();
    }

    override void EEInit()
    {
        super.EEInit();
        DZP_PrepareAsGate();
        DZP_RefreshPersistence();
        DZP_HideAttachedCodeLock();
        if (g_Game) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZP_HideAttachedCodeLock, 250, false);
    }

    override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
        DZP_HideAttachedCodeLock();
        if (g_Game) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZP_HideAttachedCodeLock, 250, false);
    }

    override void AfterStoreLoad()
    {
        super.AfterStoreLoad();
        DZP_PrepareAsGate();
        DZP_HideAttachedCodeLock();
    }

    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item, slot_name);
        if (slot_name == "Att_CombinationLock" && item && item.IsKindOf("CodeLock"))
        {
            item.SetLifetimeMax(3888000);
            item.SetLifetime(3888000);
            item.SetInvisible(false);
            item.OnInvisibleSet(false);
            item.SetScale(1.0);
            if (g_Game) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZP_HideAttachedCodeLock, 250, false);
        }
    }

    void DZP_RefreshPersistence()
    {
        if (!g_Game || !g_Game.IsServer()) return;
        SetLifetimeMax(3888000);
        SetLifetime(3888000);
        if (!GetInventory()) return;
        EntityAI lockEntity = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock"));
        if (!lockEntity || !lockEntity.IsKindOf("CodeLock")) return;
        lockEntity.SetLifetimeMax(3888000);
        lockEntity.SetLifetime(3888000);
    }

    override void EEItemDetached(EntityAI item, string slot_name)
    {
        if (slot_name == "Att_CombinationLock" && item)
        {
            item.SetInvisible(false);
            item.OnInvisibleSet(false);
            item.SetScale(1.0);
            if (g_Game) g_Game.GameScript.CallFunction(item, "UpdateVisuals", null, null);
        }
        super.EEItemDetached(item, slot_name);
        m_DZP_LocalPinAuthorized = false;
        DZP_UpdateControllerVisual();
        if (g_Game) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DZP_HideAttachedCodeLock, 250, false);
    }

    private void DZP_HideAttachedCodeLock()
    {
        if (!GetInventory()) return;
        DZP_UpdateControllerVisual();
        EntityAI item = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock"));
        if (!item || !item.IsKindOf("CodeLock")) return;
        item.SetInvisible(false);
        item.OnInvisibleSet(false);
        item.SetScale(1.0);
        item.SetPosition(GetPosition());
        item.SetOrientation(GetOrientation());
        item.SetAnimationPhase("Combination_Lock_Item", 0);
        item.SetAnimationPhase("Lock_Item_1", 0);
        item.SetAnimationPhase("Lock_Item_2", 0);
        item.SetAnimationPhase("Combination_Lock_Attached", 1);
        item.SetAnimationPhase("Lock_Attached_1", 1);
        item.SetAnimationPhase("Lock_Attached_2", 1);
    }

    override bool CanPutInCargo(EntityAI parent)
    {
        return false;
    }

    void DZP_PrepareAsGate()
    {
        SetBaseState(true);
        SetGateState(GATE_STATE_FULL);
        SetOpenedState(false);
        DZP_UpdateControllerVisual();
    }

    override bool CanPutIntoHands(EntityAI parent)
    {
        return false;
    }

    override bool CanReceiveAttachment(EntityAI attachment, int slotId)
    {
        if (!attachment || attachment.GetType() != "CodeLock") return false;
        int codeLockSlot = InventorySlots.GetSlotIdFromString("Att_CombinationLock");
        if (slotId != codeLockSlot || IsRuined()) return false;
        if (g_Game && g_Game.IsServer())
        {
            PlayerBase attachingPlayer = PlayerBase.Cast(attachment.GetHierarchyRootPlayer());
            if (!attachingPlayer || !attachingPlayer.GetIdentity() || attachingPlayer.GetIdentity().GetId().Hash() != m_DZP_OwnerHash)
                return false;
        }
        return GetInventory().FindAttachment(codeLockSlot) == null;
    }

    override bool CanDisplayAttachmentSlot(int slot_id)
    {
        if (slot_id == InventorySlots.GetSlotIdFromString("Att_CombinationLock"))
            return true;
        return false;
    }

    override bool CanDisplayCargo()
    {
        if (!g_Game || g_Game.IsServer()) return true;

        PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
        if (!player || !player.GetIdentity()) return false;

        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(GetPosition(), 12.0, objects, cargos);
        BuildingBase closest;
        float closestDistance = 999999.0;
        foreach (Object object : objects)
        {
            BuildingBase building = BuildingBase.Cast(object);
            if (!building || !building.DZPIsSupported()) continue;
            float distance = vector.DistanceSq(GetPosition(), building.GetPosition());
            if (distance < closestDistance)
            {
                closest = building;
                closestDistance = distance;
            }
        }
        if (!closest) return false;

        EntityAI lockEntity = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock"));
        bool isLockOwner;
        bool authorized = player.GetIdentity().GetId().Hash() == m_DZP_OwnerHash;
        if (lockEntity && lockEntity.IsKindOf("CodeLock"))
        {
            Param1<string> ownerCheck = new Param1<string>(player.GetIdentity().GetId());
            int ownerCall = g_Game.GameScript.CallFunctionParams(lockEntity, "IsOwner", isLockOwner, ownerCheck);
            if (ownerCall != 0 && isLockOwner) authorized = true;
        }
        if (!authorized && m_DZP_OwnerPartyId >= 0)
            authorized = player.Expansion_GetPartyID() == m_DZP_OwnerPartyId;
        if (!authorized)
            authorized = DZPOwnedPropertyState.Owns(closest.GetType(), closest.GetPosition());
        if (!authorized) return false;
        return DZPPropertyControllerService.Get().IsPlayerInside(closest, player, -1);
    }

    override bool IsPlayerInside(PlayerBase player, string selection)
    {
        if (!g_Game || g_Game.IsServer())
            return super.IsPlayerInside(player, selection);
        return CanDisplayCargo();
    }

    override bool CanReceiveItemIntoCargo(EntityAI item)
    {
        // A CodeLock must always use the attachment slot. If it enters the
        // large house cargo, it has no authoritative PIN/lock state and the
        // property would be unlocked by the next synchronization pass.
        if (item && item.IsKindOf("CodeLock")) return false;
        return super.CanReceiveItemIntoCargo(item);
    }

    override bool CanReleaseCargo(EntityAI cargo)
    {
        return super.CanReleaseCargo(cargo);
    }

    void DZP_SetPropertyId(string propertyId)
    {
        m_DZP_PropertyId = propertyId;
    }

    void DZP_SetOwnerPartyId(int partyId)
    {
        if (m_DZP_OwnerPartyId == partyId) return;
        m_DZP_OwnerPartyId = partyId;
        SetSynchDirty();
    }

    void DZP_SetOwnerId(string ownerId)
    {
        int ownerHash;
        if (ownerId != string.Empty) ownerHash = ownerId.Hash();
        if (m_DZP_OwnerHash == ownerHash) return;
        m_DZP_OwnerHash = ownerHash;
        SetSynchDirty();
    }

    string DZP_GetPropertyId()
    {
        return m_DZP_PropertyId;
    }

    bool DZP_DropRuinedCodeLock()
    {
        if (!g_Game || !g_Game.IsServer() || !GetInventory()) return false;
        int codeLockSlot = InventorySlots.GetSlotIdFromString("Att_CombinationLock");
        EntityAI lockEntity = GetInventory().FindAttachment(codeLockSlot);
        if (!lockEntity || lockEntity.GetType() != "CodeLock") return true;

        GetInventory().SetSlotLock(codeLockSlot, false);
        if (!GetInventory().DropEntity(InventoryMode.SERVER, this, lockEntity)) return false;

        lockEntity.SetInvisible(false);
        lockEntity.OnInvisibleSet(false);
        lockEntity.SetScale(1.0);
        lockEntity.SetHealth("", "Health", 0.0);
        vector dropPosition = GetPosition() + "0.35 0 0.35";
        lockEntity.SetPosition(dropPosition);
        lockEntity.PlaceOnSurface();
        g_Game.GameScript.CallFunction(lockEntity, "UpdateVisuals", null, null);
        return true;
    }

    bool DZP_GetBreachingTransform(string chargeType, out vector position, out vector orientation)
    {
        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(GetPosition(), 12.0, objects, cargos);
        BuildingBase closest;
        float closestDistance = 999999.0;
        foreach (Object object : objects)
        {
            BuildingBase candidate = BuildingBase.Cast(object);
            if (!candidate || !candidate.DZPIsSupported()) continue;
            float distance = vector.DistanceSq(GetPosition(), candidate.GetPosition());
            if (distance < closestDistance)
            {
                closest = candidate;
                closestDistance = distance;
            }
        }
        if (!closest) return false;

        DZPHouseTypeSetting setting = DZPSettingsService.Find(closest.GetType());
        if (!setting) return false;
        bool second = setting.HasSecondControllerReference && vector.DistanceSq(GetPosition(), closest.ModelToWorld(setting.SecondControllerOffset)) < vector.DistanceSq(GetPosition(), closest.ModelToWorld(setting.ControllerOffset));
        float yawOffset;
        vector offset;
        if (chargeType == "HDSN_BreachingChargeHeavy")
        {
            if (second) { offset = setting.SecondBreachingChargeHeavyOffset; yawOffset = setting.SecondBreachingChargeHeavyYawOffset; }
            else { offset = setting.BreachingChargeHeavyOffset; yawOffset = setting.BreachingChargeHeavyYawOffset; }
        }
        else
        {
            if (second) { offset = setting.SecondBreachingChargeOffset; yawOffset = setting.SecondBreachingChargeYawOffset; }
            else { offset = setting.BreachingChargeOffset; yawOffset = setting.BreachingChargeYawOffset; }
        }
        if (offset == vector.Zero) return false;
        position = closest.ModelToWorld(offset);
        orientation = closest.GetOrientation();
        orientation[0] = orientation[0] + yawOffset;
        return true;
    }

    bool DZP_HasConfiguredLockedCodeLock()
    {
        if (IsRuined()) return false;
        EntityAI lockEntity = GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("Att_CombinationLock"));
        if (!lockEntity || lockEntity.GetType() != "CodeLock") return false;

        bool locked;
        string passcode;
        int stateCall = g_Game.GameScript.CallFunction(lockEntity, "GetLockState", locked, null);
        int codeCall = g_Game.GameScript.CallFunction(lockEntity, "GetPasscode", passcode, null);
        return stateCall != 0 && codeCall != 0 && locked && passcode != string.Empty;
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(m_DZP_PropertyId);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version)) return false;
        if (!ctx.Read(m_DZP_PropertyId)) m_DZP_PropertyId = string.Empty;
        return true;
    }
}

class DZPPropertyControllerService
{
    private static ref DZPPropertyControllerService s_Instance;
    private ref map<string, DZP_PropertyDoorController> m_Controllers = new map<string, DZP_PropertyDoorController>;

    static DZPPropertyControllerService Get()
    {
        if (!s_Instance) s_Instance = new DZPPropertyControllerService();
        return s_Instance;
    }

    void DZPPropertyControllerService()
    {
        // Give the Hive enough time to restore persisted controllers,
        // attachments and cargo before EnsureIndex is allowed to recover a
        // genuinely missing controller. Starting this after only three
        // seconds could create an empty duplicate and unlock the house.
        if (g_Game && g_Game.IsServer()) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BeginPersistenceSync, 30000, false);
    }

    void ~DZPPropertyControllerService()
    {
        if (!g_Game) return;
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(BeginPersistenceSync);
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Tick);
    }

    private void BeginPersistenceSync()
    {
        Tick();
        if (g_Game && g_Game.IsServer()) g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, 3000, true);
    }

    private void Tick()
    {
        array<ref DZPPropertyRecord> records = DZPPropertyStore.Get().GetAll();
        foreach (DZPPropertyRecord record : records)
        {
            if (!record) continue;
            if (record.ControllerLayoutVersion < 2)
                MigrateControllerLayout(record);
            PlayerBase owner = PlayerBase.GetPlayerByUID(record.OwnerId);
            if (owner && record.OwnerPartyId != owner.Expansion_GetPartyID())
            {
                record.OwnerPartyId = owner.Expansion_GetPartyID();
                DZPPropertyStore.Get().Save();
            }
            array<Object> objects = new array<Object>;
            array<CargoBase> cargos = new array<CargoBase>;
            g_Game.GetObjectsAtPosition3D(record.Position, 3.0, objects, cargos);
            foreach (Object object : objects)
            {
                BuildingBase building = BuildingBase.Cast(object);
                if (building && building.GetType() == record.Type)
                {
                    Synchronize(record, building);
                    break;
                }
            }
        }
    }

    private void DeleteControllersAt(vector position)
    {
        if (position == vector.Zero) return;
        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(position, 2.0, objects, cargos);
        foreach (Object object : objects)
        {
            DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(object);
            if (controller) g_Game.ObjectDelete(controller);
        }
    }

    private void MigrateControllerLayout(DZPPropertyRecord record)
    {
        DeleteControllersAt(record.ControllerPosition);
        DeleteControllersAt(record.SecondControllerPosition);
        m_Controllers.Remove(record.PropertyId);
        m_Controllers.Remove(record.PropertyId + "#2");
        record.ControllerPosition = vector.Zero;
        record.SecondControllerPosition = vector.Zero;
        record.Raided = false;

        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(record.Position, 3.0, objects, cargos);
        foreach (Object object : objects)
        {
            BuildingBase building = BuildingBase.Cast(object);
            if (building && building.GetType() == record.Type)
            {
                PlaceAtConfiguredEntrance(record, building);
                break;
            }
        }
        record.ControllerLayoutVersion = 2;
        DZPPropertyStore.Get().Save();
        Print(string.Format("[DeutschZ PropertyZ] Controller layout migrated property=%1", record.PropertyId));
    }

    void OnBreached(DZP_PropertyDoorController breachedController)
    {
        if (!breachedController || !g_Game.IsServer()) return;
        foreach (DZPPropertyRecord record : DZPPropertyStore.Get().GetAll())
        {
            if (!record || record.PropertyId != breachedController.DZP_GetPropertyId()) continue;
            if (!breachedController.DZP_DropRuinedCodeLock())
            {
                Print(string.Format("[DeutschZ PropertyZ] Raid aborted: CodeLock could not be dropped property=%1", record.PropertyId));
                return;
            }
            record.Raided = true;
            record.LockedDoors.Clear();
            array<Object> objects = new array<Object>;
            array<CargoBase> cargos = new array<CargoBase>;
            g_Game.GetObjectsAtPosition3D(record.Position, 3.0, objects, cargos);
            foreach (Object object : objects)
            {
                BuildingBase building = BuildingBase.Cast(object);
                if (!building || building.GetType() != record.Type) continue;
                for (int door = 0; door < building.GetDoorCount(); door++) building.UnlockDoor(door);
            }
            DZP_PropertyDoorController other;
            if (m_Controllers.Find(record.PropertyId, other) && other != breachedController)
            {
                other.DZP_DropRuinedCodeLock();
                g_Game.ObjectDelete(other);
            }
            if (m_Controllers.Find(record.PropertyId + "#2", other) && other != breachedController)
            {
                other.DZP_DropRuinedCodeLock();
                g_Game.ObjectDelete(other);
            }
            m_Controllers.Remove(record.PropertyId);
            m_Controllers.Remove(record.PropertyId + "#2");
            DZPPropertyStore.Get().Save();
            Print(string.Format("[DeutschZ PropertyZ] Property breached property=%1", record.PropertyId));
            return;
        }
    }

    DZP_PropertyDoorController Ensure(DZPPropertyRecord record, PlayerBase player = null)
    {
        return EnsureIndex(record, 0, player);
    }

    private string ControllerKey(DZPPropertyRecord record, int index)
    {
        if (index == 0) return record.PropertyId;
        return record.PropertyId + "#2";
    }

    private vector StoredPosition(DZPPropertyRecord record, int index)
    {
        if (index == 0) return record.ControllerPosition;
        return record.SecondControllerPosition;
    }

    private void SetStoredPosition(DZPPropertyRecord record, int index, vector position)
    {
        if (index == 0) record.ControllerPosition = position;
        else record.SecondControllerPosition = position;
    }

    private DZP_PropertyDoorController EnsureIndex(DZPPropertyRecord record, int index, PlayerBase player = null)
    {
        if (!record) return null;
        string key = ControllerKey(record, index);
        DZP_PropertyDoorController known;
        if (m_Controllers.Find(key, known) && known)
        {
            known.DZP_RefreshPersistence();
            known.DZP_SetOwnerId(record.OwnerId);
            known.DZP_SetOwnerPartyId(record.OwnerPartyId);
            return known;
        }

        vector searchPosition = StoredPosition(record, index);
        if (index > 0 && searchPosition == vector.Zero) return null;
        if (searchPosition == vector.Zero) searchPosition = record.Position;
        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(searchPosition, 2.0, objects, cargos);
        foreach (Object object : objects)
        {
            DZP_PropertyDoorController existing = DZP_PropertyDoorController.Cast(object);
            if (existing && (existing.DZP_GetPropertyId() == record.PropertyId || existing.DZP_GetPropertyId() == string.Empty))
            {
                existing.DZP_SetPropertyId(record.PropertyId);
                existing.DZP_RefreshPersistence();
                existing.DZP_SetOwnerId(record.OwnerId);
                existing.DZP_SetOwnerPartyId(record.OwnerPartyId);
                m_Controllers.Set(key, existing);
                return existing;
            }
        }

        vector spawnPosition = StoredPosition(record, index);
        if (spawnPosition == vector.Zero && player && index == 0)
        {
            spawnPosition = player.GetPosition() + (player.GetDirection() * 0.8);
            spawnPosition[1] = g_Game.SurfaceY(spawnPosition[0], spawnPosition[2]) + 0.15;
            SetStoredPosition(record, index, spawnPosition);
            DZPPropertyStore.Get().Save();
        }
        if (spawnPosition == vector.Zero) return null;

        DZP_PropertyDoorController created = DZP_PropertyDoorController.Cast(g_Game.CreateObjectEx("DZP_PropertyDoorController", spawnPosition, ECE_PLACE_ON_SURFACE));
        if (!created) return null;
        created.SetPosition(spawnPosition);
        created.SetOrientation("0 0 0");
        created.DZP_SetPropertyId(record.PropertyId);
        created.DZP_SetOwnerId(record.OwnerId);
        created.DZP_SetOwnerPartyId(record.OwnerPartyId);
        created.DZP_PrepareAsGate();
        created.DZP_RefreshPersistence();
        m_Controllers.Set(key, created);
        return created;
    }

    DZP_PropertyDoorController PlaceAtPlayer(DZPPropertyRecord record, PlayerBase player)
    {
        if (!record || !player) return null;
        DZP_PropertyDoorController controller = Ensure(record, player);
        if (!controller) return null;

        vector position = player.GetPosition() + (player.GetDirection() * 1.2);
        position[1] = g_Game.SurfaceY(position[0], position[2]) + 1.15;
        vector orientation = player.GetOrientation();
        orientation[0] = orientation[0] + 180.0;
        controller.SetPosition(position);
        controller.SetOrientation(orientation);
        controller.DZP_PrepareAsGate();
        record.ControllerPosition = position;
        DZPPropertyStore.Get().Save();
        Print(string.Format("[DeutschZ PropertyZ] Controller repositioned property=%1 position=%2", record.PropertyId, position));
        return controller;
    }

    DZP_PropertyDoorController PlaceAtConfiguredEntrance(DZPPropertyRecord record, BuildingBase building)
    {
        if (!record || !building) return null;
        DZPHouseTypeSetting setting = DZPSettingsService.Find(record.Type);
        if (!setting || !setting.HasControllerReference) return null;

        vector position = building.ModelToWorld(setting.ControllerOffset);
        record.ControllerPosition = position;
        DZP_PropertyDoorController controller = EnsureIndex(record, 0);
        if (!controller) return null;

        vector orientation = building.GetOrientation();
        orientation[0] = orientation[0] + setting.ControllerYawOffset;
        controller.SetPosition(position);
        controller.SetOrientation(orientation);
        controller.DZP_PrepareAsGate();

        if (setting.HasSecondControllerReference)
        {
            vector secondPosition = building.ModelToWorld(setting.SecondControllerOffset);
            record.SecondControllerPosition = secondPosition;
            DZP_PropertyDoorController secondController = EnsureIndex(record, 1);
            if (!secondController) return null;
            vector secondOrientation = building.GetOrientation();
            secondOrientation[0] = secondOrientation[0] + setting.SecondControllerYawOffset;
            secondController.SetPosition(secondPosition);
            secondController.SetOrientation(secondOrientation);
            secondController.DZP_PrepareAsGate();
        }
        DZPPropertyStore.Get().Save();
        Print(string.Format("[DeutschZ PropertyZ] Controller placed at configured entrance property=%1 position=%2", record.PropertyId, position));
        return controller;
    }

    void Remove(DZPPropertyRecord record)
    {
        if (!record) return;
        DZP_PropertyDoorController controller;
        if (m_Controllers.Find(record.PropertyId, controller) && controller) g_Game.ObjectDelete(controller);
        m_Controllers.Remove(record.PropertyId);
        if (m_Controllers.Find(record.PropertyId + "#2", controller) && controller) g_Game.ObjectDelete(controller);
        m_Controllers.Remove(record.PropertyId + "#2");
    }

    bool IsCargoEmpty(DZPPropertyRecord record)
    {
        DZP_PropertyDoorController controller = Ensure(record);
        if (!controller || !controller.GetInventory() || !controller.GetInventory().GetCargo()) return true;
        if (controller.GetInventory().GetCargo().GetItemCount() != 0) return false;
        DZPHouseTypeSetting setting = DZPSettingsService.Find(record.Type);
        if (setting && setting.HasSecondControllerReference)
        {
            DZP_PropertyDoorController secondController = EnsureIndex(record, 1);
            if (secondController && secondController.GetInventory() && secondController.GetInventory().GetCargo())
                return secondController.GetInventory().GetCargo().GetItemCount() == 0;
        }
        return true;
    }

    bool HasConfiguredLocks(DZPPropertyRecord record)
    {
        if (!record) return false;
        DZP_PropertyDoorController controller = EnsureIndex(record, 0);
        if (!controller || !controller.DZP_HasConfiguredLockedCodeLock() || controller.IsOpened()) return false;
        DZPHouseTypeSetting setting = DZPSettingsService.Find(record.Type);
        if (setting && setting.HasSecondControllerReference)
        {
            DZP_PropertyDoorController secondController = EnsureIndex(record, 1);
            if (!secondController || !secondController.DZP_HasConfiguredLockedCodeLock() || secondController.IsOpened()) return false;
        }
        return true;
    }

    void Synchronize(DZPPropertyRecord record, BuildingBase building, PlayerBase player = null)
    {
        if (!record || !building) return;
        if (record.Raided)
        {
            record.LockedDoors.Clear();
            for (int openDoor = 0; openDoor < building.GetDoorCount(); openDoor++) building.UnlockDoor(openDoor);
            return;
        }
        DZP_PropertyDoorController controller = Ensure(record, player);
        if (!controller) return;
        bool lockDoors = HasConfiguredLocks(record);
        bool changed = (record.LockedDoors.Count() > 0) != lockDoors;
        record.LockedDoors.Clear();
        for (int door = 0; door < building.GetDoorCount(); door++)
        {
            if (lockDoors)
            {
                building.LockDoor(door);
                record.LockedDoors.Insert(door);
            }
            else building.UnlockDoor(door);
        }
        if (changed) DZPPropertyStore.Get().Save();
    }

    bool IsPlayerInside(BuildingBase building, PlayerBase player, int doorIndex)
    {
        if (!building || !player) return false;
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        vector entrance;
        vector secondEntrance;
        if (record)
        {
            entrance = record.ControllerPosition;
            secondEntrance = record.SecondControllerPosition;
        }
        else
        {
            DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
            if (!setting || !setting.HasControllerReference) return false;
            entrance = building.ModelToWorld(setting.ControllerOffset);
            if (setting.HasSecondControllerReference) secondEntrance = building.ModelToWorld(setting.SecondControllerOffset);
        }
        if (secondEntrance != vector.Zero && vector.DistanceSq(player.GetPosition(), secondEntrance) < vector.DistanceSq(player.GetPosition(), entrance))
            entrance = secondEntrance;
        if (entrance == vector.Zero) return false;

        vector center = building.GetPosition();
        vector inward = center - entrance;
        vector fromEntrance = player.GetPosition() - entrance;
        inward[1] = 0;
        fromEntrance[1] = 0;
        return vector.Dot(inward, fromEntrance) > 0;
    }

    bool CanOpenPropertyDoor(BuildingBase building, PlayerBase player, int doorIndex)
    {
        if (!building || !player || doorIndex < 0) return false;
        if (IsPlayerInside(building, player, doorIndex)) return true;
        DZPHouseTypeSetting setting = DZPSettingsService.Find(building.GetType());
        if (!setting || !setting.HasControllerReference) return false;
        vector entrance = building.ModelToWorld(setting.ControllerOffset);
        DZPPropertyRecord record;
        if (g_Game.IsServer()) record = DZPPropertyStore.Get().Find(building);
        if (record && record.ControllerPosition != vector.Zero) entrance = record.ControllerPosition;
        if (setting.HasSecondControllerReference)
        {
            vector second = building.ModelToWorld(setting.SecondControllerOffset);
            if (record && record.SecondControllerPosition != vector.Zero) second = record.SecondControllerPosition;
            if (vector.DistanceSq(player.GetPosition(), second) < vector.DistanceSq(player.GetPosition(), entrance)) entrance = second;
        }
        array<Object> objects = new array<Object>;
        array<CargoBase> cargos = new array<CargoBase>;
        g_Game.GetObjectsAtPosition3D(entrance, 0.5, objects, cargos);
        foreach (Object object : objects)
        {
            DZP_PropertyDoorController controller = DZP_PropertyDoorController.Cast(object);
            if (!controller) continue;
            if (record && controller.DZP_GetPropertyId() != record.PropertyId) continue;
            if (controller.DZP_IsPinAuthorized(player)) return true;
        }
        return false;
    }

    void OpenInsideExit(BuildingBase building, PlayerBase player, int doorIndex)
    {
        if (!building || !player || !CanOpenPropertyDoor(building, player, doorIndex)) return;
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        if (!record || !HasConfiguredLocks(record)) return;
        building.UnlockDoor(doorIndex);
        if (building.CanDoorBeOpened(doorIndex, true)) building.OpenDoor(doorIndex);
        int delaySeconds = DZPSettingsService.Get().InsideExitRelockSeconds;
        if (delaySeconds < 1) delaySeconds = 10;
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AutoCloseInsideExit, delaySeconds * 1000, false, building, doorIndex);
    }

    private void AutoCloseInsideExit(BuildingBase building, int doorIndex)
    {
        if (!building) return;
        DZPPropertyRecord record = DZPPropertyStore.Get().Find(building);
        if (!record || !HasConfiguredLocks(record)) return;
        if (building.CanDoorBeClosed(doorIndex)) building.CloseDoor(doorIndex);
        building.LockDoor(doorIndex);
    }
}
