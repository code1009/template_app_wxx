/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "WindowID.hpp"
#include "MyTabbedMDI.hpp"
#include "Mainframe.hpp"

#include "MyView.hpp"
#include "MyDocker.hpp"
#include "EventMessageDocker.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMainFrame::CMainFrame() :
	m_isContainerTabsAtTop(FALSE),
	m_isHideSingleTab     (TRUE),
	m_isMDITabsAtTop      (TRUE), 
	m_pActiveDocker(NULL)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	// Set m_MyTabbedMDI as the view window of the frame
	SetView(m_MyTabbedMDI);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	#if 0
	LoadRegistrySettings(_T("Win32++\\TabbedMDI Docking"));
	#endif
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

//===========================================================================
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// Call the base class function first
	CDockFrame::PreCreate(cs);

	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;

	//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
}

//===========================================================================
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
	// UseThemes(FALSE);             // Don't use themes
	// UseToolBar(FALSE);            // Don't use a ToolBar

	// call the base class function
	return CDockFrame::OnCreate(cs);
}

void CMainFrame::OnInitialUpdate()
{
	SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
//	if (!LoadDockRegistrySettings(GetRegistryKeyName()))
	{
		LoadDefaultDockers();
	}

	// Load MDI child settings
//	if (!m_MyTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
	{
		LoadDefaultMDIs();
	}

	// Hide the container's tab if it has just one tab
	HideSingleContainerTab(m_isHideSingleTab);

	// Get a copy of the Frame's menu
	CMenu frameMenu = GetFrameMenu();

	// Modify the menu
	int nMenuPos = frameMenu.GetMenuItemCount() - 1;
	CMenu winMenu = m_MyTabbedMDI.GetListMenu();
	frameMenu.InsertPopupMenu(nMenuPos, MF_BYPOSITION, winMenu, _T("&Window"));

	// Replace the frame's menu with our modified menu
	SetFrameMenu(frameMenu);

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow(GetInitValues().showCmd);
	RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

LRESULT CMainFrame::OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam)
{
	// Update the "Window" menu
	m_MyTabbedMDI.GetListMenu();

	return CDockFrame::OnInitMenuPopup(msg, wparam, lparam);
}

void CMainFrame::OnMenuUpdate(UINT id)
// Called when menu items are about to be displayed
{
	// Only for the Menu IDs we wish to modify
	if (id >= IDM_EDIT_UNDO && id <= IDM_EDIT_DELETE)
	{
		// Get the pointer to the active view
		CWnd* pView = m_pActiveDocker->GetActiveView();
    
		// Enable the Edit menu items for CViewText views, disable them otherwise
		CMenu editMenu = GetFrameMenu().GetSubMenu(1);

	//	UINT flags = (dynamic_cast<CViewText*>(pView))? MF_ENABLED : MF_GRAYED;
	//	editMenu.EnableMenuItem(id, MF_BYCOMMAND | flags);
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
	CDockFrame::SetupMenuIcons();

	// Add some extra icons for menu items
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

//	SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	/*
	// Use larger buttons with seperate imagelists for normal, hot and disabled buttons.
	SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);



	// Configure the "New" toolbar button to bring up a menu
	// Setting this style requires comctl32.dll version 4.72 or later
	if (GetComCtlVersion() >= 472)
	{
		GetToolBar().SetButtonStyle(IDM_FILE_NEW, BTNS_WHOLEDROPDOWN);
	}
	*/
}

//===========================================================================
CDocker* CMainFrame::NewDockerFromID(int dockID)
{
	CDocker* pDocker = NULL;


	switch (dockID)
	{
	case ID_DOCKER_MY:
		pDocker = new CMyDocker();
		break;

	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDocker;
}

//===========================================================================
BOOL CMainFrame::SaveRegistrySettings()
{
	#if 0
	CDockFrame::SaveRegistrySettings();
	#endif

	#if 0
	// Save the docker settings
	SaveDockRegistrySettings(GetRegistryKeyName());

	// Save the tabbedMDI settings
	m_MyTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());
	#endif

	return TRUE;
}

