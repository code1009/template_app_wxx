#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyListView;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyListViewInplaceEdit : public CEdit
{
public:
	CMyListView*  m_ListView;
	std::int32_t  m_Item;
	std::int32_t  m_Column;
	std::string   m_Text;

public:
	bool m_Cancel;

public:
	explicit CMyListViewInplaceEdit(CMyListView* listview, std::int32_t item, std::int32_t column, std::string text);
	virtual ~CMyListViewInplaceEdit();

private:
	CMyListViewInplaceEdit(const CMyListViewInplaceEdit&);
	CMyListViewInplaceEdit& operator=(const CMyListViewInplaceEdit&);

public:
	virtual void PreCreate(CREATESTRUCT& cs) override;
	virtual int  OnCreate (CREATESTRUCT& cs) override;
	virtual void OnDestroy() override;
	virtual void OnAttach () override;

public:
	virtual BOOL PreTranslateMessage(MSG& msg);

public:
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnNcDestroy (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnKeyDown   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnChar      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnSetFocus  (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnKillFocus (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnGetDlgCode(UINT msg, WPARAM wparam, LPARAM lparam);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyListView : public CListView
{
private:
	CImageList                           m_SmallImageList;
	std::vector<CMyListViewInplaceEdit*> m_InplaceEditContainer;

	std::vector < std::vector<std::string> > m_TestContainer;

	CFont m_Font;

public:
	CMyListView();
	virtual ~CMyListView();

private:
	CMyListView(const CMyListView&);
	CMyListView& operator=(const CMyListView&);

public:
	virtual void    PreCreate       (CREATESTRUCT& cs) override;
	virtual void    OnDestroy       () override;
	virtual void    OnAttach        () override;
	virtual LRESULT WndProc         (UINT msg, WPARAM wparam, LPARAM lparam) override;
	virtual LRESULT OnNotifyReflect (WPARAM wparam, LPARAM lparam) override;
	virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam) override;

public:
	LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnLButtonDown  (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnLButtonDbClk (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnHScroll      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnVScroll      (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseWheel   (UINT msg, WPARAM wparam, LPARAM lparam);

	LRESULT InplaceEdit_OnDelete   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT InplaceEdit_OnUpdate   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT InplaceEdit_OnNavigate (UINT msg, WPARAM wparam, LPARAM lparam);
	void    InplaceEdit_New        (int item, int column);

public:
	LRESULT OnLvnGetDispInfo   (WPARAM wparam, LPARAM lparam);
	LRESULT OnLvnBeginLabelEdit(WPARAM wparam, LPARAM lparam);

	LRESULT OnNmCustomDraw   (WPARAM wparam, LPARAM lparam);
	LRESULT OnPrePaint       (WPARAM wparam, LPARAM lparam);
	LRESULT OnPostPaint      (WPARAM wparam, LPARAM lparam);
	LRESULT OnPreErase       (WPARAM wparam, LPARAM lparam);
	LRESULT OnPostErase      (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPrePaint   (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostPaint  (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPreErase   (WPARAM wparam, LPARAM lparam);
	LRESULT OnItemPostErase  (WPARAM wparam, LPARAM lparam);
	LRESULT OnSubItemPrePaint(WPARAM wparam, LPARAM lparam);

public:
	LRESULT OnMeasureItem(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnDrawItem   (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnDeleteItem (UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnCompareItem(UINT msg, WPARAM wparam, LPARAM lparam);

public:
	void SetDPIImages();

	void DrawItem   (LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	int  CompareItem(LPCOMPAREITEMSTRUCT /*lpCompareItemStruct*/);
	void DeleteItem (LPDELETEITEMSTRUCT /*lpDeleteItemStruct*/);

	void GetCellRect(int header_column, const CRect& item_rect, CRect& cell_rect);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyListViewDockContainer : public CDockContainer
{
private:
	CMyListView m_Wnd;

public:
	CMyListViewDockContainer();
	~CMyListViewDockContainer();

private:
	CMyListViewDockContainer(const CMyListViewDockContainer&);
	CMyListViewDockContainer& operator=(const CMyListViewDockContainer&);

protected:
	virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
	virtual void SetupToolBar() override;
	virtual LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyListViewDocker : public CDocker
{
private:
	CMyListViewDockContainer m_DockContainer;

public:
	CMyListViewDocker();
	virtual ~CMyListViewDocker();

private:
	CMyListViewDocker(const CMyListViewDocker&);
	CMyListViewDocker& operator=(const CMyListViewDocker&);
};

