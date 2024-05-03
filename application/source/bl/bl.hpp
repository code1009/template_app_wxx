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
class bitmap
{
private:
	std::uint8_t* _data{ nullptr };
	std::size_t   _data_size{ 0 };
	BITMAPINFO    _bmi{};

private:
	std::size_t       _cy{ 0 };
	std::size_t       _cx{ 0 };
	std::size_t       _aligned_scanline_cx_bytes{ 0 };
	const std::size_t _color_bits = 32;

public:
	virtual ~bitmap();

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
	bitmap _bitmap{};

private:
	BLImage    _image{};
	BLContext  _context{};
	BLContext* _context_ptr{ nullptr };

private:
	std::size_t _cy{ 0 };
	std::size_t _cx{ 0 };

public:
	virtual ~canvas();

public:
	void set_size(std::size_t cx, std::size_t cy);

public:
	BLContext* begin(void);
	void end(void);

public:
	void paint(HDC hdc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using context = BLContext;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class drawer
{
public:
	virtual ~drawer() {};

public:
	virtual void draw(context* ctx) = 0;
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

private:
	canvas  _canvas{};
	drawer* _drawer{ nullptr };

private:
	HWND         _hwnd{ nullptr };
	std::int64_t _paint_time_usec{ 0 };
	bool         _scrollbar_enabled{ false };

private:
	BLFontFace   _status_font_face{};
	BLFont       _underlay_font{};
	BLFont       _overlay_font{};

public:
	virtual ~window();

public:
	virtual void create(HWND hwnd);
	virtual void destory();

public:
	virtual void set_drawer(drawer* d)
	{
		_drawer = d;
	}

	virtual drawer* get_drawer(void) const
	{
		return _drawer;
	}

public:
	virtual void set_window_size(int cx, int cy);

public:
	virtual double get_scale(void);
	virtual void set_scale(double s);

public:
	virtual void set_contents_size(double cx, double cy);

public:
	virtual void enable_scrollbar(bool enable);
	virtual void vscroll(std::uint32_t scroll_code);
	virtual void hscroll(std::uint32_t scroll_code);

public:
	virtual void zoom(bool zoom_in);
	virtual void fit_contents_to_window(bool vert = false);

public:
	virtual void window_to_contents(std::int64_t window_x, std::int64_t window_y, double& contents_x, double& contents_y);
	virtual void contents_to_window(double contents_x, double contents_y, std::int64_t& window_x, std::int64_t& window_y);

protected:
	virtual std::int64_t scroll(
		std::uint32_t scroll_code, std::int64_t scroll_pos,
		std::uint64_t a_view_scroll_page,
		std::uint64_t a_view_scroll_line,
		std::uint64_t a_view_scroll_min,
		std::uint64_t a_view_scroll_max,
		std::uint64_t a_view_scroll_pos);
	virtual void update_scrollbar(void);
	virtual void update_view_scroll(void);
	virtual void update_view_size(void);
	virtual void update_view_offset(void);
	virtual void repaint(void);

public:
	virtual void paint(HDC hdc);

protected:
	virtual void draw(BLContext* ctx);
	virtual void draw_underlay(BLContext* ctx);
	virtual void draw_overlay(BLContext* ctx);
	virtual void draw_window_info(BLContext* ctx);
	virtual void draw_window_grid(BLContext* ctx);
	virtual void draw_contents_grid(BLContext* ctx);
	virtual void draw_contents(BLContext* ctx);
	virtual void draw_contents_background(BLContext* ctx);
	virtual void draw_contents_foreground(BLContext* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class window_handler
{
public:
	window _window{};

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




