#ifndef PSTRING_H
#define PSTRING_H

class StringA
{
private:
	char	*m_pData;
	DWORD	m_nSize;
	DWORD	m_nMaxSize;
public:

	StringA();
	StringA(const StringA &stringSrc);
	StringA(char *pStr);
	virtual			~StringA();

		void	Free();

	virtual	void	OnEnum(DWORD param1,DWORD param2);

	inline	void	Init(){Grow(4);m_pData[m_nSize] = 0;}
	inline	DWORD	GetLength() const {return m_nSize;}
	inline	void	SetLength(DWORD size){m_nSize=size;}
	inline	BOOL	IsEmpty(){return m_nSize==0;}
	inline	void	Empty(){m_nSize = 0;if(m_pData) m_pData[0] = 0;}
	inline	char	*GetData() const {return m_pData;}

	inline	operator LPCSTR() const {return m_pData;}

	char	operator[](DWORD nIndex);
	const StringA	&operator=(const StringA &stringSrc);
	const StringA	&operator=(char nChar);
	const StringA	&operator=(LPCSTR lpsz);
//	const StringA	&operator=(LPCWSTR lpsz);
	const StringA	&operator+=(const StringA& string);
	const StringA	&operator+=(char ch);
	const StringA	&operator+=(WCHAR wch);
	const StringA	&operator+=(LPCSTR lpsz);

	const StringA	&operator=(DWORD value);
	const StringA	&operator+=(DWORD value);

	StringA	operator+(StringA &string);

	inline	BOOL	operator==(StringA &string){return *this == string.GetData();}
	inline	BOOL	operator>(StringA &string){return *this > string.GetData();}
	inline	BOOL	operator<(StringA &string){return *this < string.GetData();}
	inline	BOOL	operator>=(StringA &string){return *this >= string.GetData();}
	inline	BOOL	operator<=(StringA &string){return *this <= string.GetData();}

	BOOL	operator==(LPCSTR lpsz);
	BOOL	operator>(LPCSTR lpsz);
	BOOL	operator<(LPCSTR lpsz);
	BOOL	operator>=(LPCSTR lpsz);
	BOOL	operator<=(LPCSTR lpsz);


	DWORD	Replace(char chOld, char chNew);
	StringA	Mid(DWORD nFirst,DWORD nCount);
	StringA	Left(DWORD nCount);
	StringA	Right(DWORD nCount);
	BOOL	Find(DWORD nBegin,LPCSTR lpStr);
	int		Find(DWORD begin,char chKey);

protected:
	void	Grow(DWORD nGrowSize);
};

class StringW
{
private:
	WCHAR	*m_pData;
	DWORD	m_nSize;
	DWORD	m_nMaxSize;
public:

	StringW();
	StringW(const StringW &stringSrc);
	StringW(WCHAR *pStr);
	virtual			~StringW();

		void	Free();

	virtual	void	OnEnum(DWORD param1,DWORD param2);

	inline	void	Init(){Grow(4);m_pData[m_nSize] = 0;}
	inline	DWORD	GetLength() const {return m_nSize;}
	inline	void	SetLength(DWORD size){m_nSize=size;}
	inline	BOOL	IsEmpty(){return m_nSize==0;}
	inline	void	Empty(){m_nSize = 0;if(m_pData) m_pData[0] = 0;}
	inline	WCHAR	*GetData() const {return m_pData;}

	inline	operator LPCWSTR() const {return m_pData;}

	WCHAR	operator[](DWORD nIndex);
	const StringW	&operator=(const StringW &stringSrc);
	const StringW	&operator=(WCHAR nChar);
//	const StringW	&operator=(LPCSTR lpsz);
	const StringW	&operator=(LPCWSTR lpsz);
	const StringW	&operator+=(const StringW& string);
	const StringW	&operator+=(char ch);
	const StringW	&operator+=(WCHAR wch);
	const StringW	&operator+=(LPCWSTR lpsz);

	const StringW	&operator=(DWORD value);
	const StringW	&operator+=(DWORD value);

	StringW	operator+(StringW &string);

	inline	BOOL	operator==(StringW &string){return *this == string.GetData();}
	inline	BOOL	operator>(StringW &string){return *this > string.GetData();}
	inline	BOOL	operator<(StringW &string){return *this < string.GetData();}
	inline	BOOL	operator>=(StringW &string){return *this >= string.GetData();}
	inline	BOOL	operator<=(StringW &string){return *this <= string.GetData();}

	BOOL	operator==(LPCWSTR lpsz);
	BOOL	operator>(LPCWSTR lpsz);
	BOOL	operator<(LPCWSTR lpsz);
	BOOL	operator>=(LPCWSTR lpsz);
	BOOL	operator<=(LPCWSTR lpsz);


	DWORD	Replace(WCHAR chOld, WCHAR chNew);
	StringW	Mid(DWORD nFirst,DWORD nCount);
	StringW	Left(DWORD nCount);
	StringW	Right(DWORD nCount);
	BOOL	Find(DWORD nBegin,LPCWSTR lpStr);
	int		Find(DWORD begin,WCHAR chKey);

protected:
	void	Grow(DWORD nGrowSize);
};

template <size_t _Size>
inline	wchar_t *	wcsncpy_array(wchar_t (&_Dest)[_Size], const wchar_t * _Source, size_t _Count)
{
		wchar_t *start = _Dest;
		
		memset(_Dest,0,sizeof(_Dest));
		if(!_Size) return NULL;
		if(!_Source) return NULL;

		size_t copycnt = (_Count<_Size)?_Count:(_Size-1);
		wcsncpy(_Dest,_Source,copycnt);

		return (start);
}

#endif
