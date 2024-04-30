/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#pragma comment (lib, "comctl32.lib")




//===========================================================================
#if 0
#ifdef _DEBUG
#include <vld/include/vld.h>
#endif
#endif

//===========================================================================
#if 0
#ifdef _DEBUG
#include <crtdbg.h>
#endif 
#endif

//===========================================================================
#include <res/resource.h>

#include "window/MyWinApp.hpp"


void a()
{
	int nResult;

	//-----------------------------------------------------------------------
	try
	{
		// Start Win32++
		CMyWinApp theApp;

		//		GetComCtlVersion();
		//		MyInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);

				// Run the application
		nResult = theApp.Run();
	}

	// catch all unhandled CException types
	catch (const CException& e)
	{
		// Display the exception and quit
		MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		nResult = -1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int nCmdShow)
{
	//MessageBox(nullptr, "a", "a", MB_OK);
	a();

	return 0;
}




