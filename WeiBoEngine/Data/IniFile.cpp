#define INI_FILE_C
#include "..//..//stdafx.h"
#include "IniFile.h"

IniFile::IniFile()
{


}

IniFile::~IniFile()
{


}

BOOL	IniFile::IsDirectoryExist(CString &dir)
{
		CFileFind m_Find;
		BOOL m_IsWork=m_Find.FindFile(dir+_T("*.*"));
		m_Find.Close();
		return m_IsWork;

}

BOOL	IniFile::CreateDir(CString &dir)
{
		if(IsDirectoryExist(dir)) return TRUE;

		CString str = dir.Left(dir.GetLength()-1);

		while(1) {
			str = str.Left(str.ReverseFind(_T('\\')));
			if(str.IsEmpty()) return FALSE;
			if(IsDirectoryExist(str+_T("\\"))) break;			
		}

		while(str.GetLength() < dir.GetLength()-1) {
			str += dir.Mid(str.GetLength(),dir.Find(_T('\\'),str.GetLength()+1)-str.GetLength());
			if(!::CreateDirectory(str,NULL)) return FALSE;
		}
		return TRUE;
}

BOOL	IniFile::InitFile(WCHAR *pFileName)
{
		m_FileName = pFileName;

		CString path = m_FileName.Left(m_FileName.ReverseFind(_T('\\'))+1);

		return CreateDir(path);
}

DWORD	IniFile::GetValue(WCHAR *pSection,WCHAR *pKey,int defaultvalue)
{
		DWORD retVal = GetPrivateProfileInt(pSection,pKey,defaultvalue,m_FileName);
		return retVal;
		//return GetPrivateProfileInt(pSection,pKey,defaultvalue,m_FileName);
}

void	IniFile::GetString(WCHAR *pSection,WCHAR *pKey,CString &str)
{
		WCHAR buf[256];
		memset(buf,0,sizeof(buf));

		//Parameters : nSize [in] 
		//The size of the buffer pointed to by the lpReturnedString parameter, in characters.
		GetPrivateProfileString(pSection,pKey,NULL,buf,256,m_FileName);
		str = buf;
}

void	IniFile::SetValue(WCHAR *pSection,WCHAR *pKey,DWORD a)
{
		CString s;
		s.Format(_T("%d"),a);
		WritePrivateProfileString(pSection,pKey,s,m_FileName);
}

void	IniFile::SetString(WCHAR *pSection,WCHAR *pKey,WCHAR *pStr)
{
		WritePrivateProfileString(pSection,pKey,pStr,m_FileName);
}
