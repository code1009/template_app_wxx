#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMainFrame : public CDockFrame
{
private:
	BOOL m_isContainerTabsAtTop;
	BOOL m_isHideSingleTab     ;
	BOOL m_isMDITabsAtTop      ;

	CDocker* m_pActiveDocker;

	CMyTabbedMDI m_MyTabbedMDI;

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual void     PreCreate           (CREATESTRUCT& cs);
	virtual int      OnCreate            (CREATESTRUCT& cs);
	virtual void     OnInitialUpdate     ();
	virtual void     SetupMenuIcons      ();
	virtual void     SetupToolBar        ();
	virtual CDocker* NewDockerFromID     (int dockID);
	virtual BOOL     SaveRegistrySettings();
	virtual LRESULT  OnInitMenuPopup     (UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void     OnMenuUpdate        (UINT id);
	virtual LRESULT  OnDockActivated     (UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT  OnDockDestroyed     (UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT  WndProc             (UINT msg, WPARAM wparam, LPARAM lparam);
	virtual BOOL     OnCommand           (WPARAM wparam, LPARAM lparam);

public:
	void LoadDefaultDockers();
	void LoadDefaultMDIs   ();

	void SetContainerTabsAtTop (BOOL atTop);
	void HideSingleContainerTab(BOOL hideSingle);
	void SetMDITabsAtTop       (BOOL atTop);

	BOOL OnDefaultLayout     ();
	BOOL OnCloseMDIs         ();
	BOOL OnCloseDockers      ();
	BOOL OnContainerTabsAtTop();
	BOOL OnHideSingleTab     ();
	BOOL OnMDITabsAtTop      ();
	BOOL OnFileNew           ();
	BOOL OnFileExit          ();
};


