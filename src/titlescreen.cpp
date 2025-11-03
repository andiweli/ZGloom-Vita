#include "titlescreen.h"
#include <string>

// Ellipsize helper: kürzt Strings auf maxLen Zeichen und hängt "..." an
// (nur für die Anzeige; die originalen levelnames bleiben unverändert)
static std::string Ellipsize(const std::string& s, size_t maxLen = 38) {
    if (s.size() <= maxLen) return s;
    if (maxLen <= 3) return s.substr(0, maxLen);
    return s.substr(0, maxLen - 3) + "...";
}

void TitleScreen::Render(SDL_Surface *src, SDL_Surface *dest, Font &font)
{
	SDL_BlitSurface(src, nullptr, dest, nullptr);
	bool flash = (timer / 5) & 1;

	font.PrintMessage("PORT BY JETSTREAMSHAM AND ANDIWELI", 245, dest, 1); // added copyright text in footer

	if (status == TITLESTATUS_MAIN)
	{
		if (flash || (selection != MAINENTRY_PLAY))
			font.PrintMessage("START NEW GAME", 150, dest, 1);
		if (flash || (selection != MAINENTRY_SELECT))
			font.PrintMessage("SELECT LEVEL", 165, dest, 1);
		if (flash || (selection != MAINENTRY_GAME))
			font.PrintMessage("ABOUT THE GAME", 180, dest, 1);
		if (flash || (selection != MAINENTRY_PORT))
			font.PrintMessage("ABOUT THE PSVITA PORT", 195, dest, 1);
		if (flash || (selection != MAINENTRY_QUIT))
			font.PrintMessage("EXIT", 210, dest, 1);
	}
	else if (status == TITLESTATUS_SELECT)
	{
		for (int i = selection - 10; i < selection + 10; i++)
		{
			if ((i >= 0) && (i < (int)levelnames.size()))
			{
				const std::string label = Ellipsize(levelnames[i], 38);
				if (flash || (i != selection)) {
					font.PrintMessage(label, 100 + (i - selection) * 10, dest, 1);
				} else {
					font.PrintMessage(label, 100 + (i - selection) * 10, dest, 2);
				}
			}
		}
	}
	else if (status == TITLESTATUS_PORT)
	{
		font.PrintMessage("PSVITA PORT", 90, dest, 1);
		font.PrintMessage("BY JETSTREAMSHAM", 100, dest, 1);

		font.PrintMessage("ADDITIONAL PROGRAMMING", 115, dest, 1);
		font.PrintMessage("LAUNCHER CHEATMENU AND OPTIMIZING", 125, dest, 1);
		font.PrintMessage("BY ANDIWELI", 135, dest, 1);

		font.PrintMessage("BASED ON ZGLOOM BY SWIZPIG", 155, dest, 1);
	}
	else if (status == TITLESTATUS_GAME)
	{
		font.PrintMessage("GLOOM", 60, dest, 1);
		font.PrintMessage("BLACK MAGIC GAME ENGINE", 70, dest, 1);

		font.PrintMessage("PROGRAMMED BY MARK SIBLY", 85, dest, 1);
		font.PrintMessage("GRAPHICS BY THE BUTLER BROTHERS", 95, dest, 1);
		font.PrintMessage("MUSIC BY KEV STANNARD", 105, dest, 1);
		font.PrintMessage("AUDIO BY BLACK MAGIC", 115, dest, 1);
		font.PrintMessage("PRODUCED AND DESIGNED BY BLACK MAGIC", 125, dest, 1);

		font.PrintMessage("CODED IN DEVPAC2 AND BLITZBASIC 2", 140, dest, 1);
		
		font.PrintMessage("GLOOM AND GLOOM 3 BY BLACK MAGIC", 160, dest, 1); // added copyright for Gloom series
		font.PrintMessage("ZOMBIE MASSACRE BY ALPHA SOFTWARE", 170, dest, 1); // added copyright for Zombie Massacre
	}
}

TitleScreen::TitleScreen()
{
	status = TITLESTATUS_MAIN;
	selection = 0;
	timer = 0;
}

TitleScreen::TitleReturn TitleScreen::Update(int &levelout)
{

	if (status == TITLESTATUS_MAIN)
	{

		if (Input::GetButtonDown(SCE_CTRL_DOWN))
		{
			selection++;
			if (selection == MAINENTRY_END)
				selection = 0;
		}

		if (Input::GetButtonDown(SCE_CTRL_UP))
		{
			selection--;
			if (selection == -1)
				selection = MAINENTRY_END - 1;
		}

		if (Input::GetButtonDown(SCE_CTRL_CROSS))
		{
			if (selection == MAINENTRY_PLAY)
				return TITLERET_PLAY;
			if (selection == MAINENTRY_QUIT)
				return TITLERET_QUIT;
			if (selection == MAINENTRY_GAME)
				status = TITLESTATUS_GAME;
			if (selection == MAINENTRY_PORT)
				status = TITLESTATUS_PORT;
				if (selection == MAINENTRY_SELECT)
			{
				selection = 0;
				status = TITLESTATUS_SELECT;
			};
		}
	}
	else if (status == TITLESTATUS_SELECT)
	{

// O (CIRCLE): back to MAIN from Level Select
if (Input::GetButtonDown(SCE_CTRL_CIRCLE))
{
	status = TITLESTATUS_MAIN;
	selection = 0;
}


		if (Input::GetButtonDown(SCE_CTRL_DOWN))
		{
			selection++;
			if (selection == levelnames.size())
				selection = 0;
		}

		if (Input::GetButtonDown(SCE_CTRL_UP))
		{
			selection--;
			if (selection == -1)
				selection = levelnames.size() - 1;
		}

		if (Input::GetButtonDown(SCE_CTRL_CROSS))
		{
			levelout = selection;
			status = TITLESTATUS_MAIN;
			selection = 0;
			return TITLERET_SELECT;
		}
	}
	else
	{
		// O (CIRCLE): back to MAIN from About
		if (Input::GetButtonDown(SCE_CTRL_CIRCLE))
			status = TITLESTATUS_MAIN;
		else 
		if (Input::GetButtonDown(SCE_CTRL_CROSS))
			status = TITLESTATUS_MAIN;
	}

	return TITLERET_NOTHING;
}
