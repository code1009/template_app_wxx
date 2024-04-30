#pragma once



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyTabbedMDI : public CTabbedMDI
{
public:
    CMyTabbedMDI();
    virtual ~CMyTabbedMDI();

public:
	virtual CWnd* NewMDIChildFromID(int mdiChild);
	virtual CWnd* AddMDIChild      (CWnd* pView, LPCTSTR pTabText, int mdiChildID = 0);
};


