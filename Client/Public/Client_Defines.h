#pragma once

#include <process.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_PROLOGUE, LEVEL_GAMEPLAY, LEVEL_GAMEPLAY_LV2, LEVEL_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;



using namespace Client;
