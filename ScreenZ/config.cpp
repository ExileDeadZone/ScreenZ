class CfgPatches
{
  class ScreenZ
  {
    units[]={};
    weapons[]={};
    requiredVersion=0.1;
    requiredAddons[]={ "DZ_Data", "DZ_Scripts" };
  };
};

class CfgMods
{
  class ScreenZ
  {
    dir="ScreenZ";
    hideName=0;
    hidePicture=0;
    extra=0;
    name="ScreenZ";
    picture="ScreenZ\data\picture.paa";
    logoSmall="ScreenZ\data\logoSmall.paa";
    logo="ScreenZ\data\logo.paa";
    logoOver="ScreenZ\data\logo.paa";
    action="https://steamcommunity.com/sharedfiles/filedetails/?id=3643948795";
    credits="moldypenguins";
    author="moldypenguins";
    authorID="76561198312711389";
    version="1.7";
    type="mod";
    dependencies[]={ "Game", "Mission" };
    class defs
    {
      class engineScriptModule { value=""; files[]={ "ScreenZ\scripts\1_core" }; };
      class gameScriptModule { value=""; files[]={ "ScreenZ\scripts\3_game" }; };
      class missionScriptModule { value=""; files[]={ "ScreenZ\scripts\5_mission" }; };
    }
  };
};

class CfgSoundShaders
{
	class ScreenZ_Menu_SoundShader
	{
		samples[]={
			{ "ScreenZ\dz\sounds\music\menu_music_01", 1 },
			{ "ScreenZ\dz\sounds\music\menu_music_02", 1 },
			{ "ScreenZ\dz\sounds\music\menu_music_03", 1 }
		};
		volume=0.5;
	};
};

class CfgSoundSets {
	class ScreenZ_Menu_SoundSet
	{
		soundShaders[]={ "ScreenZ_Menu_SoundShader" };
		volumeFactor=1;
		frequencyFactor=1;
		spatial=0;
	};
};
