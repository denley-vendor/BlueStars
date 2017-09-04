/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define  WEIBODATA_CPP
#include "WeiBoData.h"

WeiBoData::WeiBoData(void)
{
		m_pLonginUser = NULL;
		m_pLonginUserItem = NULL;
		m_pAttentionItem = NULL;
		m_pAtLoginUserItem = NULL;
}

WeiBoData::~WeiBoData(void)
{
		Clear();
}

void	WeiBoData::Clear()
{
		SAFE_DELETE(m_pLonginUser);
		SAFE_DELETE(m_pLonginUserItem);
		SAFE_DELETE(m_pAttentionItem);
		SAFE_DELETE(m_pAtLoginUserItem);
}

void	WeiBoData::SetLongingUserXml(TiXmlDocument *pXml)
{
		if(pXml){
			SAFE_DELETE(m_pLonginUser);
			m_pLonginUser = pXml;
		}
}

const	char *WeiBoData::GetLonginserInfo(const char *pInfoDes)
{
		if(m_pLonginUser == NULL) return NULL;
		if(!pInfoDes) return NULL;
		//m_pLonginUser->SaveFile("D:\\userx.xml");

		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "user";
		TiXmlElement *pRootElement = m_pLonginUser->RootElement();
		TiXmlElement *pPerson = NULL;
		//if(pRootElement){
		//	pRootElement = pRootElement->FirstChildElement(pInfo);
		//}
		//if(pRootElement){
		//	pRootElement = pRootElement->FirstChildElement(pInfo1);
		//}
		if(pRootElement){
			pPerson = pRootElement->FirstChildElement(pInfoDes);
			if(pPerson){
				return pPerson->GetText();
			}
		}
		return NULL;
}

const	TiXmlElement *WeiBoData::UserFirstChildElement(const char *pDes)
{
		if(m_pLonginUser == NULL) return NULL;
		if(!pDes) return NULL;
		TiXmlElement *pRootElement = (TiXmlElement*)m_pLonginUser;
		TiXmlElement *pPerson = NULL;
		if(pRootElement){
			pPerson = pRootElement->FirstChildElement(pDes);
			if(pPerson) return pPerson;
		}

		return NULL;
}

const	TiXmlElement *WeiBoData::FirstChildElenent(const char *pDes,const TiXmlElement *pIn)
{
		if(!pDes) return NULL;
		if(!pIn) return NULL;
		return pIn->FirstChildElement(pDes);
}

BOOL	WeiBoData::IsItemBValidate(TiXmlDocument *pItem)
{
		if(!pItem) return FALSE;

		TiXmlNode *pNode = NULL;
		TiXmlElement *pEle = pItem->RootElement();
		if(!pEle) return FALSE;

		const CHAR *pInfo = "status";
		if(!strcmp(pEle->Value(),pInfo)){
			return TRUE;
		}
		
		pNode = pEle->FirstChild(pInfo);
		if(!pNode) return FALSE;

		return TRUE;
}

void	WeiBoData::SetLongingUserItem(TiXmlDocument *pItem)
{
		if(!pItem) return ;
		SAFE_DELETE(m_pLonginUserItem);
		m_pLonginUserItem = pItem;
}

DWORD	WeiBoData::GetLonginUserItemCount(const CHAR *pChar)
{
		if(!m_pLonginUserItem) return 0;
		if(!pChar) return 0;
		DWORD count = 0;
		TiXmlElement *pRootElement = m_pLonginUserItem->RootElement();
		TiXmlElement *pPerson = NULL;
		if(pRootElement){
			pPerson = pRootElement->FirstChildElement(pChar);
			while(pPerson){
				count ++;
				pPerson = pPerson->NextSiblingElement(pChar);
			}
		}
		return count;
}

