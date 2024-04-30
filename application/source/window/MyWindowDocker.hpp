#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindow : public CWnd
{
private:
	int m_cxClientMax;
	int m_cyClientMax;

public:
	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual int     OnCreate (CREATESTRUCT& cs);
	virtual void    OnDestroy();
	virtual void    OnDraw   (CDC& dc);
	virtual LRESULT WndProc  (UINT msg, WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnSize         (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnTimer        (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnPaint        (UINT msg, WPARAM wparam, LPARAM lparam);

public:
	virtual void Draw (CDC& dc);
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindowDockContainer : public CDockContainer
{
public:
	CMyWindowDockContainer(); 
	~CMyWindowDockContainer() {}

private:
	CMyWindow m_Wnd;
};



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyWindowDocker : public CDocker
{
public:
	CMyWindowDocker(); 
	virtual ~CMyWindowDocker() {}

private:
	CMyWindowDockContainer m_DockContainer;
};

