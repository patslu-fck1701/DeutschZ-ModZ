class DZCRZ_RPC
{
	static void ReceiveProgress(ParamsReadContext ctx)
	{
		bool visible;
		float progress;
		float duration;
		string label;
		if (!ctx.Read(visible) || !ctx.Read(progress) || !ctx.Read(duration) || !ctx.Read(label))
			return;
		DZCRZ_ClientProgressState.SetProgress(visible, progress, duration, label);
	}

	static void ReceiveMessage(ParamsReadContext ctx)
	{
		string message;
		int durationMs;
		if (!ctx.Read(message) || !ctx.Read(durationMs))
			return;
		DZCRZ_ClientProgressState.ShowMessage(message, durationMs);
	}

	static void ReceiveEffect(ParamsReadContext ctx)
	{
		bool start;
		string effectId;
		string soundSet;
		vector position;
		bool loop;
		if (!ctx.Read(start) || !ctx.Read(effectId) || !ctx.Read(soundSet) || !ctx.Read(position) || !ctx.Read(loop))
			return;
		DZCRZ_ClientEffectQueue.Push(start, effectId, soundSet, position, loop);
	}
}
