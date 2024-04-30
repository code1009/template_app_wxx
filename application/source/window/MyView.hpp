#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyView : public CWnd
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
