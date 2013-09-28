
/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define WEIBOENGINE_CPP
#include "../StdAfx.h"
#include <boost/regex.hpp>
#include <string> 
#include "WeiBoEngine.h"
#include "..//today.h"
#include "..//Attach/ImageTransform.h"

WeiBoEngine g_WeiBoEngine;
InStreamW g_lookImagePath;
CString g_str;
CSLock	g_lockStr;

using namespace boost;
using namespace std;

WeiBoEngine::WeiBoEngine(void)
{
		m_attentionRefreshCount = 0;
}

WeiBoEngine::~WeiBoEngine(void)
{
}

void	WeiBoEngine::Initialization()
{
		CString strTemp;
		GetExePath(strTemp);
		strTemp.Replace(_T("today.exe"),_T(""));
		g_lookImagePath << strTemp.GetBuffer(0);
		g_lookImagePath << _T("images\\imgaeTile.jpg");
}

void	WeiBoEngine::FillAttentionItem()
{
		WeiBoApi api;
		CString strTemp;
		InStreamA instrApi;

		instrApi.PutWChar(api.GetAttentionUserItem().GetBuffer(0));
		instrApi << "&count=28";

		strTemp = g_weiBoHttp.OauthRequestString(instrApi);
		TiXmlDocument *pxml = new TiXmlDocument;

		InStreamA instra;
		instra.PutWCharToUTF8(strTemp.GetBuffer(0));
		pxml->Parse(instra.GetBuf());
		g_WeiBoData.SetAttentionItem(pxml);
		delete pxml;
}


void	WeiBoEngine::ReplaceUrl(CString &str)
{
		wregex reg(_T("[http:+](\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?"));
		wcmatch   mat;
		regex_search(str.GetBuffer(0),mat,reg);

		CString strUrl;
		CString strTemp  = _T("<a class=\"wb2_3\" target=\"_blank\" href=\"[URLSHOW]\">[URLSHOW]</a>");

		if(mat[0].matched){
			wstring msg(mat[0].str());
			strUrl.Format(_T("%s"),msg.c_str());
			strTemp.Replace(_T("[URLSHOW]"),strUrl.GetBuffer(0));
			str.Replace(strUrl.GetBuffer(0),strTemp.GetBuffer(0));
		}

		DWORD i = 0;
		CStringArray arrStr;
		wstring wstrTemp;
		CString strTemp1;
		wregex regat(_T("@[a-zA-Z0-9_$\u4e00-\u9fa5]+"));
		while (regex_search(str.GetBuffer(0),mat,regat)){
			if(mat[0].matched){
				wstrTemp.clear();
				wstrTemp.append(mat[0].str());
				strTemp1.Format(_T("%s"),wstrTemp.c_str());
				arrStr.Add(strTemp1);
				strTemp.Format(_T("[ITEM%d]"),i);
				str.Replace(strTemp1.GetBuffer(0),strTemp.GetBuffer(0));
				i++;
			}
		}

		DWORD count = arrStr.GetSize();
		for(DWORD i = 0; i < count; i ++){
			strTemp.Format(_T("[ITEM%d]"),i);
			strTemp1 = arrStr.GetAt(i);
			strUrl = _T("<a class=\"wb2_3\" target=\"_blank\" href=\"http://weibo.com/n/[NAME]\" >@[NAME]</a>");
			strTemp1.Remove('@');
			strUrl.Replace(_T("[NAME]"),strTemp1.GetBuffer(0));
			str.Replace(strTemp.GetBuffer(0),strUrl.GetBuffer(0));
		}
		
		//for(wcmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr){
		//	if(mat[i].matched){
		//		wstring msg(mat[i].str());
		//		strUrl = _T("<a class=\"wb2_3\" target=\"_blank\" href=\"http://weibo.com/n/[NAME]\">@[NAME]</a>");
		//		strTemp.Format(_T("%s"),msg.c_str());
		//		str.Replace(strTemp.GetBuffer(0),_T("[@NAME]"));
		//		strTemp.Remove('@');
		//		strUrl.Replace(_T("[NAME]"),strTemp.GetBuffer(0));
		//		str.Replace(_T("[@NAME]"),strUrl.GetBuffer(0));
		//	}
		//	i ++;
		//}
}

