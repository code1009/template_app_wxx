#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CMyTabbedMDI : public CTabbedMDI
{
public:
	CMyTabbedMDI();
	virtual ~CMyTabbedMDI();

private:
	CMyTabbedMDI(const CMyTabbedMDI&);
	CMyTabbedMDI& operator=(const CMyTabbedMDI&);

public:
	virtual CWnd* NewMDIChildFromID(int mdiChild);
};


