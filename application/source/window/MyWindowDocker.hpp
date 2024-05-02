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
	virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
	virtual void SetupToolBar() override;
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

