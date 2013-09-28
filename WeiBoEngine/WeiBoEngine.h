#pragma once
#ifndef WEIBOENGINE_H
#define WEIBOENGINE_H

class  InStreamW;
class  CSLock;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_DESTROY
#define SAFE_DESTROY(p)       { if(p) { p->Destroy();p=NULL; } }
#endif    

#ifndef SAFE_FREE
#define SAFE_FREE(p)       { if(p) { free(p);p=NULL; } }
#endif    

#define CLOSEHANDLE(h)			{ if (h) { CloseHandle(h);	(h)=NULL; } }

class WeiBoEngine
{
public:
	WeiBoEngine(void);
	~WeiBoEngine(void);
protected:
			DWORD		m_attentionRefreshCount;
public:
			void		Initialization();
			void		FillAttentionItem();
			void		ReplaceUrl(CString &str);
			void		ReplaceAtName(CString &str);

			void		AnalysisAttentionItem(const TiXmlElement *pEle,CString &strHtml);
			void		AnalysisAttentionRetweeted(const TiXmlElement *pEle,CString &strHtml);
	inline	DWORD		GetAttentionRefreshCount(){return m_attentionRefreshCount;}
	inline	void		AddAttentionRefreshCount(DWORD n){m_attentionRefreshCount += n;}
			void		TransformTimeStr(WCHAR *pInStr,CString &strOut);

			void		AnalysisUserInfo(const TiXmlElement *pEle,CString &strHtml);

			void		AnalysisLoginItem(const TiXmlElement *pEle,CString &strHtml);
			void		AnalysisItemPic(CString &InUrl,CString &outStrHtml,WCHAR *pInUrl = NULL);

};

extern WeiBoEngine g_WeiBoEngine;
extern InStreamW g_lookImagePath;
extern CString g_str;
extern CSLock	g_lockStr;
#endif