/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "EventMessageDocker.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CXListViewInplaceEdit::CXListViewInplaceEdit(CXListView* listview, std::int32_t item, std::int32_t column, std::string text):
	m_ListView(listview),
	m_Item    (item)    ,
	m_Column  (column)  ,
	m_Text    (text)    ,
	m_Cancel  (false)   
{
}

CXListViewInplaceEdit::~CXListViewInplaceEdit()
{
}

//===========================================================================
void CXListViewInplaceEdit::PreCreate(CREATESTRUCT& cs)
{
	//-----------------------------------------------------------------------
	DWORD dwStyle;
	DWORD dwExStyle;


	dwStyle = 
		WS_CHILD           |
		WS_VISIBLE         |
		WS_CLIPSIBLINGS    |
		WS_CLIPCHILDREN    |
		WS_BORDER          |
		ES_AUTOHSCROLL     |
//		ES_MULTILINE       |
//		ES_UPPERCASE       |
		0u;

	dwExStyle = 
//		WS_EX_CLIENTEDGE   |
		0u;


	//-----------------------------------------------------------------------
	CEdit::PreCreate(cs);

	cs.style     |= dwStyle;
	cs.dwExStyle |= dwExStyle;
}

//===========================================================================
int CXListViewInplaceEdit::OnCreate(CREATESTRUCT& cs)
{
	int result;


	result = CEdit::OnCreate(cs);

	return result;
}

void CXListViewInplaceEdit::OnAttach()
{
#if 0
	// ES_MULTILINE
	CRect rect;

		
	GetRect(rect);
	rect.top+=5;
	SetRect(rect);
#endif


	SetFont( GetParent().GetFont() );
//	SetMargins(2+8, 8);

	SetWindowText( m_Text.c_str() );
	SetFocus();
	SetSel( 0, -1, TRUE );
}

void CXListViewInplaceEdit::OnDestroy()
{
	CEdit::OnDestroy();
}

