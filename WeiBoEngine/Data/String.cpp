#define STRING_C
#include "..//../stdafx.h"
#include "String.h"


StringA::StringA()
{
	m_pData		= NULL;
	m_nSize		= 0;
	m_nMaxSize	= 0;
	//Init();
}

StringA::StringA(const StringA &stringSrc)
{
	DWORD len = stringSrc.GetLength();
	if(len) {
		m_nMaxSize = len+1;
		m_pData = (char*)malloc(m_nMaxSize);

		//			m_pData = new char [len+1];
		memcpy(m_pData,stringSrc.GetData(),len);
		m_pData[len] = 0;
		m_nSize = len;
	}
	else {
		m_pData		= NULL;
		m_nSize		= 0;
		m_nMaxSize	= 0;
		//Init();
	}
}

StringA::StringA(char *pStr)
{
	m_pData		= NULL;
	m_nSize		= 0;
	m_nMaxSize	= 0;
	//Init();
	*this=pStr;
}

StringA::~StringA()
{
	Free();
}

void	StringA::Free()
{
	//SAFE_DELETE_ARRAY(m_pData);
	free(m_pData);
}

void	StringA::OnEnum(DWORD param1,DWORD param2)
{
	if(param1 == 1) {
		delete this;
	}
}

void	StringA::Grow(DWORD nGrowSize)
{
	DWORD newsize = m_nSize + nGrowSize + 1;
	if(newsize > m_nMaxSize) {
		m_nMaxSize += newsize;

		char *pNewData = (char*)malloc(m_nMaxSize);

		//			char *pNewData = new char[m_nMaxSize];
		if(m_nSize) memcpy(pNewData,m_pData,sizeof(char)*(m_nSize));
		//			SAFE_DELETE_ARRAY(m_pData);
		free(m_pData);

		m_pData = pNewData;
	}
}


char	StringA::operator[](DWORD nIndex)
{
	ASSERT(nIndex < m_nSize);
	return m_pData[nIndex];

}

const StringA	&StringA::operator=(const StringA &stringSrc)
{
	*this = stringSrc.GetData();
	return *this;
}

const StringA	&StringA::operator=(char nChar)
{
	Empty();
	Grow(1);
	memcpy(m_pData,&nChar,1);
	m_pData[1] = 0;
	m_nSize = 1;
	return *this;

}

const StringA	&StringA::operator=(LPCSTR lpsz)
{
	Empty();
	if(lpsz) {
		DWORD newsize = (DWORD)strlen(lpsz);
		if(newsize) {
			Grow(newsize);
			memcpy(m_pData,lpsz,newsize);
			m_pData[newsize] = 0;
			m_nSize = newsize;
		}
	}
	return *this;
}

// const StringA	&StringA::operator=(LPCWSTR lpsz)
// {
// 	Empty();
// 	if(lpsz) {
// 		int	size = WideCharToMultiByte(CP_ACP,0,lpsz, -1,NULL, 0, NULL, NULL);
// 		if(size > 0) {
// 			Grow(size);
// 			WideCharToMultiByte(CP_ACP, 0,lpsz,-1,m_pData, size, NULL, NULL);
// 			m_nSize = size;
// 			m_pData[m_nSize] = 0;
// 		}
// 	}
// 	return *this;
// }

const StringA	&StringA::operator+=(const StringA& string)
{
	*this += string.GetData();
	return *this;
}

const StringA	&StringA::operator+=(char ch)
{
	Grow(1);
	m_pData[m_nSize++] = ch;
	m_pData[m_nSize] = 0;
	return *this;
}

const StringA	&StringA::operator+=(WCHAR wch)
{
	Grow(2);
	WORD c = (WORD)(wch & 0x00FF) << 8;
	c |= (wch >> 8);
	memcpy(m_pData+m_nSize,&c,2);
	m_nSize += 2;
	m_pData[m_nSize] = 0;
	return *this;
}

