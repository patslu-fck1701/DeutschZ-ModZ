class DZToxicZAudio
{
    protected static AbstractWave s_Wave;
    static void Play(string soundSet)
    {
        SoundParams p=new SoundParams(soundSet); if (!p.IsValid()) return;
        SoundObjectBuilder b=new SoundObjectBuilder(p); SoundObject o=b.BuildSoundObject(); if (!o) return;
        o.SetKind(WaveKind.WAVEENVIRONMENT); s_Wave=GetGame().GetSoundScene().Play2D(o,b); if (s_Wave) { s_Wave.SetVolume(0.75); s_Wave.Play(); }
    }
}
