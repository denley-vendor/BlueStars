#pragma once

#ifndef STREAM_H
#define STREAM_H
typedef short int SWORD;

#pragma pack(1)

#pragma pack()

struct WMI_DATA;


#ifdef UNICODE
#define InStream  InStreamW
#define OutStream	OutStreamW
#else
#define InStream  InStreamA
#define OutStream	OutStreamA
#endif // !UNICODE


// ****** InStream MultiByte Version ****** //
class InStreamA
{
private:
	char		*m_pBuf;
	DWORD		m_nBufLen;
	DWORD		m_nPos;
	DWORD		m_nGrow;
	char		m_TmpBuf[64];

	void		Grow(DWORD nLen);
public:
	InStreamA(DWORD buflen=64,DWORD growlen=64);
	~InStreamA();


		inline		InStreamA&	operator<<(InStreamA& (__cdecl * _f)(InStreamA&)) { (*_f)(*this); return *this; }
	InStreamA&	operator<<(InStreamA&);
	InStreamA&	operator=(InStreamA&);
	InStreamA&	operator<<(const char *p);
	InStreamA&	operator<<(const char p);
	InStreamA&	operator<<(const int p);
	InStreamA&	operator<<(const DWORD p);
	InStreamA&	operator<<(const FLOAT p);

	//inline		InStreamA&	operator<<(const SDWORD p){return operator<<((int)p);}
	//inline		InStreamA&	operator<<(const SWORD p){return operator<<((int)p);}
	inline		InStreamA&	operator<<(const WORD p){return operator<<((DWORD)p);}
	inline		InStreamA&	operator<<(const SBYTE p){return operator<<((int)p);}
	inline		InStreamA&	operator<<(const BYTE p){return operator<<((DWORD)p);}
	// inline		InStreamA&	operator<<(const float p){return operator<<((double)p);}
	// 			InStreamA&	operator<<(const double p);
	InStreamA&	operator<<(const UINT64 p);
	InStreamA&	operator<<(const INT64 p);

	InStreamA&	PutWChar(WCHAR *pWChar);
	InStreamA&	PutWCharToUTF8(WCHAR *pWChar);
	void		Put(const void *p, DWORD len);
	void		PutLONG(LONG c);
	void		PutDWORD(DWORD c);
	void		PutUINT64(UINT64 c);
	void		PutINT64(INT64 c);

	void		PutWORD(WORD c);
	void		PutBYTE(BYTE c);			
	void		OutBYTE() { m_nPos--; }

	void		SetUpdate(char *strTab);
	void		SetWhere();
	void		SetSelect(char *strTab,char *strSeclect);
	void		SetInsert(char *strTab,BOOL bValue = FALSE);
	void		SetDelete(char *strTab);
	void		SetLimit(DWORD num,DWORD begin = 0);
	void		SetSelectField(char *strTab);
	void		SetSelectTables();
	void		SetShowProcessList();

	inline		void		End()		{m_pBuf[m_nPos] = '\0';}
	char		*GetBuf();
	char*		GetLastBuf();
	inline		void		AddPos(DWORD value){m_nPos += value;}
	inline		DWORD		GetBufLen()	{return m_nPos;}
	inline		BOOL		IsEmpty()	{return m_nPos == 0;}
	inline		void		Empty()		{m_nPos = 0;}			

	BOOL		IsLineEnd();

	// 			InStreamA&	PutSQL(BYTE *p,DWORD len);
	// 			InStreamA&	AddSQL(const char *idstr, const void *p, DWORD len);
	// 			InStreamA&	RemoveSQL(const char *idstr, const void *p, DWORD len, InStreamA &Where);
	// 			InStreamA&	ModifySQL(const char *idstr, const void *pOld, DWORD oldlen, const void *pNew, DWORD newlen, InStreamA &Where);
	// 			InStreamA&	LocateSQL(const char *idstr, const void *p, DWORD len);

};


// ****** InStream Unicode Version ****** //
class InStreamW
{
private:
	BYTE		*m_pBuf;
	DWORD		m_nBufLen;
	DWORD		m_nPos;
	DWORD		m_nGrow;
	WCHAR		m_TmpBuf[64];

	void		Grow(DWORD nLen);
public:
	InStreamW(DWORD buflen=64,DWORD growlen=64);
	~InStreamW();