void	WeiBoEngine::ReplaceAtName(CString &str)
{
	
}

void	WeiBoEngine::AnalysisAttentionItem(const TiXmlElement *pEle,CString &strHtml)
{
		if(!pEle) return;
		strHtml = _T("");

		WeiBoHttp weibo;
		ImageTransform transImage;
		CString strTilePath;
		CString strRetweeted;

		InStreamA inStrA;
		InStreamW inStrW;
		InStreamW inStrW2;

		InStreamW strCreateAt;
		InStreamW strItemID;
		InStreamW strText;
		InStreamW strSource;
		InStreamW strImagePath;
		InStreamW strImagePath1;
		InStreamW strImagePath2;
		InStreamW strUserId;
		InStreamW strTileImage;
		InStreamW strUserName;

		const CHAR *pInfo = "created_at";
		const CHAR *pInfo1 = "id";
		const CHAR *pInfo2 = "text";
		const CHAR *pInfo3 = "source";
		const CHAR *pInfo4 = "thumbnail_pic";
		const CHAR *pInfo5 = "bmiddle_pic";
		const CHAR *pInfo6 = "original_pic";
		const CHAR *pInfo7 = "profile_image_url";
		const CHAR *pInfo8 = "screen_name";
		const CHAR *pInfo9 = "retweeted_status";
		const TiXmlElement *pEleRetweeted = g_WeiBoData.FirstChildElenent(pInfo9,pEle);
		if(pEleRetweeted){
			AnalysisAttentionRetweeted(pEleRetweeted,strRetweeted);
		}
		const TiXmlElement *pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			strHtml.Empty();
			inStrA  << pXmlInfo->GetText();
			strCreateAt = inStrW2.PutUTF8Char(inStrA.GetBuf());
			TransformTimeStr((WCHAR*)strCreateAt.GetBuf(),strHtml);
			strCreateAt.Empty();
			strCreateAt << strHtml.GetBuffer(0);
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strItemID = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo2,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strText = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo3,pEle);
		if(pXmlInfo){
			TiXmlPrinter printer; 
			inStrW2.Empty();
			inStrA.Empty();
			pXmlInfo->Accept(&printer);
			inStrA << printer.CStr();
			strSource = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo4,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo5,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath1 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo6,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath2 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}

		const CHAR *pInfoUser = "user";
		InStreamA strTempUserId;
		pEle = g_WeiBoData.FirstChildElenent(pInfoUser,pEle);

		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strTempUserId << inStrA;
			strUserId = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo7,pEle);
		if(pXmlInfo){
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			inStrW2.Empty();
			inStrW.Empty();
			strHtml.Empty();
			inStrW = inStrW2.PutUTF8Char(inStrA.GetBuf());
			strHtml.Format(_T("%s"),inStrW.GetBuf());
			//weibo.GetUserImage(strTilePath,strTempUserId.GetBuf(),strHtml);
			strTilePath = strHtml;
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo8,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strUserName = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}

		
		CString strTemp1;
		CString strTempHtml = _T("<div class=\"lh\" id=\"[ITEMID]\">");
		strTemp1.Format(_T("%d"),m_attentionRefreshCount);
		strTempHtml += _T("<div class=\"lh_1\"><div id=\"[ITEMID]\" onmouseout=\"external.UShowUserInfo();\" onmouseover=\"external.ShowUserInfo(this.id);\" class=\"imgtitle[REFRESHCOUNT]\" style=\"float:left;display:inline;margin-top:5px;\"><img src=\"[USERTITLEIMAGE]\" width='42' height='41' /></div></div>");
		strTempHtml.Replace(_T("[ITEMID]"),(WCHAR*)strItemID.GetBuf());
		strTempHtml.Replace(_T("[REFRESHCOUNT]"),strTemp1.GetBuffer(0));
		strTempHtml.Replace(_T("[USERTITLEIMAGE]"),strTilePath.GetBuffer(0));

		strTemp1.Empty();
		strTemp1 += _T("<div class=\"lh_2\">");
		strTemp1 += _T("<div class=\"lh_2_1\"><span class=\"two\">[CREATETIME]</span><span><a  class=\"wenzi\" href=\"http://t.sina.com.cn/[USERID]\" target=\"_blank\">[CREATENAME]</a></span></div>");
		strTemp1.Replace(_T("[CREATETIME]"),(WCHAR*)strCreateAt.GetBuf());
		strTemp1.Replace(_T("[CREATENAME]"),(WCHAR*)strUserName.GetBuf());
		strTemp1.Replace(_T("[USERID]"),(WCHAR*)strUserId.GetBuf());
		strTempHtml += strTemp1;

		strTemp1.Empty();
		strTemp1 += _T("<div class=\"lh_2_2\"><span>[CREATETEXT]</span></div>");
		strTemp1 .Replace(_T("[CREATETEXT]"),(WCHAR*)strText.GetBuf());
		ReplaceUrl(strTemp1);
		strTempHtml += strTemp1;

		strHtml.Empty();
		strTemp1.Empty();
		strHtml.Format(_T("%s"),strSource.GetBuf());
		strHtml.Replace(_T("<a"),_T("<a class=\"source\" target=\"_blank\" "));
		strTemp1 += _T("<div class='wb2_3'><span>[FROM]</span>[HAVEIMAGE]</div>");
		strTemp1.Replace(_T("[FROM]"),strHtml.GetBuffer(0));
		if(strImagePath.GetBufLen()){
			strHtml = _T("<span><a class='wb2_3' src='[IMAGEURL]' onmousedown=\"external.ShowBigImage(this.src);\" onmouseout=\"external.UShowItemImage();\" onmousemove=\"external.ShowItemImage(this.src,'');\" ><img class='iconalign' src='[LOOKIMAGE]' width='18' height='13'/>查看图片</a></span>");
			strHtml.Replace(_T("[LOOKIMAGE]"),(WCHAR*)g_lookImagePath.GetBuf());
			strHtml.Replace(_T("[IMAGEURL]"),(WCHAR*)strImagePath.GetBuf());
			strTemp1.Replace(_T("[HAVEIMAGE]"),strHtml.GetBuffer(0));
		}
		else{
			strTemp1.Replace(_T("[HAVEIMAGE]"),_T(""));
		}
		strTempHtml += strTemp1;

		if(strRetweeted.GetLength()){
			strTempHtml += strRetweeted;
		}

		strTempHtml += _T("</div></div>");

		strHtml = strTempHtml;
 }

 void	WeiBoEngine::AnalysisAttentionRetweeted(const TiXmlElement *pEle,CString &strHtml)
 {
		 if(!pEle) return;
		 strHtml = _T("");

		 WeiBoHttp weibo;
		 ImageTransform transImage;
		 CString strTilePath;

		 InStreamA inStrA;
		 InStreamW inStrW;
		 InStreamW inStrW2;

		 InStreamW strCreateAt;
		 InStreamW strItemID;
		 InStreamW strText;
		 InStreamW strSource;
		 InStreamW strImagePath;
		 InStreamW strImagePath1;
		 InStreamW strImagePath2;
		 InStreamW strUserId;
		 InStreamW strTileImage;
		 InStreamW strUserName;

		 const CHAR *pInfo = "created_at";
		 const CHAR *pInfo1 = "id";
		 const CHAR *pInfo2 = "text";
		 const CHAR *pInfo3 = "source";
		 const CHAR *pInfo4 = "thumbnail_pic";
		 const CHAR *pInfo5 = "bmiddle_pic";
		 const CHAR *pInfo6 = "original_pic";
		 const CHAR *pInfo7 = "profile_image_url";
		 const CHAR *pInfo8 = "screen_name";

		 const TiXmlElement *pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 strHtml.Empty();
			 inStrA  << pXmlInfo->GetText();
			 strCreateAt = inStrW2.PutUTF8Char(inStrA.GetBuf());
			 TransformTimeStr((WCHAR*)strCreateAt.GetBuf(),strHtml);
			 strCreateAt.Empty();
			 strCreateAt << strHtml.GetBuffer(0);
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strItemID = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo2,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strText = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo3,pEle);
		 if(pXmlInfo){
			 TiXmlPrinter printer; 
			 inStrW2.Empty();
			 inStrA.Empty();
			 pXmlInfo->Accept(&printer);
			 inStrA << printer.CStr();
			 strSource = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo4,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strImagePath = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo5,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strImagePath1 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo6,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strImagePath2 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }

		 const CHAR *pInfoUser = "user";
		 InStreamA strTempUserId;
		 pEle = g_WeiBoData.FirstChildElenent(pInfoUser,pEle);

		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strTempUserId << inStrA;
			 strUserId = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo7,pEle);
		 if(pXmlInfo){
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 inStrW2.Empty();
			 inStrW.Empty();
			 strHtml.Empty();
			 inStrW = inStrW2.PutUTF8Char(inStrA.GetBuf());
			 strHtml.Format(_T("%s"),inStrW.GetBuf());
			 //weibo.GetUserImage(strTilePath,strTempUserId.GetBuf(),strHtml);
			 strTilePath = strHtml;
		 }
		 pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo8,pEle);
		 if(pXmlInfo){
			 inStrW2.Empty();
			 inStrA.Empty();
			 inStrA << pXmlInfo->GetText();
			 strUserName = inStrW2.PutUTF8Char(inStrA.GetBuf());
		 }


		 CString strTemp1;
		 CString strTempHtml = _T("<div class=\"relayline\"></div>");

		 strTemp1.Empty();
		 strTemp1 += _T("<div class=\"relayrow\"><span class=\"two\">[CREATETIME]</span><span><a href=\"http://t.sina.com.cn/[USERID]\" class=\"wenzi\" target=\"_blank\" >[CREATENAME]</a></span></div>");
		 strTemp1.Replace(_T("[CREATETIME]"),(WCHAR*)strCreateAt.GetBuf());
		 strTemp1.Replace(_T("[CREATENAME]"),(WCHAR*)strUserName.GetBuf());
		 strTemp1.Replace(_T("[USERID]"),(WCHAR*)strUserId.GetBuf());
		 strTempHtml += strTemp1;

		 strTemp1.Empty();
		 strTemp1 += _T("<div class=\"lh_2_2\"><span>[CREATETEXT]</span></div>");
		 strTemp1 .Replace(_T("[CREATETEXT]"),(WCHAR*)strText.GetBuf());
		 ReplaceUrl(strTemp1);
		 strTempHtml += strTemp1;

		 strHtml.Empty();
		 strTemp1.Empty();
		 strHtml.Format(_T("%s"),strSource.GetBuf());
		 strHtml.Replace(_T("<a"),_T("<a class=\"source\" target=\"_blank\" "));
		 strTemp1 += _T("<div class='wb2_3'><span>[FROM]</span>[HAVEIMAGE]</div>");
		 strTemp1.Replace(_T("[FROM]"),strHtml.GetBuffer(0));
		 if(strImagePath.GetBufLen()){
			 strHtml = _T("<span><a class='wb2_3' src='[IMAGEURL]' onmousedown=\"external.ShowBigImage(this.src);\" onmouseout=\"external.UShowItemImage();\" onmousemove=\"external.ShowItemImage(this.src,'');\" ><img class='iconalign' src='[LOOKIMAGE]' width='18' height='13'/>查看图片</a></span>");
			 strHtml.Replace(_T("[LOOKIMAGE]"),(WCHAR*)g_lookImagePath.GetBuf());
			 strHtml.Replace(_T("[IMAGEURL]"),(WCHAR*)strImagePath.GetBuf());
			 strTemp1.Replace(_T("[HAVEIMAGE]"),strHtml.GetBuffer(0));
		 }
		 else{
			 strTemp1.Replace(_T("[HAVEIMAGE]"),_T(""));
		 }
		 strTempHtml += strTemp1;

		 strHtml = strTempHtml;
 }