void	WeiBoData::AddNewLongingItem(TiXmlNode *pNode)
{
		TiXmlDocument *pDoc = (TiXmlDocument*)pNode;
		if(!pDoc) return;

		TiXmlElement *pEle1 = pDoc->RootElement();
		if(!pEle1) return;

		const CHAR *pInfo = "status";
		const CHAR *pInfo2 = NULL;
		TiXmlElement *pEle = NULL;
		TiXmlElement *pRootElement = NULL;
		if(m_pLonginUserItem){
			pRootElement  = m_pLonginUserItem->RootElement();
		}

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild(decl);
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 

		if(strcmp(pEle1->Value(),pInfo) != 0){
			pEle = pEle1->FirstChildElement(pInfo);
		}
		else{
			pEle = pEle1;
		}
		while(pEle){
			pRoot->LinkEndChild(pEle->Clone());
			pEle = (TiXmlElement*)pEle->NextSibling();
		}
		if(pRootElement){
			pEle = pRootElement->FirstChildElement(pInfo);
		}
		while(pEle){
			pRoot->LinkEndChild(pEle->Clone());
			pEle = (TiXmlElement*)pEle->NextSibling();
		}

		SAFE_DELETE(m_pLonginUserItem);
		m_pLonginUserItem = pNewDoc;

}

void	WeiBoData::AddPreviousLongingItem(TiXmlNode *pNode)
{
		if(!m_pLonginUserItem) return;

		TiXmlDocument *pDoc = (TiXmlDocument*)pNode;
		if(!pDoc) return;
		TiXmlElement *pEle = pDoc->RootElement();
		if(!pEle) return;

		const CHAR *pInfo = "status";
		TiXmlElement *pEle1 = NULL;
		TiXmlElement *pRootElement = m_pLonginUserItem->RootElement();

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild( decl );
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 

		pEle1 = pRootElement->FirstChildElement(pInfo);
		while(pEle1){
			pRoot->LinkEndChild(pEle1->Clone());
			pEle1 = (TiXmlElement*)pEle1->NextSibling();
		}
		pEle1 = pEle->FirstChildElement(pInfo);
		while(pEle1){
			pRoot->LinkEndChild(pEle1->Clone());
			pEle1 = (TiXmlElement*)pEle1->NextSibling();
		}

		SAFE_DELETE(m_pLonginUserItem);
		m_pLonginUserItem = pNewDoc;
		if(!m_pLonginUserItem){
			//TRACE("WEIBODATA Info: Failed m_pAttentionItem \n");
		}
}

const	TiXmlElement *WeiBoData::GetLoginUserItem(DWORD item)
{
		if(!m_pLonginUserItem) return NULL;
		const CHAR *pInfo = "status";
		DWORD count  = GetLonginUserItemCount(pInfo);

		DWORD tempIndex = 0;
		TiXmlElement *pEle2 = NULL;
		TiXmlElement *pEle =m_pLonginUserItem->RootElement();
		if(pEle){
			pEle2 = (TiXmlElement*)pEle->FirstChild(pInfo);
		}
		while(pEle2){
			if(tempIndex == item){
				return pEle2;
			}
			pEle2 = (TiXmlElement*)pEle2->NextSibling();
			tempIndex ++;
		}

		return NULL;
}

const	CHAR *WeiBoData::GetLoginLastItemId()
{
		if(!m_pLonginUserItem) return NULL;

		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "id";
		TiXmlElement *pEle2 = NULL;
		TiXmlElement *pEle =m_pLonginUserItem->RootElement();

		if(pEle){
			pEle2 = (TiXmlElement*)pEle->LastChild(pInfo);
		}
		if(pEle2){
			pEle = (TiXmlElement*)pEle2->FirstChild(pInfo1);
			if(pEle) return pEle->GetText();
		}

		return NULL;
}

const	CHAR *WeiBoData::GetLoginFirstItemId()
{
		if(!m_pLonginUserItem) return NULL;

		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "id";
		TiXmlElement *pEle2 = NULL;
		TiXmlElement *pEle =m_pLonginUserItem->RootElement();

		if(pEle){
			pEle2 = (TiXmlElement*)pEle->FirstChild(pInfo);
		}
		if(pEle2){
			pEle = (TiXmlElement*)pEle2->FirstChild(pInfo1);
			if(pEle) return pEle->GetText();
		}

		return NULL;
}

