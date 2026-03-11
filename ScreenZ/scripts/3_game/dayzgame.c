/** @file dayzgame.c */

modded class LoadingScreen
{
  // Loading screen images
  private const ref array<string> m_LoadScreenImages={
    "{066E89212856E322}ScreenZ/gui/textures/loading_screens/loading_screen_1_co.edds",
    "{0870CD6915A1ECF8}ScreenZ/gui/textures/loading_screens/loading_screen_2_co.edds",
    "{0D850EAEFEF3164E}ScreenZ/gui/textures/loading_screens/loading_screen_3_co.edds",
    "{144C45F96E4FF34C}ScreenZ/gui/textures/loading_screens/loading_screen_4_co.edds"
  };
  // Last loading screen displayed
  private int m_LastLoadScreen = -1;

  void LoadingScreen(DayZGame game)
  {
    m_ImageBackground.LoadMaskTexture("{EAF8A716BB515212}ScreenZ/gui/textures/loading_screens/loading_screen_mask.edds");

    m_LoadScreenImages.ShuffleArray();
    int imgcnt = m_LoadScreenImages.Count();
    for (int i = 0; i < imgcnt; i++) m_ImageBackground.LoadImageFile(i, m_LoadScreenImages[i]);

    SetLoadScreenImage();

    m_ModdedWarning.Show(false);
    m_ImageLogoMid.Show(false);
    m_ImageLogoCorner.Show(true);
    m_ImageWidgetBackground.Show(true);
    m_Counter=0;

    // lighten up your desktop
    game.GetBacklit().LoadingAnim();
    ProgressAsync.SetProgressData(m_ProgressLoading);
    ProgressAsync.SetUserData(m_ImageBackground);
  }

  void SetLoadScreenImage()
  {
    if (m_LastLoadScreen > m_LoadScreenImages.Count()) m_LastLoadScreen=0;
    else m_LastLoadScreen++;

    m_ImageBackground.SetImage(m_LastLoadScreen);
  }
}

modded class LoginScreenBase
{
  void LoginScreenBase()
  {
    g_Game.m_loading.SetLoadScreenImage();
  }
}

modded class LoginQueueBase
{
  override Widget Init()
  {
    layoutRoot=super.Init();
		ImageWidget image=ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
    image.LoadImageFile(0, "{0D704C4D21687199}ScreenZ/gui/textures/loading_screens/loading_screen_co.edds");
		return layoutRoot;
  }
}

modded class LoginTimeBase
{
  override Widget Init()
  {
    layoutRoot=super.Init();
		ImageWidget image=ImageWidget.Cast(layoutRoot.FindAnyWidget("Background"));
    image.LoadImageFile(0, "{0D704C4D21687199}ScreenZ/gui/textures/loading_screens/loading_screen_co.edds");
		return layoutRoot;
  }
}
