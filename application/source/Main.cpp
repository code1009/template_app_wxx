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
inline BOOL MyInitCommonControls(DWORD dwFlags = ICC_COOL_CLASSES | ICC_BAR_CLASSES)
{
	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
	BOOL bRet = ::InitCommonControlsEx(&iccx);


	if (FALSE==bRet)
	{
		TRACE("InitCommonControlsEx() failed \n");
	}


	return bRet;

}

static int RunWindow()
{
	int nRet;


	MyInitCommonControls();

	nRet = RunWinApp();


	return nRet;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static int Run()
{
	//-----------------------------------------------------------------------
	HRESULT hRes;
	int nRet;


	//-----------------------------------------------------------------------
	hRes = ::CoInitialize(NULL);
	VERIFY(SUCCEEDED(hRes));
	if (!SUCCEEDED(hRes))
	{
		return -1;
	}


	//-----------------------------------------------------------------------
	nRet = RunWindow();


	//-----------------------------------------------------------------------
	::CoUninitialize();


	return nRet;
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