const	TiXmlElement *WeiBoData::FindLoginItem(WCHAR *pInId)
{

		CString strTemp;
		const CHAR *pInfo = "id";
		const TiXmlElement *pEle = NULL;
		const TiXmlElement *pEle1 = NULL;

		pEle = GetLoginUserItem(0);
		while(pEle){
			pEle1 = pEle->FirstChildElement(pInfo);
			if(pEle1){
				strTemp.Format(_T("%S"),pEle1->GetText());
				if(!wcscmp(strTemp.GetBuffer(0),pInId)){
					return pEle;
				}
			}
			pEle = pEle->NextSiblingElement();
		}
		return NULL;
}

BOOL	WeiBoData::GetLoginItemText(WCHAR *pInId,CString &strOut,CString &strOut1)
{
		strOut1.Empty();
		strOut.Empty();
		const TiXmlElement *pEle1 = NULL;
		const TiXmlElement *pEle = FindLoginItem(pInId);
		if(pEle){
			InStreamA strTemp;
			CString strTemp1;
			const CHAR *pInfo1 = "text";
			const CHAR *pInfo2 = "user";
			const CHAR *pInfo3 = "screen_name";
			const CHAR *pInfo4 = "retweeted_status";

			pEle1 = pEle->FirstChildElement(pInfo2);
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = pEle->FirstChildElement(pInfo1);
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			InStreamW instrw;
			if(strTemp.GetBufLen()){
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}

			///retweeted_status
			strTemp.Empty();
			pEle = pEle->FirstChildElement(pInfo4);
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo2);
			}
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << ("||");
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = NULL;
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo1);
			}
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			if(strTemp.GetBufLen()){
				instrw.Empty();
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}
			return TRUE;
		}

		return FALSE;
}

void	WeiBoData::SetAttentionItem(TiXmlNode *pItem)
{
		if(pItem){
			if(m_pAttentionItem) delete m_pAttentionItem;
			m_pAttentionItem = (TiXmlDocument*)pItem->Clone();
		}
}

BOOL	WeiBoData::GetAttentionItemMaxId(CString &instrId)
{
		instrId.Empty();
		const TiXmlElement *pEle = GetAttentionItem(0);
		if(!pEle){
			return FALSE;
		}

		const CHAR *pInfo = "id";
		pEle = FirstChildElenent(pInfo,pEle);
		if(!pEle) return FALSE;

		instrId.Format(_T("%S"),pEle->GetText());
		return TRUE;
}

BOOL	WeiBoData::GetAttentionItemMinId(CString &instrId)
{
		instrId.Empty();

		DWORD count = GetAttentionItemCount();
		if(count){
			DWORD num = count - 1;
			const TiXmlElement *pEle = GetAttentionItem(num);
			if(!pEle){
				//TRACE("WEIBODATA: GetAttentionItemMinId:GetAttentionItem == NULL ");
				return FALSE;
			}

			const CHAR *pInfo = "id";
			pEle = pEle->FirstChildElement(pInfo);
			if(!pEle) {
				//TRACE("WEIBODATA: GetAttentionItemMinId:FirstChildElement(id) == NULL ");
				return FALSE;
			}

			instrId.Format(_T("%S"),pEle->GetText());
			instrId.Trim();
			//TRACE("WEIBODATA: GetAttentionItemMinId:GetAttentionItemMinId ");
			//TRACE(instrId);
			//TRACE("\n");
		}
		return TRUE;
}

BOOL	WeiBoData::GetUserIdByItem(CString &strOut,WCHAR *pIn)
{
		strOut.Empty();
		const TiXmlElement *pEle = FindAttentionItem(pIn);
		if(!pEle) return FALSE;
		const CHAR *pInfo = "user";

		const CHAR *pInfo1 = "id";
		const TiXmlElement *pEle2 = NULL;
		const TiXmlElement *pEle1 = pEle->FirstChildElement(pInfo);
		if(pEle1){
			pEle2 = pEle1->FirstChildElement(pInfo1);
			if(pEle2){
				strOut.Format(_T("%S"),pEle2->GetText());
			}
		}
		return TRUE;
}

