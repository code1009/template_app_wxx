/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

//===========================================================================
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

//===========================================================================
#include <blend2d.h>

//===========================================================================
#include "stopwatch.hpp"
#include "scroll.hpp"
#include "bl.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace bl
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bitmap::~bitmap()
{
	//OutputDebugStringA("~bitmap \n");

	destroy();
}

//===========================================================================
void bitmap::create(void)
{
	memset(&_bmi, 0, sizeof(_bmi));
	_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bmi.bmiHeader.biWidth = static_cast<LONG>(_cx);
	_bmi.bmiHeader.biHeight = -static_cast<LONG>(_cy);
	_bmi.bmiHeader.biPlanes = 1;
	_bmi.bmiHeader.biCompression = BI_RGB;
	_bmi.bmiHeader.biBitCount = static_cast<WORD>(_color_bits);


	auto raw_scanline_width_in_bits{ _cx * _color_bits };

	auto aligned_bits{ 32 - 1 };
	auto aligned_scanline_width_in_bits{ (raw_scanline_width_in_bits + aligned_bits) & ~aligned_bits };
	auto aligned_scanline_width_in_bytes{ aligned_scanline_width_in_bits / 8 };


	_aligned_scanline_cx_bytes = aligned_scanline_width_in_bytes;


	_data_size = _aligned_scanline_cx_bytes * _cy;

	if (_data_size)
	{
		_data = new (std::nothrow) std::uint8_t[_data_size];
	}
	else
	{
		_data = nullptr;
	}
}

void bitmap::destroy(void)
{
	if (nullptr != _data)
	{
		delete[] _data;
	}
	_data = nullptr;
}

//===========================================================================
bool bitmap::is_empty(void)
{
	if (nullptr == _data)
	{
		return true;
	}

	return false;
}

