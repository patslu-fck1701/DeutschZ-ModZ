class DZWM_Region
{
 string id;
 vector anchor;
 float activity;
 float noise;
 float violence;
 float pressure;
 float supply = 50;
 float recovery = 100;
 int lastUTC;
 int lastStimulusUTC;
 int lastReactionUTC;
 bool alarm;
 int presenceSamples;
 int eventSamples;
 // Transient rate limit counters are reset on every simulation tick.
 [NonSerialized()]
 float tickUsed;
}
