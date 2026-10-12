class DZWM_Save
{
 int schemaVersion = 1;
 int revision;
 float regionSize;
 float originX;
 float originZ;
 ref array<ref DZWM_Region> regions = new array<ref DZWM_Region>;
 ref array<ref DZWM_Seen> seen = new array<ref DZWM_Seen>;
}
