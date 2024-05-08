/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "stdafx.h"

//===========================================================================
#include <gdiplus.h>

//===========================================================================
#include "png_image_list_loader.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(lib, "gdiplus.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CGdiPlusLoader
{
public:
	int m_RefCount;
	ULONG_PTR m_Token;

public:
	CGdiPlusLoader();
	~CGdiPlusLoader();

public:
	bool Startup(void);
	void Shutdown(void);
};

//===========================================================================
CGdiPlusLoader::CGdiPlusLoader():
	m_RefCount{0},
	m_Token{0}
{
}

CGdiPlusLoader::~CGdiPlusLoader()
{
}

bool CGdiPlusLoader::Startup(void)
{
	//-----------------------------------------------------------------------
	m_RefCount++;
	if (1 != m_RefCount)
	{
		return true;
	}


	//-----------------------------------------------------------------------
	HRESULT hResult;


	hResult = ::OleInitialize(NULL);
	if (!SUCCEEDED(hResult))
	{

	}


	//-----------------------------------------------------------------------
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;


	Gdiplus::Status status;


	status = Gdiplus::GdiplusStartup(&m_Token, &gdiplusStartupInput, NULL);
	if (Gdiplus::Ok != status)
	{

	}


	//-----------------------------------------------------------------------

	return true;
}

void CGdiPlusLoader::Shutdown(void)
{
	//-----------------------------------------------------------------------
	m_RefCount--;
	if (0 != m_RefCount)
	{
		return;
	}


	//-----------------------------------------------------------------------
	Gdiplus::GdiplusShutdown(m_Token);


	//-----------------------------------------------------------------------
	::OleUninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CGdiPlusBitmap
{
public:
	Gdiplus::Bitmap* m_pBitmap;

public:
	CGdiPlusBitmap();
	CGdiPlusBitmap(LPCWSTR pFile);
	virtual ~CGdiPlusBitmap();

public:
	void Empty();
	bool Load(LPCWSTR pFile);
	operator Gdiplus::Bitmap*() const;
};

//===========================================================================
CGdiPlusBitmap::CGdiPlusBitmap()
{
	m_pBitmap = NULL; 
}

CGdiPlusBitmap::CGdiPlusBitmap(LPCWSTR pFile) 
{
	m_pBitmap = NULL; 
	Load(pFile); 
}

CGdiPlusBitmap::~CGdiPlusBitmap() 
{
	Empty(); 
}

//===========================================================================
void CGdiPlusBitmap::Empty()
{ 
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}
}

bool CGdiPlusBitmap::Load(LPCWSTR pFile)
{
	Empty();
	
	
	m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);


	return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
}

CGdiPlusBitmap::operator Gdiplus::Bitmap*() const 
{ 
	return m_pBitmap; 
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CGdiPlusBitmapResource : public CGdiPlusBitmap
{
protected:
	HGLOBAL m_hBuffer;

public:
	CGdiPlusBitmapResource();
	CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	CGdiPlusBitmapResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst = NULL);
	virtual ~CGdiPlusBitmapResource();

public:
	void Empty();
	bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	bool Load(UINT id, UINT type, HMODULE hInst = NULL);
};

//===========================================================================
CGdiPlusBitmapResource::CGdiPlusBitmapResource()
{
	m_hBuffer = NULL; 
}

CGdiPlusBitmapResource::CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	m_hBuffer = NULL; 
	
	Load(pName, pType, hInst);
}

CGdiPlusBitmapResource::CGdiPlusBitmapResource(UINT id, LPCTSTR pType, HMODULE hInst)
{
	m_hBuffer = NULL; 
	
	Load(id, pType, hInst);
}

CGdiPlusBitmapResource::CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst)
{
	m_hBuffer = NULL; 
	
	Load(id, type, hInst);
}

CGdiPlusBitmapResource::~CGdiPlusBitmapResource() 
{
	Empty(); 
}

//===========================================================================
void CGdiPlusBitmapResource::Empty()
{
	CGdiPlusBitmap::Empty();

	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
}

bool CGdiPlusBitmapResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	Empty();

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
	{
		return false;
	}

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
	{
		return false;
	}

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
	{
		return false;
	}

	m_hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{
					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
					{
						return true;
					}

					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}

	return false;
}

bool CGdiPlusBitmapResource::Load(UINT id, LPCTSTR pType, HMODULE hInst)
{
	return Load(MAKEINTRESOURCE(id), pType, hInst);
}

bool CGdiPlusBitmapResource::Load(UINT id, UINT type, HMODULE hInst)
{
	return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst);
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static CGdiPlusLoader _GdiPlusLoader;

//===========================================================================
png_image_list_loader::png_image_list_loader(CImageList* image_list, int cx, int cy)
{
	//-----------------------------------------------------------------------
	_GdiPlusLoader.Startup();


	//-----------------------------------------------------------------------
	_image_list = image_list;


	_image_list->Create(cx, cy, ILC_COLOR32 | ILC_MASK, 0, 0);
}

png_image_list_loader::~png_image_list_loader()
{
	//-----------------------------------------------------------------------
	_GdiPlusLoader.Shutdown();
}

bool png_image_list_loader::add(const char* name)
{
	HBITMAP hBitmap = NULL;

	CGdiPlusBitmapResource gdibmp;


	if (gdibmp.Load(name, _T("PNG"), NULL))
	{
		gdibmp.m_pBitmap->GetHBITMAP(Gdiplus::Color::Transparent, &hBitmap);


		ImageList_AddMasked(_image_list->operator HIMAGELIST (), hBitmap, 0);

		return true;
	}


	return false;
}
