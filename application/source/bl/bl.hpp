#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <blend2d.h>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace bl
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class bitmap32
{
private:
	std::uint8_t* _data{ nullptr };
	std::size_t _data_size{ 0 };
	BITMAPINFO _bmi{};

private:
	std::size_t       _cy{ 0 };
	std::size_t       _cx{ 0 };
	std::size_t       _aligned_scanline_cx_bytes{ 0 };
	const std::size_t _color_bits = 32;

public:
	virtual ~bitmap32();

private:
	void create(void);
	void destroy(void);

public:
	bool is_empty(void);
	void set_size(std::size_t cx, std::size_t cy);

public:
	std::size_t get_scanline_bytes(void)
	{
		return _aligned_scanline_cx_bytes;
	}

	std::uint8_t* get_data(void)
	{
		return _data;
	}

	BITMAPINFO* get_bitmap_info(void)
	{
		return &_bmi;
	}

	std::size_t get_cx(void)
	{
		return _cx;
	}

	std::size_t get_cy(void)
	{
		return _cy;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class canvas
{
private:
	bitmap32 _bitmap{};

private:
	BLImage _image{};
	BLContext _context{};
	BLContext* _context_ptr{ nullptr };

private:
	std::size_t _cy{ 0 };
	std::size_t _cx{ 0 };

public:
	virtual ~canvas();

public:
	void set_size(std::size_t cx, std::size_t cy);
	BLContext* begin(void);
	void end(void);
	void paint(HDC hdc);
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class window
{
private:
	double _contents_x{ 0 };
	double _contents_y{ 0 };

	double _contents_cx{ 0 };
	double _contents_cy{ 0 };

	double _scale{ 1.0 };

	std::int64_t _view_x{ 0 };
	std::int64_t _view_y{ 0 };

	std::int64_t _view_cx{ 0 };
	std::int64_t _view_cy{ 0 };

	std::int64_t _view_x_scroll_min{ 0 };
	std::int64_t _view_x_scroll_max{ 0 };
	std::int64_t _view_x_scroll_page{ 0 };
	std::int64_t _view_x_scroll_line{ 0 };

	std::int64_t _view_y_scroll_min{ 0 };
	std::int64_t _view_y_scroll_max{ 0 };
	std::int64_t _view_y_scroll_page{ 0 };
	std::int64_t _view_y_scroll_line{ 0 };

	std::int64_t _window_cx{ 0 };
	std::int64_t _window_cy{ 0 };

	std::int64_t _paint_time_usec{ 0 };

private:
	canvas _canvas;
	BLFontFace _font_face;
	BLFontFace _font_face_status;
	BLFont _font;
	BLFont _underlay_font;
	BLFont _overlay_font;
	HWND _hwnd;
	bool _scrollbar_enabled{ false };

public:
	virtual ~window();

public:
	void create(HWND hwnd);
	void destory();

public:
	void set_window_size(int cx, int cy);
	double get_scale(void);
	void set_scale(double s);
	void set_contents_size(double cx, double cy);
	void enable_scrollbar(bool enable);
	void fit_contents_to_window(bool vert = false);
	void on_vscroll(std::uint32_t scroll_code);
	void on_hscroll(std::uint32_t scroll_code);

private:
	void update_scrollbar(void);
	void update_view_scroll(void);
	void update_view_size(void);
	void update_view_offset(void);

	void repaint(void);

	//--------------------------------------------------------------------------
public:
	void window_to_contents(std::int64_t window_x, std::int64_t window_y, double& contents_x, double& contents_y);
	void contents_to_window(double contents_x, double contents_y, std::int64_t& window_x, std::int64_t& window_y);

	//--------------------------------------------------------------------------
public:
	void paint(HDC hdc);
	void draw(BLContext* ctx);
	void draw_underlay(BLContext* ctx);
	void draw_overlay(BLContext* ctx);
	void draw_window_info(BLContext* ctx);
	void draw_window_grid(BLContext* ctx);
	void draw_contents_grid(BLContext* ctx);
	void draw_contents(BLContext* ctx);
	void draw_contents_background(BLContext* ctx);
	void draw_contents_foreground(BLContext* ctx);

	void draw_ex5(BLContext* ctx);
	void draw_ex7(BLContext* ctx);
	void draw_t1(BLContext* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class window_handler
{
private:
	window _window;

public:
	virtual ~window_handler();

public:
	void OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void OnHScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnVScroll(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnMouseWheel(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




