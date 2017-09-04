/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define  WEIBOHTTP_CPP
#include "../StdAfx.h"
#include <curl/curl.h>
#include "WeiBoHttp.h"
#include "..//OAuth/oauth.h"

struct TMemoryStruct {
	char *data;
	size_t size; //< bytes remaining (r), bytes accumulated (w)

	size_t start_size; //< only used with ..AndCall()
	void (*callback)(void*,int,size_t,size_t); //< only used with ..AndCall()
	void *callback_data; //< only used with ..AndCall()
};
static size_t TWriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) {
	size_t realsize = size * nmemb;
	struct TMemoryStruct *mem = (struct TMemoryStruct *)data;

	mem->data = (char *)realloc(mem->data, mem->size + realsize + 1);
	if (mem->data) {
		memcpy(&(mem->data[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->data[mem->size] = 0;
	}
	return realsize;
}

int	split_str(char *str, char ch,CStringArray &arrChars)
{
	char *t_str = NULL;
	char *p = NULL;
	char *t = NULL;
	int i = 0;
	assert(str);
	t_str = (char *)malloc(strlen(str)+1);
	if(!t_str){
		return -1;
	}
	t = t_str;
	strcpy(t_str, str);
	p = t_str;

	CString strTemp;

	while(*p){    
		if(*p == ch){
			*p++ = '\0';
			strTemp.Empty();
			strTemp.Format(_T("%S"),t);
			arrChars.Add(strTemp);
			i++;
			t = p;
		}
		else
			p++;
	}
	if(t_str){
		strTemp.Empty();
		strTemp.Format(_T("%S"),t);
		arrChars.Add(strTemp);
		assert(t && p);
		if( i != 0 )
			free(t_str);
		else{
			t_str = t = p = NULL;
			return -1;
		}
		t_str = t = p = NULL;        
	} 
	return 0;
}

WeiBoHttp::WeiBoHttp(void)
{
		Clear();

		m_strConsumerkey << APPKEY;
		m_strConsumerSecrect << APPSECRET;
}

WeiBoHttp::~WeiBoHttp(void)
{
		Clear();
}

void	WeiBoHttp::Clear()
{
		m_strKey.Empty();
		m_Str.Empty();
		m_loginKey.Empty();

		m_strConsumerkey.Empty();
		m_strConsumerSecrect.Empty();
		m_strToken.Empty();
		m_strSecrect.Empty();
		m_instrReturn.Empty();
}

void	WeiBoHttp::SetValidate(CString strInfo)
{
		m_loginKey = strInfo;
		int len = 128;
		char sChar[128];
		char tChar[128];
		memset(&sChar,0,sizeof(sChar));
		memset(&tChar,0,sizeof(tChar));
		wcstombs(sChar,strInfo.GetBuffer(0),strInfo.GetLength());
		Base64Encode((BYTE*)sChar,strlen(sChar),tChar,&len);
		m_strKey.Empty();
		m_strKey.Format(_T("Authorization: Basic %S"),tChar);
}

CString	&WeiBoHttp::PostImageString(CString &inStr,InStreamA &postData,BOOL bR /* = FALSE */)
{
		m_Str.Empty();

		INTERNET_PORT   nPort = 80;
		CString strServerName = _T("api.t.sina.com.cn");

		CHttpFile	*pHttpFile = NULL;
		CHttpConnection *pHttpConnection = NULL;
		CInternetSession	session;
		CString strHaed  = _T("Content-Type:multipart/form-data;boundary=---------------------------7d33a816d302b6\r\n");

		try{
			pHttpConnection = session.GetHttpConnection(strServerName,nPort);
			pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,inStr.GetBuffer(0));
		}
		catch(CInternetException *pException){
			pHttpFile = NULL;
			session.Close();
			return m_Str;
		}

		if(pHttpFile){
			DWORD dwStatus;
			CString strTemp;
			InStreamW okStr;

			DWORD dwBuffLen = sizeof(dwStatus);
			BOOL bSuccess = pHttpFile->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);

			if( bSuccess ){
				pHttpFile->AddRequestHeaders(m_strKey);
				BOOL result = pHttpFile->SendRequest(strHaed, (LPVOID)postData.GetBuf(), postData.GetBufLen());
				if(result && bR){
					while(pHttpFile->ReadString(strTemp)){
						okStr.Empty();
						okStr.PutUTF8Char((char*)strTemp.GetBuffer(0));
						strTemp.Format(_T("%s"),(WCHAR*)okStr.GetBuf());
						m_Str += strTemp;
						strTemp.Empty();
					}
				}

			}
			pHttpFile->Close();
			delete pHttpFile;
			pHttpConnection->Close();
			delete pHttpConnection;
		}
		session.Close();

		return m_Str;

}

CString	&WeiBoHttp::PostString(CString &inStr,InStreamA &postData,BOOL bR /* = FALSE */)
{
		m_Str.Empty();

		INTERNET_PORT   nPort = 80;
		CString strServerName = _T("api.t.sina.com.cn");

		CHttpFile	*pHttpFile = NULL;
		CHttpConnection *pHttpConnection = NULL;
		CInternetSession	session;
		CString strHeaders = _T("Content-Type: application/x-www-form-urlencoded\r\n");

		try{
			pHttpConnection = session.GetHttpConnection(strServerName,nPort);
			pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,inStr.GetBuffer(0));
		}
		catch(CInternetException *pException){
			pHttpFile = NULL;
			session.Close();
			return m_Str;
		}

		if(pHttpFile){
			DWORD dwStatus;
			CString strTemp;
			InStreamW okStr;
			
			DWORD dwBuffLen = sizeof(dwStatus);
			BOOL bSuccess = pHttpFile->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);

			if( bSuccess ){
				pHttpFile->AddRequestHeaders(m_strKey);
				BOOL result = pHttpFile->SendRequest(strHeaders, (LPVOID)postData.GetBuf(), postData.GetBufLen());
				if(result && bR && pHttpFile){
					while(pHttpFile->ReadString(strTemp)){
						okStr.Empty();
						okStr.PutUTF8Char((char*)strTemp.GetBuffer(0));
						strTemp.Format(_T("%s"),(WCHAR*)okStr.GetBuf());
						m_Str += strTemp;
						strTemp.Empty();
					}
				}

			}
			pHttpFile->Close();
			delete pHttpFile;
			pHttpConnection->Close();
			delete pHttpConnection;
		}
		session.Close();

		return m_Str;
}

