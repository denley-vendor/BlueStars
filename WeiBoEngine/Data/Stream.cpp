#define STREAM_C
#include "..//..//stdafx.h"
#include "Stream.h"


InStreamA::InStreamA(DWORD buflen,DWORD growlen)
{
	m_nBufLen = buflen;
	m_nGrow = growlen;

	m_pBuf = (char*)malloc(m_nBufLen);

	//		m_pBuf = new char [m_nBufLen];

	memset(m_pBuf,0,m_nBufLen);
	m_nPos = 0;
}

InStreamA::~InStreamA()
{
	free(m_pBuf);
	//		SAFE_DELETE_ARRAY(m_pBuf);
	m_nPos = 0;
	m_nGrow = 0;
}

void	InStreamA::Grow(DWORD nLen)
{
	if(m_nPos + nLen >= (m_nBufLen - 1)) {
		DWORD nTmpLen = m_nBufLen + m_nGrow + nLen;
		char *pTmp = (char*)malloc(nTmpLen);

		//			char *pTmp = new char [nTmpLen];
		if(m_pBuf) {
			memcpy(pTmp,m_pBuf,m_nBufLen);
			//				SAFE_DELETE_ARRAY(m_pBuf);
			free(m_pBuf);
		}
		m_pBuf    = pTmp;
		m_nBufLen = nTmpLen;
	}
}

InStreamA&	InStreamA::operator=(InStreamA &from)
{
	m_nPos  = 0;
	DWORD	nLen = from.GetBufLen();
	if(nLen) {
		Grow(nLen);
		memcpy(m_pBuf+m_nPos,from.GetBuf(),nLen);
		m_nPos += nLen;
	}
	End();
	return *this;
}

InStreamA&	InStreamA::PutWChar(WCHAR *pWChar)
{
	char *pC = NULL;
	int ccnt = WideCharToMultiByte(CP_ACP,0,pWChar,-1,NULL,0,NULL,NULL);
	if(ccnt > 0) {
		pC = (char*)malloc(ccnt);
		memset(pC,0,ccnt);
		WideCharToMultiByte(CP_ACP,0,pWChar,-1,pC,ccnt,NULL,NULL);
	}

	*this << pC;

	free(pC);

	return *this;
}

InStreamA&	InStreamA::PutWCharToUTF8(WCHAR *pWChar)
{
	char *pC = NULL;
	int ccnt = WideCharToMultiByte(CP_UTF8,0,pWChar,-1,NULL,0,NULL,NULL);
	if(ccnt > 0) {
		pC = (char*)malloc(ccnt);
		memset(pC,0,ccnt);
		WideCharToMultiByte(CP_UTF8,0,pWChar,-1,pC,ccnt,NULL,NULL);
	}

	*this << pC;

	free(pC);

	return *this;	
}

void	InStreamA::Put(const void *p, DWORD nLen)
{
	if(nLen == 0) return ;
	Grow(nLen);
	memcpy(m_pBuf+m_nPos,p,nLen);
	m_nPos += nLen;
	End();
}

char	*InStreamA::GetBuf()
{
	if (IsEmpty()) return NULL;
	return m_pBuf;
}

char*	InStreamA::GetLastBuf()
{
	if (IsEmpty()) return NULL;
	return m_pBuf+m_nPos;
}

void	InStreamA::PutLONG(LONG c)
{
	Grow(sizeof(LONG));
	memcpy(m_pBuf+m_nPos,&c,sizeof(LONG));
	m_nPos += sizeof(LONG);
	End();

}

void	InStreamA::PutINT64(INT64 c)
{
	Grow(sizeof(INT64));
	memcpy(m_pBuf+m_nPos,&c,sizeof(INT64));
	m_nPos += sizeof(INT64);
	End();		
}

void	InStreamA::PutUINT64(UINT64 c)
{
	Grow(sizeof(UINT64));
	memcpy(m_pBuf+m_nPos,&c,sizeof(UINT64));
	m_nPos += sizeof(UINT64);
	End();
}

