#pragma once
#ifndef WEIBOHTTP_H
#define WEIBOHTTP_H
#define MAX_READBUFFCOUNT  256
#include <afxinet.h>
#include <atlenc.h>
#include "Attach/Lock.h"
#include "..//WeiBoEngine/Data/Stream.h"

#define	APPKEY		544758078
#define APPSECRET	"ec144cec7fadb6e3ee3c2f635de14c02"
#define TEMPUSERPATH	"tempuser"
#define TEMPITEMPATH	"tempitem"

class WeiBoHttp
{
public:
		WeiBoHttp(void);
		~WeiBoHttp(void);
public:
			void		Clear();
			void		SetValidate(CString strInfo);
			CString		&PostImageString(CString &inStr,InStreamA &postData,BOOL bR = FALSE);
			CString		&PostString(CString &inStr,InStreamA &postData,BOOL bR = FALSE);
			CString		&RequestString(CString inUrl);
			BOOL		RequestImage(CString inUrl,CString strPath);
			BOOL		GetUserImage(CString &outPath,const CHAR *pChar,CString &inUrl);
			BOOL		GetInfoImage(CString &outPath,WCHAR *pChar,CString &inUrl);

			void		OpenUrl(CString &str);
	inline	CString		&GetStrKey(){return m_strKey;}
	inline	CString		&GetLoginKey(){return m_loginKey;}

			void		ConvertUTF8toGB2312(char *pInChar,CString &outStr);

			void		SetOauthToken(CHAR *pKey,CHAR *pSecrect);
			CString		OauthRequestString(InStreamA &inUrl);
			CString		OauthPostPic(InStreamA &inUrl,InStreamA &insText,InStreamA &inPath);
private:
			 CString	m_Str;
			 CString	m_strKey;
			 CString	m_loginKey;
			 CSLock		m_lock;

			 InStreamW	m_instrReturn;

			 InStreamA  m_strConsumerkey;
			 InStreamA  m_strConsumerSecrect;
			 InStreamA  m_strToken;
			 InStreamA  m_strSecrect;

};
extern WeiBoHttp g_weiBoHttp;
class	WeiBoApi
{
public:
	WeiBoApi(){
		m_appKey.Format(_T("%d"),APPKEY);
	}
						//获取登录用户信息
	inline	CString		GetUserInfo(){return _T("http://api.t.sina.com.cn/account/verify_credentials.xml?source=") + m_appKey;}
						//获取登录用户发布条目
	inline	CString		GetUserItem(){return _T("http://api.t.sina.com.cn/statuses/user_timeline.xml?source=") + m_appKey;}
						//获取我关注的人发布条目
	inline	CString		GetAttentionUserItem(){return _T("http://api.t.sina.com.cn/statuses/home_timeline.xml?source=")+m_appKey;}

	inline  CString		GetOtherUserInfo(){return _T("http://api.t.sina.com.cn/users/show.xml?source=") + m_appKey;}
						//@我的条目
	inline	CString		GetAtLoginUserItem(){return _T("http://api.t.sina.com.cn/statuses/mentions.xml?source=") + m_appKey;}
						//发布一条文本微
	inline	CString		PublishItem(){return _T("http://api.t.sina.com.cn/statuses/update.xml?source=") + m_appKey;}

						//转发一条信息
	inline	CString		Retweeted(){return _T("http://api.t.sina.com.cn/statuses/repost.xml?source=") + m_appKey;}

	inline	CString		LookItem(){return _T("http://api.t.sina.com.cn/[USERID]/statuses/[ITEMID]?source=") + m_appKey;}

	inline	CString		DelMyItem(){return _T("http://api.t.sina.com.cn/statuses/destroy/[ITEMID].xml?source=") + m_appKey;}
private:
			CString		m_appKey;
};
int split_str(char *str, char ch,CStringArray &arrChars);
#endif