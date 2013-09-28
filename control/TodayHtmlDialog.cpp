#define TODAYHTMLDIALOG_CPP

#include "..//stdafx.h"
#include "TodayHtmlDialog.h"


TodayHtmlDialog::TodayHtmlDialog(UINT nIDTemplate, UINT nHtmlResID /* = 0 */, CWnd *pParentWnd /* = NULL */)
	:	CDHtmlDialog(nIDTemplate,nHtmlResID,pParentWnd)
{
		m_pMenu = NULL;
		m_bMenu = FALSE;
}

TodayHtmlDialog::~TodayHtmlDialog()
{
}


STDMETHODIMP	TodayHtmlDialog::GetExternal(IDispatch **ppDispatch)
{
		if(ppDispatch == NULL)
			return E_POINTER;

		*ppDispatch = NULL;
		if (m_spExternalDisp.p && CanAccessExternal())
		{
			m_spExternalDisp.p->AddRef();
			*ppDispatch = m_spExternalDisp.p;
			return S_OK;
		}
		*ppDispatch = GetIDispatch(TRUE);
		return S_OK;
}

STDMETHODIMP	TodayHtmlDialog::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
		m_bMenu = TRUE;
		return S_OK;
}

void	TodayHtmlDialog::DoDataExchange(CDataExchange* pDX)
{
		CDHtmlDialog::DoDataExchange(pDX);
}

BOOL	TodayHtmlDialog::OnInitDialog()
{
		CDHtmlDialog::OnInitDialog();
		EnableAutomation();
		return TRUE;  
}

BEGIN_MESSAGE_MAP(TodayHtmlDialog, CDHtmlDialog)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(TodayHtmlDialog)
END_DHTML_EVENT_MAP()

//BEGIN_DISPATCH_MAP(TodayHtmlDialog,CDHtmlDialog)
//END_DISPATCH_MAP()
void	TodayHtmlDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
		IHTMLWindow2* pWnd = NULL;
		IHTMLDocument2*   pDoc = NULL;
		GetDHtmlDocument(&pDoc);
		LONG pos = GetScrollPos(SB_VERT);

		if(nSBCode == SB_LINEDOWN){
			pos += 50;
			nPos = pos;
		}
		else if(nSBCode == SB_LINEUP){
			pos -= 50;
			nPos = pos;
		}

		int max = 0;
		int min = 0;
		GetScrollRange(SB_VERT,&min,&max);
		if(nPos >= max) return;
		if(pDoc && (nSBCode == SB_THUMBTRACK || nSBCode == SB_LINEUP || nSBCode == SB_LINEDOWN)){
			pDoc->get_parentWindow(&pWnd);
			pWnd->scrollTo(0,nPos);
			SetScrollPos(SB_VERT,nPos);
		}
		else{
			return;
		}

		//CRect rect;
		//GetClientRect(&rect);
		//LONG tempH = rect.bottom - rect.top;

		//SCROLLINFO scrollInfo = {sizeof(SCROLLINFO)};
		//GetScrollInfo(SB_VERT,&scrollInfo,SIF_PAGE);
		//scrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
		//scrollInfo.nMin = min;
		//scrollInfo.nMax = max;
		//scrollInfo.nPage = max * tempH / (max);
		//scrollInfo.nPos = nPos;
		//SetScrollInfo(SB_VERT, &scrollInfo, true);
}

BOOL	TodayHtmlDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

		LONG pos = GetScrollPos(SB_VERT);
		INT min = 0;
		INT max = 0;
		GetScrollRange(SB_VERT,&min,&max); 

		if(zDelta < 0){
			pos += 50;
			if(max <= zDelta) return FALSE;
		}
		else{
			pos -= 50;
			if(min > zDelta) return FALSE;
		}

		if(pos >= max) pos = max;
		IHTMLWindow2* pWnd = NULL;
		IHTMLDocument2*   pDoc = NULL;
		GetDHtmlDocument(&pDoc);
		if(pDoc){
			pDoc->get_parentWindow(&pWnd);
			pWnd->scrollTo(0,pos);
		}
		else{
			return TRUE;
		}
		SetScrollPos(SB_VERT,pos);

		//CRect rect;
		//GetClientRect(&rect);
		//LONG tempH = rect.bottom - rect.top;

		//SCROLLINFO scrollInfo = {sizeof(SCROLLINFO)};
		//GetScrollInfo(SB_VERT,&scrollInfo,SIF_PAGE);
		//scrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_DISABLENOSCROLL;
		//scrollInfo.nMin = min;
		//scrollInfo.nMax = max;
		//scrollInfo.nPage = max* tempH / (max);
		//scrollInfo.nPos = pos;
		//SetScrollInfo(SB_VERT, &scrollInfo, true);
		return TRUE;
}

