/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define DIALOGPUBLISH_CPP

#include "stdafx.h"
#include <curl/curl.h>
#include "today.h"
#include "DialogPublish.h"
#include "PicDialog.h"
#include "TodayMain.h"
#include "ontentDialog1.h"
#include "ontentDialog3.h"

UINT	__cdecl Thread_Publish(LPVOID pParam)
{
		DialogPublish *pDlg = (DialogPublish *)pParam;
		if(!pDlg) return 0;

		CString strTemp = pDlg->GetText();


		WeiBoApi api;
		CString postStr = _T("&status=") + strTemp;
		CString strApi = api.PublishItem() + postStr;

		InStreamA instra;
		instra.PutWCharToUTF8(strApi.GetBuffer(0));
		strTemp = g_weiBoHttp.OauthRequestString(instra);

		instra.Empty();
		instra.PutWCharToUTF8(strTemp.GetBuffer(0));

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
			AfxMessageBox(strTemp);
		}
		SAFE_DELETE(pXml);

		pDlg->DoOk();
		return 0;
}

UINT	__cdecl Thread_CurlPublish(LPVOID pParam)
{
		DialogPublish *pDlg = (DialogPublish *)pParam;
		if(!pDlg) return 0;

		CString str = pDlg->GetText();
		CString strPath = pDlg->GetPath();

		InStreamA instPath;
		instPath.PutWChar(strPath.GetBuffer(0));
		InStreamA instrDes;
		instrDes.PutWCharToUTF8(str.GetBuffer(0));


		InStreamA inApi;
		inApi << "http://api.t.sina.com.cn/statuses/upload.xml?status=";
		inApi << instrDes.GetBuf();

		str = g_weiBoHttp.OauthPostPic(inApi,instrDes,instPath);

		instrDes.Empty();
		instrDes.PutWCharToUTF8(str.GetBuffer(0));

		TiXmlNode *pXml2 = NULL;
		TiXmlDocument *pXml = new TiXmlDocument;
		pXml->Parse(instrDes.GetBuf());
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
			AfxMessageBox(str);
		}
		SAFE_DELETE(pXml);
		return 0;
}


IMPLEMENT_DYNAMIC(DialogPublish, CDialog)
DialogPublish::DialogPublish(CWnd* pParent /*=NULL*/)
	: CDialog(DialogPublish::IDD, pParent)
{	
		m_strText.Empty();
		m_strPath.Empty();
}

DialogPublish::~DialogPublish()
{
}

void	DialogPublish::DoDataExchange(CDataExchange* pDX)
{
		CDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_RICHEDIT21, m_richEdit);
}


BEGIN_MESSAGE_MAP(DialogPublish, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &DialogPublish::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &DialogPublish::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CUTINSERT, &DialogPublish::OnBnClickedButtonCutinsert)
	ON_BN_CLICKED(IDC_BUTTON_CUT, &DialogPublish::OnBnClickedButtonCut)
END_MESSAGE_MAP()


BOOL	DialogPublish::OnInitDialog()
{
		CDialog::OnInitDialog();
		dsSkinWindow(GetSafeHwnd(),SKIN_TYPE_DIALOG,_T("publishialog"),FALSE);
		dsSkinWindow(GetDlgItem(IDOK)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonPublish"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_TITLE)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_BUTTON_CLOSE)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonClose"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_BUTTON_CUTINSERT)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonInsert"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_BUTTON_CUT)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("dialogButtonCut"),FALSE);
		dsSkinWindow(GetDlgItem(IDC_STATIC_PATHSHOW)->GetSafeHwnd(),SKIN_TYPE_STATIC,_T("StaticLogin"),FALSE);
		dsSetSelfPaint(GetSafeHwnd(),TRUE);

		CHARFORMAT cf;
		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
			CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf.dwEffects = 0;
		cf.yHeight = 14*14;
	//	wcscpy(cf.szFaceName ,_T("隶书"));

		m_richEdit.SetSelectionCharFormat(cf);
		return TRUE; 
}