CString	&WeiBoHttp::RequestString(CString inUrl)
{
		CHttpFile	*pHttpFile = NULL;
		m_Str.Empty();
		CInternetSession	m_session;
		m_session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);
		m_session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000);
		m_session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 10000);
		m_session.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 2000);
		m_session.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 7000);
		m_session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); 
		try{
			pHttpFile = (CHttpFile*)m_session.OpenURL(inUrl);
		}
		catch(CInternetException *pException){
			pHttpFile = NULL;
			//TRACE("WeiBoHttp INFO: m_session.OpenURL(inUrl); pException");
			m_session.Close();
			return m_Str;
		}
		
		if(pHttpFile){
			DWORD dwStatus;
			DWORD dwBuffLen = sizeof(dwStatus);
			CString strTemp;
			InStreamW okStr;
			BOOL bSuccess = pHttpFile->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBuffLen);
			if( bSuccess && dwStatus>= 200 && pHttpFile) {
				pHttpFile->AddRequestHeaders(m_strKey);
				BOOL result = pHttpFile->SendRequest();
				if(result){
					try{
						while(pHttpFile->ReadString(strTemp)){
							okStr.Empty();
							okStr.PutUTF8Char((char*)strTemp.GetBuffer(0));
							strTemp.Format(_T("%s"),(WCHAR*)okStr.GetBuf());
							m_Str += strTemp;
							strTemp.Empty();
						}
					}
					catch(CException *pException){
						//TRACE("WeiBoHttp INFO: ReadString CException");
						pHttpFile->Close();
						SAFE_DELETE(pHttpFile);
						return m_Str;
					}

				}
			}
			pHttpFile->Close();
			SAFE_DELETE(pHttpFile);

		}
		return m_Str;
}

BOOL	WeiBoHttp::RequestImage(CString inUrl,CString strPath)
{
		HRESULT ret = ::URLDownloadToFile(NULL,inUrl,strPath,NULL,NULL);
		if(FAILED(ret)){
			return FALSE;
		}
		return TRUE;
}

