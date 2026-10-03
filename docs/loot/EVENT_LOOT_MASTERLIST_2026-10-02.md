# DeutschZ – Event-Loot Masterliste

Stand: 03.10.2026  
Verbindliche IST-Basis: `Server_Stand_02.10.2026_20_00_Uhr.zip`

Geprüfte Primärdateien:
- `DeutschZ_Morty_MapLoot_types.xml`
- `DeutschZ_WeaponPool_types.xml`
- `DeutschZ_Anzio20mm_types.xml`
- `FOG_Types.xml`
- `DeutschZ_KotHZ/Config/KotHZLoot.json`
- `AIConvoy/Vehicles/Truck01.json`
- `AIConvoy/Vehicles/M1025.json`

## Bewertungsregel
`nominal=0` ist in der verbindlichen DeutschZ-Konfiguration **Event-/Special-Loot**: diese Klassen sollen nicht regulär über die Map-/Central-Economy spawnen, sondern ausschließlich über Events/Spezialausgaben in Umlauf kommen. Für die konkrete Zuordnung zu RAVEN, KOTH, AIConvoyZ usw. werden zusätzlich Seltenheit, Waffenrolle, vorhandene Eventverwendung und Balance herangezogen. KOTH ist die Balance-Referenz. Gute Eventkisten kombinieren EVENT/SPECIAL mit passenden HYBRID- und MAP/CE-Ergänzungen.

## EVENT / SPECIAL
| Klasse | CE / Beleg | Verwendung / Rolle |
|---|---|---|
| TTC_Beowulf | nominal 0, min 0, Military T4 | KOTH + beide AIConvoy-Fahrzeuge; Assault/Heavy-Hit |
| TTC_GEVAR43 (+ Green/silver/Tan) | nominal 0, Military T4 | KOTH + beide AIConvoy-Fahrzeuge; DMR/Battle Rifle |
| TTC_TAVOR | nominal 0, Military T4 | KOTH + beide AIConvoy-Fahrzeuge; Assault |
| TTC_TAVOR_DMR | nominal 0, Military T4 | KOTH; Recon/Sniper |
| TTC_ModDMR* 5.56/7.62 | nominal 0 | Recon/KOTH/Convoy-Boss/Battleground-Kandidaten |
| TTC_AKMod* / TTC_AKModPK* | nominal 0, min 0 | AIConvoy/RAVEN Assault/Battleground; Gold seltene Prestigevariante |
| Anzio20mm | direkt in KOTH, ca. 3 % | SPECIAL/ENDGAME/extrem selten |

Beowulf-Zubehör: `TTC_50Beo_mag`, `TTC_Ammo_50Beo`, `TTC_AmmoBox_50Beo_20rnd`.  
GEVAR43-Magazin: `TTC_GEVAR43_Magazine_10rnd`.  
TAVOR: `TTC_TAVOR_Magazine_30rnd`, `TTC_TAVOR_Magazine_60rnd`.

ModDMR nominal-0:
`TTC_ModDMRTan_556`, `TTC_ModDMRTwotone_556`, `TTC_ModDMRCamo_556`, `TTC_ModDMRDigiTan_556`, `TTC_ModDMRUPC_556`, `TTC_ModDMRSnow_556`, `TTC_ModDMR_762`, `TTC_ModDMRTan_762`, `TTC_ModDMRTwotone_762`, `TTC_ModDMRCamo_762`, `TTC_ModDMRDigiTan_762`, `TTC_ModDMRUPC_762`, `TTC_ModDMRSnow_762`.

AKMod nominal-0:
`TTC_AKMod`, `TTC_AKMod_Gold`, `TTC_AKMod_Snow`, `TTC_AKMod_Multi`, `TTC_AKMod_Digi`, `TTC_AKMod_Tan`, `TTC_AKModPK`, `TTC_AKModPK_Gold`, `TTC_AKModPK_Snow`, `TTC_AKModPK_Multi`, `TTC_AKModPK_Digi`, `TTC_AKModPK_Tan`.

