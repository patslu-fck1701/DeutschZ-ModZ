class DZFuelZService
{
    static bool IsManagedPump(Object object)
    {
        if (!object || !object.IsKindOf("Land_FuelStation_Feed")) return false;
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        if (!settings || !settings.Stations) return false;
        foreach (DZFuelZStation station : settings.Stations)
        {
            if (station && vector.Distance(object.GetPosition(), station.BuildingPosition.ToVector()) <= settings.VehicleRange)
                return true;
        }
        return false;
    }

    static bool FillContainerForPayment(PlayerBase player, Object pump, ItemBase container)
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        if (!player || !pump || !container || !IsManagedPump(pump)) return false;
        if (vector.Distance(player.GetPosition(), pump.GetPosition()) > settings.InteractionDistance + 1.0) return false;
        if (!Liquid.CanFillContainer(container, LIQUID_GASOLINE))
        {
            Notify(player, "Dieser Behälter kann nicht mit Kraftstoff befüllt werden.");
            return false;
        }
        float missingUnits = container.GetQuantityMax() - container.GetQuantity();
        if (missingUnits <= 0.0)
        {
            Notify(player, "Der Kraftstoffbehälter ist bereits voll.");
            return false;
        }
        float liters = missingUnits / 1000.0;
        int cost = Math.Ceil(liters * DZFuelZLivePriceService.GetPricePerLiter());
        if (cost < 1) cost = 1;
        if (!TakeEuros(player, cost))
        {
            Notify(player, string.Format("Befüllung kostet %1 Euro. Nicht genug Bargeld dabei.", cost));
            return false;
        }
        Liquid.FillContainerEnviro(container, LIQUID_GASOLINE, missingUnits, false);
        container.SetSynchDirty();
        Notify(player, string.Format("Kraftstoffbehälter mit %1 Litern befüllt. Bezahlt: %2 Euro.", liters.ToString(), cost));
        return true;
    }

    static Object ResolveFuelNPC(ActionTarget target)
    {
        if (!target)
            return null;

        Object object = target.GetObject();
        if (object && object.IsKindOf("DZ_FuelZ_NPC"))
            return object;

        object = target.GetParent();
        if (object && object.IsKindOf("DZ_FuelZ_NPC"))
            return object;

        return null;
    }

    static bool IsFuelNPC(ActionTarget target)
    {
        return ResolveFuelNPC(target) != null;
    }

    static Car FindNearestCar(vector position, float radius)
    {
        array<Object> objects = new array<Object>;
        array<CargoBase> cargo = new array<CargoBase>;
        GetGame().GetObjectsAtPosition3D(position, radius, objects, cargo);

        Car closest;
        float closestDistance = radius + 1.0;
        foreach (Object object : objects)
        {
            Car car = Car.Cast(object);
            if (!car || car.IsDamageDestroyed())
                continue;

            float distance = vector.Distance(position, car.GetPosition());
            if (distance < closestDistance)
            {
                closest = car;
                closestDistance = distance;
            }
        }
        return closest;
    }

    static float GetMissingLiters(Car car)
    {
        if (!car)
            return 0;

        float capacity = car.GetFluidCapacity(CarFluid.FUEL);
        float fraction = car.GetFluidFraction(CarFluid.FUEL);
        return Math.Max(0.0, capacity * (1.0 - fraction));
    }

    protected static int GetCurrencyUnits(ItemBase item)
    {
        if (!item)
            return 0;

        if (!item.HasQuantity())
            return 1;

        return Math.Floor(item.GetQuantity());
    }

    static int GetEuroBalance(PlayerBase player)
    {
        if (!player)
            return 0;

        int balance = 0;
        array<EntityAI> inventory = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
        foreach (EntityAI entity : inventory)
        {
            ItemBase item = ItemBase.Cast(entity);
            if (!item || item == player)
                continue;

            int unitValue = DZFuelZSettingsService.Get().GetCurrencyValue(item.GetType());
            if (unitValue > 0)
                balance += GetCurrencyUnits(item) * unitValue;
        }
        return balance;
    }

    static bool TakeEuros(PlayerBase player, int amount)
    {
        if (!player || amount <= 0 || GetEuroBalance(player) < amount)
            return false;

        int remaining = amount;
        array<EntityAI> inventory = new array<EntityAI>;
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, inventory);
        foreach (EntityAI entity : inventory)
        {
            ItemBase item = ItemBase.Cast(entity);
            if (!item || item == player)
                continue;

            int unitValue = DZFuelZSettingsService.Get().GetCurrencyValue(item.GetType());
            if (unitValue <= 0)
                continue;

            int availableUnits = GetCurrencyUnits(item);
            int requiredUnits = (remaining + unitValue - 1) / unitValue;
            int usedUnits = Math.Min(availableUnits, requiredUnits);
            if (usedUnits <= 0)
                continue;

            remaining -= usedUnits * unitValue;
            if (usedUnits >= availableUnits)
                GetGame().ObjectDelete(item);
            else
            {
                item.AddQuantity(-usedUnits);
                item.SetSynchDirty();
            }

            if (remaining <= 0)
                return true;
        }

        return false;
    }

    static void Notify(PlayerBase player, string message)
    {
        if (player)
            player.MessageStatus("[FuelZ] " + message);
    }

    static string FormatEuro(float amount)
    {
        int cents = Math.Round(amount * 100.0);
        int euros = cents / 100;
        int remainder = cents % 100;
        string centText = remainder.ToString();
        if (remainder < 10)
            centText = "0" + centText;
        return euros.ToString() + "," + centText;
    }

    static bool RefuelForPayment(PlayerBase player, Object npc, int paymentValue)
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        if (!player || !npc || !npc.IsKindOf("DZ_FuelZ_NPC") || vector.Distance(player.GetPosition(), npc.GetPosition()) > settings.InteractionDistance + 0.5)
            return false;

        Car car = FindNearestCar(npc.GetPosition(), settings.VehicleRange);
        if (!car)
        {
            Notify(player, "Kein Fahrzeug im Tankstellenbereich.");
            return false;
        }

        float missing = GetMissingLiters(car);
        if (missing < 0.01)
        {
            Notify(player, "Das Fahrzeug ist bereits vollgetankt.");
            return false;
        }

        float pricePerLiter = DZFuelZLivePriceService.GetPricePerLiter();
        float affordable = paymentValue / pricePerLiter;
        float liters = Math.Min(missing, affordable);
        int cost = Math.Ceil(liters * pricePerLiter);
        if (liters <= 0 || !TakeEuros(player, cost))
        {
            Notify(player, "Der aktuelle Literpreis ist " + FormatEuro(pricePerLiter) + " Euro. Nicht genug ExpansionBanknoteEuro dabei.");
            return false;
        }

        car.Fill(CarFluid.FUEL, liters);
        Notify(player, string.Format("%1 Liter getankt. Bezahlt: %2 Euro.", liters.ToString(), cost));
        string playerId = "offline";
        if (player.GetIdentity()) playerId = player.GetIdentity().GetId();
        Print(string.Format("[FuelZ] PURCHASE player=%1 vehicle=%2 liters=%3 cost=%4", playerId, car.GetType(), liters, cost));
        return true;
    }

    static bool RefuelFull(PlayerBase player, Object npc)
    {
        DZFuelZSettings settings = DZFuelZSettingsService.Get();
        if (!player || !npc || !npc.IsKindOf("DZ_FuelZ_NPC") || vector.Distance(player.GetPosition(), npc.GetPosition()) > settings.InteractionDistance + 0.5)
            return false;

        Car car = FindNearestCar(npc.GetPosition(), settings.VehicleRange);
        if (!car)
        {
            Notify(player, "Kein Fahrzeug im Tankstellenbereich.");
            return false;
        }

        float liters = GetMissingLiters(car);
        if (liters < 0.01)
        {
            Notify(player, "Das Fahrzeug ist bereits vollgetankt.");
            return false;
        }

        int cost = Math.Ceil(liters * DZFuelZLivePriceService.GetPricePerLiter());
        if (!TakeEuros(player, cost))
        {
            Notify(player, string.Format("Volltanken kostet %1 Euro. Nicht genug ExpansionBanknoteEuro dabei.", cost));
            return false;
        }

        car.Fill(CarFluid.FUEL, liters);
        Notify(player, string.Format("Vollgetankt: %1 Liter fuer %2 Euro.", liters.ToString(), cost));
        string playerId = "offline";
        if (player.GetIdentity()) playerId = player.GetIdentity().GetId();
        Print(string.Format("[FuelZ] FULL_TANK player=%1 vehicle=%2 liters=%3 cost=%4", playerId, car.GetType(), liters, cost));
        return true;
    }
};