void	InStreamA::PutDWORD(DWORD c)
{
	Grow(sizeof(DWORD));
	memcpy(m_pBuf+m_nPos,&c,sizeof(DWORD));
	m_nPos += sizeof(DWORD);
	End();
}

void	InStreamA::PutWORD(WORD c)
{
	Grow(sizeof(WORD));
	memcpy(m_pBuf+m_nPos,&c,sizeof(WORD));
	m_nPos += sizeof(WORD);
	End();
}

void	InStreamA::PutBYTE(BYTE c)
{
	Grow(sizeof(BYTE));
	memcpy(m_pBuf+m_nPos,&c,sizeof(BYTE));
	m_nPos += sizeof(BYTE);
	End();
}


void	InStreamA::SetUpdate(char *strTab)
{
	m_nPos = 0;
	*this << "update " << strTab << " set ";
}
void	InStreamA::SetWhere()
{
	*this << " where ";
}

void	InStreamA::SetSelect(char *strTab,char *strSeclect)
{
	m_nPos = 0;
	*this << "select " << strSeclect << " from " << strTab;
}
void	InStreamA::SetInsert(char *strTab,BOOL bValue)
{
	m_nPos = 0;
	*this << "insert into " << strTab;
	if (bValue) {
		*this << " values ";
	}
	else {
		*this << " set ";
	}
}

void	InStreamA::SetDelete(char *strTab)
{
	m_nPos = 0;
	*this << "delete from " << strTab;
}

void	InStreamA::SetLimit(DWORD num,DWORD begin)
{
	*this << " limit ";
	if(begin > 0) *this << begin << ",";
	*this << num;
}

void	InStreamA::SetSelectField(char *strTab)
{
	m_nPos = 0;
	*this << "SHOW COLUMNS from " << strTab;
}

void	InStreamA::SetSelectTables()
{
	m_nPos = 0;
	*this << "show tables";		
}

void	InStreamA::SetShowProcessList()
{
	m_nPos = 0;
	*this << "show processlist";		
}

InStreamA&	InStreamA::operator<<(InStreamA &from)
{
	DWORD	nLen = from.GetBufLen();
	if(nLen == 0) return (*this);
	Grow(nLen);
	memcpy(m_pBuf+m_nPos,from.GetBuf(),nLen);
	m_nPos += nLen;
	End();
	return *this;
}

InStreamA&	InStreamA::operator<<(const char *p)
{
	if(!p)	return (InStreamA&)(*this);
	DWORD nLen = (DWORD)strlen(p);
	if(nLen == 0) return (*this);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos,p,nLen);
	m_nPos += nLen;
	End();
	return *this;
}

InStreamA&	InStreamA::operator<<(const char p)
{
	Grow(1);
	m_pBuf[m_nPos++] = p;
	End();
	return *this;
}

InStreamA&	InStreamA::operator<<(const int p)
{
	_itoa_s(p,m_TmpBuf,10);

	DWORD len = (DWORD)strlen(m_TmpBuf);
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;
}