Anzio-Zubehör im geprüften CE/KOTH: `Ammo_Anzio20mm` nominal 8, `Ammo_Anzio20mmTracer_Green` nominal 4, `Anzio_AmmoBox_20rnd` nominal 2, KOTH referenziert `Mag_Anzio20mm_5Rnd`. Die Waffe selbst ist in KOTH nachgewiesen, nicht in der separat geprüften Anzio-types.xml.

## HYBRID / HIGH EVENT
- `TTC_AWM` — nominal 2/min 1/Military T4; KOTH + beide Convoy-Fahrzeuge; `TTC_AWM_Magazine_5rnd`.
- `TTC_HK417` — nominal 3/Military T4; KOTH + beide Convoy-Fahrzeuge; `TTC_HK417_Magazine_20rnd`.
- `TTC_M24_NEW_Black` — nominal 4/min 1/Hunting T3/4; KOTH + beide Convoy-Fahrzeuge; `TTC_M24New_mag_5rnd`.
- `TTC_M110_Black` — nominal 3/ContaminatedArea; KOTH.
- `TTC_SCARHBlack` — nominal 4/ContaminatedArea; KOTH; `TTC_SCARHMag`.

Weitere Recon/DMR-Hybride: `TTC_M24`, `TTC_M24_Bjorn`, `TTC_M24_NEW_Camo`, `TTC_M24_NEW_CamoW`, `TTC_M24_NEW_Wood`, `TTC_M24_Woodland`, `TTC_M24_WoodlandOld`, `TTC_R700`, `TTC_R700_Black`, `TTC_M14`, `TTC_M14_Camo`, `TTC_M14_Snow`, `TTC_M1A_Black`, `TTC_M1A_Green`, `TTC_M1A_Snow`, `TTC_M1A_Tan`, `TTC_M4DMR`, `TTC_M4DMRDesert`, `TTC_M4DMROD`, `TTC_M4DMRSNOW`. Die letzten drei sind nominal 0/ContaminatedArea und besonders für RAVEN Recon/Toxic/Rify/Battleground interessant.

Assault-Hybride: `TTC_HK416Black` 4, `TTC_HK416OD` 4, `TTC_HK416Tan` 3, `TTC_HK416Comp` 3, `TTC_MK18` 4, `TTC_MK18_Black` 3, `TTC_SCARL` 4, `TTC_SCARLBlack` 3, `TTC_SCARLSnow` 0, `TTC_SCARH` 4, `TTC_SCARHBlack` 4, `TTC_AKM` 7, `TTC_KAC` 9, `TTC_FAL` 9, `TTC_MCX_300blk` 4, `TTC_M4Tac` 4, `TTC_M4Tac_Black` 4, `TTC_CAR15` 4, `TTC_Car15V2` 9.

## MAP / CE – hochwertiger Füllloot
Military/Assault: `TTC_AEK973` 25, `TTC_M16` 25, `TTC_SG550` 25, `TTC_SG552` 25, `TTC_SG552_Black` 25, `TTC_VHS` 22, `TTC_L85` 17, `TTC_M16A4` 83.

SMG: `TTC_UZI` 33, `TTC_MP7A1` 33, `TTC_MP5SD` 73, `TTC_PP91` 83. KOTH nutzt u. a. UZI und PP91.

Pistolen: `TTC_P320` 42, `TTC_Glock17` 66, `TTC_M9_Custom` 33, `TTC_M9` 99, `TTC_Deagle` 42; `TTC_Deagle_Gold` 9 = Hybrid/Prestige.

Jagd/Historisch: `TTC_MAS36` 63, `TTC_kar98k` 63, `TTC_LeeEnfield` 22, `TTC_M1Garand` 42, `TTC_AVS36` 22, `TTC_M1903` 22, `TTC_SVT40` 42, `TTC_STG44` 42, `TTC_Winchester1873` 124, `TTC_Mossberg` 42.

## FOG – Event Gear
FOG umfasst im geprüften Stand über 1.400 CE-Einträge, mehrere hundert nominal 0. Auch hier gilt für DeutschZ: **nominal 0 = Event-/Special-Loot, kein regulärer Mapspawn**. Welcher konkrete Eventpool die jeweilige FOG-Klasse erhält, wird nach Rolle, Stil und Balance entschieden.

