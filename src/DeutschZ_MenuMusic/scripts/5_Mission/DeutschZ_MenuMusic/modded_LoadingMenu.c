modded class LoadingMenu
{
	override Widget Init()
	{
		Widget root = super.Init();
		if (!root)
			return root;

		ImageWidget background = ImageWidget.Cast(root.FindAnyWidget("ImageBackground"));
		if (background)
		{
			DZKOTHG_LoadingScreenStore.Ensure();
			background.LoadImageFile(0, DZKOTHG_LoadingScreenStore.GetRandomScreen());
		}

		Widget hintFrame = root.FindAnyWidget("hint_frame");
		if (hintFrame)
			hintFrame.Show(false);

		Widget hintFrameAlt = root.FindAnyWidget("hint_frame0");
		if (hintFrameAlt)
			hintFrameAlt.Show(false);

		TextWidget title = TextWidget.Cast(root.FindAnyWidget("TextWidget"));
		if (title)
		{
			title.SetPos(0, 0.005);
			title.SetText("DeutschZ Jokes");
		}

		return root;
	}
};
