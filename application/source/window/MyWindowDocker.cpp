/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "MyWindowDocker.hpp"



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void CMyWindow::PreCreate(CREATESTRUCT& cs)
{
	CWnd::PreCreate(cs);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
}

//===========================================================================
int CMyWindow::OnCreate(CREATESTRUCT& cs)
{
	UNREFERENCED_PARAMETER(cs);

	SetTimer (1, 250, NULL) ;
	return 0;
}

void CMyWindow::OnDestroy()
{
	KillTimer(1);
}

//===========================================================================
void CMyWindow::OnDraw(CDC& dc)
{
	// OnDraw is called automatically whenever a part of the window needs to be redrawn.
	Draw(dc);
}

//===========================================================================
LRESULT CMyWindow::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
	case WM_SIZE:           return OnSize(msg, wparam, lparam);
	case WM_TIMER:          return OnTimer(msg, wparam, lparam);
//	case WM_PAINT:          return OnPaint(msg, wparam, lparam);
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CMyWindow::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse click on the window
{
	SetFocus();
	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyWindow::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);

	// If not minimized, save the window size
	if (wparam != SIZE_MINIMIZED)
	{
		m_cxClientMax = LOWORD(lparam);
		if (m_cxClientMax < 1)
			m_cxClientMax = 1;

		m_cyClientMax = HIWORD(lparam);
		if (m_cyClientMax < 1)
			m_cyClientMax = 1;
	}

	if ( IsWindow() )
	{
		Invalidate(FALSE);
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CMyWindow::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	return 0;
}

LRESULT CMyWindow::OnPaint(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	CPaintDC dc(GetHwnd());


	Draw(dc);

	return 0;
}

//===========================================================================
void CMyWindow::Draw (CDC& dc)
{
	CRect rc = GetClientRect();


	TCHAR text[256];
	
	
	sprintf_s(text, "Docker %d %d - %d %d %d %d ",
		m_cxClientMax, m_cyClientMax,
		rc.left,
		rc.right,
		rc.top,
		rc.bottom
		);

	dc.DrawText(text, -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMyWindowDockContainer::CMyWindowDockContainer()
{
	SetView        (m_Wnd); 
	SetDockCaption (_T("MyWindow - Docking container"));
	SetTabText     (_T("MyWindow"));
	SetTabIcon     (IDI_DOCKER);
} 





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMyWindowDocker::CMyWindowDocker()
{
	SetView     (m_DockContainer);
//	SetBarWidth (4);
}




