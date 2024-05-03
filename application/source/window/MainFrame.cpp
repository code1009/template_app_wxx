/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "MyID.hpp"
#include "MyTabbedMDI.hpp"
#include "Mainframe.hpp"

#include "MyWndDocker.hpp"
#include "MyListViewDocker.hpp"
#include "MyView.hpp"

#include "../bl/BLWnd.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMainFrame::CMainFrame() :
	m_isContainerTabsAtTop(FALSE),
	m_isHideSingleTab(TRUE),
	m_isMDITabsAtTop(TRUE),
	m_pActiveDocker(NULL)
{
	// Set m_MyTabbedMDI as the view window of the frame
	SetView(m_MyTabbedMDI);
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

//===========================================================================
HWND CMainFrame::Create(HWND parent)
{
	// Set the registry key name, and load the initial window position.
	// Use a registry key name like "CompanyName\\Application".
	
	// LoadRegistrySettings(_T("Win32++\\TabbedMDI Docking"));

	return CDockFrame::Create(parent);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// Call the base class function first
	CDockFrame::PreCreate(cs);

	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.

	// A menu is added if the IDW_MAIN menu resource is defined.
	// Frames have all options enabled by default. 
	// Use the following functions to disable options.

	// UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
	// UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
	// UseReBar(FALSE);              // Don't use a ReBar
	// UseStatusBar(FALSE);          // Don't use a StatusBar
	UseThemes(FALSE);                // Don't use themes
	// UseToolBar(FALSE);            // Don't use a ToolBar

	// call the base class function
	int rv;

	rv = CDockFrame::OnCreate(cs);
	if (0 != rv)
	{
		return rv;
	}

	return 0;
}

//===========================================================================
void CMainFrame::OnInitialUpdate()
{
	// Load dock settings
	// if (!LoadDockRegistrySettings(GetRegistryKeyName()))
	{
		LoadDefaultDockers();
	}

	// Load MDI child settings
	// if (!m_MyTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
	{
		LoadDefaultMDIs();
	}

	// Hide the container's tab if it has just one tab
	HideSingleContainerTab(m_isHideSingleTab);


	// 윈도우 창 메뉴 항목 기억
	CMenu frameMenu = GetFrameMenu();
	CMenu windowMenu = frameMenu.GetSubMenu(frameMenu.GetMenuItemCount() - 1 - 1);

	m_windowMenuItemCount = windowMenu.GetMenuItemCount();


	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow(GetInitValues().showCmd);
	RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

LRESULT CMainFrame::OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam)
{
	CMenu frameMenu = GetFrameMenu();
	CMenu windowMenu = frameMenu.GetSubMenu(frameMenu.GetMenuItemCount() - 1 - 1);
	int count;
	int i;


	count = windowMenu.GetMenuItemCount();
	for (i = m_windowMenuItemCount; i < count; i++)
	{
		windowMenu.DeleteMenu(m_windowMenuItemCount, MF_BYPOSITION);
	}


	// Update the "Window" menu
	CMenu tabbedMDIMenu = m_MyTabbedMDI.GetListMenu();


	MENUITEMINFO src_mii;
	MENUITEMINFO dst_mii;
	char text[256];
	

	count = tabbedMDIMenu.GetMenuItemCount();
	if (count > 0)
	{
		windowMenu.AppendMenu(MF_BYPOSITION | MF_SEPARATOR, windowMenu.GetMenuItemCount());
	}


	for (i = 0; i < count; i++)
	{
		memset(&src_mii, 0, sizeof(src_mii));
		src_mii.cbSize = sizeof(src_mii);
		src_mii.fMask = MIIM_STRING | MIIM_ID;
		src_mii.fType = MFT_STRING;
		src_mii.dwTypeData = text;
		src_mii.cch = sizeof(text)/sizeof(char);
		tabbedMDIMenu.GetMenuItemInfo(i, src_mii, MF_BYPOSITION);


		memset(&dst_mii, 0, sizeof(dst_mii));
		dst_mii.cbSize = sizeof(dst_mii);
		dst_mii.fMask = MIIM_STRING | MIIM_ID;
		dst_mii.fType = MFT_STRING;
		dst_mii.wID = src_mii.wID;
		dst_mii.dwTypeData = text;

		windowMenu.InsertMenuItem(windowMenu.GetMenuItemCount(), dst_mii, MF_BYPOSITION);
	}


	return CDockFrame::OnInitMenuPopup(msg, wparam, lparam);
}

void CMainFrame::OnMenuUpdate(UINT id)
{
	// Called when menu items are about to be displayed

	// Only for the Menu IDs we wish to modify
	if (id >= IDM_EDIT_UNDO && id <= IDM_EDIT_DELETE)
	{
		// Get the pointer to the active view
		CWnd* pView; 


		if (m_pActiveDocker)
		{
			pView = m_pActiveDocker->GetActiveView();
		}
		else
		{
			pView = NULL;
		}


		// Enable the Edit menu items for CViewText views, disable them otherwise
		// CMenu editMenu = GetFrameMenu().GetSubMenu(1);

		// UINT flags = (dynamic_cast<CViewText*>(pView))? MF_ENABLED : MF_GRAYED;
		// editMenu.EnableMenuItem(id, MF_BYCOMMAND | flags);
	}


	UINT check;


	switch (id)
	{
	case IDM_CONTAINER_TOP:
		check = (m_isContainerTabsAtTop) ? MF_CHECKED : MF_UNCHECKED;
		GetFrameMenu().CheckMenuItem(id, check);
		break;

	case IDM_HIDE_SINGLE_TAB:
		check = (m_isHideSingleTab) ? MF_CHECKED : MF_UNCHECKED;
		GetFrameMenu().CheckMenuItem(id, check);
		break;

	case IDM_TABBEDMDI_TOP:
		check = (m_isMDITabsAtTop) ? MF_CHECKED : MF_UNCHECKED;
		GetFrameMenu().CheckMenuItem(id, check);
	}


	CDockFrame::OnMenuUpdate(id);
}

//===========================================================================
void CMainFrame::SetupMenuIcons()
{
	// Load the defualt set of icons from the toolbar


	std::vector<UINT> data = GetToolBarData();
	if ((GetMenuIconHeight() >= 24) && (GetWindowDpi(*this) != 192))
	{
		SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
	}
	else
	{
		SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
	}

	// Add some extra icons for menu items
	// AddMenuIcon(IDM_FILE_NEW, IDI_FILE_NEW);
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN,  FALSE );
	AddToolBarButton( IDM_FILE_SAVE,  FALSE );

	AddToolBarButton( 0 );  // Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );

	AddToolBarButton( 0 );  // Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );

	AddToolBarButton( 0 );  // Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

BOOL CMainFrame::SaveRegistrySettings()
{
#if 0
	CDockFrame::SaveRegistrySettings();

	// Save the docker settings
	SaveDockRegistrySettings(GetRegistryKeyName());

	// Save the tabbedMDI settings
	m_MyTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());
#endif

	return TRUE;
}