BOOL	DialogPublish::OnEraseBkgnd(CDC* pDC)
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

void	DialogPublish::OnBnClickedOk()
{		
		CString strTemp;
		m_richEdit.GetWindowText(m_strText);
		if(!m_strPath.IsEmpty()){
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_CurlPublish,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		else{
			CWinThread		*pThread = NULL;
			pThread = AfxBeginThread(Thread_Publish,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
		}

		OnOK();
}

void	DialogPublish::OnBnClickedButtonClose()
{
		OnOK();
}

void	DialogPublish::OnBnClickedButtonCutinsert()
{		
		WCHAR Buffer[MAX_PATH]; 
		GetCurrentDirectory(MAX_PATH,Buffer);
		CFileDialog dlg(TRUE, 
			_T("Jpeg Files (*jpeg)"), 
			NULL, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
			_T("图片格式 (*.JPEG;*.PNG;*.JPG)|*.JPEG;*.PNG;*.JPG;)||"),
			NULL);
		if(dlg.DoModal() == IDOK){
			m_strPath = dlg.GetPathName();

			CString str = dlg.GetFileName();
			SetPathText(str.GetBuffer(0));
		}
		SetCurrentDirectory(Buffer);
	//	CurlPublish();
}

void	DialogPublish::SetPathText(WCHAR *pStr)
{
		CStatic *pStatic = (CStatic*)GetDlgItem(IDC_STATIC_PATHSHOW);
		if(pStatic){
			pStatic->SetWindowText(pStr);
		}
}

void	DialogPublish::CurlPublish()
{

		CString strPath;
		WCHAR Buffer[MAX_PATH]; 
		GetCurrentDirectory(MAX_PATH,Buffer);
		CFileDialog dlg(TRUE);
		if(dlg.DoModal() == IDOK){
			strPath = dlg.GetPathName();

			InStreamA instr1;
			instr1.PutWCharToUTF8(strPath.GetBuffer(0));
			InStreamA instr2;
			CString str;
			m_richEdit.GetWindowText(str);
			instr2.PutWCharToUTF8(str.GetBuffer(0));
			struct curl_httppost *post = NULL;
			struct curl_httppost *last = NULL;

			CURL *curl  = curl_easy_init();
			curl_easy_setopt(curl,CURLOPT_URL,"http://api.t.sina.com.cn/statuses/upload.xml?source=544758078");
			curl_easy_setopt(curl,CURLOPT_POST,1);
			curl_formadd(&post,&last,CURLFORM_COPYNAME,"source",CURLFORM_COPYCONTENTS,"544758078",CURLFORM_END);
			curl_formadd(&post,&last,CURLFORM_COPYNAME,"pic",CURLFORM_FILE,instr1.GetBuf(),CURLFORM_END);
			curl_formadd(&post,&last,CURLFORM_COPYNAME,"status",CURLFORM_COPYCONTENTS,instr2.GetBuf(),CURLFORM_END);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

			curl_slist *chunk = NULL;
			curl_slist_append(chunk,"Content-Type: multipart/form-data; boundary=-------------------------7b4a6d158c9");
			curl_easy_setopt(curl,CURLOPT_HTTPHEADER,chunk);
			//curl_easy_setopt(curl,CURLOPT_RETURNTRANSFER,1);
			instr1.Empty();
			instr1.PutWCharToUTF8(g_weiBoHttp.GetLoginKey().GetBuffer(0));
			curl_easy_setopt(curl,CURLOPT_USERPWD,instr1.GetBuf());
			curl_easy_perform(curl);

			curl_formfree(post);
			curl_easy_cleanup(curl);
			curl_global_cleanup();
		}
		SetCurrentDirectory(Buffer);
}

void	DialogPublish::DoOk()
{
		m_strText.Empty();
		m_strPath.Empty();
}
void	DialogPublish::OnBnClickedButtonCut()
{
		CPicDialog dlg;
		if(dlg.DoModal() == IDOK) {
			m_strPath = dlg.GetImagePath();
			SetPathText(_T("cut.jpg"));
		}
}
