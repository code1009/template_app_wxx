/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "MyView.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr UINT_PTR MYVIEW_TIMER_EVENTID = 1;
constexpr UINT     MYVIEW_TIMER_ELAPSE = 500;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMyView::CMyView():
	m_cxClientMax(0),
	m_cyClientMax(0)
{

}

CMyView::~CMyView()
{

}

//===========================================================================
void CMyView::PreCreate(CREATESTRUCT& cs)
{
	CWnd::PreCreate(cs);

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
}

int CMyView::OnCreate(CREATESTRUCT& cs)
{
	UNREFERENCED_PARAMETER(cs);

	SetTimer(MYVIEW_TIMER_EVENTID, MYVIEW_TIMER_ELAPSE, NULL);

	return 0;
}

void CMyView::OnDestroy()
{
	KillTimer(1);
}

void CMyView::OnDraw(CDC& dc)
{
	// OnDraw is called automatically whenever a part of the window needs to be redrawn.
	Draw(dc);
}

LRESULT CMyView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	try
	{
		switch (msg)
		{
		case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
		case WM_SIZE:           return OnSize(msg, wparam, lparam);
		case WM_TIMER:          return OnTimer(msg, wparam, lparam);
		//case WM_PAINT:        return OnPaint(msg, wparam, lparam);
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
LRESULT CMyView::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Called in response to a WM_DPICHANGED_BEFOREPARENT message which is sent to child
	// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
	// application is DPI_AWARENESS_PER_MONITOR_AWARE.

	SetDPIImages();

	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyView::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Respond to a mouse click on the window

	SetFocus();
	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CMyView::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
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

LRESULT CMyView::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	//UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	UINT_PTR nIDEvent = (UINT_PTR)wparam;


	if (MYVIEW_TIMER_EVENTID == nIDEvent)
	{

	}


	return 0;
}

LRESULT CMyView::OnPaint(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);

	CPaintDC dc(GetHwnd());


	Draw(dc);

	return 0;
}

//===========================================================================
void CMyView::SetDPIImages()
{
#if 0
	// Resize the image list.
	CBitmap bmImage(IDB_CLASSVIEW);
	bmImage = DpiScaleUpBitmap(bmImage);
	int scale = bmImage.GetSize().cy / 15;
	CImageList normalImages;
	normalImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	normalImages.Add(bmImage, RGB(255, 0, 0));
	SetImageList(normalImages, LVSIL_NORMAL);

	// Reset the item indentation.
	int imageWidth = normalImages.GetIconSize().cx;
	SetIndent(imageWidth);
#endif
}

void CMyView::Draw(CDC& dc)
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


