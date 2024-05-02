#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindow : public CWnd
{
private:
	int m_cxClientMax;
	int m_cyClientMax;

public:
	CMyWindow();
	virtual ~CMyWindow();

private:
	CMyWindow(const CMyWindow&);
	CMyWindow& operator=(const CMyWindow&);

public:
	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual int     OnCreate(CREATESTRUCT& cs);
	virtual void    OnDestroy();
	virtual void    OnDraw(CDC& dc);
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	void SetDPIImages();
	void Draw(CDC& dc);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindowDockContainer : public CDockContainer
{
private:
	CMyWindow m_Wnd;

public:
	CMyWindowDockContainer();
	~CMyWindowDockContainer();

private:
	CMyWindowDockContainer(const CMyWindowDockContainer&);
	CMyWindowDockContainer& operator=(const CMyWindowDockContainer&);

protected:
	virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
	virtual void SetupToolBar();
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindowDocker : public CDocker
{
private:
	CMyWindowDockContainer m_DockContainer;

public:
	CMyWindowDocker(); 
	virtual ~CMyWindowDocker();

private:
	CMyWindowDocker(const CMyWindowDocker&);
	CMyWindowDocker& operator=(const CMyWindowDocker&);
};