		inline		InStreamW&	operator<<(InStreamW& (__cdecl * _f)(InStreamW&)) { (*_f)(*this); return *this; }
	InStreamW&	operator<<(InStreamW&);
	InStreamW&	operator=(InStreamW&);
	InStreamW&	operator<<(const WCHAR *p);
	InStreamW&	operator<<(const WCHAR p);
	InStreamW&	operator<<(const int p);
	InStreamW&	operator<<(const DWORD p);
	InStreamW&	operator<<(const FLOAT p);

	inline		InStreamW&	operator<<(const WORD p){return operator<<((DWORD)p);}
	inline		InStreamW&	operator<<(const SBYTE p){return operator<<((int)p);}
	inline		InStreamW&	operator<<(const BYTE p){return operator<<((DWORD)p);}

	InStreamW&	operator<<(const UINT64 p);
	InStreamW&	operator<<(const INT64 p);

	InStreamW&	PutChar(char *pChar);
	InStreamW&	PutUTF8Char(char *pChar);
	void		Put(const void *p, DWORD len);
	void		Put_WCHAR(WCHAR *p,DWORD characterLen);
	void		PutLONG(LONG c);
	void		PutDWORD(DWORD c);
	void		PutUINT64(UINT64 c);
	void		PutINT64(INT64 c);

	void		PutWORD(WORD c);
	void		PutBYTE(BYTE c);			
	void		OutBYTE() { m_nPos--; }

	void		SetUpdate(WCHAR *strTab);
	void		SetWhere();
	void		SetSelect(WCHAR *strTab,WCHAR *strSeclect);
	void		SetInsert(WCHAR *strTab,BOOL bValue = FALSE);
	void		SetDelete(WCHAR *strTab);
	void		SetLimit(DWORD num,DWORD begin = 0);
	void		SetSelectField(WCHAR *strTab);
	void		SetSelectTables();
	void		SetShowProcessList();

	inline		void		End()		{m_pBuf[m_nPos] = 0x00; m_pBuf[m_nPos+1] = 0x00;}
	BYTE		*GetBuf();
	BYTE*		GetLastBuf();
	inline		void		AddPos(DWORD value){m_nPos += value;}
	inline		DWORD		GetBufLen()	{return m_nPos;}
	inline		BOOL		IsEmpty()	{return m_nPos == 0;}
	inline		void		Empty()		{m_nPos = 0;}			

	BOOL		IsLineEnd();

};


// ****** OutStream MultiByte Version ****** //
class OutStreamA
{
private:
	BYTE		*m_pData;
	DWORD		m_nPos;
	DWORD		m_nLength;

public:
	OutStreamA(void *buf, DWORD nLen);
	~OutStreamA();

	void		*GetBuf();
	void		*Get(DWORD len);
	void		Get(void *buf, DWORD len);
	DWORD		Get();
	DWORD		GetDWORD();
	WORD		GetWORD();
	SWORD		GetSWORD();
	BYTE		GetBYTE();
	SBYTE		GetSBYTE();
	LONG		GetLONG();
	UINT64		GetUINT64();
	FLOAT		GetFLOAT();

	void		BackDWORD();

	inline		LONG		GetBufLen()	{return m_nLength-m_nPos;}
	inline		void		Reset()	{m_nPos = 0;}
	char		*GetString(DWORD len);
				BOOL		IsEnd();

};


// ****** OutStream Unicode Version ****** //
class OutStreamW
{
private:
	BYTE		*m_pData;
	DWORD		m_nPos;
	DWORD		m_nLength;

public:
	OutStreamW(void *buf, DWORD nLen);
	~OutStreamW();

	void		*GetBuf();
	void		*Get(DWORD len);
	void		Get(void *buf, DWORD len);
	DWORD		Get();
	DWORD		GetDWORD();
	WORD		GetWORD();
	SWORD		GetSWORD();
	BYTE		GetBYTE();
	SBYTE		GetSBYTE();
	LONG		GetLONG();
	UINT64		GetUINT64();
	FLOAT		GetFLOAT();

	void		BackDWORD();

	inline		LONG		GetBufLen()	{return m_nLength-m_nPos;}
	inline		void		Reset()	{m_nPos = 0;}
	WCHAR		*GetString(DWORD len);
				BOOL		IsEnd();

};


#endif