//===========================================================================
CDocker* CMainFrame::NewDockerFromID(int dockID)
{
	CDocker* pDocker = NULL;


	switch (dockID)
	{
	case ID_DOCKER_MY_WND:
		pDocker = new CMyWndDocker();
		break;

	default:
		TRACE("Unknown Dock ID \n");
		break;
	}

	return pDocker;
}

LRESULT CMainFrame::OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Called when a docker is activated.
	// Store the active docker in preparation for menu input. Excludes active 
	// docker change for undocked dockers when using the menu.

	TRACE("CMainFrame::OnDockActivated() \n");


	CPoint pt = GetCursorPos();


	if (WindowFromPoint(pt) != GetMenuBar())
	{
		m_pActiveDocker = GetActiveDocker();
	}

	return CDockFrame::OnDockActivated(msg, wparam, lparam);
}

LRESULT CMainFrame::OnDockDestroyed(UINT msg, WPARAM wparam, LPARAM lparam)
{
	TRACE("CMainFrame::OnDockDestroyed() \n");


	//CDocker* pDocker = reinterpret_cast<CDocker*>(wparam);
	//int id = pDocker->GetDockID();
	//TRACE("OnDockDestroyed(): id = %d \n", id);


	return CDockFrame::OnDockDestroyed(msg, wparam, lparam);
}