BOOL	WeiBoData::GetUserNameByItem(CString &strOut,WCHAR *pIn)
{
		strOut.Empty();
		const TiXmlElement *pEle = FindAttentionItem(pIn);
		if(!pEle) return FALSE;
		const CHAR *pInfo = "user";

		const CHAR *pInfo1 = "screen_name";
		const TiXmlElement *pEle2 = NULL;
		const TiXmlElement *pEle1 = pEle->FirstChildElement(pInfo);
		if(!pEle1) return FALSE;
		pEle2 = pEle1->FirstChildElement(pInfo1);
		if(!pEle2) return FALSE;

		InStreamW instrW;
		InStreamA instrA;
		instrA << pEle2->GetText();
		instrW.PutUTF8Char(instrA.GetBuf());
		strOut.Format(_T("%s"),(WCHAR*)instrW.GetBuf());
		return TRUE;
}

BOOL	WeiBoData::GetUserText(CString &strOut,WCHAR *pIn)
{
		strOut.Empty();
		const TiXmlElement *pEle = FindAttentionItem(pIn);
		if(!pEle) return FALSE;
		const CHAR *pInfo = "text";

		const TiXmlElement *pEle1 = pEle->FirstChildElement(pInfo);
		if(!pEle1) return FALSE;

		InStreamW instr;
		InStreamA instrA;
		instrA << pEle1->GetText();
		instr.PutUTF8Char(instrA.GetBuf());
		strOut.Format(_T("%s"),(WCHAR*)instr.GetBuf());
		return TRUE;
}		

const	TiXmlElement *WeiBoData::FindAttentionItem(WCHAR *pInId)
{
		DWORD count = GetAttentionItemCount();
		const CHAR *pInfo = "id";
		const TiXmlElement *pEle = NULL;
		const TiXmlElement *pEle1 = NULL;

		CString strTemp;
		for(DWORD i = 0 ; i < count; i ++){
			pEle = GetAttentionItem(i);
			if(!pEle) continue;
			pEle1 = pEle->FirstChildElement(pInfo);

			if(pEle1){
				strTemp.Format(_T("%S"),pEle1->GetText());
				if(!wcscmp(strTemp.GetBuffer(0),pInId)){
					return pEle;
				}
			}
		}
		return NULL;
}

DWORD	WeiBoData::GetAttentionItemCount()
{
		if(!m_pAttentionItem) return 0;

		DWORD count = 0;
		const CHAR *pInfo = "status";
		TiXmlElement *pRootElement = m_pAttentionItem->RootElement();
		TiXmlElement *pEle = NULL;
		if(pRootElement){
			if(strcmp(pRootElement->Value(),pInfo)){
				pEle = pRootElement->FirstChildElement(pInfo);
			}
			else{
				pEle = pRootElement;
			}
			
			while(pEle){
				count ++;
				pEle = (TiXmlElement*)pEle->NextSibling();
			}
		}
		return count;
}

void	WeiBoData::AddNewAttentionItem(TiXmlNode *pNode)
{
		TiXmlDocument *pDoc = (TiXmlDocument*)pNode;
		if(!pDoc) return;
		
		TiXmlElement *pEle1 = pDoc->RootElement();
		if(!pEle1) return;

		const CHAR *pInfo = "status";
		const CHAR *pInfo2 = NULL;
		TiXmlElement *pEle = NULL;
		TiXmlElement *pRootElement = NULL;
		if(m_pAttentionItem){
			pRootElement = m_pAttentionItem->RootElement();
		}

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild(decl);
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 
		if(strcmp(pEle1->Value(),pInfo) != 0){
			pEle = pEle1->FirstChildElement(pInfo);
		}
		else{
			pEle = pEle1;
		}
		while(pEle){
			pRoot->LinkEndChild(pEle->Clone());
			pEle = (TiXmlElement*)pEle->NextSibling();
		}
		if(pRootElement){
			pEle = pRootElement->FirstChildElement(pInfo);
		}
		while(pEle){
			pRoot->LinkEndChild(pEle->Clone());
			pEle = (TiXmlElement*)pEle->NextSibling();
		}

		SAFE_DELETE(m_pAttentionItem);
		m_pAttentionItem = pNewDoc;
	//	m_pAttentionItem->SaveFile("D:\\mp.XML");
}

