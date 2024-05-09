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
	int m_windowMenuItemCount;
	CImageList m_ilToolbar;
	CImageList m_ilCommand_32;
	CImageList m_ilCommand_24;
	CImageList m_ilCommand_16;

	//-----------------------------------------------------------------------
public:
	CMainFrame();
	virtual ~CMainFrame();

	//-----------------------------------------------------------------------
private:
	CMainFrame(const CMainFrame&);
	CMainFrame& operator=(const CMainFrame&);

	//-----------------------------------------------------------------------
	// Virtual functions that override base class functions
public:
	virtual HWND     Create(HWND parent = 0) override;
	virtual void     PreCreate(CREATESTRUCT& cs) override;
	virtual int      OnCreate(CREATESTRUCT& cs) override;

	virtual void     OnInitialUpdate() override;
	virtual LRESULT  OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam) override;
	virtual void     OnMenuUpdate(UINT id) override;

	virtual void     SetupMenuIcons() override;
	virtual void     SetupToolBar() override;
	virtual BOOL     SaveRegistrySettings() override;

	virtual CDocker* NewDockerFromID(int dockID) override;
	virtual LRESULT  OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam) override;
	virtual LRESULT  OnDockDestroyed(UINT msg, WPARAM wparam, LPARAM lparam) override;

	virtual void     OnClose() override;
	virtual BOOL     OnCommand(WPARAM wparam, LPARAM lparam) override;
	virtual LRESULT  WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

	virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;

	//-----------------------------------------------------------------------
public:
	void LoadImageList();
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