Bereits durch KOTH gestützte Hauptgruppen:
- Helm: `FOG_Helmet_Airframe_OD` + passende Airframe-/Helmvarianten.
- NVG: `FOG_ANVIS9` + Alternativen; HIGH EVENT/HYBRID, nicht garantiert in jeder Kiste.
- Tactical Clothing: `FOG_AEM01_Jacket_Black` + Alternativen; u. a. `FOG_Crye_G3_Shirt_*`, `FOG_Crye_G2_Shirt_*`, `FOG_Shirt_FRACU_Top_*`.
- Rucksack: `FOG_Bag_ArcteryxLEAF_Black` + Varianten.
- Headset: `FOG_AMP_Headset_Black` + Varianten.
- Westen/Plate Carrier: `FOG_Vest_Plateframe_*`, `FOG_Vest_JPC2_*`, `FOG_Vest_JPC_*`, `FOG_Vest_CPC_*`, `FOG_Vest_LBT6094_*`, `FOG_Vest_TacTec_*`, `FOG_Vest_MMAC_*`, `FOG_Vest_TV110T_*`, `FOG_Vest_FCPC_*`, `FOG_Vest_LV119_*`, `FOG_Vest_LVMBAV_*`, `FOG_Vest_Thor_*`, `FOG_Vest_Osprey_*`, `FOG_Vest_Gen4_*`.

Für ernsthafte Military-Pools bevorzugt Black, OD/RG, MC, MCB, M81, CB/Tan; Pink/White/Funvarianten nicht blind übernehmen.

## Kein eigenständiger High-End-Jackpot
Normale Pistolen, häufige SMGs, historische Standardgewehre, gewöhnliche Map-Shotguns, einzelne Magazine/Stocks/Handguards, gewöhnliche FOG-Shirts/kosmetische Varianten oder normale Munition allein. Sie sind Ergänzung, nicht der erkennbare Eventmehrwert.

## RAVEN Pool-Basis
### RECON
Hauptkandidaten: `TTC_TAVOR_DMR`, `TTC_GEVAR43`, `TTC_AWM`, `TTC_M110_Black`, `TTC_M24_NEW_Black`, `TTC_M4DMRDesert`, `TTC_M4DMROD`, `TTC_M4DMRSNOW`, ModDMR-Varianten. Dazu kompatible Magazine/Munition, Optik, Suppressor, FOG Camo/Ghillie, geringe NVG-Chance, Rucksack, Medizin.

### ASSAULT
Hauptkandidaten: `TTC_Beowulf`, `TTC_TAVOR`, `TTC_HK417`, `TTC_SCARHBlack`, `TTC_SCARLBlack`, `TTC_HK416Black`, `TTC_MK18_Black`, `TTC_MCX_300blk`, AKMod-/AKModPK-Specials. Dazu Magazine/Munition, Optik, FOG Plate Carrier/Helm/Tactical Clothing/Headset und ggf. NVG.

### NBC / SURVIVAL
Kein garantierter Super-Sniper. Wertkern: kompletter ABC/NBC-Satz, Maske, Filter, Medizin, Survival und ggf. FOG-Rucksack. Geeignete Sekundär-/Normalwaffen: `TTC_Glock17`, `TTC_P320`, `TTC_M9_Custom`, `TTC_UZI`, `TTC_MP7A1`, `TTC_MP5SD`, `TTC_PP91`.

## Globale Eventregel
Eine ausgewogene hochwertige Eventkiste enthält typischerweise:
1. eine seltene bzw. klar wertvolle Hauptkomponente,
2. kompatible Magazine und Munition,
3. 1–2 sinnvolle Attachments,
4. FOG-Ausrüstung,
5. Medizin/Utility,
6. einige hochwertige MAP/CE-Ergänzungen.

RAVEN nutzt daraus thematische Recon-, Assault- und NBC-Pools. AIConvoy verwendet nachweislich bereits Teile derselben TTC-/FOG-Basis. Anzio bleibt extrem seltenes SPECIAL/ENDGAME-Loot.