void bitmap::set_size(std::size_t cx, std::size_t cy)
{
	if (!is_empty())
	{
		destroy();
	}


	_cx = cx;
	_cy = cy;


	create();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
canvas::~canvas()
{
	//OutputDebugStringA("~canvas \n");
}

//===========================================================================
void canvas::set_size(std::size_t cx, std::size_t cy)
{
	bool reset = false;


	_cx = cx;
	_cy = cy;


	_bitmap.set_size(cx, cy);
		
	if (_bitmap.get_data())
	{
		BLResult result;


		result = _image.createFromData(
			static_cast<int>(_cx), static_cast<int>(_cy),
			BL_FORMAT_PRGB32,
			_bitmap.get_data(),
			_bitmap.get_scanline_bytes()
		);
		if (result != BL_SUCCESS)
		{
			printf("_image.createFromData() (%u)\n", result);
		}
	}
}

//===========================================================================
BLContext* canvas::begin(void)
{
	if (_bitmap.get_data())
	{
		BLContextCreateInfo createInfo{};


		createInfo.threadCount = 8;
			
		//_context.begin(_image, createInfo);
		_context = BLContext(_image, createInfo);

		_context_ptr = &_context;
	}
	else
	{
		_context_ptr = nullptr;
	}

	/*
	BLResult result;

	result = _image.createFromData(
		static_cast<int>(_cx), static_cast<int>(_cy),
		BL_FORMAT_PRGB32,
		_bitmap.get_data(),
		_bitmap.get_scanline_bytes()
	);
	if (result != BL_SUCCESS)
	{
		printf("_image.createFromData() (%u)\n", result);
	}
	*/

	return _context_ptr;
}

void canvas::end(void)
{
	if (_context_ptr)
	{
		_context_ptr->end();
	}

	_context_ptr = nullptr;
}

//===========================================================================
void canvas::paint(HDC hdc)
{
	if (_bitmap.get_data())
	{
		StretchDIBits(hdc,
			static_cast<int>(0), static_cast<int>(0), static_cast<int>(_cx), static_cast<int>(_cy),
			static_cast<int>(0), static_cast<int>(0), static_cast<int>(_cx), static_cast<int>(_cy),
			_bitmap.get_data(),
			_bitmap.get_bitmap_info(),
			DIB_RGB_COLORS, SRCCOPY);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window::~window()
{
	//OutputDebugStringA("~window \n");
}

//===========================================================================
void window::create(HWND hwnd)
{
	//-----------------------------------------------------------------------
	_hwnd = hwnd;


	//-----------------------------------------------------------------------
	BLResult result;


	result = _status_font_face.createFromFile("C:/Users/USER/AppData/Local/Microsoft/Windows/Fonts/NanumGothicCoding-Bold.ttf");
	if (result != BL_SUCCESS)
	{
		printf("_font_face.createFromFile() (%u)\n", result);
	}
		

	_underlay_font.createFromFace(_status_font_face, 12.0f);
	_overlay_font.createFromFace(_status_font_face, 16.0f);
}

void window::destory()
{
	//-----------------------------------------------------------------------
	_hwnd = nullptr;
}

//===========================================================================
void window::set_window_size(int cx, int cy)
{
	_window_cx = cx;
	_window_cy = cy;

		
	_canvas.set_size(cx, cy);
		
		
	update_view_size();
	update_view_offset();
	update_view_scroll();
	update_scrollbar();


	//repaint(); 호출 금지
	//윈도우 최초 실행을 제외 하고
	//WM_SIZE이후 WM_PAINT호출되기떄문에 다시 그릴 필요 없음
}

//===========================================================================
double window::get_scale(void)
{
	return _scale;
}
	
void window::set_scale(double s)
{
	if (_scale > 0.0)
	{
		_scale = s;

		update_view_size();
		update_view_offset();
		update_view_scroll();
		update_scrollbar();

		repaint();
	}
}

void window::set_contents_size(double cx, double cy)
{
	_contents_x  = 0;
	_contents_y  = 0;
	_contents_cx = cx;
	_contents_cy = cy;

	update_view_size();
	update_view_offset();
	update_view_scroll();
	update_scrollbar();

	repaint();
}

//===========================================================================
void window::enable_scrollbar(bool enable)
{
	_scrollbar_enabled = enable;

	update_view_scroll();
	update_scrollbar();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::int64_t window::scroll(
	std::uint32_t scroll_code, std::int64_t scroll_pos,
	std::uint64_t a_view_scroll_page,
	std::uint64_t a_view_scroll_line,
	std::uint64_t a_view_scroll_min,
	std::uint64_t a_view_scroll_max,
	std::uint64_t a_view_scroll_pos
)
{
	std::int64_t view_scroll_min;
	std::int64_t view_scroll_max;
	std::int64_t view_scroll_top;
	std::int64_t view_scroll_bottom;
	std::int64_t view_scroll_pos;
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_page;
	std::int64_t view_scroll_line;


	view_scroll_page = a_view_scroll_page;
	view_scroll_line = a_view_scroll_line;


	view_scroll_min = a_view_scroll_min;
	view_scroll_max = a_view_scroll_max;


	view_scroll_top = view_scroll_min;
	if (view_scroll_page < view_scroll_max)
	{
		view_scroll_bottom = view_scroll_max - view_scroll_page;
	}
	else
	{
		view_scroll_bottom = view_scroll_min;
	}


	view_scroll_pos = a_view_scroll_pos;
	view_scroll_pos_current = a_view_scroll_pos;


	switch (scroll_code)
	{
	case SB_TOP:
		view_scroll_pos_current = view_scroll_top;
		break;

	case SB_BOTTOM:
		view_scroll_pos_current = view_scroll_bottom;
		break;

	case SB_LINEUP:
		if (view_scroll_top < (view_scroll_pos - view_scroll_line))
		{
			view_scroll_pos_current = view_scroll_pos - view_scroll_line;
		}
		else
		{
			view_scroll_pos_current = view_scroll_top;
		}
		break;

	case SB_LINEDOWN:
		if ((view_scroll_pos + view_scroll_line) < view_scroll_bottom)
		{
			view_scroll_pos_current = view_scroll_pos + view_scroll_line;
		}
		else
		{
			view_scroll_pos_current = view_scroll_bottom;
		}
		break;

	case SB_PAGEUP:
		if (view_scroll_top < (view_scroll_pos - view_scroll_page))
		{
			view_scroll_pos_current = view_scroll_pos - view_scroll_page;
		}
		else
		{
			view_scroll_pos_current = view_scroll_top;
		}
		break;

	case SB_PAGEDOWN:
		if ((view_scroll_pos + view_scroll_page) < view_scroll_bottom)
		{
			view_scroll_pos_current = view_scroll_pos + view_scroll_page;
		}
		else
		{
			view_scroll_pos_current = view_scroll_bottom;
		}
		break;

	case SB_THUMBTRACK:
		view_scroll_pos_current = scroll_pos;
		break;

	case SB_THUMBPOSITION:
		view_scroll_pos_current = scroll_pos;
		break;

	case SB_ENDSCROLL:
		break;
	}

	if (view_scroll_pos_current < view_scroll_top)
	{
		view_scroll_pos_current = view_scroll_top;
	}

	if (view_scroll_bottom < view_scroll_pos_current)
	{
		view_scroll_pos_current = view_scroll_bottom;
	}


	return view_scroll_pos_current;
}

void window::vscroll(std::uint32_t scroll_code)
{
	std::int64_t pos;


	pos = GetScrollPos64(_hwnd, SB_VERT, SIF_TRACKPOS, _view_y_scroll_max);


	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _view_y;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_view_y_scroll_page,
		_view_y_scroll_line,
		_view_y_scroll_min,
		_view_y_scroll_max,
		_view_y
		);


	if (view_scroll_pos != view_scroll_pos_current)
	{
		_view_y = view_scroll_pos_current;
		_contents_y = view_scroll_pos_current / _scale;

		update_scrollbar();

		repaint();
	}
}

void window::hscroll(std::uint32_t scroll_code)
{
	std::int64_t pos;


	pos = GetScrollPos64(_hwnd, SB_HORZ, SIF_TRACKPOS, _view_x_scroll_max);


	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _view_x;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_view_x_scroll_page,
		_view_x_scroll_line,
		_view_x_scroll_min,
		_view_x_scroll_max,
		_view_x
	);


	if (view_scroll_pos != view_scroll_pos_current)
	{
		_view_x = view_scroll_pos_current;
		_contents_x = view_scroll_pos_current / _scale;

		update_scrollbar();

		repaint();
	}
}

void window::update_scrollbar(void)
{
	SetScrollInfo64(_hwnd, SB_HORZ, SIF_ALL, _view_x_scroll_max, _view_x, _view_x_scroll_page, TRUE);
	SetScrollInfo64(_hwnd, SB_VERT, SIF_ALL, _view_y_scroll_max, _view_y, _view_y_scroll_page, TRUE);
}
	
void window::update_view_scroll(void)
{
	if (_scrollbar_enabled)
	{
		if (_window_cx < _view_cx)
		{
			_view_x_scroll_min = 0;
			_view_x_scroll_max = _view_cx;
			_view_x_scroll_page = _window_cx;
			_view_x_scroll_line = 10;
		}
		else
		{
			_view_x_scroll_min = 0;
			_view_x_scroll_max = 0;
			_view_x_scroll_page = 0;
			_view_x_scroll_line = 0;
		}


		if (_window_cy < _view_cy)
		{
			_view_y_scroll_min = 0;
			_view_y_scroll_max = _view_cy;
			_view_y_scroll_page = _window_cy;
			_view_y_scroll_line = 10;
		}
		else
		{
			_view_y_scroll_min = 0;
			_view_y_scroll_max = 0;
			_view_y_scroll_page = 0;
			_view_y_scroll_line = 0;
		}
	}
	else
	{
		_view_x_scroll_min = 0;
		_view_x_scroll_max = 0;
		_view_x_scroll_page = 0;
		_view_x_scroll_line = 0;

		_view_y_scroll_min = 0;
		_view_y_scroll_max = 0;
		_view_y_scroll_page = 0;
		_view_y_scroll_line = 0;
	}
}

void window::update_view_size(void)
{
	_view_cx = static_cast<std::int64_t>(_contents_cx * _scale);
	_view_cy = static_cast<std::int64_t>(_contents_cy * _scale);
}

void window::update_view_offset(void)
{
	_view_x = static_cast<std::int64_t>(_contents_x * _scale);
	if (_view_cx < _window_cx)
	{
		_view_x = 0;
	}
	else
	{
		if (_view_cx < (_view_x + _window_cx))
		{
			_view_x = _view_cx - _window_cx;
		}
	}


	_view_y = static_cast<std::int64_t>(_contents_y * _scale);
	if (_view_cy < _window_cy)
	{
		_view_y = 0;
	}
	else
	{
		if (_view_cy < (_view_y + _window_cy))
		{
			_view_y = _view_cy - _window_cy;
		}
	}

	_contents_x = _view_x / _scale;
	_contents_y = _view_y / _scale;
}

//===========================================================================
void window::fit_contents_to_window(bool vert)
{
	if (vert)
	{
		if (_window_cy)
		{
			set_scale(_window_cy / _contents_cy);
		}
	}
	else
	{
		if (_window_cx)
		{
			set_scale(_window_cx / _contents_cx);
		}
	}
}

void window::zoom(bool zoom_in)
{
	double viewscale_delta;
	double viewscale_max;
	double viewscale_min;


	viewscale_max = 10.0f;
	viewscale_min = 0.1f;
	viewscale_delta = 0.1f;


	double viewscale;
	int viewscale_x10;


	viewscale = get_scale();
	viewscale_x10 = static_cast<int>((viewscale + 0.05) * 10);
	viewscale = viewscale_x10 / 10.0;


	if (zoom_in)
	{
		viewscale = viewscale + viewscale_delta;
	}
	else
	{
		viewscale = viewscale - viewscale_delta;
	}


	if (viewscale > viewscale_max)
	{
		viewscale = viewscale_max;
	}
	if (viewscale < viewscale_min)
	{
		viewscale = viewscale_min;
	}


	set_scale(viewscale);
}

void window::window_to_contents(std::int64_t window_x, std::int64_t window_y, double& contents_x, double& contents_y)
{
	contents_x = _contents_x + (window_x / _scale);
	contents_y = _contents_y + (window_y / _scale);
}

void window::contents_to_window(double contents_x, double contents_y, std::int64_t& window_x, std::int64_t& window_y)
{
	window_x = static_cast<std::int64_t>((contents_x - _contents_x) * _scale);
	window_y = static_cast<std::int64_t>((contents_y - _contents_y) * _scale);
}

//===========================================================================
void window::repaint(void)
{
	//_canvas.get_context()->reset();
	//InvalidateRect(_hwnd, nullptr, TRUE);
	//UpdateWindow(_hwnd);

	/*
	HDC hdc = GetDC(_hwnd);
	paint(hdc);
	ReleaseDC(_hwnd, hdc);
	*/

	InvalidateRect(_hwnd, nullptr, TRUE);
}

void window::paint(HDC hdc)
{
	stopwatch sw("paint");

	sw.start();
	{
		//scoped_time_measurer stm(&sw);


		BLContext* ctx;


		ctx = _canvas.begin();
		if (!ctx)
		{
			return;
		}
		draw(ctx);
		_canvas.end();


		{
			//stopwatch sw("gdi");
			//scoped_time_measurer stm(&sw);

			_canvas.paint(hdc);
		}	
	}
	sw.stop();
	sw.measure();

	_paint_time_usec = sw._duration.count(); // 1500~5100 usec
}

void window::draw(BLContext* ctx)
{
	//stopwatch sw("blend2d");
	//scoped_time_measurer stm(&sw);


	ctx->clearAll();

	draw_underlay(ctx);
	draw_contents(ctx);
	draw_overlay(ctx);
}

void window::draw_underlay(BLContext* ctx)
{
	BLContextCookie context_cookie;


	ctx->save(context_cookie);


	draw_window_grid(ctx);


	ctx->restore(context_cookie);
}

void window::draw_overlay(BLContext* ctx)
{
	BLContextCookie context_cookie;


	ctx->save(context_cookie);

		
	draw_window_info(ctx);


	ctx->restore(context_cookie);
}

void window::draw_window_info(BLContext* ctx)
{
	std::int64_t x;
	std::int64_t y;
	std::int64_t line;

	std::int64_t text_offset_x;
	std::int64_t text_offset_y;

	char label[128];


	line = 8;

	text_offset_x = 10;
	text_offset_y = 25;

	x = _window_cx - 360;
	y = _window_cy - (line * text_offset_y);


	ctx->setFillStyle(BLRgba32(0xC00000FF));

	/*
	ctx->fillBox(
		static_cast<double>(x), static_cast<double>(y),
		static_cast<double>(_window_cx), static_cast<double>(_window_cy)
	);
	*/

	ctx->fillRoundRect(
		static_cast<double>(x), static_cast<double>(y),
		static_cast<double>(360 - 10), static_cast<double>(line * text_offset_y - 10),
		5
		);


	ctx->setFillStyle(BLRgba32(0xFFFFFFFF));


	sprintf_s(label, "render   = %lld usec", _paint_time_usec);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;


	sprintf_s(label, "window   = (%lld, %lld)", _window_cx, _window_cy);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

	sprintf_s(label, "contents = (%.1f, %.1f)-(%.1f, %.1f)", _contents_x, _contents_y, _contents_cx, _contents_cy);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

	sprintf_s(label, "scale    = %.1f", _scale);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

	sprintf_s(label, "view     = (%lld, %lld)-(%lld, %lld)", _view_x, _view_y, _view_cx, _view_cy);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

	sprintf_s(label, "scroll:x = %lld, %lld, (%lld~%lld)", 
		_view_x_scroll_line,
		_view_x_scroll_page,
		_view_x_scroll_min, _view_x_scroll_max
	);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

	sprintf_s(label, "scroll:y = %lld, %lld, (%lld~%lld)",
		_view_y_scroll_line, 
		_view_y_scroll_page,
		_view_y_scroll_min, _view_y_scroll_max
	);
	ctx->fillUtf8Text(BLPoint(static_cast<double>(x + text_offset_x), y + static_cast<double>(text_offset_y)), _overlay_font, label);
	y += text_offset_y;

}

void window::draw_window_grid(BLContext* ctx)
{
	if (1.0 < _scale)
	{
		return;
	}


	std::int64_t x;
	std::int64_t y;
	std::int64_t cx;
	std::int64_t cy;

	std::int64_t text_offset_x;
	std::int64_t text_offset_y;

	double contents_x;
	double contents_y;

	char label[128];


	text_offset_x = 5;
	text_offset_y = 15;

	cx = _window_cx;
	cy = _window_cy;

	ctx->setStrokeWidth(1);
	ctx->setStrokeStyle(BLRgba32(0x20FFFFFF));
	ctx->setFillStyle(BLRgba32(0xFFFFFFFF));
	for (y = 0; y < cy; y += 100)
	{
		for (x = 0; x < cx; x += 100)
		{
			ctx->strokeLine(BLPoint(static_cast<double>(x), static_cast<double>(0)), BLPoint(static_cast<double>(x), static_cast<double>(cy)));
			ctx->strokeLine(BLPoint(static_cast<double>(0), static_cast<double>(y)), BLPoint(static_cast<double>(cx), static_cast<double>(y)));


			contents_x = _contents_x + x / _scale;
			contents_y = _contents_y + y / _scale;
			sprintf_s(label, "(%.1f, %.1f)", contents_x, contents_y);
			ctx->fillUtf8Text(
				BLPoint(static_cast<double>(x + text_offset_x), static_cast<double>(y + text_offset_y)),
				_underlay_font, label);
		}
	}
}

void window::draw_contents_grid(BLContext* ctx)
{
	if (_scale <= 1.0)
	{
		return;
	}


	double x;
	double y;

	double cx;
	double cy;

	double text_offset_x;
	double text_offset_y;

	char label[128];


	text_offset_x = 5.0;
	text_offset_y = 15.0;

	cx = _contents_x + _window_cx / _scale;
	cy = _contents_y + _window_cy / _scale;

	ctx->setStrokeWidth(1);
	//ctx->setStrokeStartCap(BL_STROKE_CAP_ROUND);
	//ctx->setStrokeEndCap(BL_STROKE_CAP_BUTT);
	ctx->setStrokeStyle(BLRgba32(0x20FFFFFF));
	ctx->setFillStyle(BLRgba32(0xFFFFFFFF));
	for (y = 0.0; y < cy; y += 100.0)
	{
		for (x = 0.0; x < cx; x += 100.0)
		{
			ctx->strokeLine(BLPoint(x, 0), BLPoint(x, cy));
			ctx->strokeLine(BLPoint(0, y), BLPoint(cx, y));


			sprintf_s(label, "(%.1f, %.1f)", x, y);
			ctx->fillUtf8Text(BLPoint(x + text_offset_x, y + text_offset_y), _underlay_font, label);
		}
	}
}

void window::draw_contents(BLContext* ctx)
{
	BLContextCookie context_cookie;


	ctx->save(context_cookie);


	ctx->scale(_scale);
	ctx->translate(-_contents_x, -_contents_y);


	draw_contents_background(ctx);
	draw_contents_foreground(ctx);


	ctx->restore(context_cookie);
}

void window::draw_contents_background(BLContext* ctx)
{
	BLContextCookie context_cookie;


	ctx->save(context_cookie);


	ctx->setFillStyle(BLRgba32(0x40808080));
	ctx->fillBox(0, 0, _contents_cx, _contents_cy);

	draw_contents_grid(ctx);


	ctx->restore(context_cookie);
}

void window::draw_contents_foreground(BLContext* ctx)
{
	BLContextCookie context_cookie;


	ctx->save(context_cookie);

	if (_drawer)
	{
		_drawer->draw(ctx);
	}

	ctx->restore(context_cookie);
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window_handler::~window_handler()
{
	// OutputDebugStringA("~window_handler \n");
}

void window_handler::OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//-----------------------------------------------------------------------
	_window.create(hWnd);


	//-----------------------------------------------------------------------
	RECT rect;


	GetClientRect(hWnd, &rect);


	// 최초 윈도우 생성시 WM_PAINT가 먼저 수행 되고
	// WM_SIZE가 나중에 수행 된다.
	// 최초 실행시 윈도우 크기를 먼저 지정하지 않으면,
	// 크기가 지정되지 않은 채로 그리기 동작이 먼저 수행한다.
	// 따라서, 먼저 윈도우 크기를 지정한다.
	// 그 이후에 WM_SIZE 이후에 WM_PAINT가 수행 된다.

	_window.set_window_size(rect.right, rect.bottom);
	//_window.set_contents_size(1920*4, 1080*4);
	//_window.fit_contents_to_window(true);
	//_window.enable_scrollbar(true);
}

void window_handler::OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_window.destory();
}

void window_handler::OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nType = (UINT)wParam;
	SIZE size = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }; // lParam specifies the new of the client area


	_window.set_window_size(size.cx, size.cy);
	//_window.fit_contents_to_window(true);
}