const StringA	&StringA::operator+=(LPCSTR lpsz)
{
	if(lpsz) {
		DWORD growsize = (DWORD)strlen(lpsz);
		Grow(growsize);
		memcpy(m_pData+m_nSize,lpsz,growsize);
		m_nSize += growsize;
		m_pData[m_nSize] = 0;
	}
	return *this;
}

const StringA	&StringA::operator=(DWORD value)
{
	Empty();
	char cChar[64];
	memset(cChar,0,sizeof(cChar));
	_ltoa_s(value,cChar,sizeof(cChar)-1,10);
	DWORD newsize = strlen(cChar);
	if(newsize) {
		Grow(newsize);
		memcpy(m_pData,cChar,newsize);
		m_pData[newsize] = 0;
		m_nSize = newsize;
	}
	return *this;

}

const StringA	&StringA::operator+=(DWORD value)
{
	char cChar[64];
	memset(cChar,0,sizeof(cChar));
	_ltoa_s(value,cChar,sizeof(cChar)-1,10);
	DWORD growsize = strlen(cChar);
	if (growsize) {
		Grow(growsize);
		memcpy(m_pData+m_nSize,cChar,growsize);
		m_nSize += growsize;
		m_pData[m_nSize] = 0;
	}
	return *this;
}

StringA	StringA::operator+(StringA &string)
{
	StringA retstr(*this);
	retstr += string;
	return retstr;
}

BOOL	StringA::operator==(LPCSTR lpsz)
{
	DWORD len = 0;
	if(lpsz) len = (DWORD)strlen(lpsz);
	if(m_nSize != len) return FALSE;
	if(len == 0) return TRUE;
	return (_stricoll(m_pData,lpsz) == 0);
}

BOOL	StringA::operator>(LPCSTR lpsz)
{
	if(*this == lpsz) return FALSE;
	DWORD len = 0;
	if(lpsz) len = (DWORD)strlen(lpsz);
	if(!m_nSize || !len) return (m_nSize > len);

	return _stricoll(m_pData,lpsz)>0;
}

BOOL	StringA::operator<(LPCSTR lpsz)
{

	if(*this == lpsz) return FALSE;
	DWORD len = 0;
	if(lpsz) len = (DWORD)strlen(lpsz);
	if(!m_nSize || !len) return (m_nSize > len);

	return _stricoll(m_pData,lpsz)<0;
}

BOOL	StringA::operator>=(LPCSTR lpsz)
{
	return ((*this == lpsz) || (*this > lpsz));
}

BOOL	StringA::operator<=(LPCSTR lpsz)
{
	return ((*this == lpsz) || (*this < lpsz));
}

DWORD	StringA::Replace(char chOld,char chNew)
{
	DWORD num = 0;
	for(DWORD i = 0; i < m_nSize; i ++) {
		if(m_pData[i] == chOld) {
			m_pData[i] = chNew;
			num ++;
		}
	}
	return num;
}


StringA	StringA::Mid(DWORD nFirst,DWORD nCount)
{
	StringA retstr;
	if(nFirst < m_nSize) {
		if(nFirst + nCount > m_nSize) nCount = m_nSize - nFirst;
		retstr.Grow(nCount);
		char *p = retstr.GetData();
		memcpy(p,m_pData+nFirst,nCount);
		p[nCount] = 0;
		retstr.SetLength(nCount);
	}
	return retstr;

}

StringA	StringA::Left(DWORD nCount)
{
	StringA retstr;
	if(nCount > m_nSize) nCount = m_nSize;
	retstr.Grow(nCount);
	char *p = retstr.GetData();
	memcpy(p,m_pData,nCount);
	p[nCount] = 0;
	retstr.SetLength(nCount);
	return retstr;
}

StringA	StringA::Right(DWORD nCount)
{

	StringA retstr;
	if(nCount) {
		if(nCount > m_nSize) nCount = m_nSize;
		retstr.Grow(nCount);
		char *p = retstr.GetData();
		memcpy(p,m_pData+(m_nSize - nCount),nCount);
		p[nCount] = 0;
		retstr.SetLength(nCount);
	}
	return retstr;

}

