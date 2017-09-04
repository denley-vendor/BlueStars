/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define  TODAY_COLOR_DLG_CPP
#include "stdafx.h"
#include "today.h"
#include "TodayColorDlg.h"
#include <math.h>


IMPLEMENT_DYNAMIC(TodayColorDlg, CDialog)

TodayColorDlg::TodayColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TodayColorDlg::IDD, pParent)
{

}

TodayColorDlg::~TodayColorDlg()
{
}

void	TodayColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TodayColorDlg, CDialog)
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_COLOR1, &TodayColorDlg::OnBnClickedButtonColor1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR2, &TodayColorDlg::OnBnClickedButtonColor2)
	ON_BN_CLICKED(IDC_BUTTON_COLOR3, &TodayColorDlg::OnBnClickedButtonColor3)
	ON_BN_CLICKED(IDC_BUTTON_COLOR4, &TodayColorDlg::OnBnClickedButtonColor4)
	ON_BN_CLICKED(IDC_BUTTON_COLOR5, &TodayColorDlg::OnBnClickedButtonColor5)
	ON_BN_CLICKED(IDC_BUTTON_COLOR6, &TodayColorDlg::OnBnClickedButtonColor6)
	ON_BN_CLICKED(IDC_BUTTON_COLOR7, &TodayColorDlg::OnBnClickedButtonColor7)
	ON_BN_CLICKED(IDC_BUTTON_COLOR8, &TodayColorDlg::OnBnClickedButtonColor8)
	ON_BN_CLICKED(IDC_BUTTON_COLORRESET, &TodayColorDlg::OnBnClickedButtonColorreset)
END_MESSAGE_MAP()


BOOL	TodayColorDlg::OnInitDialog()
{
		CDialog::OnInitDialog();

		BOOL bOk = FALSE;
		bOk = dsSkinWindow(GetSafeHwnd(),SKIN_TYPE_DIALOG,_T("colordialog"),FALSE);
		if(!bOk) AfxMessageBox(_T("bOk = dsSkinWindow(GetSafeHwnd()"));

		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR1)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton1"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR2)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton2"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR3)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton3"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR4)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton4"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR5)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton5"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR6)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton6"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR7)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton7"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLOR8)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("colorbutton8"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));
		bOk = dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)->GetSafeHwnd(),SKIN_TYPE_BUTTON,_T("buttonColor"),FALSE);
		if(!bOk) AfxMessageBox(_T("dsSkinWindow(GetDlgItem(IDC_BUTTON_COLORRESET)"));

		dsSetSelfPaint( GetSafeHwnd(), TRUE);
		return TRUE;  
}

void	TodayColorDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
		CDialog::OnActivate(nState, pWndOther, bMinimized);

		if ( nState == WA_INACTIVE )
		{
			ShowWindow( SW_HIDE);
		}
}

BOOL	TodayColorDlg::OnNcActivate(BOOL bActive)
{
		ShowWindow( SW_HIDE);
		return CDialog::OnNcActivate(bActive);
}

BOOL	TodayColorDlg::OnEraseBkgnd(CDC* pDC)
{
		CRect rect;
		GetClientRect(&rect);
		rect.left += 3;
		rect.top += 3;
		rect.right -= 3;
		rect.bottom -= 3;
		pDC->FillRect(&rect,NULL);
		//return CDialog::OnEraseBkgnd(pDC);
		return TRUE;
}

void   TodayColorDlg::RGBtoHSI(BYTE r,BYTE g,BYTE b,double& h,double& s,double& i) 
{ 
		i   =   (r+g+b)   /   3.0; 
		if((r+g+b)==0)
			s=0;
		else
			s=1-(3*(min(min(r,g),b))/(r+g+b)); 


		double temp;
		if((((r-g)*(r-g))+((r-b)*(g-b)))==0)
			h=0;
		else{
			temp=((r-g)+(r-b))/(2*sqrt(double((r-g)*(r-g))+((r-b)*(g-b))));
			if(g>=b)
				h=acos(temp); 
			else
				h=2*3.1416-acos(temp); 
		}
}
void	TodayColorDlg::OnBnClickedButtonColor1()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( 0,118,0);
}

void	TodayColorDlg::OnBnClickedButtonColor2()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( 0,100,0);
}

void	TodayColorDlg::OnBnClickedButtonColor3()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( 61,100,0);
}

void	TodayColorDlg::OnBnClickedButtonColor4()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( -92,78,0);
}

void	TodayColorDlg::OnBnClickedButtonColor5()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( -170,89,0);
}

void	TodayColorDlg::OnBnClickedButtonColor6()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( -180,69,0);
}

void	TodayColorDlg::OnBnClickedButtonColor7()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( 104,67,0);
}

void	TodayColorDlg::OnBnClickedButtonColor8()
{
		ShowWindow( SW_HIDE);
		dsAdjustColor( 0,0,0);
}

void	TodayColorDlg::OnBnClickedButtonColorreset()
{
		ShowWindow(SW_HIDE);
		dsAdjustColor( 0,100,0);
}