void	WeiBoEngine::TransformTimeStr(WCHAR *pInStr,CString &strOut)
{
		if(!pInStr) return;
		strOut.Empty();

		CString strTemp;
		strTemp.Format(_T("%s"),pInStr);
		strTemp.Replace(_T(":"),_T(" "));
		WCHAR char1[10];
		WCHAR char2[10];
		WCHAR char3[10];
		WCHAR char4[10];
		WCHAR char5[10];
		WCHAR char6[10];
		WCHAR char7[10];
		WCHAR char8[10];
		swscanf(strTemp.GetBuffer(0),_T("%s %s %s %s %s %s %s %s"),char1,char2,char3,char4,char5,char6,char7,char8);

		strTemp.Format(_T("%s %s, %s %s:%s:%s"),char2,char3,char8,char4,char5,char6);
		COleVariant varTime = strTemp;
		varTime.ChangeType(VT_DATE);
		COleDateTime time(varTime);
		CTime time1 = CTime::GetCurrentTime();
		if(time1.GetYear() == time.GetYear()){
			strOut = time.Format(_T("%m-%d %H:%M:%S"));
		}
		else{
			strOut = time.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
}

void	WeiBoEngine::AnalysisUserInfo(const TiXmlElement *pEle, CString &strHtml)
{
		if(!pEle) return;
		strHtml.Empty();

		const CHAR *pInfo = "screen_name";
		const CHAR *pInfo1 = "location";
		const CHAR *pInfo2 = "description";
		const CHAR *pInfo3 = "url";
		const CHAR *pInfo4 = "profile_image_url";
		const CHAR *pInfo5 = "gender";
		const CHAR *pInfo6 = "followers_count";
		const CHAR *pInfo7 = "friends_count";
		const CHAR *pInfo8 = "statuses_count";
		const CHAR *pInfo9 = "favourites_count";
		const CHAR *pInfo10 = "verified";

		InStreamA strA;
		InStreamW strName;
		InStreamW strLocation;
		InStreamW strDes;
		InStreamW strUrl;
		InStreamW strProfileImage;
		InStreamW strGender;
		InStreamW strFollowers;
		InStreamW strFriends;
		InStreamW strStatuses;
		InStreamW strFavourites;
		InStreamW strVerified;

		const TiXmlElement *pEleTemp = NULL;

		pEleTemp = pEle->FirstChildElement(pInfo);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strName.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo1);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strLocation.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo2);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strDes.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo3);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strUrl.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo4);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strProfileImage.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo5);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strGender.PutUTF8Char(strA.GetBuf());
		}

		pEleTemp = pEle->FirstChildElement(pInfo6);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strFollowers.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo7);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strFriends.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo8);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strStatuses.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo9);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strFavourites.PutUTF8Char(strA.GetBuf());
		}
		pEleTemp = pEle->FirstChildElement(pInfo10);
		if(pEleTemp){
			strA.Empty();
			strA << pEleTemp->GetText();
			strVerified.PutUTF8Char(strA.GetBuf());
		}


		CString strTemp1;
		CString strTempHtml = _T("<div class=\"lhuserinfo\">");

		strTemp1 = _T("<div  class=\"lh_1\"><div class=\"imgtitle\"><img src=\"[USERIMAGE]\" width=\"88\" height=\"87\" /></div></div>");
		strTemp1.Replace(_T("[USERIMAGE]"),(WCHAR*)strProfileImage.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lh_2\">");
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lhuserinfo_2_1\"><span><a href=\"#\" class=\"wenzi\">[USERNAME]</a>&nbsp;&nbsp;</span></div>");
		strTemp1.Replace(_T("[USERNAME]"),(WCHAR*)strName.GetBuf());
		strTempHtml += strTemp1;

		CString strTemp2;
		strTemp2.Format(_T("%s"),(WCHAR*)strGender.GetBuf());
		strTemp2.Replace(_T("m"),_T("男"));
		strTemp2.Replace(_T("f"),_T("女"));
		strTemp2.Replace(_T("n"),_T(""));
		strTemp1 = _T("<div class=\"lhuserinfo_2_1\"><span><a href=\"#\" class=\"wenzi\">[BOY]</a></span></div>");
		strTemp1.Replace(_T("[BOY]"),strTemp2.GetBuffer(0));
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"userdes\"><span></span></div>");
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lhuserinfo_2_1\"><span><a href=\"#\" class=\"wenzi\">[LOCATION]</a>&nbsp;&nbsp;</span></div>");
		strTemp1.Replace(_T("[LOCATION]"),(WCHAR*)strLocation.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lhuserinfo_2_1\"><span><a href=\"#\" class=\"wenzi\">粉丝:[FLOW]</a></span><span><a href=\"#\" class=\"wenzi\">发布:[STATUSES]</a></span></div>");
		strTemp1.Replace(_T("[FLOW]"),(WCHAR*)strFollowers.GetBuf());
		strTemp1.Replace(_T("[STATUSES]"),(WCHAR*)strStatuses.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lhuserinfo_2_1\"><span><a href=\"#\" class=\"wenzi\">关注:[FAVOURITES]</a></span></div>");
		strTemp1.Replace(_T("[FAVOURITES]"),(WCHAR*)strFavourites.GetBuf());
		strTempHtml += strTemp1;

		strTempHtml += _T("<div></div></div></div>");

		strTemp1 = _T("<div class=\"userdesdiv\"><span><a >&nbsp;&nbsp;介绍:</a></span><span class=\"userdes\">[USERDES]</span></div>");
		strTemp1.Replace(_T("[USERDES]"),(WCHAR*)strDes.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<script type=\"text/javascript\" src=\"D:\\work\\client\\today\\bRoundCurve 1.0.js\"></script>");
		strTemp1 += _T("<script type=\"text/javascript\">");
		strTemp1 += _T(" b_RoundCurve(\"imgtitle\", \"#C0D0F1\", \"#C0D0F1\", 2);");
		strTemp1 += _T("</script>");
		strTempHtml += strTemp1;

		strHtml = strTempHtml;
}