BOOL	WeiBoHttp::GetUserImage(CString &outPath,const CHAR *pChar,CString &inUrl)
{
		if(!pChar) return FALSE;

		CString strTemp;
		CString strTemp2;
		WCHAR FilePath[_MAX_PATH];
		GetCurrentDirectory(_MAX_PATH,FilePath);
		strTemp2.Format(_T("%s\\%S"),FilePath,TEMPUSERPATH);
		strTemp.Format(_T("%s\\%S.jpg"),strTemp2,pChar);
		CFileFind find;
		DWORD   dwAttr = GetFileAttributes(strTemp2.GetBuffer(0));
		if(dwAttr == 0xFFFFFFFF){
			if(!CreateDirectory(strTemp2.GetBuffer(0),NULL)){
				ASSERT(FALSE);
				return FALSE;
			}
		}
		if(find.FindFile(strTemp.GetBuffer(0))){
			outPath.Format(_T("%s"),strTemp.GetBuffer(0));
			return TRUE;
		}
		else{
			if(RequestImage(inUrl,strTemp)){
				outPath.Format(_T("%s"),strTemp.GetBuffer(0));
				return TRUE;
			}
		}

		return FALSE;
}

BOOL	WeiBoHttp::GetInfoImage(CString &outPath,WCHAR *pChar,CString &inUrl)
{
		if(!pChar) return FALSE;
		CString strTemp;
		CString strTemp2;
		WCHAR FilePath[_MAX_PATH];
		GetCurrentDirectory(_MAX_PATH,FilePath);
		strTemp2.Format(_T("%s\\%S"),FilePath,TEMPITEMPATH);
		strTemp.Format(_T("%s\\%s.jpg"),strTemp2,pChar);

		CFileFind find;
		DWORD   dwAttr = GetFileAttributes(strTemp2.GetBuffer(0));
		if(dwAttr == 0xFFFFFFFF){
			if(!CreateDirectory(strTemp2.GetBuffer(0),NULL)){
				ASSERT(FALSE);
				return FALSE;
			}
		}

		if(find.FindFile(strTemp.GetBuffer(0))){
			outPath.Format(_T("%s"),strTemp.GetBuffer(0));
			return TRUE;
		}
		else{
			if(RequestImage(inUrl,strTemp)){
				outPath.Format(_T("%s"),strTemp.GetBuffer(0));
				return TRUE;
			}
		}

		return FALSE;
}

void	WeiBoHttp::OpenUrl(CString &str)
{
		CString strUrl;
		strUrl.Format(_T("http://%s@weibo.com/n/%s"),m_loginKey.GetBuffer(0),str.GetBuffer(0));
		//ShellExecute(NULL, _T("open"), strUrl.GetBuffer(0),NULL, NULL, SW_SHOWNORMAL);
		IWebBrowser2*    pBrowser2;
		CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, 
			IID_IWebBrowser2, (void**)&pBrowser2);
		if(pBrowser2){
			BSTR bstrURL = strUrl.AllocSysString();
			COleVariant str(_T("_blank"));
			COleVariant strHead(m_strKey.GetBuffer(0));
			pBrowser2->Navigate(bstrURL,NULL,&str,NULL,&strHead);
			pBrowser2->put_Visible(VARIANT_TRUE);
			pBrowser2->Release();
			SysFreeString(bstrURL);
		}
}

void	WeiBoHttp::ConvertUTF8toGB2312(char *pInChar,CString &outStr)
{
		InStreamW inStr;
		InStreamW okStr;
		inStr = okStr.PutUTF8Char(pInChar);
		outStr.Format(_T("%s"),inStr.GetBuf());
}

void	WeiBoHttp::SetOauthToken(CHAR *pKey,CHAR *pSecrect)
{
		m_strToken.Empty();
		m_strSecrect.Empty();
		m_strToken << pKey;
		m_strSecrect << pSecrect;

		m_strConsumerkey.Empty();
		m_strConsumerSecrect.Empty();
		m_strConsumerkey << APPKEY;
		m_strConsumerSecrect << APPSECRET;
}

