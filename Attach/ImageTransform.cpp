/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define  IMAGETRANSFORM_CPP
#include "..//StdAfx.h"
#include "ImageTransform.h"

ImageTransform::ImageTransform(void)
{
}

ImageTransform::~ImageTransform(void)
{
}

void	ImageTransform::SaveToImage(CString &strPath,CString strSuffix)
{
		GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		Image *pImage = Image::FromFile(strPath.GetBuffer(0));
		if(!pImage) return;

		CLSID   gifcodec;   
		ImageCodecInfo* pImageCodecInfo = NULL;
		GetCodecClsid(_T("image/") + strSuffix,pImageCodecInfo,&gifcodec);


		strPath += _T(".") + strSuffix;
		pImage->Save(strPath.GetBuffer(0),&gifcodec,0); 
		delete pImage;
		free(pImageCodecInfo);
		pImageCodecInfo = NULL;
		GdiplusShutdown(gdiplusToken);
}

int		ImageTransform::GetCodecClsid(const WCHAR* format,ImageCodecInfo* pImageCodecInfo,CLSID* pClsid)
{
		UINT num = 0;  
		UINT size = 0; 
		pImageCodecInfo = NULL;
		GetImageEncodersSize(&num, &size);
		if(size == 0) return -1; 

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL) return -1; 
		GetImageEncoders(num, size, pImageCodecInfo);

		for(UINT j = 0; j < num; ++j)
		{
			if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j; // Success
			}
		} 
		return -1;
}