void	WeiBoData::AddPreviousAttentionItem(TiXmlNode *pNode)
{
		if(!m_pAttentionItem) return;

		TiXmlDocument *pDoc = (TiXmlDocument*)pNode;
		if(!pDoc) return;
		TiXmlElement *pEle = pDoc->RootElement();
		if(!pEle) return;

		const CHAR *pInfo = "status";
		TiXmlElement *pEle1 = NULL;
		TiXmlElement *pRootElement = m_pAttentionItem->RootElement();

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild( decl );
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 

		pEle1 = pRootElement->FirstChildElement(pInfo);
		while(pEle1){
			pRoot->LinkEndChild(pEle1->Clone());
			pEle1 = (TiXmlElement*)pEle1->NextSibling();
		}
		pEle1 = pEle->FirstChildElement(pInfo);
		while(pEle1){
			pRoot->LinkEndChild(pEle1->Clone());
			pEle1 = (TiXmlElement*)pEle1->NextSibling();
		}

		SAFE_DELETE(m_pAttentionItem);
		m_pAttentionItem = pNewDoc;
		if(!m_pAttentionItem){
			//TRACE("WEIBODATA Info: Failed m_pAttentionItem \n");
		}
}

BOOL	WeiBoData::GetAttentionItemText(WCHAR *pInId,CString &strOut,CString &strOut1)
{
		strOut1 = _T("");
		strOut = _T("");

		const TiXmlElement *pEle1 = NULL;
		const TiXmlElement *pEle = FindAttentionItem(pInId);
		if(pEle){
			InStreamA strTemp;
			CString strTemp1;
			const CHAR *pInfo1 = "text";
			const CHAR *pInfo2 = "user";
			const CHAR *pInfo3 = "screen_name";
			const CHAR *pInfo4 = "retweeted_status";

			pEle1 = pEle->FirstChildElement(pInfo2);
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << "//@";
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = pEle->FirstChildElement(pInfo1);
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			InStreamW instrw;
			if(strTemp.GetBufLen()){
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}

			///retweeted_status
			strTemp.Empty();
			pEle = pEle->FirstChildElement(pInfo4);
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo2);
			}
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << ("//@");
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = NULL;
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo1);
			}
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			if(strTemp.GetBufLen()){
				instrw.Empty();
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut1.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}
			return TRUE;
		}

		return FALSE;
}

const	TiXmlElement *WeiBoData::GetAttentionItem(DWORD item)
{
		DWORD n = 0;
		if(!m_pAttentionItem) return NULL;

		TiXmlElement *pRootElement = m_pAttentionItem->RootElement();
		TiXmlElement *pPerson = NULL;
		const CHAR *pFind = ("status");
		if(pRootElement){
			//TRACE("WEIBODATA Info: GetAttentionItem pRootElement OK\n");
			if(strcmp(pRootElement->Value(),pFind)){
				//TRACE("WEIBODATA Info: Failed GetAttentionItem pPerson = pRootElement->FirstChildElement(pFind);\n");
				pPerson = pRootElement->FirstChildElement(pFind);
				//TRACE(strTemp.Left(100));//TRACE("\n");
			}
			else{
				//TRACE("WEIBODATA Info: GetAttentionItem pPerson = pRootElement;\n");
				pPerson = pRootElement;
			}
			if(pPerson){
				//TRACE("WEIBODATA Info: GetAttentionItem pPerson OK\n");
				if(item == 0) return pPerson;
			}
			while(pPerson){
				n ++;
				pPerson = pPerson->NextSiblingElement(pFind);
				if(n == item) return pPerson;
			}
		}
		//TRACE("WEIBODATA Info: GetAttentionItem Return NULL\n");
		return NULL;
}

void	WeiBoData::SetAtLoginUserItem(TiXmlNode *pItem)
{
		if(pItem){
			SAFE_DELETE(m_pAtLoginUserItem);
			m_pAtLoginUserItem = (TiXmlDocument *)pItem;
		}
}