//===========================================================================
BOOL CXListViewInplaceEdit::PreTranslateMessage(MSG& msg)
{
	if (msg.message == WM_KEYDOWN)
	{
		if (msg.wParam == VK_RETURN|| 
		    msg.wParam == VK_DELETE|| 
		    msg.wParam == VK_ESCAPE|| 
		    GetKeyState(VK_CONTROL))
		{
			::TranslateMessage(&msg);
			::DispatchMessage (&msg);
			return TRUE; // DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(msg);
}

//===========================================================================
LRESULT CXListViewInplaceEdit::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_NCDESTROY : return OnNcDestroy  (msg, wparam, lparam);
	case WM_KEYDOWN   : return OnKeyDown    (msg, wparam, lparam);
	case WM_CHAR      : return OnChar       (msg, wparam, lparam);
	case WM_SETFOCUS  : return OnSetFocus   (msg, wparam, lparam);
	case WM_KILLFOCUS : return OnKillFocus  (msg, wparam, lparam);
	case WM_GETDLGCODE: return OnGetDlgCode (msg, wparam, lparam);

	default:
		break;
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListViewInplaceEdit::OnNcDestroy(UINT msg, WPARAM wparam, LPARAM lparam)
{
	//-----------------------------------------------------------------------
	LRESULT lResult;
	
	
	lResult = WndProcDefault(msg, wparam, lparam);


	//-----------------------------------------------------------------------
	void* pointer;


	pointer = this;


	//-----------------------------------------------------------------------
	UINT   nm_msg;
	WPARAM nm_wparam;
	LPARAM nm_lparam;


	nm_msg    = WM_USER+20;
	nm_wparam = WPARAM(0);
	nm_lparam = LPARAM(pointer);

	GetParent().PostMessage(nm_msg, nm_wparam, nm_lparam);


	//-----------------------------------------------------------------------
	return lResult;
}

LRESULT CXListViewInplaceEdit::OnKeyDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
	//-----------------------------------------------------------------------
	int lLow, lHigh;


	//-----------------------------------------------------------------------
	void* pointer;


	pointer = this;


	//-----------------------------------------------------------------------
	UINT   nm_msg;
	WPARAM nm_wparam;
	LPARAM nm_lparam;


	nm_msg    = WM_USER+22;
	nm_wparam = WPARAM(wparam);
	nm_lparam = LPARAM(pointer);


	//-----------------------------------------------------------------------
	switch (wparam) 
	{
	case VK_ESCAPE:
		{
			m_Cancel = true;
			::SetFocus(GetParent());
			return 0;
		}
		break;

	case VK_RETURN:
		{
			::SetFocus(GetParent());
			// FIX: Allowing a multiline EDIT to VK_ESCAPE will send a WM_CLOSE
			//      to the list control if it's embedded in a dialog!?
			return 0;
		}
		break;

	case VK_UP:
		{
			if (0==m_Item)
			{
				return WndProcDefault(msg, wparam, lparam);
			}

			GetParent().PostMessage(nm_msg, nm_wparam, nm_lparam);
			::SetFocus(GetParent());
			return 0;
		}
		break;

	case VK_DOWN:
	case VK_TAB:
		{
			if ((m_Item+1)>=m_ListView->GetItemCount())
			{
				return WndProcDefault(msg, wparam, lparam);
			}

			GetParent().PostMessage(nm_msg, nm_wparam, nm_lparam);
			::SetFocus(GetParent());
			return 0;
		}
		break;

	case VK_LEFT:
		{
			GetSel(lLow, lHigh);
			if (lLow != lHigh || lLow != 0)
			{
				return WndProcDefault(msg, wparam, lparam);
			}


			if (0==m_Column)
			{
				return WndProcDefault(msg, wparam, lparam);
			}


			GetParent().PostMessage(nm_msg, nm_wparam, nm_lparam);
			::SetFocus(GetParent());
			return 0;
		}
		break;

	case VK_RIGHT:
		{
			GetSel(lLow, lHigh);
			if (lLow != lHigh || lLow != GetWindowTextLength())
			{
				return WndProcDefault(msg, wparam, lparam);
			}


			CHeader header;
			int column_count;


			header.Attach(m_ListView->GetHeader());
			column_count = header.GetItemCount();
			header.Detach();

			if( (m_Column+1) >= column_count )
			{
				return WndProcDefault(msg, wparam, lparam);
			}


			GetParent().PostMessage(nm_msg, nm_wparam, nm_lparam);
			::SetFocus(GetParent());
			return 0;
		}
		break;

	default:
		break;
	}


	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListViewInplaceEdit::OnChar(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch( LOWORD(wparam) ) 
	{
	case VK_RETURN:
	case VK_ESCAPE:
		{
			// Do not BEEP!!!!
			return 0;
		}
		break;

	default:
		break;
	}


	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListViewInplaceEdit::OnSetFocus(UINT msg, WPARAM wparam, LPARAM lparam)
{
	m_Cancel = false;


	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListViewInplaceEdit::OnKillFocus(UINT msg, WPARAM wparam, LPARAM lparam)
{
	LRESULT lResult;
	
	
	lResult = WndProcDefault(msg, wparam, lparam);

	m_Cancel |= (GetModify() == FALSE);


	//-----------------------------------------------------------------------
	CString s;


	s = GetWindowText();
	m_Text = s.c_str();


	//-----------------------------------------------------------------------
	void* pointer;


	pointer = this;


	//-----------------------------------------------------------------------
	UINT   nm_msg;
	WPARAM nm_wparam;
	LPARAM nm_lparam;


	nm_msg    = WM_USER+21;
	nm_wparam = WPARAM(m_Cancel ? 0 : 1);
	nm_lparam = LPARAM(pointer);

	GetParent().SendMessage(nm_msg, nm_wparam, nm_lparam);


	//-----------------------------------------------------------------------
	Destroy();


	//-----------------------------------------------------------------------
	return lResult;
}

LRESULT CXListViewInplaceEdit::OnGetDlgCode(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return WndProcDefault(msg, wparam, lparam) | DLGC_WANTALLKEYS | DLGC_WANTARROWS;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CXListView::CXListView()
{
}

CXListView::~CXListView()
{
	//--------------------------------------------------------------------------
//	if (IsWindow()) DeleteAllItems();


	//--------------------------------------------------------------------------
	std::vector<CXListViewInplaceEdit*>::iterator i;

	CXListViewInplaceEdit* e;


	for (i=m_InplaceEditContainer.begin();i!=m_InplaceEditContainer.end();i++)
	{
		e = *i;
		delete e;
	}
}

//===========================================================================
void CXListView::PreCreate(CREATESTRUCT& cs)
{
	//-----------------------------------------------------------------------
	DWORD dwStyle;
	DWORD dwExStyle;


	dwStyle = 
//		WS_CHILD           |
//		WS_VISIBLE         |
//		WS_CLIPSIBLINGS    |
//		WS_CLIPCHILDREN    |
		LVS_REPORT         |
		LVS_SHOWSELALWAYS  |
		LVS_OWNERDATA      |
		LVS_OWNERDRAWFIXED |
		0u;

	dwExStyle = 
//		WS_EX_CLIENTEDGE   |
		0u;


	//-----------------------------------------------------------------------
	CListView::PreCreate(cs);

	cs.style     |= dwStyle;
	cs.dwExStyle |= dwExStyle;
}

void CXListView::OnDestroy()
{
	SetImageList(NULL, LVSIL_SMALL);
}

void CXListView::OnAttach()
{
	//-----------------------------------------------------------------------
	m_Font.CreatePointFont (120, _T("맑은 고딕"));

	
	SetFont(m_Font);


	//-----------------------------------------------------------------------
	DWORD dwStyle;
	DWORD dwExtendedStyle;


	dwStyle = 
		WS_CHILD          | 
		WS_VISIBLE        | 
		WS_CLIPSIBLINGS   | 
		WS_CLIPCHILDREN   | 
		LVS_REPORT        | 
		LVS_SHOWSELALWAYS | 
		LVS_OWNERDATA     |
		LVS_OWNERDRAWFIXED|
		0u;

	dwExtendedStyle = 
		LVS_EX_DOUBLEBUFFER     |
		LVS_EX_GRIDLINES        |
		LVS_EX_FULLROWSELECT    |
//		LVS_EX_LABELTIP 	    |
//		LVS_EX_SUBITEMIMAGES    |
//		LVS_EX_TRANSPARENTBKGND |
		0u;

	SetStyle         (dwStyle);
	SetExtendedStyle (dwExtendedStyle);


	//-----------------------------------------------------------------------
	m_SmallImageList.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);

//	SetImageList(m_SmallImageList, LVSIL_SMALL);


	//--------------------------------------------------------------------------
	LVCOLUMN LvColumn;


	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	LvColumn.fmt       = LVCFMT_RIGHT;
	LvColumn.pszText   = const_cast<char*>("번호");
	LvColumn.cx        = 50;
	LvColumn.iSubItem  = 0;
	InsertColumn(0, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_LEFT;
	LvColumn.pszText   = const_cast<char*>("시간");
	LvColumn.cx        = 180;
	LvColumn.iSubItem  = 1;
	InsertColumn(1, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_CENTER;
	LvColumn.pszText   = const_cast<char*>("항목");
	LvColumn.cx        = 80;
	LvColumn.iSubItem  = 2;
	InsertColumn(2, LvColumn);

	memset (&LvColumn, 0, sizeof(LvColumn));
	LvColumn.mask      = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	LvColumn.fmt       = LVCFMT_LEFT;
	LvColumn.pszText   = const_cast<char*>("내용");
	LvColumn.cx        = 800;
	LvColumn.cchTextMax= 1024*3; // GetItem올때 가져올수있는 크기 (시하는 텍스트는 259+1[MAX_PATH]널문자로 제한됨)
	LvColumn.iSubItem  = 3;
	InsertColumn(3, LvColumn);


	//--------------------------------------------------------------------------
	int x;
	int y;


	for (y=0; y<100; y++)
	{
		std::vector<std::string> c;


		for (x=0; x<4; x++)
		{
			std::stringstream ss;


			ss << x << _T(":") << y;

			c.push_back(ss.str());
		}

		m_TestContainer.push_back(c);
	}


	//--------------------------------------------------------------------------
	SetItemCountEx (100, LVSICF_NOSCROLL);
}

//===========================================================================
LRESULT CXListView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_MOUSEACTIVATE: return OnMouseActivate(msg, wparam, lparam);
	case WM_LBUTTONDOWN  : return OnLButtonDown  (msg, wparam, lparam);
	case WM_LBUTTONDBLCLK: return OnLButtonDbClk (msg, wparam, lparam);
	case WM_MOUSEWHEEL   : return OnMouseWheel   (msg, wparam, lparam); 
	case WM_HSCROLL      : return OnHScroll      (msg, wparam, lparam);
	case WM_VSCROLL      : return OnVScroll      (msg, wparam, lparam);

	case WM_USER+20: return InplaceEdit_OnDelete  (msg, wparam, lparam);
	case WM_USER+21: return InplaceEdit_OnUpdate  (msg, wparam, lparam);
	case WM_USER+22: return InplaceEdit_OnNavigate(msg, wparam, lparam);

	default:
		break;
	}

	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
	SetFocus();
	return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CXListView::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
	CPoint ptMouse((short)LOWORD(lparam), (short)HIWORD(lparam));
	UINT   uMouseFlag = (UINT)wparam;
	WORD   fwKeys     = LOWORD(wparam);
	SHORT  zDelta     = HIWORD(wparam);


	//-----------------------------------------------------------------------
	int item;
	int column;

	int result;
	LVHITTESTINFO hti;
	UINT flag = LVIS_FOCUSED;


	ZeroMemory(&hti, sizeof(hti));
	hti.flags = 0u;
	hti.pt    = ptMouse;
	result    = HitTest(hti);
	item      = hti.iItem;

	ZeroMemory(&hti, sizeof(hti));
	hti.flags = 0u;
	hti.pt    = ptMouse;
	result    = SubItemHitTest(hti);   
	column    = hti.iSubItem;


	if (0<=item && 0<=column)
	{
		if( flag==(GetItemState(item, flag ) & flag) )
		{
			//if( GetWindowLong(GWL_STYLE) & LVS_EDITLABELS )

			if ( m_InplaceEditContainer.empty() )
			{
				InplaceEdit_New( item, column );
				return 0;
			}
		}
		else
		{
//			SetItemState(item, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED); 
		}
	}


	//-----------------------------------------------------------------------
	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::OnLButtonDbClk(UINT msg, WPARAM wparam, LPARAM lparam)
{
	CPoint ptMouse((short)LOWORD(lparam), (short)HIWORD(lparam));
	UINT   uMouseFlag = (UINT)wparam;
	WORD   fwKeys     = LOWORD(wparam);
	SHORT  zDelta     = HIWORD(wparam);


	//-----------------------------------------------------------------------
	int item;
	int column;

	int result;
	LVHITTESTINFO hti;
	UINT flag = LVIS_FOCUSED;


	ZeroMemory(&hti, sizeof(hti));
	hti.flags = 0u;
	hti.pt    = ptMouse;
	result    = HitTest(hti);
	item      = hti.iItem;

	ZeroMemory(&hti, sizeof(hti));
	hti.flags = 0u;
	hti.pt    = ptMouse;
	result    = SubItemHitTest(hti);   
	column    = hti.iSubItem;


	if (0<=item && 0<=column)
	{
		if ( m_InplaceEditContainer.empty() )
		{
			InplaceEdit_New( item, column );
			return 0;
		}
	}


	//-----------------------------------------------------------------------
	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::OnHScroll (UINT msg, WPARAM wparam, LPARAM lparam)
{
	int  code     ;
	HWND scrollbar;
	int  position ;


	//-----------------------------------------------------------------------
	code       = (int )LOWORD(wparam);
	scrollbar  = (HWND)lparam;

	if (SB_THUMBTRACK   == code || 
		SB_THUMBPOSITION== code  )
	{
		position = (SHORT)HIWORD(wparam);
	}
	else
	{
		position = GetScrollPos(SB_HORZ);
	}


	//-----------------------------------------------------------------------
	if( GetFocus() != GetHwnd() )
	{
		SetFocus();
	}


	//-----------------------------------------------------------------------
	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::OnVScroll (UINT msg, WPARAM wparam, LPARAM lparam)
{
	int  code     ;
	HWND scrollbar;

	int  position ;


	//-----------------------------------------------------------------------
	code       = (int )LOWORD(wparam);
	scrollbar  = (HWND)lparam;

	if (SB_THUMBTRACK   == code || 
		SB_THUMBPOSITION== code  )
	{
		position = (SHORT)HIWORD(wparam);
	}
	else
	{
		position = GetScrollPos(SB_VERT);
	}


	//-----------------------------------------------------------------------
	if( GetFocus() != GetHwnd() )
	{
		SetFocus();
	}


	//-----------------------------------------------------------------------
	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::OnMouseWheel (UINT msg, WPARAM wparam, LPARAM lparam)
{
	CPoint ptMouse((short)LOWORD(lparam), (short)HIWORD(lparam));
	UINT   uMouseFlag = (UINT)wparam;

	WORD  fwKeys = LOWORD(wparam);
	SHORT zDelta = HIWORD(wparam);


	//-----------------------------------------------------------------------
	if( GetFocus() != GetHwnd() )
	{
		SetFocus();
	}


	//-----------------------------------------------------------------------
	return WndProcDefault(msg, wparam, lparam);
}

LRESULT CXListView::InplaceEdit_OnDelete (UINT msg, WPARAM wparam, LPARAM lparam)
{
	//-----------------------------------------------------------------------
	CXListViewInplaceEdit* e;


	e = (CXListViewInplaceEdit*)lparam;


	//-----------------------------------------------------------------------
	std::vector<CXListViewInplaceEdit*>::iterator i;


	i = std::find(m_InplaceEditContainer.begin(), m_InplaceEditContainer.end(), e);
	if (i!=m_InplaceEditContainer.end())
	{
		m_InplaceEditContainer.erase(i);
	}


	delete e;


	//CX_DEBUG_TRACEF(CX_TWA_NORMAL, "Edit(%x): Delete", e);
	return 0;
}

LRESULT CXListView::InplaceEdit_OnUpdate (UINT msg, WPARAM wparam, LPARAM lparam)
{
	//-----------------------------------------------------------------------
	CXListViewInplaceEdit* e;
	UINT ok;


	ok = (UINT)wparam;
	e = (CXListViewInplaceEdit*)lparam;

	if (ok)
	{
		m_TestContainer[e->m_Item][e->m_Column] = e->m_Text;
	}

	//CX_DEBUG_TRACEF(CX_TWA_NORMAL, "Edit(%x): Update=%d", e, ok);
	return 0;
}

LRESULT CXListView::InplaceEdit_OnNavigate (UINT msg, WPARAM wparam, LPARAM lparam)
{
	//-----------------------------------------------------------------------
	CXListViewInplaceEdit* e;
	UINT vkey;


	vkey = (UINT)wparam;
	e = (CXListViewInplaceEdit*)lparam;


	//-----------------------------------------------------------------------
	switch (vkey)
	{
	case VK_TAB:
		InplaceEdit_New(e->m_Item+1, e->m_Column);
		break;

	case VK_UP:
		InplaceEdit_New(e->m_Item-1, e->m_Column);
		break;

	case VK_DOWN:
		InplaceEdit_New(e->m_Item+1, e->m_Column);
		break;

	case VK_LEFT:
		InplaceEdit_New(e->m_Item, e->m_Column-1);
		break;

	case VK_RIGHT:
		InplaceEdit_New(e->m_Item, e->m_Column+1);
		break;

	default:
		break;
	}


	//CX_DEBUG_TRACEF(CX_TWA_NORMAL, "Edit(%x)_Navigate: key=%d", e, vkey);
	return 0;
}

void CXListView::InplaceEdit_New( int item, int column )
{
	//-----------------------------------------------------------------------
	if( !EnsureVisible( item, TRUE ) )
	{
		return;
	}


	//-----------------------------------------------------------------------
	// Make sure that nCol is valid
	CHeader header;
	int column_count;


	header.Attach(GetHeader());
	column_count = header.GetItemCount();
	header.Detach();

	if( column >= column_count || GetColumnWidth(column) < 10 )
	{
		return;
	}


	//-----------------------------------------------------------------------
	// Get the column offset
	int offset;
	int i;


	offset = 0;
	for(i = 0; i < column; i++ )
	{
		offset += GetColumnWidth( i );
	}


	//-----------------------------------------------------------------------
	CRect rect;


	GetItemRect( item, rect, LVIR_BOUNDS );
 

	//-----------------------------------------------------------------------
	// Now scroll if we need to expose the column
	CRect rcClient;


	rcClient = GetClientRect();
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;


		size.cx = offset + rect.left;
		size.cy = 0;
		
		Scroll( size );

		rect.left -= size.cx;
	}
 

	//-----------------------------------------------------------------------
	// Get Column alignment
	LV_COLUMN lvcolumn;
	
	
	lvcolumn.mask = LVCF_FMT;
	GetColumn( column, lvcolumn );


	//-----------------------------------------------------------------------
	DWORD dwStyle;


	if     ((lvcolumn.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT ) { dwStyle = ES_LEFT  ; }
	else if((lvcolumn.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT) { dwStyle = ES_RIGHT ; }
	else                                                     { dwStyle = ES_CENTER; }

	dwStyle |= WS_VISIBLE|WS_CHILD|WS_BORDER|ES_AUTOHSCROLL;
 

	//-----------------------------------------------------------------------
//	rect.left += offset+4;
//	rect.right = rect.left + GetColumnWidth( column ) - 3 ;

	rect.left += offset;
	rect.right = rect.left + GetColumnWidth( column ) + 1;

	rect.left++;
	rect.bottom--;
	rect.right--;

	if( rect.right > rcClient.right)
	{
		rect.right = rcClient.right;
	}


	//-----------------------------------------------------------------------
	CXListViewInplaceEdit *pEdit;


	pEdit = new CXListViewInplaceEdit(
		this,
		item, column, 
		m_TestContainer[item][column]
//		std::string("text")
		);


//	pEdit->Create( dwStyle, rect, this, 1000 );
	pEdit->Create(GetHwnd());
	pEdit->MoveWindow(rect);
/*
	pEdit->SetFocus();

	{
		CRect rect;

		
		pEdit->GetRect(rect);
		rect.top+=10;
		pEdit->SetRect(rect);
	}
*/

	m_InplaceEditContainer.push_back(pEdit);

	//CX_DEBUG_TRACEF(CX_TWA_NORMAL, "Edit(%x): New", pEdit);
}

//===========================================================================
LRESULT CXListView::OnNotifyReflect(WPARAM wparam, LPARAM lparam)
{
	LPNMHDR NmHdr = reinterpret_cast<LPNMHDR>(lparam);


	switch (NmHdr->code)
	{
	case LVN_GETDISPINFO:
//		return OnLvnGetDispInfo(wparam, lparam);
		break;

	case LVN_BEGINLABELEDIT:
		break;

	case NM_CUSTOMDRAW:
//		return OnNmCustomDraw(wparam, lparam);
		break;

	default:
		break;
	}


	return CListView::OnNotifyReflect(wparam, lparam);
}

LRESULT CXListView::OnLvnGetDispInfo (WPARAM wparam, LPARAM lparam)
{
	NMLVDISPINFO *NmLvDispInfo = reinterpret_cast<NMLVDISPINFO*>(lparam);
	LVITEM*       LvItem       = &NmLvDispInfo->item;


	if (LvItem->mask & LVIF_TEXT)
	{
		strncpy (LvItem->pszText, "Text", LvItem->cchTextMax);
	}

	return 0;
}

LRESULT CXListView::OnLvnBeginLabelEdit (WPARAM wparam, LPARAM lparam)
{
	return CListView::OnNotify(wparam, lparam);
}

//---------------------------------------------------------------------------
LRESULT CXListView::OnNmCustomDraw (WPARAM wparam, LPARAM lparam)
{
	LPNMCUSTOMDRAW NmCustomDraw = reinterpret_cast<LPNMCUSTOMDRAW>(lparam);


	switch(NmCustomDraw->dwDrawStage)
	{
	case  CDDS_PREPAINT       : return OnPrePaint       (wparam, lparam);
	case  CDDS_POSTPAINT      : return OnPostPaint      (wparam, lparam);
	case  CDDS_PREERASE       : return OnPreErase       (wparam, lparam);
	case  CDDS_POSTERASE      : return OnPostErase      (wparam, lparam);
	case  CDDS_ITEMPREPAINT   : return OnItemPrePaint   (wparam, lparam);
	case  CDDS_ITEMPOSTPAINT  : return OnItemPostPaint  (wparam, lparam);
	case  CDDS_ITEMPREERASE   : return OnItemPreErase   (wparam, lparam);
	case  CDDS_ITEMPOSTERASE  : return OnItemPostErase  (wparam, lparam);
	case (CDDS_ITEMPREPAINT |
	      CDDS_SUBITEM      ) : return OnSubItemPrePaint(wparam, lparam);

	default:
		break;
	}
	
	return CListView::OnNotify(wparam, lparam);
}

LRESULT CXListView::OnPrePaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_NOTIFYITEMDRAW;
}

LRESULT CXListView::OnPostPaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnPreErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnPostErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnItemPrePaint(WPARAM wparam, LPARAM lparam)
{
	NMLVCUSTOMDRAW* NmLvCustomDraw = reinterpret_cast<LPNMLVCUSTOMDRAW>(lparam);


	NmLvCustomDraw->clrText = RGB(0x00u, 0x00u, 0xFFu);

	if (0)
	{
//		:: SelectObject(NmLvCustomDraw->nmcd.hdc, m_BoldFont.m_hFont);
	
		return CDRF_NEWFONT; 
	}

	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnItemPostPaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnItemPreErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnItemPostErase(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

LRESULT CXListView::OnSubItemPrePaint(WPARAM wparam, LPARAM lparam)
{
	return CDRF_DODEFAULT;
}

//===========================================================================
LRESULT CXListView::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DRAWITEM    : return OnDrawItem   (msg, wparam, lparam);
	case WM_MEASUREITEM : return OnMeasureItem(msg, wparam, lparam);
	case WM_COMPAREITEM : return OnCompareItem(msg, wparam, lparam);
	case WM_DELETEITEM  : return OnDeleteItem (msg, wparam, lparam);

	default:
		break;
	}

	return CListView::OnMessageReflect(msg, wparam, lparam);
}

LRESULT CXListView::OnDrawItem(UINT msg, WPARAM wparam, LPARAM lparam)
{
	DrawItem((LPDRAWITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

LRESULT CXListView::OnMeasureItem(UINT msg, WPARAM wparam, LPARAM lparam)
{
	MeasureItem((LPMEASUREITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

LRESULT CXListView::OnCompareItem(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return CompareItem((LPCOMPAREITEMSTRUCT)lparam);
}

LRESULT CXListView::OnDeleteItem(UINT msg, WPARAM wparam, LPARAM lparam)
{
	DeleteItem((LPDELETEITEMSTRUCT)lparam);

	return (LRESULT)TRUE;
}

//---------------------------------------------------------------------------
void CXListView::GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect)
{
	CRect header_rect;


	CHeader header;
	

	header.Attach(GetHeader());
	header_rect = header.GetItemRect(header_column);
	header.Detach();


	// If we don't do this, when we scroll to the right, we will be 
	// drawing as if we weren't and your cells won't line up with the
	// columns.
	int x_offset = -GetScrollPos(SB_HORZ);

	cell_rect.left   = x_offset + header_rect.left;
	cell_rect.right  = x_offset + header_rect.right;
	cell_rect.top    = item_rect.top;
	cell_rect.bottom = item_rect.bottom;
}

void CXListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_LISTVIEW)
	{
		return;
	}

	if(lpDrawItemStruct->itemAction != ODA_DRAWENTIRE)
	{
		return;
	}


	//-----------------------------------------------------------------------
	int     column_count;
	CHeader header;
	

	header.Attach(GetHeader());
	column_count = header.GetItemCount();
	header.Detach();


	//-----------------------------------------------------------------------
	HDC   dc       ;
	CRect item_rect;
	int   item_id  ;


	dc        = lpDrawItemStruct->hDC;
	item_rect = lpDrawItemStruct->rcItem;
	item_id   = lpDrawItemStruct->itemID;


	//-----------------------------------------------------------------------
	COLORREF normal_text_color;
	COLORREF normal_bg_color;
	COLORREF normal_fg_color;
	HPEN     normal_pen ;
	HBRUSH   normal_brush;

	COLORREF selected_text_color;
	COLORREF selected_bg_color;
	COLORREF selected_fg_color;
	HPEN     selected_pen  ;
	HBRUSH   selected_brush;


	normal_text_color   = GetSysColor(COLOR_BTNTEXT);
	normal_bg_color     = RGB(255,255,255);
	normal_fg_color     = RGB(255,255,255);
	normal_pen          = CreatePen(PS_SOLID, 1, normal_fg_color);
	normal_brush        = CreateSolidBrush(normal_bg_color);

	selected_text_color = GetSysColor(COLOR_HIGHLIGHTTEXT);
	selected_bg_color   = GetSysColor(COLOR_HIGHLIGHT);
	selected_fg_color   = GetSysColor(COLOR_HIGHLIGHT);
	selected_pen        = CreatePen(PS_SOLID, 1, selected_fg_color);
	selected_brush      = CreateSolidBrush(selected_bg_color);


	//-----------------------------------------------------------------------
	UINT     text_format;
	COLORREF text_color;
	HPEN     pen ;
	HBRUSH   brush;

	CRect cell_rect;
	int   n;

	std::string text;


	for (n=0; n<column_count; n++)
	{
		//-------------------------------------------------------------------
		GetCellRect(n, item_rect, cell_rect);


		//-------------------------------------------------------------------
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			pen   = selected_pen  ;
			brush = selected_brush;
			text_color = selected_text_color;
		}
		else
		{
			pen   = normal_pen  ;
			brush = normal_brush;
			text_color = normal_text_color;
		}


		//-------------------------------------------------------------------
		SelectObject(dc, pen);
		SelectObject(dc, brush);
		Rectangle   (dc, cell_rect.left, cell_rect.top, cell_rect.right, cell_rect.bottom);


		//-------------------------------------------------------------------
		std::stringstream ss;
		ss << item_id << _T(":") << n;


		//-------------------------------------------------------------------
		text_format = /*DT_CENTER |*/ DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE;


		//-------------------------------------------------------------------
		::SetTextColor(dc, text_color);
		
		cell_rect.DeflateRect(4,2,0,2);
//		::DrawText    (dc, ss.str().c_str(), ss.str().size(), cell_rect, text_format);
		
		text = m_TestContainer[item_id][n];
		::DrawText    (
			dc, 
			text.c_str(), 
			static_cast<int>(text.size()), 
			cell_rect, text_format
		);
	}


	DeleteObject(selected_pen  );
	DeleteObject(selected_brush);
	DeleteObject(normal_pen    );
	DeleteObject(normal_brush  );
}

void CXListView::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(lpMeasureItemStruct->CtlType != ODT_MENU)
	{
		// return default height for a system font
		//-------------------------------------------------------------------
		HWND hWnd;


		hWnd = GetDlgItem(lpMeasureItemStruct->CtlID);


		//-------------------------------------------------------------------
		CClientDC dc(hWnd);
		TEXTMETRIC tm = { 0 };


		dc.GetTextMetrics(tm);


		//-------------------------------------------------------------------
		lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 10;
	}
	else
	{
		lpMeasureItemStruct->itemHeight = ::GetSystemMetrics(SM_CYMENU);
	}
}

int CXListView::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct)
{
	// all items are equal
	return 0;
}

void CXListView::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// default - nothing
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CEventMessageDockContainer::CEventMessageDockContainer()
{
	SetView        (m_Wnd); 
	SetDockCaption (_T("EventMessage - Docking container"));
	SetTabText     (_T("EventMessage"));
	SetTabIcon     (IDI_DOCKER);
} 





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CEventMessageDocker::CEventMessageDocker()
{
	SetView     (m_DockContainer);
//	SetBarWidth (4);
}




