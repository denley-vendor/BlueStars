#pragma once
#ifndef WEIBODATA_H
#define WEIBODATA_H
#include "..//StdAfx.h"
class WeiBoData
{
public:
	WeiBoData(void);
	~WeiBoData(void);
private:
		TiXmlDocument *m_pLonginUser;
		TiXmlDocument *m_pLonginUserItem;
		TiXmlDocument *m_pAttentionItem;
		TiXmlDocument *m_pAtLoginUserItem;

public:
				void			Clear();
				void			SetLongingUserXml(TiXmlDocument *pXml);
	inline		TiXmlDocument	*GetLongingUserXml(){return m_pLonginUser;}
	const		char			*GetLonginserInfo(const char *pInfoDes);

	const		TiXmlElement	*UserFirstChildElement(const char *pDes);
	const		TiXmlElement	*FirstChildElenent(const char *pDes,const TiXmlElement *pIn);


				BOOL			IsItemBValidate(TiXmlDocument *pItem);
				void			SetLongingUserItem(TiXmlDocument *pItem);
	inline		TiXmlDocument	*GetLonginUserItem(){return m_pLonginUserItem;}
				DWORD			GetLonginUserItemCount(const CHAR *pChar);
				void			AddNewLongingItem(TiXmlNode *pNode);
				void			AddPreviousLongingItem(TiXmlNode *pNode);
	const		TiXmlElement	*GetLoginUserItem(DWORD item);
	const		CHAR			*GetLoginLastItemId();
	const		CHAR			*GetLoginFirstItemId();
	const		TiXmlElement	*FindLoginItem(WCHAR *pInId);
				BOOL			GetLoginItemText(WCHAR *pInId,CString &strOut,CString &strOut1);

				void			SetAttentionItem(TiXmlNode *pItem);
	const		TiXmlElement	*GetAttentionItem(DWORD item);
	inline		TiXmlDocument	*GetAttentionItem(){return m_pAttentionItem;}
				BOOL			GetAttentionItemMaxId(CString &instrId);
				BOOL			GetAttentionItemMinId(CString &instrId);
				DWORD			GetAttentionItemCount();
				BOOL			GetUserIdByItem(CString &strOut,WCHAR *pIn);
				BOOL			GetUserNameByItem(CString &strOut,WCHAR *pIn);
				BOOL			GetUserText(CString &strOut,WCHAR *pIn);
	const		TiXmlElement	*FindAttentionItem(WCHAR *pInId);
				void			AddNewAttentionItem(TiXmlNode *pNode);
				void			AddPreviousAttentionItem(TiXmlNode *pNode);
				BOOL			GetAttentionItemText(WCHAR *pInId,CString &strOut,CString &strOut1);

				void			SetAtLoginUserItem(TiXmlNode *pItem);
				void			AddAtLoginUserItem(TiXmlNode *pItem);
				void			AddAtPreviousUserItem(TiXmlNode *pItem);
				BOOL			GetAtItemMinId(CString &instrId);
				BOOL			GetAtItemMaxId(CString &instrId);
	const		TiXmlElement	*GetAtItem(CString &inStrId);
				void			GetUserIDByAtItem(CString &strOut,WCHAR *pIn);
				BOOL			GetAtItemText(CString &instrId,CString &strOut,CString &strOut1);
public:
	static		char			*CStringToChar(CString &inStr);
	static		BOOL			UTF8_2_GB2312(const void * pIn,int inLen,CString &outStr);
};

extern	WeiBoData g_WeiBoData;
#endif