/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <res/resource.h>

//===========================================================================
#include "MyID.hpp"
#include "MyTabbedMDI.hpp"
#include "MyView.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CMyTabbedMDI::CMyTabbedMDI()
{
}

CMyTabbedMDI::~CMyTabbedMDI()
{
}

//===========================================================================
CWnd* CMyTabbedMDI::NewMDIChildFromID(int mdiChild)
{
	CWnd* pView = NULL;

	
	switch(mdiChild)
	{
	case ID_MDI_VIEW_MY:
		pView = new CMyView();
		break;

	default:
		TRACE("Unknown TabbedMDI id\n");
		break;
	}

	return pView;
}