int		TodayHtmlDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
		if (CDHtmlDialog::OnCreate(lpCreateStruct) == -1)
			return -1;

		return 0;
}

void	TodayHtmlDialog::ReFresh()
{
		//TRACE("HTMLINFO: ReFresh \n");
		UpdateVscroll();
}

LRESULT	TodayHtmlDialog::ReFresh(WPARAM wp, LPARAM lp)
{
		ReFresh();
		return 0;
}

void	TodayHtmlDialog::UpdateVscroll()
{	
		IHTMLWindow2* pWnd = NULL;
		IHTMLDocument2*   pDoc = NULL;
		IHTMLElement2 *pElement = NULL;
		IHTMLElement *pEl = NULL;
		GetDHtmlDocument(&pDoc);
		int pos = GetScrollPos(SB_VERT);

		CRect rect;
		GetClientRect(&rect);
		LONG tempH = rect.bottom - rect.top;
		long h = 0;
		if(pDoc){
			//pDoc->get_parentWindow(&pWnd);
			//pWnd->scrollTo(0,0);
			pDoc->get_body(&pEl);
			if(pEl){
				pEl->QueryInterface(IID_IHTMLElement2,( void** )&pElement);
			}
			if(pElement){
				pElement->get_scrollHeight(&h);
				h -=  tempH;
				SetScrollRange(SB_VERT,0,h);
				SetScrollPos(SB_VERT,pos);

				//SCROLLINFO scrollInfo = {sizeof(SCROLLINFO)};
				//scrollInfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
				//scrollInfo.nMin = 0;
				//scrollInfo.nMax = h;
				//scrollInfo.nPage = h * tempH / (h);
				//scrollInfo.nPos = pos;
				//SetScrollInfo(SB_VERT, &scrollInfo, true);
			}
		}
}

void	TodayHtmlDialog::UpdateWebWidth()
{
		CRect rect;
		LONG tempW = 0;
		GetWindowRect(&rect);
		tempW = rect.right - rect.left;
		m_pBrowserApp->put_Width(tempW);
}

LONG	TodayHtmlDialog::GetHtmlHeight()
{
		LONG h = 0;
		m_pBrowserApp->get_Height(&h);
		return h;
}

BOOL	TodayHtmlDialog::OnEraseBkgnd(CDC* pDC)
{

		CRect rect;
		GetClientRect(rect);
		pDC->FillRect(rect,NULL);
		return TRUE;
	//return CDHtmlDialog::OnEraseBkgnd(pDC);
}

LRESULT	TodayHtmlDialog::ExecScript(WPARAM wp, LPARAM lp)
{
		CString *pStr = (CString*)wp;
		ExecScript(pStr);
		return 0;
}

void	TodayHtmlDialog::ExecScript(CString *pStrJS)
{
		if(!pStrJS) return;

		IHTMLDocument2*   pDoc = NULL;
		IHTMLWindow2 *pWin2 = NULL;
		BSTR   bs1=   pStrJS->AllocSysString(); 
		BSTR   bs2(_T("JScript"));
		VARIANT   v;  

		VariantInit(&v);
		GetDHtmlDocument(&pDoc);
		if(pDoc){
			pDoc->get_parentWindow(&pWin2);
			if(pWin2){
				pWin2->execScript(bs1,bs2,&v);
			}
		}
		SysFreeString(bs1);
}

