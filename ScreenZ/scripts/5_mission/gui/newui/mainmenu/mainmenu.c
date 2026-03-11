modded class MainMenu
{
	override Widget Init()
	{
		layoutRoot=g_Game.GetWorkspace().CreateWidgets("ScreenZ/gui/layouts/new_ui/main_menu.layout");

		m_Play=layoutRoot.FindAnyWidget("play");
		m_ChooseServer=layoutRoot.FindAnyWidget("choose_server");
		m_CustomizeCharacter=layoutRoot.FindAnyWidget("customize_character");
		m_PlayVideo=layoutRoot.FindAnyWidget("play_video");
		m_Feedback=layoutRoot.FindAnyWidget("feedback_button");
		m_Tutorials=layoutRoot.FindAnyWidget("tutorials");
		m_TutorialButton=layoutRoot.FindAnyWidget("tutorial_button");
		m_MessageButton=layoutRoot.FindAnyWidget("message_button");
		m_SettingsButton=layoutRoot.FindAnyWidget("settings_button");
		m_Exit=layoutRoot.FindAnyWidget("exit_button");
		m_PrevCharacter=layoutRoot.FindAnyWidget("prev_character");
		m_NextCharacter=layoutRoot.FindAnyWidget("next_character");

		m_DlcFrame=layoutRoot.FindAnyWidget("dlc_Frame");
		m_Version=TextWidget.Cast(layoutRoot.FindAnyWidget("version"));
		m_ModdedWarning=TextWidget.Cast(layoutRoot.FindAnyWidget("ModdedWarning"));
    m_ModdedWarning.Show(false);
		m_CharacterRotationFrame=layoutRoot.FindAnyWidget("character_rotation_frame");

		m_LastPlayedTooltip=layoutRoot.FindAnyWidget("last_server_info");
		m_LastPlayedTooltipLabel=m_LastPlayedTooltip.FindAnyWidget("last_server_info_label");
		m_LastPlayedTooltipName=TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_name"));
		m_LastPlayedTooltipIP=TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_ip"));
		m_LastPlayedTooltipPort=TextWidget.Cast(m_LastPlayedTooltip.FindAnyWidget("last_server_info_port"));
		m_LastPlayedTooltipTimer=new WidgetFadeTimer();

		m_Stats=new MainMenuStats(layoutRoot.FindAnyWidget("character_stats_root"));

		m_Mission=MissionMainMenu.Cast(g_Game.GetMission());

		m_LastFocusedButton=m_Play;

		m_ScenePC=m_Mission.GetIntroScenePC();
		if (m_ScenePC) m_ScenePC.GetIntroCamera().SetActive(false);

		m_PlayVideo.Show(false);

		m_PlayerName=TextWidget.Cast(layoutRoot.FindAnyWidget("character_name_text"));

		// Set Version
		string version;
		g_Game.GetVersion(version);
		m_Version.SetText("#main_menu_version" + " " + version);

		g_Game.GetUIManager().ScreenFadeOut(0);

		SetFocus(null);
		Refresh();
		LoadMods();
		PopulateDlcFrame();

		g_Game.GetBacklit().MainMenu_OnShow();
		g_Game.SetLoadState(DayZLoadState.MAIN_MENU_CONTROLLER_SELECT);

		return layoutRoot;
	}

	void ~MainMenu();

	override void LoadMods()
	{
		array<ref ModInfo> modArray=new array<ref ModInfo>();
		g_Game.GetModInfos(modArray);
		if (modArray.Count() > 0)
		{
			modArray.Remove(modArray.Count() - 1);
			modArray.Invert();
		}

    FilterDlcs(modArray);

		if (m_ModsSimple) delete m_ModsSimple;
		if (m_ModsDetailed) delete m_ModsDetailed;

    m_ModdedWarning.Show(false);

		if (modArray.Count() > 0)
		{
			layoutRoot.FindAnyWidget("ModsSimple").Show(true);
			m_ModsTooltip=new ModsMenuTooltip(layoutRoot);

			m_ModsDetailed=new ModsMenuDetailed(modArray, layoutRoot.FindAnyWidget("ModsDetailed"), m_ModsTooltip, this);

			m_ModsSimple=new ModsMenuSimple(modArray, layoutRoot.FindAnyWidget("ModsSimple"), m_ModsDetailed);
		}
	}

	override void FilterDlcs(inout array<ref ModInfo> modArray)
	{
		if (!m_AllDlcsMap) m_AllDlcsMap = new map<string,ref ModInfo>();
		m_AllDlcsMap.Clear();

		int count = modArray.Count();
		ModInfo info;
		for (int i = count - 1; i > -1; i--)
		{
			info = modArray[i];
			if (info.GetIsDLC())
			{
				m_AllDlcsMap.Set(info.GetName(),info);
				modArray.Remove(i);
			}
		}
	}

	override void ShowDlcFrame(bool show)
	{
		m_DlcFrame.Show(false);
		if (m_DisplayedDlcHandler) m_DisplayedDlcHandler.ShowInfoPanel(false);
	}


	override void PopulateDlcFrame()
	{
		if (!m_DlcHandlers) m_DlcHandlers = new array<ref MainMenuDlcHandlerBase>();

		m_DlcData = DlcDataLoader.GetData();
		int count = m_DlcData.DLCs.Count();
		JsonDataDLCInfo data;
		ModInfo info;

		for (int i = 0; i < count; i++)
		{
			data = m_DlcData.DLCs[i];
			info = m_AllDlcsMap.Get(data.Name);
			MainMenuDlcHandlerBase handler = new MainMenuDlcHandlerBase(info, m_DlcFrame, data);

			handler.ShowInfoPanel(true);
			m_DisplayedDlcHandler = handler;//TODO: carousel will take care of this later

			m_DlcHandlers.Insert(handler);
		}
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w == m_CharacterRotationFrame)
		{
			if (m_ScenePC)
				m_ScenePC.CharacterRotationStart();
			return true;
		}

		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (m_ScenePC)
			m_ScenePC.CharacterRotationStop();
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (button == MouseState.LEFT)
		{
			if (w == m_Play)
			{
				m_LastFocusedButton=m_Play;
				Play();
				return true;
			}
			else if (w == m_ChooseServer)
			{
				m_LastFocusedButton=m_ChooseServer;
				OpenMenuServerBrowser();
				return true;
			}
			else if (w == m_CustomizeCharacter)
			{
				OpenMenuCustomizeCharacter();
				return true;
			}
			else if (w == m_TutorialButton)
			{
				OpenTutorials();
				return true;
			}
			else if (w == m_MessageButton)
			{
				OpenCredits();
				return true;
			}
			else if (w == m_SettingsButton)
			{
				OpenSettings();
				return true;
			}
			else if (w == m_Exit)
			{
				Exit();
				return true;
			}
			else if (w == m_PrevCharacter)
			{
				PreviousCharacter();
				return true;
			}
			else if (w == m_NextCharacter)
			{
				NextCharacter();
				return true;
			}
			else if (w == m_PlayVideo)
			{
				m_LastFocusedButton=m_PlayVideo;
				PlayVideo();
				return true;
			}
			else if (w == m_Tutorials)
			{
				m_LastFocusedButton=m_Tutorials;
				OpenTutorials();
				return true;
			}
			else if (w == m_Feedback)
			{
				m_LastFocusedButton=m_Feedback;
				OpenFeedback();
			}
		}
		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (IsFocusable(w))
		{
			ColorHighlight(w);
			return true;
		}
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (IsFocusable(w))
		{
			ColorNormal(w);
			return true;
		}
		return false;
	}

	override bool OnFocus(Widget w, int x, int y)
	{
		if (IsFocusable(w))
		{
			ColorHighlight(w);
			return true;
		}
		return false;
	}

	override bool OnFocusLost(Widget w, int x, int y)
	{
		if (IsFocusable(w))
		{
			ColorNormal(w);
			return true;
		}
		return false;
	}

	override bool IsFocusable(Widget w)
	{
		if (w)
		{
			if (w == m_Play || w == m_ChooseServer || w == m_CustomizeCharacter || w == m_TutorialButton || w == m_MessageButton || w == m_SettingsButton)
			{
				return true;
			}

			if (w == m_Exit || w == m_PlayVideo || w == m_Feedback)
			{
				return true;
			}

			if (w == m_NewsMain || w == m_NewsSec1 || w == m_NewsSec2 || w == m_PrevCharacter || w == m_NextCharacter)
			{
				return true;
			}
		}
		return false;
	}

	override void Refresh()
	{
		string name;
		if (m_ScenePC && g_Game.GetGameState() == DayZGameState.MAIN_MENU)
			OnChangeCharacter();

		string version;
		g_Game.GetVersion(version);
		m_Version.SetText("#main_menu_version" + " " + version);

		if (m_DisplayedDlcHandler)
			m_DisplayedDlcHandler.UpdateAllPromotionInfo();
	}

	override void OnShow()
	{
		if (m_DisplayedDlcHandler)
			m_DisplayedDlcHandler.ShowInfoPanel(true);

		SetFocus(null);
		OnChangeCharacter(false);
		m_Stats.UpdateStats();
		LoadMods();
		return;
	}

	override void OnHide()
	{
		if (m_DisplayedDlcHandler)
			m_DisplayedDlcHandler.ShowInfoPanel(false);

		g_Game.GetBacklit().MainMenu_OnHide();
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (g_Game && GetUApi().GetInputByID(UAUIBack).LocalPress())
		{
			if (!g_Game.GetUIManager().IsDialogHiding())
			{
				Exit();
			}
		}
	}

	override void Play()
	{
		if (!g_Game.IsNewCharacter())
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectLastSession");
		}
		else
		{
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallByName(this, "ConnectBestServer");
		}
	}

	override bool CanSaveDefaultCharacter()
	{
		if (m_ScenePC && m_ScenePC.GetIntroCharacter() && m_ScenePC.GetIntroCharacter().GetCharacterID() == -1)
		{
			return true;
		}
		return false;
	}

	override void OpenMenuServerBrowser()
	{
		EnterScriptedMenu(MENU_SERVER_BROWSER);
	}

	override void OpenMenuCustomizeCharacter()
	{
		EnterScriptedMenu(MENU_CHARACTER);
	}

	override void NextCharacter()
	{
		if (m_ScenePC && m_ScenePC.GetIntroCharacter())
		{
			int charID=m_ScenePC.GetIntroCharacter().GetNextCharacterID();
			if (charID != m_ScenePC.GetIntroCharacter().GetCharacterID())
			{
				m_ScenePC.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}

	override void PreviousCharacter()
	{
		if (m_ScenePC && m_ScenePC.GetIntroCharacter())
		{
			int charID=m_ScenePC.GetIntroCharacter().GetPrevCharacterID();
			if (charID != m_ScenePC.GetIntroCharacter().GetCharacterID())
			{
				m_ScenePC.GetIntroCharacter().SetCharacterID(charID);
				OnChangeCharacter();
			}
		}
	}

	override void OnChangeCharacter(bool create_character=true)
	{
		if (m_ScenePC && m_ScenePC.GetIntroCharacter())
		{
			int charID=m_ScenePC.GetIntroCharacter().GetCharacterID();
			if (create_character)
			{
				m_ScenePC.GetIntroCharacter().CreateNewCharacterById(charID);
			}
			m_PlayerName.SetText(m_ScenePC.GetIntroCharacter().GetCharacterNameById(charID));

      string ip="";
      string name="";
      int port=0;
      m_ScenePC.GetIntroCharacter().GetLastPlayedServer(charID, ip, name, port);
      m_LastPlayedTooltipName.SetText("#server_details_name " + name);
      m_LastPlayedTooltipIP.SetText("#main_menu_IP " + ip);
      m_LastPlayedTooltipPort.SetText("#main_menu_port " + port);

			Widget w=m_CustomizeCharacter.FindAnyWidget(m_CustomizeCharacter.GetName() + "_label");
			if (w)
			{
				TextWidget text=TextWidget.Cast(w);

				if (m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
				{
					text.SetText("#layout_main_menu_customize_char");
				}
				else
				{
					text.SetText("#layout_main_menu_rename");
				}
			}
			if (m_ScenePC.GetIntroCharacter().GetCharacterObj())
			{
				if (m_ScenePC.GetIntroCharacter().GetCharacterObj().IsMale())
					m_ScenePC.GetIntroCharacter().SetCharacterGender(ECharGender.Male);
				else
					m_ScenePC.GetIntroCharacter().SetCharacterGender(ECharGender.Female);
			}
			//update character stats
			m_Stats.UpdateStats();
		}
	}

	override void OpenStats()
	{

	}

	override void OpenMessages()
	{

	}

	override void OpenSettings()
	{
		EnterScriptedMenu(MENU_OPTIONS);
	}

	override void PlayVideo()
	{
		EnterScriptedMenu(MENU_VIDEO);
	}

	override protected void OpenFeedback()
	{
		g_Game.OpenURL("https://feedback.bistudio.com/project/view/2/");
	}

	override void OpenTutorials()
	{
		EnterScriptedMenu(MENU_TUTORIAL);
	}

	override void OpenCredits()
	{
		EnterScriptedMenu(MENU_CREDITS);
		m_Mission.OnMenuEnter(MENU_CREDITS);
	}

	override void Exit()
	{
		g_Game.GetUIManager().ShowDialog("#main_menu_exit", "#main_menu_exit_desc", IDC_MAIN_QUIT, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
	}

	override bool TryConnectLastSession(out string ip, out int port)
	{
		if (g_Game.GetLastVisitedServer(ip, port))
		{
			return true;
		}
		return false;
	}

	override void AutoConnect()
	{
		OnlineServices.AutoConnectToEmptyServer();
	}

	override void ConnectLastSession()
	{
		string ip="";
		string name="";
		int port=0;
		int steamQueryPort=0;

		if (!m_ScenePC.GetIntroCharacter().IsDefaultCharacter())
		{
			int charID=m_ScenePC.GetIntroCharacter().GetCharacterID();
			m_ScenePC.GetIntroCharacter().GetLastPlayedServerEx(charID, ip, name, port, steamQueryPort);
		}

		if (ip.Length() > 0)
		{
			g_Game.ConnectFromServerBrowserEx(ip, port, steamQueryPort, "");
		}
		else
		{
			OpenMenuServerBrowser();
		}
	}

	override void ConnectBestServer()
	{
		ConnectLastSession();
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		if (code == IDC_MAIN_QUIT)
		{
			if (result == 2)
				g_Game.GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.RequestExit, IDC_MAIN_QUIT);
			if (result == 3)
				ColorNormal(GetFocus());
			return true;
		}
		else if (code == 555)
		{
			if (result == 2)
				OpenTutorials();
		}
		return false;
	}

	//Coloring functions (Until WidgetStyles are useful)
	override void ColorHighlight(Widget w)
	{
		if (!w)
			return;

		int color_pnl=ARGB(255, 0, 0, 0);
		int color_lbl=ARGB(255, 255, 0, 0);
		int color_img=ARGB(255, 200, 0, 0);

		ButtonSetColor(w, color_pnl);
		ButtonSetTextColor(w, color_lbl);
		ImagenSetColor(w, color_img);
	}

	override void ColorNormal(Widget w)
	{
		if (!w)
			return;

		int color_pnl=ARGB(0, 0, 0, 0);
		int color_lbl=ARGB(255, 255, 255, 255);
		int color_img=ARGB(255, 255, 255, 255);

		ButtonSetColor(w, color_pnl);
		ButtonSetTextColor(w, color_lbl);
		ImagenSetColor(w, color_img);
	}

	override void ButtonSetText(Widget w, string text)
	{
		if (!w)
			return;

		TextWidget label=TextWidget.Cast(w.FindWidget(w.GetName() + "_label"));

		if (label)
		{
			label.SetText(text);
		}

	}

	override void ButtonSetColor(Widget w, int color)
	{
		if (!w)
			return;

		Widget panel=w.FindWidget(w.GetName() + "_panel");

		if (panel)
		{
			panel.SetColor(color);
		}
	}

	override void ImagenSetColor(Widget w, int color)
	{
		if (!w)
			return;

		Widget panel=w.FindWidget(w.GetName() + "_image");

		if (panel)
		{
			panel.SetColor(color);
		}
	}

	override void ButtonSetTextColor(Widget w, int color)
	{
		if (!w)
			return;

		TextWidget label	= TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_label"));
		TextWidget text		= TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_text"));
		TextWidget text2	= TextWidget.Cast(w.FindAnyWidget(w.GetName() + "_text_1"));

		if (label)
		{
			label.SetColor(color);
		}

		if (text)
		{
			text.SetColor(color);
		}

		if (text2)
		{
			text2.SetColor(color);
		}
	}
}
