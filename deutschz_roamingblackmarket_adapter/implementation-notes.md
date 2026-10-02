# DeutschZ Mobile Roaming Black Market

## Entwicklung

Diese Implementierung wurde eigenstaendig fuer DeutschZ entwickelt. Der Controller
verwendet eine eigene Architektur, eigene Zustandsdaten, eine yaw-basierte
Wegpunktlenkung und eigene Recovery- und Persistenzregeln.

## Routen

Produktivrouten werden bewusst nicht mitgeliefert. Der Server erzeugt beim ersten
Start den Ordner:

`$profile:DeutschZ-System/RoamingBlackMarket/Routes/`

Eine Route enthaelt `RouteID`, `Enabled`, `Loop`, `Waypoints` und `TradeStops`.
Jeder Handelsstopp verweist ueber `WaypointIndex` auf einen Fahrpunkt und besitzt
separate Park-, Ausrichtungs- und Haendlerpositionen.

## Sicherer Zustand ohne Route

Wenn keine gueltige Route vorhanden ist, werden weder Fahrzeug noch Haendler
erzeugt. Der Server protokolliert stattdessen `WAITING_FOR_ROUTE`.
