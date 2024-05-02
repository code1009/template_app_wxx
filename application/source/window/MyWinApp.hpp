#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "MyTabbedMDI.hpp"
#include "MainFrame.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWinApp : public CWinApp
{
private:
	CMainFrame m_MainFrame;

public:
	CMyWinApp();
	virtual ~CMyWinApp();

private:
	CMyWinApp(const CMyWinApp&);
	CMyWinApp& operator=(const CMyWinApp&);

public:
	virtual BOOL InitInstance();

public:
	CMainFrame* GetMainFrame() { return &m_MainFrame; }
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// returns a reference to the CMyWinApp object
inline CMyWinApp* GetMyWinApp() 
{
	return static_cast<CMyWinApp*>(GetApp()); 
}
