#pragma once
#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <gdiplus.h>
using namespace Gdiplus;

class ImageTransform
{
public:
	ImageTransform(void);
	~ImageTransform(void);
public:				
		int		GetCodecClsid(const WCHAR* format,ImageCodecInfo* pImageCodecInfo,CLSID* pClsid);
		void	SaveToImage(CString &strPath,CString strSuffix);
};

#endif