InStreamA&	InStreamA::operator<<(const DWORD p)
{
	//itoa(p,m_TmpBuf,10);
	_ultoa_s(p,m_TmpBuf,10);

	DWORD len = (DWORD)strlen(m_TmpBuf);
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

InStreamA&	InStreamA::operator<<(const FLOAT p)
{
	sprintf_s(m_TmpBuf,sizeof(m_TmpBuf)-1,"%f",p);

	DWORD len = (DWORD)strlen(m_TmpBuf);
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

// InStreamA&	InStreamA::operator<<(const double p)
// {
// 
// 		m_TmpBuf
// 
// 		DWORD len = strlen(buf);
// 		Grow(len);
// 		memcpy(m_pBuf + m_nPos,buf,len);
// 		m_nPos += len;
// 		End();
// 		return *this;
// }

InStreamA&	InStreamA::operator<<(const INT64 p)
{
	_i64toa_s(p,m_TmpBuf,64,10);
	DWORD len = (DWORD)strlen(m_TmpBuf);
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;
}

InStreamA&	InStreamA::operator<<(const UINT64 p)
{
	_ui64toa_s(p,m_TmpBuf,64,10);

	DWORD len = (DWORD)strlen(m_TmpBuf);
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

BOOL	InStreamA::IsLineEnd()
{
	if(m_nPos < 2) return FALSE;

	if(m_pBuf[m_nPos-1] != '\n') return FALSE;
	if(m_pBuf[m_nPos-2] != '\r') return FALSE;
	return TRUE;
}



//============================================================================
/*
InStreamA&	InStreamA::PutSQL(BYTE *p,DWORD len)
{
Grow(len*2);
m_nPos += mysql_escape_string(m_pBuf+m_nPos,(char *)p,len);
End();
return *this;
}


InStreamA&	InStreamA::AddSQL(const char *idstr, const void *p, DWORD len)
{
if(m_nPos == 0) {
*this << "ifnull(" << idstr << ",'')";
}
DWORD xLen = len * 2 + 256;
Grow(xLen);
memmove(m_pBuf+15, m_pBuf, m_nPos);
memcpy(m_pBuf, "concat(replace(", 15);
m_nPos += 15;
*this << ",'";
DWORD esclen = 0;
DWORD escpos = m_nPos;
if(p && len) {
esclen = mysql_escape_string(m_pBuf+m_nPos,(char *)p,len);
m_nPos += esclen;
}
*this << "',''),'";
if(esclen) {
memcpy(m_pBuf+m_nPos, m_pBuf+escpos, esclen);
m_nPos += esclen;
}
*this << "')";	
return *this;
}

InStreamA&	InStreamA::RemoveSQL(const char *idstr, const void *p, DWORD len, InStreamA &Where)
{
if(m_nPos == 0) {
*this << "ifnull(" << idstr << ",'')";
}
DWORD xLen = len * 2 + 256;
Grow(xLen);
memmove(m_pBuf+8, m_pBuf, m_nPos);
memcpy(m_pBuf, "replace(", 8);
m_nPos += 8;

*this << ",'";
DWORD esclen = 0;
DWORD escpos = m_nPos;
if(p && len) {
esclen = mysql_escape_string(m_pBuf+m_nPos,(char *)p,len);
m_nPos += esclen;
}
*this << "','')";
if(esclen) {
Where << " and locate('";
Where.Put(m_pBuf+escpos, esclen);
Where << "'," << idstr << ") != 0"; 
}
return *this;
}

InStreamA&	InStreamA::ModifySQL(const char *idstr, const void *pOld, DWORD oldlen, const void *pNew, DWORD newlen,InStreamA &Where)
{
if(m_nPos == 0) {
*this << "ifnull(" << idstr << ",'')";
}
DWORD xLen = (oldlen + newlen) * 2 + 256;
Grow(xLen);
memmove(m_pBuf+8, m_pBuf, m_nPos);
memcpy(m_pBuf, "replace(", 8);
m_nPos += 8;
*this << ",'";
DWORD escoldlen = 0;
DWORD escoldpos = m_nPos;
if(pOld && oldlen) {
escoldlen = mysql_escape_string(m_pBuf+m_nPos,(char *)pOld,oldlen);
m_nPos += escoldlen;
}
*this << "','";
if(pNew && newlen) {
DWORD escnewlen = mysql_escape_string(m_pBuf+m_nPos,(char *)pNew,newlen);
m_nPos += escnewlen;
}
*this << "')";
if(pOld && oldlen) {
Where << " and locate('";
Where.Put(m_pBuf+escoldpos, escoldlen);
Where << "', " << idstr << ") != 0";	
}
return *this;
}

InStreamA&	InStreamA::LocateSQL(const char *idstr, const void *p, DWORD len)
{
DWORD nLen = len * 2 + 256;
Grow(nLen);
*this << " and locate('";
if(p && len) {
DWORD esclen = mysql_escape_string(m_pBuf+m_nPos,(char *)p,len);
m_nPos += esclen;
}
*this << "', " << idstr << ") != 0";	
return *this;
}
*/



//==============================================================================

InStreamW::InStreamW(DWORD buflen,DWORD growlen)
{
	m_nBufLen = buflen;
	m_nGrow = growlen;

	m_pBuf = (BYTE*)malloc(m_nBufLen);

	//		m_pBuf = new BYTE [m_nBufLen];

	memset(m_pBuf,0,m_nBufLen);
	m_nPos = 0;
}

InStreamW::~InStreamW()
{
	free(m_pBuf);
	//		SAFE_DELETE_ARRAY(m_pBuf);
	m_nPos = 0;
	m_nGrow = 0;
}

void	InStreamW::Grow(DWORD nLen)
{
	if(m_nPos + nLen >= (m_nBufLen - 2)) {
		DWORD nTmpLen = m_nBufLen + m_nGrow + nLen;
		BYTE *pTmp = (BYTE*)malloc(nTmpLen);

		//			BYTE *pTmp = new BYTE [nTmpLen];
		if(m_pBuf) {
			memcpy(pTmp,m_pBuf,m_nBufLen);
			//				SAFE_DELETE_ARRAY(m_pBuf);
			free(m_pBuf);
		}
		m_pBuf    = pTmp;
		m_nBufLen = nTmpLen;
	}
}

InStreamW&	InStreamW::operator=(InStreamW &from)
{
	m_nPos  = 0;
	DWORD	nLen = from.GetBufLen();
	if(nLen) {
		Grow(nLen);
		memcpy(m_pBuf+m_nPos,from.GetBuf(),nLen);
		m_nPos += nLen;
	}
	End();
	return *this;
}

InStreamW&	InStreamW::PutChar(char *pChar)
{
	WCHAR *pW = NULL;
	int wccnt = MultiByteToWideChar(CP_ACP,0,pChar,-1,NULL,0);
	if(wccnt > 0) {
		int len = wccnt * sizeof(WCHAR);
		pW = (WCHAR*)malloc(len);
		memset(pW,0,len);
		MultiByteToWideChar(CP_ACP,0,pChar,-1,pW,wccnt);
	}

	*this << pW;

	free(pW);

	return *this;
}

InStreamW&	InStreamW::PutUTF8Char(char *pChar)
{
	WCHAR *pW = NULL;
	int wccnt = MultiByteToWideChar(CP_UTF8,0,pChar,-1,NULL,0);
	if(wccnt > 0) {
		int len = wccnt * sizeof(WCHAR);
		pW = (WCHAR*)malloc(len);
		memset(pW,0,len);
		MultiByteToWideChar(CP_UTF8,0,pChar,-1,pW,wccnt);
	}

	*this << pW;

	free(pW);

	return *this;
}

void	InStreamW::Put_WCHAR(WCHAR *p,DWORD characterLen)
{
	DWORD nLen = characterLen * sizeof(WCHAR);
	if(nLen == 0) return ;	
	Put(p,nLen);
}

void	InStreamW::Put(const void *p, DWORD nLen)
{
	if(nLen == 0) return ;
	Grow(nLen);
	memcpy(m_pBuf+m_nPos,p,nLen);
	m_nPos += nLen;
	End();
}

BYTE	*InStreamW::GetBuf()
{
	if (IsEmpty()) return NULL;
	return m_pBuf;
}

BYTE*	InStreamW::GetLastBuf()
{
	if (IsEmpty()) return NULL;
	return m_pBuf+m_nPos;
}

void	InStreamW::PutLONG(LONG c)
{
	Grow(sizeof(LONG));
	memcpy(m_pBuf+m_nPos,&c,sizeof(LONG));
	m_nPos += sizeof(LONG);
	End();

}

void	InStreamW::PutINT64(INT64 c)
{
	Grow(sizeof(INT64));
	memcpy(m_pBuf+m_nPos,&c,sizeof(INT64));
	m_nPos += sizeof(INT64);
	End();		
}

void	InStreamW::PutUINT64(UINT64 c)
{
	Grow(sizeof(UINT64));
	memcpy(m_pBuf+m_nPos,&c,sizeof(UINT64));
	m_nPos += sizeof(UINT64);
	End();
}

void	InStreamW::PutDWORD(DWORD c)
{
	Grow(sizeof(DWORD));
	memcpy(m_pBuf+m_nPos,&c,sizeof(DWORD));
	m_nPos += sizeof(DWORD);
	End();
}

void	InStreamW::PutWORD(WORD c)
{
	Grow(sizeof(WORD));
	memcpy(m_pBuf+m_nPos,&c,sizeof(WORD));
	m_nPos += sizeof(WORD);
	End();
}

void	InStreamW::PutBYTE(BYTE c)
{
	Grow(sizeof(BYTE));
	memcpy(m_pBuf+m_nPos,&c,sizeof(BYTE));
	m_nPos += sizeof(BYTE);
	End();
}


void	InStreamW::SetUpdate(WCHAR *strTab)
{
	m_nPos = 0;
	*this << L"update " << strTab << L" set ";
}
void	InStreamW::SetWhere()
{
	*this << L" where ";
}

void	InStreamW::SetSelect(WCHAR *strTab,WCHAR *strSeclect)
{
	m_nPos = 0;
	*this << L"select " << strSeclect << L" from " << strTab;
}
void	InStreamW::SetInsert(WCHAR *strTab,BOOL bValue)
{
	m_nPos = 0;
	*this << L"insert into " << strTab;
	if (bValue) {
		*this << L" values ";
	}
	else {
		*this << L" set ";
	}
}

void	InStreamW::SetDelete(WCHAR *strTab)
{
	m_nPos = 0;
	*this << L"delete from " << strTab;
}

void	InStreamW::SetLimit(DWORD num,DWORD begin)
{
	*this << L" limit ";
	if(begin > 0) *this << begin << L",";
	*this << num;
}

void	InStreamW::SetSelectField(WCHAR *strTab)
{
	m_nPos = 0;
	*this << L"SHOW COLUMNS from " << strTab;
}

void	InStreamW::SetSelectTables()
{
	m_nPos = 0;
	*this << L"show tables";		
}

void	InStreamW::SetShowProcessList()
{
	m_nPos = 0;
	*this << L"show processlist";		
}

InStreamW&	InStreamW::operator<<(InStreamW &from)
{
	DWORD	nLen = from.GetBufLen();
	if(nLen == 0) return (*this);
	Grow(nLen);
	memcpy(m_pBuf+m_nPos,from.GetBuf(),nLen);
	m_nPos += nLen;
	End();
	return *this;
}

InStreamW&	InStreamW::operator<<(const WCHAR *p)
{
	if(!p)	return (InStreamW&)(*this);
	DWORD nLen = (DWORD)(wcslen(p) * sizeof(WCHAR));
	if(nLen == 0) return (*this);
	Grow(nLen);
	memcpy(m_pBuf + m_nPos,p,nLen);
	m_nPos += nLen;
	End();
	return *this;
}

InStreamW&	InStreamW::operator<<(const WCHAR p)
{
	Grow(sizeof(WCHAR));
	//m_pBuf[m_nPos++] = p;
	memcpy(m_pBuf + m_nPos,&p,sizeof(WCHAR));
	m_nPos += sizeof(WCHAR);
	End();
	return *this;
}

InStreamW&	InStreamW::operator<<(const int p)
{
	//_itoa_s(p,m_TmpBuf,10);
	_itow_s(p,m_TmpBuf,10);

	//DWORD len = (DWORD)(strlen(m_TmpBuf));
	DWORD len = (DWORD)(wcslen(m_TmpBuf) * sizeof(WCHAR));
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;
}

InStreamW&	InStreamW::operator<<(const DWORD p)
{
	//_ultoa_s(p,m_TmpBuf,10);
	_ultow_s(p,m_TmpBuf,10);

	//DWORD len = (DWORD)strlen(m_TmpBuf);
	DWORD len = (DWORD)(wcslen(m_TmpBuf) * sizeof(WCHAR));
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

InStreamW&	InStreamW::operator<<(const FLOAT p)
{
	//sprintf_s(m_TmpBuf,sizeof(m_TmpBuf)-1,"%f",p);
	swprintf_s(m_TmpBuf,64,L"%f",p);

	//DWORD len = (DWORD)strlen(m_TmpBuf);
	DWORD len = (DWORD)(wcslen(m_TmpBuf) * sizeof(WCHAR));
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

InStreamW&	InStreamW::operator<<(const INT64 p)
{
	//_i64toa_s(p,m_TmpBuf,64,10);
	_i64tow_s(p,m_TmpBuf,64,10);

	//DWORD len = (DWORD)strlen(m_TmpBuf);
	DWORD len = (DWORD)(wcslen(m_TmpBuf) * sizeof(WCHAR));
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;
}

InStreamW&	InStreamW::operator<<(const UINT64 p)
{
	//_ui64toa_s(p,m_TmpBuf,64,10);
	_ui64tow_s(p,m_TmpBuf,64,10);

	//DWORD len = (DWORD)strlen(m_TmpBuf);
	DWORD len = (DWORD)(wcslen(m_TmpBuf) * sizeof(WCHAR));
	Grow(len);
	memcpy(m_pBuf + m_nPos,m_TmpBuf,len);
	m_nPos += len;
	End();
	return *this;

}

BOOL	InStreamW::IsLineEnd()
{
	if(m_nPos < 2 * sizeof(WCHAR)) return FALSE;

	WCHAR *szLineEnd = L"\r\n";

	if(memcmp(m_pBuf+m_nPos-4,szLineEnd,4) != 0) return FALSE;

	return TRUE;
}

//==============================================================================

//////////////////////////////////////////////////////////////////////////



OutStreamA::OutStreamA(void *buf, DWORD nLen)
{
	m_pData		= (BYTE *)buf;
	m_nPos		= 0;
	m_nLength	= nLen;
}

OutStreamA::~OutStreamA()
{
}

void	OutStreamA::BackDWORD()
{
	if (m_nPos >= sizeof(DWORD)) {
		m_nPos -= sizeof(DWORD);
	}
	else {
		m_nPos = 0;
	}
}

void	*OutStreamA::GetBuf()
{
	if(m_nPos >= m_nLength) return NULL;
	return m_pData + m_nPos;
}

void	OutStreamA::Get(void *buf, DWORD len)
{
	if((m_nPos+len) > m_nLength) return;
	memcpy(buf, m_pData+m_nPos, len);
	m_nPos += len;
}

void	*OutStreamA::Get(DWORD len)
{
	if((m_nPos+len) > m_nLength) return NULL;
	BYTE *buf = m_pData + m_nPos;
	m_nPos += len;
	return buf;
}

DWORD	OutStreamA::Get()
{
	DWORD *buf = (DWORD *)Get(sizeof(DWORD));
	if(!buf) return 0;
	return *buf;
}

UINT64	OutStreamA::GetUINT64()
{
	UINT64 *buf = (UINT64 *)Get(sizeof(UINT64));
	if(!buf) return 0;
	return *buf;
}

DWORD	OutStreamA::GetDWORD()
{
	DWORD *buf = (DWORD *)Get(sizeof(DWORD));
	if(!buf) return 0;
	return *buf;
}

WORD	OutStreamA::GetWORD()
{
	WORD *buf = (WORD *)Get(sizeof(WORD));
	if(!buf) return 0;
	return *buf;
}

SWORD	OutStreamA::GetSWORD()
{
	SWORD *buf = (SWORD *)Get(sizeof(SWORD));
	if(!buf) return 0;
	return *buf;
}

SBYTE	OutStreamA::GetSBYTE()
{
	SBYTE *buf = (SBYTE *)Get(sizeof(SBYTE));
	if(!buf) return 0;
	return *buf;
}

BYTE	OutStreamA::GetBYTE()
{
	BYTE *buf = (BYTE *)Get(sizeof(BYTE));
	if(!buf) return 0;
	return *buf;
}

LONG	OutStreamA::GetLONG()
{
	LONG *buf = (LONG *)Get(sizeof(LONG));
	if(!buf) return 0;
	return *buf;
}

FLOAT	OutStreamA::GetFLOAT()
{
	FLOAT *buf = (FLOAT*)Get(sizeof(FLOAT));
	if(!buf) return 0;
	return *buf;		
}

char	*OutStreamA::GetString(DWORD len)//len是包含结束符的长度
{
	if(len < 2) return NULL;
	if((m_nPos+len) > m_nLength) return NULL;
	BYTE *buf = m_pData + m_nPos;
	m_nPos += len;
	buf[len-1] = 0;
	return (char*)buf;
}

BOOL	OutStreamA::IsEnd()
{
	if(m_nPos >= m_nLength) return TRUE;
	return FALSE;
}


//======================================================================

OutStreamW::OutStreamW(void *buf, DWORD nLen)
{
	m_pData		= (BYTE *)buf;
	m_nPos		= 0;
	m_nLength	= nLen;
}

OutStreamW::~OutStreamW()
{
}

void	OutStreamW::BackDWORD()
{
	if (m_nPos >= sizeof(DWORD)) {
		m_nPos -= sizeof(DWORD);
	}
	else {
		m_nPos = 0;
	}
}

void	*OutStreamW::GetBuf()
{
	if(m_nPos >= m_nLength) return NULL;
	return m_pData + m_nPos;
}

void	OutStreamW::Get(void *buf, DWORD len)
{
	if((m_nPos+len) > m_nLength) return;
	memcpy(buf, m_pData+m_nPos, len);
	m_nPos += len;
}

void	*OutStreamW::Get(DWORD len)
{
	if((m_nPos+len) > m_nLength) return NULL;
	BYTE *buf = m_pData + m_nPos;
	m_nPos += len;
	return buf;
}

DWORD	OutStreamW::Get()
{
	DWORD *buf = (DWORD *)Get(sizeof(DWORD));
	if(!buf) return 0;
	return *buf;
}

UINT64	OutStreamW::GetUINT64()
{
	UINT64 *buf = (UINT64 *)Get(sizeof(UINT64));
	if(!buf) return 0;
	return *buf;
}

DWORD	OutStreamW::GetDWORD()
{
	DWORD *buf = (DWORD *)Get(sizeof(DWORD));
	if(!buf) return 0;
	return *buf;
}

WORD	OutStreamW::GetWORD()
{
	WORD *buf = (WORD *)Get(sizeof(WORD));
	if(!buf) return 0;
	return *buf;
}

SWORD	OutStreamW::GetSWORD()
{
	SWORD *buf = (SWORD *)Get(sizeof(SWORD));
	if(!buf) return 0;
	return *buf;
}

SBYTE	OutStreamW::GetSBYTE()
{
	SBYTE *buf = (SBYTE *)Get(sizeof(SBYTE));
	if(!buf) return 0;
	return *buf;
}

BYTE	OutStreamW::GetBYTE()
{
	BYTE *buf = (BYTE *)Get(sizeof(BYTE));
	if(!buf) return 0;
	return *buf;
}

LONG	OutStreamW::GetLONG()
{
	LONG *buf = (LONG *)Get(sizeof(LONG));
	if(!buf) return 0;
	return *buf;
}

FLOAT	OutStreamW::GetFLOAT()
{
	FLOAT *buf = (FLOAT*)Get(sizeof(FLOAT));
	if(!buf) return 0;
	return *buf;		
}

WCHAR	*OutStreamW::GetString(DWORD len)//len是包含结束符的长度
{
	if(len < 1) return NULL;
	len *= sizeof(WCHAR);
	if((m_nPos+len) > m_nLength) return NULL;
	BYTE *buf = m_pData + m_nPos;
	m_nPos += len;
	buf[len-1] = 0;
	buf[len-2] = 0;
	return (WCHAR*)buf;
}

BOOL	OutStreamW::IsEnd()
{
	if(m_nPos >= m_nLength) return TRUE;
	return FALSE;
}
