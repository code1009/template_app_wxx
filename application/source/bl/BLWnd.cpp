/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "BLWnd.hpp"

//===========================================================================
#include "bl.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr UINT_PTR BLWND_TIMER_EVENTID = 1;
constexpr UINT     BLWND_TIMER_ELAPSE = 500;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define blwh_get()  (reinterpret_cast<bl::window_handler*>(_bl_window_handler))
#define blwh_set(p) (_bl_window_handler = reinterpret_cast<void*>(p))





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CBLWnd::CBLWnd():
	_bl_window_handler(nullptr),

	m_cxClientMax(0),
	m_cyClientMax(0)
{
}

CBLWnd::~CBLWnd()
{
}

//===========================================================================
void CBLWnd::PreCreate(CREATESTRUCT& cs)
{
	CWnd::PreCreate(cs);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style |= (WS_VSCROLL | WS_HSCROLL);
}

int CBLWnd::OnCreate(CREATESTRUCT& cs)
{
	UNREFERENCED_PARAMETER(cs);


	blwh_set(reinterpret_cast<bl::window_handler*>(new bl::window_handler()));
	blwh_get()->OnCreate(GetHwnd(), WM_CREATE, 0, 0);


	SetTimer(BLWND_TIMER_EVENTID, BLWND_TIMER_ELAPSE, NULL);

//	ShowScrollBar(_hwnd, SB_BOTH, TRUE);

	return 0;
}

void CBLWnd::OnDestroy()
{
	KillTimer(1);


	blwh_get()->OnDestroy(GetHwnd(), WM_DESTROY, 0, 0);
	if (blwh_get())
	{
		delete blwh_get();

		blwh_set(nullptr);
	}
}

void CBLWnd::OnDraw(CDC& dc)
{
	// OnDraw is called automatically whenever a part of the window needs to be redrawn.

	Draw(dc);
}

LRESULT CBLWnd::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	try
	{
		switch (msg)
		{
		case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
		case WM_SIZE:           return OnSize(msg, wparam, lparam);
		case WM_TIMER:          return OnTimer(msg, wparam, lparam);
		case WM_PAINT:          return OnPaint(msg, wparam, lparam);
		case WM_ERASEBKGND:     return OnEraseBkgnd(msg, wparam, lparam);
		case WM_HSCROLL:        return OnHScroll(msg, wparam, lparam);
		case WM_VSCROLL:        return OnVScroll(msg, wparam, lparam);
		case WM_MOUSEWHEEL:     return OnMouseWheel(msg, wparam, lparam);
		}

		return WndProcDefault(msg, wparam, lparam);
	}

	// Catch all CException types.
	catch (const CException& e)
	{
		// Display the exception and continue.
		::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return 0;
	}
}

//===========================================================================
LRESULT CBLWnd::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Called in response to a WM_DPICHANGED_BEFOREPARENT message which is sent to child
	// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
	// application is DPI_AWARENESS_PER_MONITOR_AWARE.

	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CBLWnd::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Respond to a mouse click on the window

	SetFocus();
	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CBLWnd::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);

	// If not minimized, save the window size
	if (wparam != SIZE_MINIMIZED)
	{
		m_cxClientMax = LOWORD(lparam);
		if (m_cxClientMax < 1)
		{
			m_cxClientMax = 1;
		}

		m_cyClientMax = HIWORD(lparam);
		if (m_cyClientMax < 1)
		{
			m_cyClientMax = 1;
		}
	}

	if ( IsWindow() )
	{
		blwh_get()->OnSize(GetHwnd(), msg, wparam, lparam);
		Invalidate(FALSE);
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	//UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	UINT_PTR nIDEvent = (UINT_PTR)wparam;


	if (BLWND_TIMER_EVENTID == nIDEvent)
	{

	}


	return 0;
}

LRESULT CBLWnd::OnPaint(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	//CPaintDC dc(GetHwnd());


	//Draw(dc);


	blwh_get()->OnPaint(GetHwnd(), msg, wparam, lparam);


	return 0;
}

LRESULT CBLWnd::OnEraseBkgnd(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// return WndProcDefault(msg, wparam, lparam);
	return 1;
}

LRESULT CBLWnd::OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	blwh_get()->OnHScroll(GetHwnd(), msg, wparam, lparam);

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnVScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	blwh_get()->OnVScroll(GetHwnd(), msg, wparam, lparam);

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnMouseWheel(UINT msg, WPARAM wparam, LPARAM lparam)
{
	blwh_get()->OnMouseWheel(GetHwnd(), msg, wparam, lparam);

	return WndProcDefault(msg, wparam, lparam);
}

//===========================================================================
void CBLWnd::Draw(CDC& dc)
{
	CRect rc = GetClientRect();


	TCHAR text[256];


	sprintf_s(text, "%d %d - %d %d %d %d ",
		m_cxClientMax, m_cyClientMax,
		rc.left,
		rc.right,
		rc.top,
		rc.bottom
		);

	dc.FillRect(rc, (HBRUSH)(COLOR_WINDOW + 1));

	dc.DrawText(text, -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