BOOL StringA::Find(DWORD nBegin,LPCSTR lpStr)
{
	if(!lpStr) return NULL;
	DWORD len = (DWORD)strlen(lpStr);
	if(!len) return FALSE;
	if(nBegin+len >= m_nSize) return FALSE;
	return (_strnicmp(m_pData+nBegin,lpStr,len) == 0);
}

int	StringA::Find(DWORD begin,char chKey)
{
	for(DWORD i = begin; i < m_nSize; i ++) {
		if(m_pData[i] == chKey) {
			return i;	
		}
	}
	return -1;
}

// *****************************************

StringW::StringW()
{
	m_pData		= NULL;
	m_nSize		= 0;
	m_nMaxSize	= 0;
	//Init();
}

StringW::StringW(const StringW &stringSrc)
{
	DWORD len = stringSrc.GetLength();
	if(len) {
		m_nMaxSize = len+1;
		DWORD sizeInBytes = m_nMaxSize * sizeof(WCHAR);
		m_pData = (WCHAR*)malloc(sizeInBytes);
		m_nMaxSize = sizeInBytes / 2;

		//			m_pData = new char [len+1];
		memcpy(m_pData,stringSrc.GetData(),len*sizeof(WCHAR));
		m_pData[len] = 0;
		m_nSize = len;
	}
	else {
		m_pData		= NULL;
		m_nSize		= 0;
		m_nMaxSize	= 0;
		//Init();
	}
}

StringW::StringW(WCHAR *pStr)
{
	m_pData		= NULL;
	m_nSize		= 0;
	m_nMaxSize	= 0;
	//Init();
	*this=pStr;
}

StringW::~StringW()
{
	Free();
}

void	StringW::Free()
{
	//SAFE_DELETE_ARRAY(m_pData);
	free(m_pData);
}

void	StringW::OnEnum(DWORD param1,DWORD param2)
{
	if(param1 == 1) {
		delete this;
	}
}

void	StringW::Grow(DWORD nGrowSize)
{
	DWORD newsize = m_nSize + nGrowSize + 1;
	if(newsize > m_nMaxSize) {
		m_nMaxSize += newsize;

		DWORD sizeInBytes = m_nMaxSize * sizeof(WCHAR);
		WCHAR *pNewData = (WCHAR*)malloc(sizeInBytes);
		m_nMaxSize = sizeInBytes / 2;

		if(m_nSize) memcpy(pNewData,m_pData,sizeof(WCHAR)*(m_nSize));
		
		free(m_pData);

		m_pData = pNewData;
	}
}


WCHAR	StringW::operator[](DWORD nIndex)
{
	ASSERT(nIndex < m_nSize);
	return m_pData[nIndex];

}

const StringW	&StringW::operator=(const StringW &stringSrc)
{
	*this = stringSrc.GetData();
	return *this;
}

const StringW	&StringW::operator=(WCHAR nChar)
{
	Empty();
	Grow(1);
	memcpy(m_pData,&nChar,sizeof(WCHAR));
	m_pData[1] = 0;
	m_nSize = 1;
	return *this;

}

// const StringW	&StringW::operator=(LPCSTR lpsz)
// {
// 	Empty();
// 	if(lpsz) {
// 		DWORD newsize = (DWORD)strlen(lpsz);
// 		if(newsize) {
// 			Grow(newsize);
// 			memcpy(m_pData,lpsz,newsize);
// 			m_pData[newsize] = 0;
// 			m_nSize = newsize;
// 		}
// 	}
// 	return *this;
// }

const StringW	&StringW::operator=(LPCWSTR lpsz)
{
	Empty();
	if(lpsz) {
		DWORD size = (DWORD)wcslen(lpsz);
		if(size) {
			Grow(size);
			memcpy(m_pData,lpsz,size*sizeof(WCHAR));
			m_nSize = size;
			m_pData[m_nSize] = 0;
		}
	}
	return *this;
}

const StringW	&StringW::operator+=(const StringW& string)
{
	*this += string.GetData();
	return *this;
}

