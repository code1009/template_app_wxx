#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "MyTabbedMDI.hpp"
#include "Mainframe.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWinApp : public CWinApp
{
private:
	CMainFrame m_Mainframe;

public:
	CMyWinApp();
	virtual ~CMyWinApp();

public:
	virtual BOOL InitInstance();

public:
	CMainFrame* GetMainFrame() { return &m_Mainframe; }
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// returns a reference to the CMyWinApp object
inline CMyWinApp* GetMyWinApp() 
{
	return static_cast<CMyWinApp*>(GetApp()); 
}
