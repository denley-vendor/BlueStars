/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define DIALOGRETWEETED_CPP
#include "stdafx.h"
#include "today.h"
#include "todayDlg.h"
#include "DialogRetweeted.h"
#include "ontentDialog1.h"
#include "ontentDialog2.h"
#include "ontentDialog3.h"

UINT	__cdecl Thread_Retweeted(LPVOID pParam)
{		
		DialogRetweeted *pDlg = (DialogRetweeted *)pParam;
		if(!pDlg) return 0;

		CString strApi;
		CString strId;
		CString strDes;
		CString postStr;
		WeiBoApi api;
		strApi = api.Retweeted();
		strDes = pDlg->GetText();
		strId = pDlg->GetId();


		postStr = _T("&id=") + strId + _T("&status=") + strDes;
		strApi += postStr;
		InStreamA instra;
		instra.PutWCharToUTF8(strApi.GetBuffer(0));

		strDes  = g_weiBoHttp.OauthRequestString(instra);

		instra.Empty();
		instra.PutWCharToUTF8(strDes.GetBuffer(0));

		TiXmlNode *pXml2 = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instra.GetBuf());
		g_WeiBoData.AddNewAttentionItem(pXml);
		g_WeiBoData.AddNewLongingItem(pXml);

		if(g_WeiBoData.IsItemBValidate(pXml)){ 
			g_WeiBoEngine.AddAttentionRefreshCount(1);

			pXml2 = pXml->RootElement();
			g_WeiBoEngine.AnalysisAttentionItem((TiXmlElement*)pXml2,g_str);
			g_contentDialog1.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));
			g_str.Empty();
			g_str.Format(_T("b_RoundCurve(\"imgtitle%d\", \"#C0D0F1\", \"#C0D0F1\", 2);"),g_WeiBoEngine.GetAttentionRefreshCount());
			g_contentDialog1.SendMessage(MSG_EXECSCRIPT,(WPARAM)&g_str);

			g_WeiBoEngine.AnalysisLoginItem((TiXmlElement*)pXml2,g_str);
			g_contentDialog3.SendMessage(MSG_ADDHTMLSTR,(WPARAM)g_str.GetBuffer(0));

		}
		else{
			AfxMessageBox(strDes);
		}
		return 0;
}
IMPLEMENT_DYNAMIC(DialogRetweeted, CDialog)

DialogRetweeted::DialogRetweeted(CWnd* pParent /*=NULL*/)
	: CDialog(DialogRetweeted::IDD, pParent)
{

}

DialogRetweeted::~DialogRetweeted()
{
}

void	DialogRetweeted::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_richEdit);
}


BEGIN_MESSAGE_MAP(DialogRetweeted, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &DialogRetweeted::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDOK, &DialogRetweeted::OnBnClickedOk)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL	DialogRetweeted::OnInitDialog()
{
		CDialog::OnInitDialog();
		dsSkinWindow(GetSafeHwnd(),SKIN_TYPE_DIALOG,_T("publishialog"),FALSE);
		dsSkinWindow(GetDlgItem(IDOK)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonPublish"),FALSE);
		dsSkinWindow(GetDlgItem(IDCANCEL)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonPublish"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_TEXT)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_BUTTON_CLOSE)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonClose"),FALSE);
		dsSetSelfPaint( GetSafeHwnd(), TRUE);
		CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
			CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf.dwEffects = 0;
		cf.yHeight = 14*14;
		m_richEdit.SetSelectionCharFormat(cf);
		return TRUE; 
}

void	DialogRetweeted::InitData(WCHAR *pItemID)
{
		if(!pItemID) {
			OnOK();
			return;
		}

		m_strId.Format(_T("%s"),pItemID);
		CString strTemp;
		CString strTemp1;
		//g_WeiBoData.GetUserText(strText,pItemID);
		//g_WeiBoData.GetUserNameByItem(strName,pItemID);
		//strTemp = strName + _T(":") + strText;

		g_WeiBoData.GetAttentionItemText(pItemID,strTemp,strTemp1);
		if(strTemp.IsEmpty()){
			g_WeiBoData.GetAtItemText(m_strId,strTemp,strTemp1);
			if(strTemp.IsEmpty()){
				g_WeiBoData.GetLoginItemText(pItemID,strTemp,strTemp1);
			}
		}
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_STATIC_TEXT);
		if(pStatic){
			m_richEdit.SetWindowText(_T(""));
			pStatic->SetWindowText(_T(""));
			if(!strTemp1.IsEmpty()){
				pStatic->SetWindowText(strTemp1);
				m_richEdit.SetWindowText(strTemp);
			}
			else{
				pStatic->SetWindowText(strTemp.GetBuffer(0));
			}
		}

		m_richEdit.SetFocus();
}

void	DialogRetweeted::OnBnClickedButtonClose()
{
		OnOK();
}

void	DialogRetweeted::OnBnClickedOk()
{
		m_richEdit.GetWindowText(m_strText);

		CWinThread		*pThread = NULL;
		pThread = AfxBeginThread(Thread_Retweeted,this,THREAD_PRIORITY_NORMAL,0,0,NULL);

		OnOK();
}

void	DialogRetweeted::DoOk()
{
		m_richEdit.Clear();
		m_strText.Empty();
		OnOK();
}

BOOL	DialogRetweeted::OnEraseBkgnd(CDC* pDC)
{
		CRect rect;
		GetClientRect(&rect);
		rect.left += 3;
		rect.top += 3;
		rect.right -= 3;
		rect.bottom -= 3;
		pDC->FillRect(&rect,NULL);
		return TRUE;
}

DialogRetweeted g_dialogRetweeted;