const StringW	&StringW::operator+=(char ch)
{
	//正确性待验证.....
	Grow(1);
	WORD wch = (WORD)ch;
	WORD c = (WORD)(wch & 0x00FF) << 8;
	c &= 0xFF00;
	memcpy(m_pData+m_nSize,&c,2);
	m_nSize += 1;
	m_pData[m_nSize] = 0;
	return *this;

}

const StringW	&StringW::operator+=(WCHAR wch)
{
	Grow(1);
	m_pData[m_nSize++] = wch;
	m_pData[m_nSize] = 0;
	return *this;
}

const StringW	&StringW::operator+=(LPCWSTR lpsz)
{
	if(lpsz) {
		DWORD growsize = (DWORD)wcslen(lpsz);
		Grow(growsize);
		memcpy(m_pData+m_nSize,lpsz,growsize*sizeof(WCHAR));
		m_nSize += growsize;
		m_pData[m_nSize] = 0;
	}
	return *this;
}

const StringW	&StringW::operator=(DWORD value)
{
	Empty();
	WCHAR cChar[64];
	memset(cChar,0,sizeof(cChar));
	_ltow_s(value,cChar,sizeof(cChar)/sizeof(WCHAR)-1,10);
	DWORD newsize = wcslen(cChar);
	if(newsize) {
		Grow(newsize);
		memcpy(m_pData,cChar,newsize*sizeof(WCHAR));
		m_pData[newsize] = 0;
		m_nSize = newsize;
	}
	return *this;

}

const StringW	&StringW::operator+=(DWORD value)
{
	WCHAR cChar[64];
	memset(cChar,0,sizeof(cChar));
	_ltow_s(value,cChar,sizeof(cChar)/sizeof(WCHAR)-1,10);
	DWORD growsize = wcslen(cChar);
	if (growsize) {
		Grow(growsize);
		memcpy(m_pData+m_nSize,cChar,growsize*sizeof(WCHAR));
		m_nSize += growsize;
		m_pData[m_nSize] = 0;
	}
	return *this;
}



StringW	StringW::operator+(StringW &string)
{
	StringW retstr(*this);
	retstr += string;
	return retstr;
}

BOOL	StringW::operator==(LPCWSTR lpsz)
{
	DWORD len = 0;
	if(lpsz) len = (DWORD)wcslen(lpsz);
	if(m_nSize != len) return FALSE;
	if(len == 0) return TRUE;
	return (_wcsicoll(m_pData,lpsz) == 0);
}

BOOL	StringW::operator>(LPCWSTR lpsz)
{
	if(*this == lpsz) return FALSE;
	DWORD len = 0;
	if(lpsz) len = (DWORD)wcslen(lpsz);
	if(!m_nSize || !len) return (m_nSize > len);

	return _wcsicoll(m_pData,lpsz)>0;
}

BOOL	StringW::operator<(LPCWSTR lpsz)
{

	if(*this == lpsz) return FALSE;
	DWORD len = 0;
	if(lpsz) len = (DWORD)wcslen(lpsz);
	if(!m_nSize || !len) return (m_nSize > len);

	return _wcsicoll(m_pData,lpsz)<0;
}

BOOL	StringW::operator>=(LPCWSTR lpsz)
{
	return ((*this == lpsz) || (*this > lpsz));
}

BOOL	StringW::operator<=(LPCWSTR lpsz)
{
	return ((*this == lpsz) || (*this < lpsz));
}

DWORD	StringW::Replace(WCHAR chOld,WCHAR chNew)
{
	DWORD num = 0;
	for(DWORD i = 0; i < m_nSize; i ++) {
		if(m_pData[i] == chOld) {
			m_pData[i] = chNew;
			num ++;
		}
	}
	return num;
}


StringW	StringW::Mid(DWORD nFirst,DWORD nCount)
{
	StringW retstr;
	if(nFirst < m_nSize) {
		if(nFirst + nCount > m_nSize) nCount = m_nSize - nFirst;
		retstr.Grow(nCount);
		WCHAR *p = retstr.GetData();
		memcpy(p,m_pData+nFirst,nCount*sizeof(WCHAR));
		p[nCount] = 0;
		retstr.SetLength(nCount);
	}
	return retstr;

}

