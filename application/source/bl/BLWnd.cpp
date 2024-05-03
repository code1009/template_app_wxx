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
class bl_drawer : public bl::drawer
{
public:
	double _contents_cx{ 0 };
	double _contents_cy{ 0 };

public:
	BLFontFace _font_face;
	BLFont _font;

public:
	bl_drawer();
	virtual ~bl_drawer();

public:
	virtual void draw(bl::context* ctx) override;

public:
	void draw_ex5(BLContext* ctx);
	void draw_ex7(BLContext* ctx);
	void draw_t1(BLContext* ctx);
};

//===========================================================================
bl_drawer::bl_drawer()
{
	BLResult result;


	result = _font_face.createFromFile("C:/Windows/Fonts/malgun.ttf");
	if (result != BL_SUCCESS)
	{
		printf("_font_face.createFromFile() (%u)\n", result);
	}

	_font.createFromFace(_font_face, 50.0f);

	_contents_cx = 1920 * 2;
	_contents_cy = 1080 * 2;
}

bl_drawer::~bl_drawer()
{

}

void bl_drawer::draw(bl::context* ctx)
{
	draw_ex5(ctx);
	draw_ex7(ctx);
	draw_t1(ctx);
}

void bl_drawer::draw_ex5(BLContext* ctx)
{
	// First shape filled with a radial gradient.
	// By default, SRC_OVER composition is used.
	ctx->setCompOp(BL_COMP_OP_SRC_OVER);

	BLGradient radial(
		BLRadialGradientValues(180, 180, 180, 180, 180));
	radial.addStop(0.0, BLRgba32(0xFFFFFFFF));
	radial.addStop(1.0, BLRgba32(0xFFFF6F3F));
	ctx->fillCircle(180, 180, 160, radial);

	// Second shape filled with a linear gradient.
	BLGradient linear(
		BLLinearGradientValues(195, 195, 470, 470));
	linear.addStop(0.0, BLRgba32(0xFFFFFFFF));
	linear.addStop(1.0, BLRgba32(0xFF3F9FFF));

	// Use 'setCompOp()' to change a composition operator.
	ctx->setCompOp(BL_COMP_OP_DIFFERENCE);
	ctx->fillRoundRect(
		BLRoundRect(195, 195, 270, 270, 25), linear);

	ctx->setCompOp(BL_COMP_OP_SRC_OVER);
}

void bl_drawer::draw_ex7(BLContext* ctx)
{
	const char* regularText = reinterpret_cast<const char*>("Hello Blend2D!");
	const char* rotatedText = reinterpret_cast<const char*>(u8"Rotated Text 한글");

	BLContextCookie cookie1;
	BLContextCookie cookie2;

	double x = 200;
	double y = 400;


	ctx->save(cookie1);
	{
		ctx->scale(0.5);
		{
			ctx->setFillStyle(BLRgba32(0xFFFF0000));
			ctx->fillUtf8Text(BLPoint(x, y), _font, regularText);
		}

		ctx->save(cookie2);
		{
			ctx->scale(0.5);
			{
				ctx->setFillStyle(BLRgba32(0xFFFFFFFF));
				ctx->fillUtf8Text(BLPoint(x, y), _font, regularText);
			}
		}
		ctx->restore(cookie2);
	}
	ctx->restore(cookie1);


	ctx->save(cookie1);
	{
		//BLFont _font;
		//_font.createFromFace(_font_face, 50.0f);

		ctx->setFillStyle(BLRgba32(0xFF0000FF));
		ctx->fillUtf8Text(BLPoint(x, y), _font, regularText);

		ctx->translate(x, y + 50);
		ctx->rotate(0.785398);
		ctx->fillUtf8Text(BLPoint(0, 0), _font, rotatedText);
	}
	ctx->restore(cookie1);
}

void bl_drawer::draw_t1(BLContext* ctx)
{
	//ctx->fillAll(BLRgba32(0xFF000000u));
	double _angle = 45;

	BLPath p;

	int count = 1000;
	double PI = 3.14159265359;

	double cx = _contents_cx / 2.0f;
	double cy = _contents_cy / 2.0f;
	double maxDist = 1000.0;
	double baseAngle = _angle / 180.0 * PI;

	for (int i = 0; i < count; i++) {
		double t = double(i) * 1.01 / 1000;
		double d = t * 1000.0 * 0.4 + 10;
		double a = baseAngle + t * PI * 2 * 20;
		double x = cx + std::cos(a) * d;
		double y = cy + std::sin(a) * d;
		double r = std::min(t * 8 + 0.5, 10.0);
		p.addCircle(BLCircle(x, y, r));
	}

	ctx->fillPath(p, BLRgba32(0xFF00FFFFu));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr UINT_PTR BLWND_TIMER_EVENTID = 1;
constexpr UINT     BLWND_TIMER_ELAPSE = 500;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define bl_window_get()  (reinterpret_cast<bl::window_handler*>(_bl_window))
#define bl_window_set(p) (_bl_window = reinterpret_cast<void*>(p))





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CBLWnd::CBLWnd():
	_bl_window(nullptr),
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
	
	// cs.style |= (WS_VSCROLL | WS_HSCROLL);
}

int CBLWnd::OnCreate(CREATESTRUCT& cs)
{
	//-----------------------------------------------------------------------
	UNREFERENCED_PARAMETER(cs);


	//-----------------------------------------------------------------------
	bl_drawer* drawer;


	drawer = new bl_drawer();


	bl_window_set(new bl::window_handler());

	bl_window_get()->_window.set_drawer(drawer);

	bl_window_get()->OnCreate(GetHwnd(), WM_CREATE, 0, 0);

	bl_window_get()->_window.set_contents_size(drawer->_contents_cx, drawer->_contents_cy);
	bl_window_get()->_window.fit_contents_to_window(true);
	bl_window_get()->_window.enable_scrollbar(true);


	//-----------------------------------------------------------------------
	// SetTimer(BLWND_TIMER_EVENTID, BLWND_TIMER_ELAPSE, NULL);

	// ShowScrollBar(_hwnd, SB_BOTH, TRUE);

	return 0;
}

void CBLWnd::OnDestroy()
{
	//-----------------------------------------------------------------------
	// KillTimer(BLWND_TIMER_EVENTID);


	bl_window_get()->OnDestroy(GetHwnd(), WM_DESTROY, 0, 0);

	
	bl_drawer* drawer;


	drawer = reinterpret_cast<bl_drawer*>(bl_window_get()->_window.get_drawer());
	if (drawer)
	{
		delete drawer;
	}


	if (bl_window_get())
	{
		delete bl_window_get();

		bl_window_set(nullptr);
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
		bl_window_get()->OnSize(GetHwnd(), msg, wparam, lparam);
		Invalidate(FALSE);
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(msg);
	// UNREFERENCED_PARAMETER(wparam);
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


	bl_window_get()->OnPaint(GetHwnd(), msg, wparam, lparam);


	return 0;
}

LRESULT CBLWnd::OnEraseBkgnd(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// return WndProcDefault(msg, wparam, lparam);
	return 1;
}

LRESULT CBLWnd::OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	bl_window_get()->OnHScroll(GetHwnd(), msg, wparam, lparam);

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnVScroll(UINT msg, WPARAM wparam, LPARAM lparam)
{
	bl_window_get()->OnVScroll(GetHwnd(), msg, wparam, lparam);

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CBLWnd::OnMouseWheel(UINT msg, WPARAM wparam, LPARAM lparam)
{
	bl_window_get()->OnMouseWheel(GetHwnd(), msg, wparam, lparam);

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


