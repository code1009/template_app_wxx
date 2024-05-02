#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMainFrame : public CDockFrame
{
private:
	CMyTabbedMDI m_MyTabbedMDI;
	CDocker* m_pActiveDocker;
	BOOL m_isContainerTabsAtTop;
	BOOL m_isHideSingleTab;
	BOOL m_isMDITabsAtTop;

	//-----------------------------------------------------------------------
public:
	CMainFrame();
	virtual ~CMainFrame();

	//-----------------------------------------------------------------------
private:
	CMainFrame(const CMainFrame&);               // Disable copy construction
	CMainFrame& operator=(const CMainFrame&);    // Disable assignment operator

	//-----------------------------------------------------------------------
	// Virtual functions that override base class functions
public:
	virtual HWND     Create(HWND parent = 0);
	virtual void     PreCreate(CREATESTRUCT& cs);
	virtual int      OnCreate(CREATESTRUCT& cs);

	virtual void     OnInitialUpdate();
	virtual LRESULT  OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void     OnMenuUpdate(UINT id);

	virtual void     SetupMenuIcons();
	virtual void     SetupToolBar();
	virtual BOOL     SaveRegistrySettings();

	virtual CDocker* NewDockerFromID(int dockID);
	virtual LRESULT  OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT  OnDockDestroyed(UINT msg, WPARAM wparam, LPARAM lparam);

	virtual void     OnClose();
	virtual BOOL     OnCommand(WPARAM wparam, LPARAM lparam);
	virtual LRESULT  WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	//-----------------------------------------------------------------------
public:
	void LoadDefaultDockers();
	void LoadDefaultMDIs();

	void SetContainerTabsAtTop(BOOL atTop);
	void HideSingleContainerTab(BOOL hideSingle);
	void SetMDITabsAtTop(BOOL atTop);

	//-----------------------------------------------------------------------
	// Message handlers
public:
	LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

	//-----------------------------------------------------------------------
	// Command handlers
public:
	BOOL OnDefaultLayout();
	BOOL OnCloseMDIs();
	BOOL OnCloseDockers();

	BOOL OnContainerTabsAtTop();
	BOOL OnHideSingleTab();
	BOOL OnMDITabsAtTop();

	BOOL OnFileNew();
	BOOL OnFileExit();
};