//===========================================================================
void CMainFrame::OnClose()
{
	// SaveRegistrySettings();

	Destroy();
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
	// OnCommand responds to menu and and toolbar input
	UINT id = LOWORD(wparam);
	switch (id)
	{
	// File menu
	case IDM_FILE_NEW:          return OnFileNew();
	case IDM_FILE_EXIT:         return OnFileExit();

	// HELP menu
	case IDM_HELP_ABOUT:        return OnHelp();

	// View menu
	case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:      return OnViewToolBar();

	// Layout menu
	case IDM_DEFAULT_LAYOUT:    return OnDefaultLayout();
	case IDM_CLOSE_MDIS:        return OnCloseMDIs();
	case IDM_CLOSE_DOCKERS:     return OnCloseDockers();

	// Option menu
	case IDM_TABBEDMDI_TOP:     return OnMDITabsAtTop();
	case IDM_CONTAINER_TOP:     return OnContainerTabsAtTop();
	case IDM_HIDE_SINGLE_TAB:   return OnHideSingleTab();

	// Window menu
	case IDW_FIRSTCHILD:
	case IDW_FIRSTCHILD + 1:
	case IDW_FIRSTCHILD + 2:
	case IDW_FIRSTCHILD + 3:
	case IDW_FIRSTCHILD + 4:
	case IDW_FIRSTCHILD + 5:
	case IDW_FIRSTCHILD + 6:
	case IDW_FIRSTCHILD + 7:
	case IDW_FIRSTCHILD + 8:
	{
		int tab = LOWORD(wparam) - IDW_FIRSTCHILD;
		m_MyTabbedMDI.SetActiveMDITab(tab);
		return TRUE;
	}

	case IDW_FIRSTCHILD + 9:
	{
		m_MyTabbedMDI.ShowListDialog();
		return TRUE;
	}

	default:
	{
		// Pass the command on to the view of the active docker
		if (m_pActiveDocker)
		{
			m_pActiveDocker->GetActiveView()->SendMessage(WM_COMMAND, wparam, lparam);
		}
	}
	}

	return FALSE;
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
	try
	{
		switch (msg)
		{
		case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
		}

		// Always pass unhandled messages on to WndProcDefault.
		return WndProcDefault(msg, wparam, lparam);
	}

	// Catch all CException types.
	catch (const CException& e)
	{
		// Display the exception.
		::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

		return 0;
	}
}

LRESULT CMainFrame::OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Called when the effective dots per inch (dpi) for a window has changed.
	// This occurs when:
	//  - The window is moved to a new monitor that has a different DPI.
	//  - The DPI of the monitor hosting the window changes.


	// Call the base class function. This recreates the toolbars.
	return CDockFrame::OnDpiChanged(msg, wparam, lparam);
}

//===========================================================================
void CMainFrame::SetContainerTabsAtTop(BOOL atTop)
{
	m_isContainerTabsAtTop = atTop;


	//-----------------------------------------------------------------------
	std::vector<DockPtr>::const_iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetTabsAtTop(atTop);
		}
	}
}

void CMainFrame::HideSingleContainerTab(BOOL hideSingle)
{
	m_isHideSingleTab = hideSingle;


	//-----------------------------------------------------------------------
	std::vector<DockPtr>::const_iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetHideSingleTab(hideSingle);
		}
	}
}

