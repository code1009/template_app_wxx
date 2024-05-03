#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CBLWnd : public CWnd
{
private:
	void* _bl_window_handler;

private:
	int m_cxClientMax;
	int m_cyClientMax;

public:
	CBLWnd();
	virtual ~CBLWnd();

private:
	CBLWnd(const CBLWnd&);
	CBLWnd& operator=(const CBLWnd&);

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
	LRESULT OnEraseBkgnd(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnVScroll(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseWheel(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	void Draw(CDC& dc);
};
