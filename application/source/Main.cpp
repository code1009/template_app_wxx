/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0
#if defined(_DEBUG)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif

//===========================================================================
#pragma comment (lib, "comctl32.lib")

//===========================================================================





/////////////////////////////////////////////////////////////////////////////
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

//===========================================================================
#include "window/MyWinApp.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static int RunWinApp()
{
	//-----------------------------------------------------------------------
	int nResult;


	//-----------------------------------------------------------------------
	try
	{
		// Start Win32++
		CMyWinApp theApp;


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

	return nResult;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static int Run()
{
	RunWinApp();

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int WINAPI WinMain(
	HINSTANCE /*hInstance*/,
	HINSTANCE /*hPrevInstance*/,
	LPTSTR /*lpstrCmdLine*/, 
	int /*nCmdShow*/
)
{
	//UNREFERENCED_PARAMETER(hInstance);
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(lpstrCmdLine);
	//UNREFERENCED_PARAMETER(nCmdShow);
 

	Run();


	return 0;
}