CString	WeiBoHttp::OauthRequestString(InStreamA &inUrl)
{	
		char *postarg = NULL;
		char *req_url = NULL;
		char *reply   = NULL;
		req_url = oauth_sign_url2(inUrl.GetBuf(), &postarg, OA_HMAC, NULL, m_strConsumerkey.GetBuf(), m_strConsumerSecrect.GetBuf(), m_strToken.GetBuf(), m_strSecrect.GetBuf());

		reply = oauth_http_post(req_url,postarg);
		SAFE_FREE(req_url);
		SAFE_FREE(postarg);

		m_instrReturn.Empty();
		m_instrReturn.PutUTF8Char(reply);
		free(reply);

		CString strTemp;
		strTemp.Format(_T("%s"),(WCHAR*)m_instrReturn.GetBuf());

		return strTemp;
}
 
CString	WeiBoHttp::OauthPostPic(InStreamA &inUrl,InStreamA &insText,InStreamA &inPath)
{
		
		char *postarg = NULL;
		char *req_url = NULL;
		req_url = oauth_sign_url2(inUrl.GetBuf(), &postarg, OA_HMAC, NULL, m_strConsumerkey.GetBuf(), m_strConsumerSecrect.GetBuf(), m_strToken.GetBuf(), m_strSecrect.GetBuf());
		inUrl.Empty();
		inUrl << "http://api.t.sina.com.cn/statuses/upload.xml"; 
		SAFE_FREE(req_url);

		struct curl_httppost *post = NULL;
		struct curl_httppost *last = NULL;
		CURL *curl  = curl_easy_init();

		int rc = 0;
		int find = 0;
		CString strTemp;
		CString strTemp2;
		CString strTemp3;
		InStreamA insa1;
		InStreamA insa2;
		char **rv = NULL;
		CStringArray arrStr;
		split_str(postarg,'&', arrStr);
		rc = arrStr.GetSize();
		for(DWORD i = 0; i < rc; i ++){
			strTemp = arrStr.GetAt(i);
			find = strTemp.Find(_T("="));
			strTemp2 = strTemp.Left(find);
			strTemp3 = strTemp.Right(strTemp.GetLength() - find -1);
			if(strTemp2.Find(_T("status")) != -1){
				continue;
			}
			insa1.Empty();
			insa2.Empty();
			insa1.PutWChar(strTemp2.GetBuffer(0));
			insa2.PutWChar(strTemp3.GetBuffer(0));
			curl_formadd(&post,&last,CURLFORM_COPYNAME,insa1.GetBuf(),CURLFORM_COPYCONTENTS,insa2.GetBuf(),CURLFORM_END);
		}

		curl_formadd(&post,&last,CURLFORM_COPYNAME,"status",CURLFORM_COPYCONTENTS,insText.GetBuf(),CURLFORM_END);

		insa2.Empty();
		strTemp.Format(_T("%S"),postarg);
		SAFE_FREE(postarg);

		find = strTemp.Find(_T("oauth_signature="));
		strTemp3 = strTemp.Right(strTemp.GetLength()-find);
		strTemp3.Replace(_T("oauth_signature="),_T(""));
		insa2.PutWChar(strTemp3.GetBuffer(0));

		char *pOut = oauth_url_unescape(insa2.GetBuf(),NULL);
		curl_formadd(&post,&last,CURLFORM_COPYNAME,"oauth_signature",CURLFORM_COPYCONTENTS,pOut,CURLFORM_END);
		SAFE_FREE(pOut);
		curl_formadd(&post,&last,CURLFORM_COPYNAME,"pic",CURLFORM_FILE,inPath.GetBuf(),CURLFORM_END);


		struct TMemoryStruct chunk;
		chunk.data=NULL;
		chunk.size = 0;
		curl_easy_setopt(curl,CURLOPT_URL,inUrl.GetBuf());
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TWriteMemoryCallback);

		CURLcode r = curl_easy_perform(curl);

		curl_formfree(post);
		curl_easy_cleanup(curl);
		curl_global_cleanup();

		m_instrReturn.Empty();
		m_instrReturn.PutUTF8Char(chunk.data);
		SAFE_FREE(chunk.data);

		strTemp.Empty();
		strTemp.Format(_T("%s"),m_instrReturn.GetBuf());

		return strTemp;
}

WeiBoHttp g_weiBoHttp;
