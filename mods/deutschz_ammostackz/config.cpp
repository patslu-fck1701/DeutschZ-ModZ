class CfgPatches
{
 class deutschz_ammostackz
 {
  units[]={}; weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Weapons_Ammunition"};
 };
};
// Verified against the test server's weapons_ammunition.pbo. Only loose 20-round stacks.
class CfgMagazines
{
 class Ammunition_Base;
 class Ammo_308Win: Ammunition_Base {count=80;};
 class Ammo_308WinTracer: Ammunition_Base {count=80;};
 class Ammo_556x45: Ammunition_Base {count=80;};
 class Ammo_556x45Tracer: Ammunition_Base {count=80;};
 class Ammo_762x54: Ammunition_Base {count=80;};
 class Ammo_762x54Tracer: Ammunition_Base {count=80;};
 class Ammo_762x39: Ammunition_Base {count=80;};
 class Ammo_762x39Tracer: Ammunition_Base {count=80;};
 class Ammo_9x39AP: Ammunition_Base {count=80;};
 class Ammo_9x39: Ammunition_Base {count=80;};
 class Ammo_357: Ammunition_Base {count=80;};
 class Ammo_545x39: Ammunition_Base {count=80;};
 class Ammo_545x39Tracer: Ammunition_Base {count=80;};
};
