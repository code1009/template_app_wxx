#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class png_image_list_loader
{
private:
	CImageList* _image_list;

public:
	png_image_list_loader(CImageList* image_list, int cx, int cy);
	virtual ~png_image_list_loader();

public:
	bool add(const char* name);
};