StringW	StringW::Left(DWORD nCount)
{
	StringW retstr;
	if(nCount > m_nSize) nCount = m_nSize;
	retstr.Grow(nCount);
	WCHAR *p = retstr.GetData();
	memcpy(p,m_pData,nCount*sizeof(WCHAR));
	p[nCount] = 0;
	retstr.SetLength(nCount);
	return retstr;
}

StringW	StringW::Right(DWORD nCount)
{

	StringW retstr;
	if(nCount) {
		if(nCount > m_nSize) nCount = m_nSize;
		retstr.Grow(nCount);
		WCHAR *p = retstr.GetData();
		memcpy(p,m_pData+(m_nSize - nCount),nCount*sizeof(WCHAR));
		p[nCount] = 0;
		retstr.SetLength(nCount);
	}
	return retstr;

}

BOOL StringW::Find(DWORD nBegin,LPCWSTR lpStr)
{
	if(!lpStr) return NULL;
	DWORD len = (DWORD)wcslen(lpStr);
	if(!len) return FALSE;
	if(nBegin+len >= m_nSize) return FALSE;
	return (_wcsnicmp(m_pData+nBegin,lpStr,len) == 0);
}

int	StringW::Find(DWORD begin,WCHAR chKey)
{
	for(DWORD i = begin; i < m_nSize; i ++) {
		if(m_pData[i] == chKey) {
			return i;	
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////


/*
BOOL	BreakString(CString &s,char c,CString &r)
{
	if(s.IsEmpty()) return FALSE;
	int index = s.Find(c);
	if(index < 0) {
		r = s;
		s.Empty();
		return TRUE;
	}

	r = s.Left(index);
	s = s.Right(s.GetLength()-index-1);

	return TRUE;
}
*/

BOOL	BreakString(CString &s,WCHAR c,CString &r)
{
	if(s.IsEmpty()) return FALSE;
	int index = s.Find(c);
	if(index < 0) {
		r = s;
		s.Empty();
		return TRUE;
	}

	r = s.Left(index);
	s = s.Right(s.GetLength()-index-1);

	return TRUE;
}

/*
BOOL	DivideString(CString& strSrc,CString strStart,CString strEnd,CString& strDesc,CString& strMid)
{
	if(strSrc.IsEmpty()) return FALSE;
	int iFirst = strSrc.Find(strStart);
	if(iFirst < 0) {
		strDesc = strSrc;
		strSrc.Empty();
		strMid.Empty();
		return TRUE;
	}
	int iSecond = strSrc.Find(strEnd);
	if(iSecond < 0) {
		strDesc = strSrc;
		strSrc.Empty();
		strMid.Empty();
		return TRUE;
	} 

	strDesc = strSrc.Left(iFirst);
	strMid  = strSrc.Mid(iFirst+strStart.GetLength(),iSecond - iFirst - strStart.GetLength());
	strSrc  = strSrc.Right(strSrc.GetLength() - iSecond - strEnd.GetLength());

	return TRUE;
}
*/

BOOL	DivideString(CString& strSrc,CString strStart,CString strEnd,CString& strDesc,CString& strMid)
{
	if(strSrc.IsEmpty()) return FALSE;
	int iFirst = strSrc.Find(strStart);
	if(iFirst < 0) {
		strDesc = strSrc;
		strSrc.Empty();
		strMid.Empty();
		return TRUE;
	}
	int iSecond = strSrc.Find(strEnd);
	if(iSecond < 0) {
		strDesc = strSrc;
		strSrc.Empty();
		strMid.Empty();
		return TRUE;
	} 

	strDesc = strSrc.Left(iFirst);
	strMid  = strSrc.Mid(iFirst+strStart.GetLength(),iSecond - iFirst - strStart.GetLength());
	strSrc  = strSrc.Right(strSrc.GetLength() - iSecond - strEnd.GetLength());

	return TRUE;
}
