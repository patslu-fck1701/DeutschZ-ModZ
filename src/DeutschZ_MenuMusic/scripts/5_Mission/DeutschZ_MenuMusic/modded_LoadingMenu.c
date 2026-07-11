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
			background.LoadImageFile(0, DZKOTHG_MenuContentStore.GetNextScreen(), true);
			background.SetImage(0);
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
			title.SetPos(0, 0.125);
			DZKOTHG_LoadingEntry entry = DZKOTHG_MenuContentStore.GetNextLoadingEntry();
			string titleText = "DEUTSCHZ";
			if (entry)
				titleText = "DEUTSCHZ " + entry.Category;
			title.SetText(titleText);
			title.SetColor(ARGB(255, 195, 0, 24));
		}

		return root;
	}
};