void	WeiBoData::AddAtLoginUserItem(TiXmlNode *pItem)
{
		TiXmlDocument *pDoc = (TiXmlDocument*)pItem;
		if(!pDoc) return;

		TiXmlElement *pEle1 = pDoc->RootElement();
		if(!pEle1) return;

		const CHAR *pInfo = "status";
		const CHAR *pInfo2 = NULL;
		TiXmlElement *pEle = NULL;
		TiXmlElement *pRootElement = NULL;
		if(m_pAtLoginUserItem){
			pRootElement = m_pAtLoginUserItem->RootElement();
		}

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild(decl);
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 

		if(pEle1){
			if(strcmp(pEle1->Value(),pInfo) != 0){
				pEle = pEle1->FirstChildElement(pInfo);
			}
			else{
				pEle = pEle1;
			}
			while(pEle){
				pRoot->LinkEndChild(pEle->Clone());
				pEle = (TiXmlElement*)pEle->NextSibling();
			}
		}

		if(pRootElement){
			pEle = pRootElement->FirstChildElement(pInfo);
			while(pEle){
				pRoot->LinkEndChild(pEle->Clone());
				pEle = (TiXmlElement*)pEle->NextSibling();
			}
		}

		SAFE_DELETE(m_pAtLoginUserItem);
		m_pAtLoginUserItem = pNewDoc;
}

void	WeiBoData::AddAtPreviousUserItem(TiXmlNode *pItem)
{

		TiXmlDocument *pDoc = (TiXmlDocument*)pItem;
		if(!pDoc) return;
		TiXmlElement *pEle = pDoc->RootElement();
		if(!pEle) return;

		const CHAR *pInfo = "status";
		TiXmlElement *pEle1 = NULL;
		TiXmlElement *pRootElement = NULL;
		if(m_pAtLoginUserItem){
			pRootElement = m_pAtLoginUserItem->RootElement();
		}

		TiXmlDocument *pNewDoc = new TiXmlDocument;
		TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" ); 
		pNewDoc->LinkEndChild( decl );
		TiXmlElement *pRoot = new TiXmlElement("statuses"); 
		pNewDoc->LinkEndChild(pRoot); 

		if(pRootElement){
			pEle1 = pRootElement->FirstChildElement(pInfo);
			while(pEle1){
				pRoot->LinkEndChild(pEle1->Clone());
				pEle1 = (TiXmlElement*)pEle1->NextSibling();
			}
		}

		pEle1 = pEle->FirstChildElement(pInfo);
		while(pEle1){
			pRoot->LinkEndChild(pEle1->Clone());
			pEle1 = (TiXmlElement*)pEle1->NextSibling();
		}

		SAFE_DELETE(m_pAtLoginUserItem);
		m_pAtLoginUserItem = pNewDoc;
		if(!m_pAtLoginUserItem){
			//TRACE("WEIBODATA Info: Failed m_pAtLoginUserItem \n");
		}
}

BOOL	WeiBoData::GetAtItemMinId(CString &instrId)
{
		instrId.Empty();
		if(!m_pAtLoginUserItem) {
			instrId  = _T("0");
			return TRUE;
		}

		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "id";

		TiXmlElement *pEle1 = NULL;
		TiXmlElement *pEle = m_pAtLoginUserItem->RootElement();
		if(pEle){
			pEle1 = (TiXmlElement*)pEle->LastChild(pInfo);
		}
		if(pEle1){
			pEle = pEle1->FirstChildElement(pInfo1);
			if(pEle){
				instrId.Format(_T("%S"),pEle->GetText());
			}
		}
		return TRUE;
}

BOOL	WeiBoData::GetAtItemMaxId(CString &instrId)
{
		instrId.Empty();

		if(!m_pAtLoginUserItem){
			return FALSE;
		}

		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "id";

		TiXmlElement *pEle1 = NULL;
		TiXmlElement *pEle = m_pAtLoginUserItem->RootElement();
		if(pEle){
			pEle1 = pEle->FirstChildElement(pInfo);
		}
		if(pEle1){
			pEle = pEle1->FirstChildElement(pInfo1);
			if(pEle){
				instrId.Format(_T("%S"),pEle->GetText());
			}
		}
		return TRUE;
}