void window_handler::OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	
	
	hdc = BeginPaint(hWnd, &ps);


	_window.paint(hdc);


	EndPaint(hWnd, &ps);
}

void window_handler::OnHScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nSBCode    = (int)LOWORD(wParam);
	UINT nPos       = (short)(HIWORD(wParam));
	HWND pScrollBar = (HWND)lParam;


	_window.hscroll(nSBCode);
}

void window_handler::OnVScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT nSBCode    = (int)LOWORD(wParam);
	UINT nPos       = (short)HIWORD(wParam);
	HWND pScrollBar = (HWND)lParam ;


	_window.vscroll(nSBCode);
}

void window_handler::OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT  nFlags = (UINT)LOWORD(wParam);
	short zDelta = (short)HIWORD(wParam);
	POINT pt     = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };


	bool scale = false;

	switch (nFlags)
	{
	case MK_CONTROL:
		scale = true;
		break;

	case MK_LBUTTON:
	case MK_RBUTTON:
		break;
	case MK_MBUTTON:
		break;

	case MK_SHIFT:
		break;

	case MK_XBUTTON1:
	case MK_XBUTTON2:
		break;

	default:
		break;
	}

	if (scale)
	{
		if (zDelta > 0)
		{
			_window.zoom(true);
		}
		else
		{
			_window.zoom(false);
		}
	}
	else
	{
		if (zDelta > 0)
		{
			_window.vscroll(SB_LINEUP);
		}
		else
		{
			_window.vscroll(SB_LINEDOWN);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