void CMainFrame::SetMDITabsAtTop(BOOL atTop)
{
	m_isMDITabsAtTop = atTop;

	
	//-----------------------------------------------------------------------
	m_MyTabbedMDI.GetTab().SetTabsAtTop(atTop);
}

//===========================================================================
void CMainFrame::LoadDefaultDockers()
{
	CDocker* pDockTop;
	CDocker* pDockRight;
	CDocker* pDockBottom;
	CDocker* pDockLeft;


	pDockTop = NULL;
	pDockRight = NULL;
	pDockBottom = NULL;
	pDockLeft = NULL;


	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry
	DWORD style;

	style = DS_CLIENTEDGE;


	// Add the parent dockers
	pDockLeft = AddDockedChild(new CMyWndDocker(), DS_DOCKED_LEFT | style, DpiScaleInt(400), ID_DOCKER_MY_WND);

	// Add the remaining dockers
	pDockLeft->AddDockedChild(new CMyListViewDocker(), DS_DOCKED_CONTAINER | style, DpiScaleInt(400), ID_DOCKER_MY_LISTVIEW);


	SetDockStyle(style);
}

void CMainFrame::LoadDefaultMDIs()
{
	m_MyTabbedMDI.GetTab().SetPadding(200, 100);


	// Add some MDI tabs
	m_MyTabbedMDI.AddMDIChild(new CMyView(), _T("MyView"), ID_MDI_VIEW_MY);
	m_MyTabbedMDI.AddMDIChild(new CBLWnd(), _T("blend2d"), ID_MDI_VIEW_BL);
	
	m_MyTabbedMDI.AddMDIChild(new CBLWnd(), _T("blend2d1"), ID_MDI_VIEW_BL+1);
	m_MyTabbedMDI.AddMDIChild(new CBLWnd(), _T("blend2d2"), ID_MDI_VIEW_BL+2);

	if (m_MyTabbedMDI.IsWindow())
	{
		m_MyTabbedMDI.SetActiveMDITab(0);
	}
}

//===========================================================================
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
	const CSize minimumSize(600, 400);
	lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
	lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
	return FinalWindowProc(msg, wparam, lparam);
}

//===========================================================================
BOOL CMainFrame::OnDefaultLayout()
{
	SetRedraw(FALSE);


	CloseAllDockers();
	m_MyTabbedMDI.CloseAllMDIChildren();


	LoadDefaultDockers();
	LoadDefaultMDIs();


	SetContainerTabsAtTop(m_isContainerTabsAtTop);
	HideSingleContainerTab(m_isHideSingleTab);
	SetMDITabsAtTop(m_isMDITabsAtTop);


	SetRedraw(TRUE);
	RedrawWindow();

	return TRUE;
}

BOOL CMainFrame::OnCloseDockers()
{
	CloseAllDockers();
	return TRUE;
}

BOOL CMainFrame::OnCloseMDIs()
{
	m_MyTabbedMDI.CloseAllMDIChildren();
	return TRUE;
}

//===========================================================================
BOOL CMainFrame::OnContainerTabsAtTop()
{
	SetContainerTabsAtTop(!m_isContainerTabsAtTop);
	return TRUE;
}

BOOL CMainFrame::OnHideSingleTab()
{
	HideSingleContainerTab(!m_isHideSingleTab);
	return TRUE;
}

BOOL CMainFrame::OnMDITabsAtTop()
{
	SetMDITabsAtTop(!m_isMDITabsAtTop);
	return TRUE;
}

//===========================================================================
BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	m_MyTabbedMDI.AddMDIChild(new CMyView() , _T("MyView") , ID_MDI_VIEW_MY );


	if (m_MyTabbedMDI.IsWindow())
	{
		m_MyTabbedMDI.SetActiveMDITab(0);
	}

	return TRUE;
}