const	TiXmlElement *WeiBoData::GetAtItem(CString &inStrId)
{
		if(!m_pAtLoginUserItem) return NULL;

		CString strTemp;
		const CHAR *pInfo = "status";
		const CHAR *pInfo1 = "id";
		const TiXmlElement *pEle1 = NULL;
		const TiXmlElement *pEle = m_pAtLoginUserItem->RootElement();
		if(pEle){
			pEle1 = pEle->FirstChildElement(pInfo);
		}
		while(pEle1){
			pEle = pEle1->FirstChildElement(pInfo1);
			if(pEle){
				strTemp.Format(_T("%S"),pEle->GetText());
				if(strTemp == inStrId) return pEle1;
			}
			pEle1  = pEle1->NextSiblingElement();
		}

		return NULL;
}

void	WeiBoData::GetUserIDByAtItem(CString &strOut,WCHAR *pIn)
{
		if(!pIn) return;

		CString strId;
		strId.Format(_T("%s"),pIn);
		const CHAR *pInfo = "user";
		const CHAR *pInfo1 = "id";

		const TiXmlElement *pEle = GetAtItem(strId);
		const TiXmlElement *pEle1 = NULL;
		if(pEle){
			pEle1 = pEle->FirstChildElement(pInfo);
			if(!pEle1) return;

			pEle = pEle1->FirstChildElement(pInfo1);
			if(pEle){
				strOut.Format(_T("%S"),pEle->GetText());
			}
		}
}

BOOL	WeiBoData::GetAtItemText(CString &instrId,CString &strOut,CString &strOut1)
{
		strOut.Empty();
		strOut1.Empty();
		const TiXmlElement *pEle1 = NULL;
		const TiXmlElement *pEle = GetAtItem(instrId);
		if(pEle){
			InStreamA strTemp;
			CString strTemp1;
			const CHAR *pInfo1 = "text";
			const CHAR *pInfo2 = "user";
			const CHAR *pInfo3 = "screen_name";
			const CHAR *pInfo4 = "retweeted_status";

			pEle1 = pEle->FirstChildElement(pInfo2);
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << "//@";
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = pEle->FirstChildElement(pInfo1);
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			InStreamW instrw;
			if(strTemp.GetBufLen()){
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}

			///retweeted_status
			strTemp.Empty();
			pEle = pEle->FirstChildElement(pInfo4);
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo2);
			}
			if(pEle1){
				pEle1 = pEle1->FirstChildElement(pInfo3);
			}
			if(pEle1){
				strTemp << ("//");
				strTemp << pEle1->GetText();
				strTemp << (":");
			}
			pEle1 = NULL;
			if(pEle){
				pEle1 = pEle->FirstChildElement(pInfo1);
			}
			if(pEle1){
				strTemp << pEle1->GetText();
			}
			if(strTemp.GetBufLen()){
				instrw.Empty();
				instrw.PutUTF8Char(strTemp.GetBuf());
				strOut1.Format(_T("%s"),(WCHAR*)instrw.GetBuf());
			}
			return TRUE;
		}

		return FALSE;
}

char	*WeiBoData::CStringToChar(CString &inStr)
{
		wchar_t *pWchar = inStr.GetBuffer(0);
		int len = wcslen(pWchar)*2+1;
		int byts = WideCharToMultiByte(0,0,pWchar,wcslen(pWchar),NULL,0,NULL,NULL);
		char *pChar = new char[byts];
		WideCharToMultiByte(0,0,pWchar,wcslen(pWchar),pChar,byts,NULL,NULL);
		return pChar;
}

BOOL	WeiBoData::UTF8_2_GB2312(const void * pIn,int inLen,CString &outStr)
{
		if(!pIn) return FALSE;

		int len=MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pIn, -1, NULL,0);
		wchar_t *pWchar = new wchar_t[len+1];
		if (NULL==pWchar)
			return FALSE;

		memset(pWchar,0,(len+1)* sizeof(wchar_t));
		MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pIn,inLen, pWchar, len);
		outStr.Format(_T("%s"),pWchar);
		delete[]pWchar;
		return TRUE;
}

WeiBoData g_WeiBoData;