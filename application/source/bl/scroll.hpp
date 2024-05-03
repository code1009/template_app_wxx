#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace bl
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if  0
https://www.codeproject.com/Articles/1042516/Custom-Controls-in-Win-API-Scrolling

typedef struct tagSCROLLINFO {
	UINT cbSize;
	UINT fMask;
	int  nMin;
	int  nMax;
	UINT nPage;
	int  nPos;
	int  nTrackPos;
} SCROLLINFO, * LPSCROLLINFO;



https://www.catch22.net/tuts/win32/64bit-scrollbars/

void example(HWND hwnd)
{
	UINT64 max = 0xffffffffffffffff;
	UINT64 pos = 0x1234567812345678;

	SetScrollInfo64(hwnd, SB_VERT, SIF_ALL, max, pos, 15, TRUE);

	// when reacting to a WM_VSCROLL, with SB_THUMBTRACK/SB_THUMBPOS:
	pos = GetScrollPos64(hwnd, SB_VERT, SIF_TRACKPOS, max);
}

#endif

#define WIN16_SCROLLBAR_MAX 0x7fff
#define WIN32_SCROLLBAR_MAX 0x7fffffff

// Wrapper around SetScrollInfo, performs scaling to 
// allow massive 64bit scroll ranges
BOOL SetScrollInfo64(HWND hwnd,
	int nBar,
	UINT fMask,
	UINT64 nMax64,
	UINT64 nPos64,
	UINT64 nPage,
	BOOL fRedraw
)
{
	SCROLLINFO si = { static_cast<UINT>(sizeof(si)), fMask };

	// normal scroll range requires no adjustment
	if (nMax64 <= WIN32_SCROLLBAR_MAX)
	{
		si.nMin = (int)0;
		si.nMax = (int)nMax64;
		si.nPage = (int)nPage;
		si.nPos = (int)nPos64;
	}
	// scale the scrollrange down into allowed bounds
	else
	{
		si.nMin = (int)0;
		si.nMax = (int)WIN16_SCROLLBAR_MAX;
		si.nPage = (int)nPage;
		si.nPos = (int)(nPos64 / (nMax64 / WIN16_SCROLLBAR_MAX));
	}

	return SetScrollInfo(hwnd, nBar, &si, fRedraw);
}

UINT64 GetScrollPos64(HWND hwnd,
	int nBar,
	UINT fMask,
	UINT64 nMax64
)
{
	SCROLLINFO si = { static_cast<UINT>(sizeof(si)), fMask | SIF_PAGE };
	UINT64 nPos32;


	if (!GetScrollInfo(hwnd, nBar, &si))
	{
		return 0;
	}


	nPos32 = (fMask & SIF_TRACKPOS) ? si.nTrackPos : si.nPos;

	// special-case: scroll position at the very end
	if (nPos32 == WIN16_SCROLLBAR_MAX - si.nPage + 1)
	{
		return nMax64 - si.nPage + 1;
	}// normal scroll range requires no adjustment
	else if (nMax64 <= WIN32_SCROLLBAR_MAX)
	{
		return nPos32;
	}
	// adjust the scroll position to be relative to maximum value
	else
	{
		return nPos32 * (nMax64 / WIN16_SCROLLBAR_MAX);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::int64_t on_scroll(
	std::uint32_t scroll_code, std::int64_t pos,
	std::uint64_t _view_y_scroll_page,
	std::uint64_t _view_y_scroll_line,
	std::uint64_t _view_y_scroll_min,
	std::uint64_t _view_y_scroll_max,
	std::uint64_t _view_y
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


	view_scroll_page = _view_y_scroll_page;
	view_scroll_line = _view_y_scroll_line;


	view_scroll_min = _view_y_scroll_min;
	view_scroll_max = _view_y_scroll_max;


	view_scroll_top = view_scroll_min;
	if (view_scroll_page < view_scroll_max)
	{
		view_scroll_bottom = view_scroll_max - view_scroll_page;
	}
	else
	{
		view_scroll_bottom = view_scroll_min;
	}


	view_scroll_pos = _view_y;
	view_scroll_pos_current = _view_y;


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
		view_scroll_pos_current = pos;
		break;

	case SB_THUMBPOSITION:
		view_scroll_pos_current = pos;
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




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