void	WeiBoEngine::AnalysisLoginItem(const TiXmlElement *pEle, CString &strHtml)
{
		if(!pEle) return;
		strHtml = _T("");

		WeiBoHttp weibo;
		ImageTransform transImage;
		CString strTilePath;
		CString strRetweeted;

		InStreamA inStrA;
		InStreamW inStrW;
		InStreamW inStrW2;

		InStreamW strCreateAt;
		InStreamW strItemID;
		InStreamW strText;
		InStreamW strSource;
		InStreamW strImagePath;
		InStreamW strImagePath1;
		InStreamW strImagePath2;
		InStreamW strUserId;
		InStreamW strTileImage;
		InStreamW strUserName;

		const CHAR *pInfo = "created_at";
		const CHAR *pInfo1 = "id";
		const CHAR *pInfo2 = "text";
		const CHAR *pInfo3 = "source";
		const CHAR *pInfo4 = "thumbnail_pic";
		const CHAR *pInfo5 = "bmiddle_pic";
		const CHAR *pInfo6 = "original_pic";
		const CHAR *pInfo8 = "screen_name";
		const CHAR *pInfo9 = "retweeted_status";
		const TiXmlElement *pEleRetweeted = g_WeiBoData.FirstChildElenent(pInfo9,pEle);
		
		if(pEleRetweeted){
			AnalysisAttentionRetweeted(pEleRetweeted,strRetweeted);
		}
		const TiXmlElement *pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			strHtml.Empty();
			inStrA  << pXmlInfo->GetText();
			strCreateAt = inStrW2.PutUTF8Char(inStrA.GetBuf());
			TransformTimeStr((WCHAR*)strCreateAt.GetBuf(),strHtml);
			strCreateAt.Empty();
			strCreateAt << strHtml.GetBuffer(0);
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strItemID = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo2,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strText = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo3,pEle);
		if(pXmlInfo){
			TiXmlPrinter printer; 
			inStrW2.Empty();
			inStrA.Empty();
			pXmlInfo->Accept(&printer);
			inStrA << printer.CStr();
			strSource = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo4,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo5,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath1 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}
		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo6,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strImagePath2 = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}

		const CHAR *pInfoUser = "user";
		InStreamA strTempUserId;
		pEle = g_WeiBoData.FirstChildElenent(pInfoUser,pEle);

		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo1,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strTempUserId << inStrA;
			strUserId = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}

		pXmlInfo = g_WeiBoData.FirstChildElenent(pInfo8,pEle);
		if(pXmlInfo){
			inStrW2.Empty();
			inStrA.Empty();
			inStrA << pXmlInfo->GetText();
			strUserName = inStrW2.PutUTF8Char(inStrA.GetBuf());
		}


		CString strTemp1;
		CString strTempHtml;
		strTemp1 = _T("<div class=\"lh\" id=\"[ITEMID]\">");
		strTemp1.Replace(_T("[ITEMID]"),(WCHAR*)strItemID.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lh_2\">");
		strTemp1 += _T("<div class=\"lh_2_1\"><span class=\"two\">[CREATETIME]</span><span><a href=\"http://t.sina.com.cn/[USERID]\" target=\"_blank\" class=\"wenzi\">[CREATENAME]</a></span></div>");
		strTemp1.Replace(_T("[CREATETIME]"),(WCHAR*)strCreateAt.GetBuf());
		strTemp1.Replace(_T("[CREATENAME]"),(WCHAR*)strUserName.GetBuf());
		strTemp1.Replace(_T("[USERID]"),(WCHAR*)strUserId.GetBuf());
		strTempHtml += strTemp1;

		strTemp1 = _T("<div class=\"lh_2_2\"><span>[CREATETEXT]</span></div>");
		strTemp1 .Replace(_T("[CREATETEXT]"),(WCHAR*)strText.GetBuf());
		ReplaceUrl(strTemp1);
		strTempHtml += strTemp1;

		strHtml.Empty();
		strHtml.Format(_T("%s"),strSource.GetBuf());
		strHtml.Replace(_T("<a"),_T("<a class=\"source\" target=\"_blank\" "));
		strTemp1 = _T("<div class='wb2_3'><span>[FROM]</span>[HAVEIMAGE]</div>");
		strTemp1.Replace(_T("[FROM]"),strHtml.GetBuffer(0));
		if(strImagePath.GetBufLen()){
			strHtml = _T("<span><a class='wb2_3' src='[IMAGEURL]' onmousedown=\"external.ShowBigImage(this.src);\" onmouseout=\"external.UShowItemImage();\" onmousemove=\"external.ShowItemImage(this.src,'');\" ><img class='iconalign' src='[LOOKIMAGE]' width='18' height='13'/>查看图片</a></span>");
			strHtml.Replace(_T("[LOOKIMAGE]"),(WCHAR*)g_lookImagePath.GetBuf());
			strHtml.Replace(_T("[IMAGEURL]"),(WCHAR*)strImagePath.GetBuf());
			strTemp1.Replace(_T("[HAVEIMAGE]"),strHtml.GetBuffer(0));
		}
		else{
			strTemp1.Replace(_T("[HAVEIMAGE]"),_T(""));
		}
		strTempHtml += strTemp1;

		if(strRetweeted.GetLength()){
			strTempHtml += strRetweeted;
		}

		strTempHtml += _T("</div></div>");

		strHtml = strTempHtml;
}

void	WeiBoEngine::AnalysisItemPic(CString &InUrl,CString &outStrHtml,WCHAR *pInUrl)
{
		outStrHtml  = _T("");


		CString strTemp = _T("<table class=\"showItemImage\"><tr><td>");
		strTemp += _T("<a href='#' onclick=\"javascript:HtmlShowBigImage();\" ><img  src=\"[IMAGESOURE]\"> </img></a></td></tr>");
		strTemp += _T("</table>");
		strTemp.Replace(_T("[IMAGESOURE]"),InUrl.GetBuffer(0));
		if(pInUrl){
			strTemp += _T("<span id=\"bigimage\" style=\"display:none\">[URL2]</span>");
			strTemp.Replace(_T("[URL2]"),pInUrl);
		}
		outStrHtml = strTemp;
}