LRESULT TodayHtmlDialog::AddHtmlString(WPARAM wp, LPARAM lp)
{
		WCHAR *pInStr = (WCHAR*)wp;
		BOOL b = (BOOL)lp;

		AddHtmlString(pInStr,b);
		return 0;
}

void	TodayHtmlDialog::AddHtmlString(WCHAR *pStr,BOOL bEnd /* = FALSE */)
{
		if(!pStr) return;

		IHTMLDocument2*   pDoc = NULL;
		IHTMLElement *pEl = NULL;
		GetDHtmlDocument(&pDoc);

		CString strAddPos;
		if(!bEnd){
			strAddPos = _T("afterBegin");
		}
		else{
			strAddPos = _T("beforeEnd");
		}
		try{
			if(pDoc){
				pDoc->get_body(&pEl);
				if(pEl){
					pEl->insertAdjacentHTML(strAddPos.GetBuffer(0),pStr);
				}
			}
		}
		catch(CException *pException){
			AfxMessageBox(_T("error"));
		}
}

LRESULT	TodayHtmlDialog::SetHtmlString(WPARAM wp, LPARAM lp)
{
		SetHtmlString((WCHAR*)wp);
		return 0;
}

void	TodayHtmlDialog::SetHtmlString(WCHAR *pStr)
{
		if(!pStr) return;

		IHTMLDocument2*   pDoc = NULL;
		IHTMLElement *pEl = NULL;
		GetDHtmlDocument(&pDoc);
		if(pDoc){
			try{
				pDoc->get_body(&pEl);
				if(pEl) pEl->put_innerHTML(pStr);
			}
			catch(CException *pException){
				AfxMessageBox(_T("error"));
			}
		}
}

void	TodayHtmlDialog::AddHeadString(CString *pInStr)
{
		if(!pInStr) return;
		IHTMLElement *pHead = NULL;
		CComBSTR str;
		GetElementByName(_T("head"),&pHead);
		if(pHead){
			pHead->get_innerHTML(&str);
			pHead->insertAdjacentHTML(_T("afterBegin"),pInStr->GetBuffer(0));
		}
}

BOOL	TodayHtmlDialog::GetElementByName(CString strName,IHTMLElement **pElement)
{
		IHTMLDocument2*   pDoc = NULL;
		GetDHtmlDocument(&pDoc);
		pDoc->clear();

		IHTMLElementCollection*   pIHTMLElementCollection=NULL; 
		if(pDoc-> get_all(&pIHTMLElementCollection)==S_OK) 
		{ 
			long   l; 
			pIHTMLElementCollection-> get_length(&l); 
			_variant_t index;  
			index.vt=VT_I4;   
			index.intVal=0;
			IDispatch*   pDisp; 

			CComBSTR   str; 
			strName.MakeUpper();
			CString strStemp = _T("");
			for(DWORD i = 0; i < l; i ++){
				index.intVal = i;
				pIHTMLElementCollection-> item(index,index,&pDisp); 
				CComQIPtr <IHTMLElement,   &IID_IHTMLElement> pHTMLElement(pDisp); 
				if(pHTMLElement!=NULL) 
				{ 
					pHTMLElement->get_innerHTML(&str);
					pHTMLElement->get_tagName(&str);
					strStemp.Format(_T("%s"),str);
					strStemp.MakeUpper();
					if(strStemp == strName){
						pDisp->QueryInterface(IID_IHTMLElement,(void**)&*pElement);
						return TRUE;
					}
				} 	
			}
		}
		return FALSE;
}

void	TodayHtmlDialog::CloseMenu()
{
		//if(!m_bMenu) return;
		//if(!m_pMenu) return;
		//HWND hWnd =  m_pMenu->GetSafeHwnd();
		//if(IsWindow(m_pMenu->m_hWnd)){
		//	m_pMenu->DestroyWindow();
		//} 
		if(m_pMenu){
			m_pMenu->PostMessage(WM_CLOSE);
		}
		m_bMenu = FALSE;
		m_pMenu = NULL;

}

void	TodayHtmlDialog::Clear()
{
		CString strTemp;
		strTemp = _T(".");
		SetHtmlString(strTemp.GetBuffer(0));
}