//===========================================================================
LRESULT CMainFrame::OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam)
// Called when a docker is activated.
// Store the active docker in preparation for menu input. Excludes active 
// docker change for undocked dockers when using the menu.
{
	TRACE ("CMainFrame::OnDockActivated()\n");

	CPoint pt = GetCursorPos();
	if (WindowFromPoint(pt) != GetMenuBar())
	{
		m_pActiveDocker = GetActiveDocker();
	}

	return CDockFrame::OnDockActivated(msg, wparam, lparam);
}

LRESULT CMainFrame::OnDockDestroyed(UINT msg, WPARAM wparam, LPARAM lparam)
{
	TRACE ("CMainFrame::OnDockDestroyed()\n");

	CDocker* pDocker = reinterpret_cast<CDocker*>(wparam);

	int id = pDocker->GetDockID();

	//TRACE (_T("OnDockDestroyed() : id = %d"), id);

	return CDockFrame::OnDockDestroyed(msg, wparam, lparam);
}

//===========================================================================
void CMainFrame::LoadDefaultDockers()
{
	CDocker* pDockTop   ;
	CDocker* pDockRight ;
	CDocker* pDockBottom;
	CDocker* pDockLeft  ;


	pDockTop   	= NULL;
	pDockRight 	= NULL;
	pDockBottom	= NULL;
	pDockLeft  	= NULL;


	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry
	DWORD style;

	style = 0;//DS_CLIENTEDGE; // The style added to each docker

	
	// Add the parent dockers
	pDockLeft = AddDockedChild(new CMyDocker (), DS_DOCKED_LEFT | style, 400, ID_DOCKER_MY );

	// Add the remaining dockers
	pDockLeft->AddDockedChild(new CEventMessageDocker (), DS_DOCKED_CONTAINER | style, 400, ID_DOCKER_EVENTMESSAGE );
}

void CMainFrame::LoadDefaultMDIs()
{
	m_MyTabbedMDI.GetTab().SetPadding(200,100);
	
	// Add some MDI tabs
	m_MyTabbedMDI.AddMDIChild(new CMyView() , _T("MyView") , ID_MDI_VIEW_MY );


	if (m_MyTabbedMDI.IsWindow())
	{
		m_MyTabbedMDI.SetActiveMDITab(0);
	}
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
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//	switch (msg)
//	{
//	
//	}

	// Always pass unhandled messages on to WndProcDefault
	return WndProcDefault(msg, wparam, lparam);
}

//===========================================================================
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
	// OnCommand responds to menu and and toolbar input
	UINT id = LOWORD(wparam);
	switch (id)
	{
	case IDM_CLOSE_DOCKERS:     return OnCloseDockers();
	case IDM_CLOSE_MDIS:        return OnCloseMDIs();
	case IDM_DEFAULT_LAYOUT:    return OnDefaultLayout();

	case IDM_CONTAINER_TOP:     return OnContainerTabsAtTop();
	case IDM_HIDE_SINGLE_TAB:   return OnHideSingleTab();
	case IDM_TABBEDMDI_TOP:     return OnMDITabsAtTop();

	case IDM_FILE_NEW:          return OnFileNew();
	case IDM_FILE_EXIT:         return OnFileExit();

	case IDM_HELP_ABOUT:        return OnHelp();

	case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:      return OnViewToolBar();

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
			m_pActiveDocker->GetActiveView()->SendMessage(WM_COMMAND, wparam, lparam);
		}
	}

	return FALSE;
}

//===========================================================================
BOOL CMainFrame::OnDefaultLayout()
{
	SetRedraw(FALSE);

    
	CloseAllDockers();
	m_MyTabbedMDI.CloseAllMDIChildren();
    
	
	LoadDefaultDockers();
	LoadDefaultMDIs();

	SetContainerTabsAtTop (m_isContainerTabsAtTop);
	HideSingleContainerTab(m_isHideSingleTab);
	SetMDITabsAtTop       (m_isMDITabsAtTop);
	

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
// Reposition the tabs in the containers
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
// Reposition TabbedMDI's tabs
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
