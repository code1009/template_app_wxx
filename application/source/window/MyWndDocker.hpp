#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWnd : public CWnd
{
private:
	int m_cxClientMax;
	int m_cyClientMax;

public:
	CMyWnd();
	virtual ~CMyWnd();

private:
	CMyWnd(const CMyWnd&);
	CMyWnd& operator=(const CMyWnd&);

public:
	virtual void    PreCreate(CREATESTRUCT& cs) override;
	virtual int     OnCreate(CREATESTRUCT& cs) override;
	virtual void    OnDestroy() override;
	virtual void    OnDraw(CDC& dc) override;
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

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
class CMyWndDockContainer : public CDockContainer
{
private:
	CMyWnd m_Wnd;

public:
	CMyWndDockContainer();
	~CMyWndDockContainer();

private:
	CMyWndDockContainer(const CMyWndDockContainer&);
	CMyWndDockContainer& operator=(const CMyWndDockContainer&);

protected:
	virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
	virtual void SetupToolBar() override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWndDocker : public CDocker
{
private:
	CMyWndDockContainer m_DockContainer;

public:
	CMyWndDocker(); 
	virtual ~CMyWndDocker();

private:
	CMyWndDocker(const CMyWndDocker&);
	CMyWndDocker& operator=(const CMyWndDocker&);
};

