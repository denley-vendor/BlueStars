#ifndef ARRAY_H
#define ARRAY_H

typedef int (CALLBACK *ARRAYFNCOMPARE)(const void *p1, const void *p2,DWORD col,BOOL bDesc);

template<class TYPE> class Array
{
protected:
	TYPE	*m_pData;
	DWORD	m_nSize;
	DWORD	m_nMaxSize;
public:

		Array()
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	~Array()
	{
		//delete [] m_pData;
		free(m_pData);
	}
	inline	DWORD	GetSize(){return m_nSize;}
	inline	void	RemoveAll(){m_nSize=0;}
	inline	TYPE	*GetData() {return m_pData;}
	inline	TYPE	*GetPoint(DWORD nIndex)
	{
		if(nIndex >= m_nSize) {
			ASSERT(FALSE);
				//	RaiseException(EXCEPTION_ARRAY_OVERFLOW,EXCEPTION_NONCONTINUABLE,0,NULL);
		}
		return m_pData+nIndex;
	}

	inline	TYPE	GetAt(DWORD nIndex)
	{
		if(nIndex >= m_nSize) {
			ASSERT(FALSE);
				//				RaiseException(EXCEPTION_ARRAY_OVERFLOW,EXCEPTION_NONCONTINUABLE,0,NULL);
		}
		return m_pData[nIndex];
	}
	inline	void	SetAt(DWORD nIndex,TYPE newElement)
	{
		ASSERT(nIndex < m_nSize);
		if(nIndex >= m_nSize) return ;

		m_pData[nIndex] = newElement;
	}


	inline	TYPE &operator[](DWORD nIndex){return GetAt(nIndex);}

	void	Grow(DWORD nGrowSize)
	{
		DWORD newsize = m_nSize + nGrowSize;
		if(newsize > m_nMaxSize) {
			DWORD grow = (newsize < 40) ? 4 : (newsize/8);
			m_nMaxSize += nGrowSize+grow;

			//TYPE *pNewData = new TYPE[m_nMaxSize];
			newsize = sizeof(TYPE)*m_nMaxSize;
			TYPE *pNewData = (TYPE*)malloc(newsize);
			m_nMaxSize = newsize / sizeof(TYPE);

			if(m_nSize > 0) {
				memcpy(pNewData,m_pData,sizeof(TYPE)*m_nSize);
			}
			//				delete [] m_pData;
			free(m_pData);
			m_pData = pNewData;
		}
	}
	void	Add(TYPE *pNewElement,DWORD num)
	{
		if(num) {
			Grow(num);
			memcpy(m_pData+m_nSize,pNewElement,sizeof(TYPE)*num);
			m_nSize += num;
		}
	}
	void	SetSize(DWORD nSize)
	{
		if(nSize > m_nMaxSize) {
			m_nMaxSize = nSize;
			//				TYPE *pNewData = new TYPE[m_nMaxSize];
			DWORD maxbuffsize = sizeof(TYPE)*m_nMaxSize;
			TYPE *pNewData = (TYPE*)malloc(maxbuffsize);
			m_nMaxSize = maxbuffsize / sizeof(TYPE);

			if(m_nSize > 0) {
				memcpy(pNewData,m_pData,sizeof(TYPE)*m_nSize);
			}
			//delete [] m_pData;
			free(m_pData);

			m_pData = pNewData;
		}
	}
	void	Add(TYPE newElement)
	{
		Grow(1);
		memcpy(m_pData+m_nSize,&newElement,sizeof(TYPE));
		m_nSize++;
		//			m_pData[m_nSize++] = newElement;
	}
	void	InsertAt(DWORD nIndex, TYPE newElement)
	{		
		ASSERT(nIndex <= m_nSize);
		if(nIndex <= m_nSize) {
			Grow(1);
			if(m_nSize > nIndex)
				memmove(m_pData+nIndex+1,m_pData+nIndex,(m_nSize-nIndex) * sizeof(TYPE));
			m_pData[nIndex] = newElement;
			m_nSize ++;
		}
	}
	BOOL	RemoveAt(DWORD nIndex,DWORD num=1)
	{
		if(nIndex+num > m_nSize) return FALSE;
		if(m_nSize > 0 && m_nSize > nIndex+num) {
			memmove(m_pData+nIndex,m_pData+nIndex+num,(m_nSize - nIndex - num)*sizeof(TYPE));
		}
		m_nSize -= num;
		return TRUE;
	}

	TYPE	RemoveLast()
	{
		if(!m_nSize) return 0;
		m_nSize --;
		return m_pData[m_nSize];
	}

	BOOL	Remove(TYPE element)
	{
		for(DWORD i = 0; i < m_nSize; i ++) {
			if(m_pData[i] == element) {
				m_nSize --;
				if(m_nSize > 0 && m_nSize > i) {
					memmove(m_pData+i,m_pData+i+1,(m_nSize - i)*sizeof(TYPE));
				}
				return TRUE;
			}
		}
		return FALSE;
	}

	void	Swap(DWORD i,DWORD j)
	{
		if(i == j) return ;
		TYPE t;
		memcpy(&t,m_pData+i,sizeof(t));
		memcpy(m_pData+i,m_pData+j,sizeof(t));
		memcpy(m_pData+j,&t,sizeof(t));
	}

	void	Sort(ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		DWORD nCount = GetSize();
		if(nCount < 2) return ;
		QuickSort(0,GetSize()-1,lpfnCompare,col,bDesc);
	}
	void	QuickSort(DWORD nLeft,DWORD nRight,ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		if(nLeft >= nRight) return;
		if(nRight - nLeft <= 8) {
			InsertSort(nLeft,nRight,lpfnCompare,col,bDesc);
			return ;
		}

		long i,j;
		i=nLeft; j=nRight;

		TYPE Pivot;
		memcpy(&Pivot,m_pData+i,sizeof(TYPE));

		while(i<j)
		{
			while((*lpfnCompare)(&Pivot,GetPoint(j),col,bDesc)<=0 && i<j)j--;
			if(i != j) {
				memcpy(m_pData+i,m_pData+j,sizeof(TYPE));
			}
			while((*lpfnCompare)(&Pivot,GetPoint(i),col,bDesc)>=0 && i<j)i++;
			if(i != j) {
				memcpy(m_pData+j,m_pData+i,sizeof(TYPE));
			}
		}
		memcpy(m_pData+i,&Pivot,sizeof(TYPE));
		if(i-nLeft>1) QuickSort(nLeft,i-1,lpfnCompare,col,bDesc);
		if(nRight-j>1) QuickSort(j+1,nRight,lpfnCompare,col,bDesc);

	}
	void	InsertSort(DWORD nLeft,DWORD nRight,ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		TYPE Begin;
		for(DWORD i = nLeft+1; i <= nRight; i++) {
			memcpy(&Begin,m_pData+i,sizeof(TYPE));
			TYPE Tmp;
			BOOL b = FALSE;
			DWORD index = 0;
			for(DWORD j = i; j >= nLeft+1; j--) {
				if((*lpfnCompare)(&Begin,GetPoint(j-1),col,bDesc) < 0) {
					if(!b) {
						memcpy(&Tmp,m_pData+j,sizeof(TYPE));
						b = TRUE;
					}
					index = j-1;
					memcpy(m_pData+j,m_pData+index,sizeof(TYPE));
				}
			}
			if(b) {
				memcpy(m_pData+index,&Tmp,sizeof(TYPE));
			}
		}
	}
};



template<class TYPE> class ObjectArray
{
protected:
	TYPE	*m_pData;
	DWORD	m_nSize;
	DWORD	m_nMaxSize;
public:

		ObjectArray()
	{
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}
	~ObjectArray()
	{
		for(DWORD i = 0; i < m_nSize; i ++) {
			delete m_pData[i];
		}
		//delete [] (BYTE*)m_pData;
		free(m_pData);

	}
	inline	DWORD		GetSize(){return m_nSize;}
	inline	void		RemoveAllWithoutFree(){m_nSize=0;}
	void		RemoveAll()
	{
		for(DWORD i = 0; i < m_nSize; i ++) {
			delete m_pData[i];
		}
		m_nSize=0;
	}
	inline	TYPE	*GetData() {return m_pData;}
	inline	TYPE	GetAt(DWORD nIndex)
	{
		if(nIndex >= m_nSize) {
			ASSERT(FALSE);
				//RaiseException(EXCEPTION_ARRAY_OVERFLOW,EXCEPTION_NONCONTINUABLE,0,NULL);
		}
		return m_pData[nIndex];
	}
	inline	void	SetAt(DWORD nIndex, TYPE newElement)
	{
		ASSERT(nIndex < m_nSize);
		if(nIndex >= m_nSize) return ;

		m_pData[nIndex] = newElement;
	}


	inline	TYPE &operator[](DWORD nIndex){return GetAt(nIndex);}

	void	Grow(DWORD nGrowSize)
	{
		DWORD newsize = m_nSize + nGrowSize;
		if(newsize > m_nMaxSize) {
			DWORD grow = (newsize < 40) ? 4 : (newsize/8);
			m_nMaxSize += grow;
			//				TYPE *pNewData = new TYPE[m_nMaxSize];

			newsize = sizeof(TYPE)*m_nMaxSize;
			TYPE *pNewData = (TYPE*)malloc(newsize);
			m_nMaxSize = newsize / sizeof(TYPE);

			if(m_nSize > 0) {
				memcpy(pNewData,m_pData,sizeof(TYPE)*m_nSize);
			}
			//delete [] (BYTE*)m_pData;
			free(m_pData);

			m_pData = pNewData;
		}
	}
	void	SetSize(DWORD nSize)
	{
		if(nSize > m_nMaxSize) {
			m_nMaxSize = nSize;
			DWORD maxbuffsize = sizeof(TYPE)*m_nMaxSize;
			TYPE *pNewData = (TYPE*)malloc(maxbuffsize);
			m_nMaxSize = maxbuffsize / sizeof(TYPE);

			if(m_nSize > 0) {
				memcpy(pNewData,m_pData,sizeof(TYPE)*m_nSize);
			}
			//delete [] (BYTE*)m_pData;
			free(m_pData);

			m_pData = pNewData;
		}
	}


	void	Add(TYPE newElement)
	{
		Grow(1);
		m_pData[m_nSize++] = newElement;
	}
	void	InsertAt(DWORD nIndex,TYPE newElement)
	{
		if(nIndex <= m_nSize) {
			Grow(1);
			if(m_nSize > nIndex)
				memmove(m_pData+nIndex+1,m_pData+nIndex,(m_nSize-nIndex) * sizeof(TYPE));
			m_pData[nIndex] = newElement;
			m_nSize ++;
		}
	}

	BOOL	Remove(TYPE element)
	{
		for(DWORD i = 0; i < m_nSize; i ++) {
			if(m_pData[i] == element) {
				delete m_pData[i];
				m_nSize --;
				if(m_nSize > 0 && m_nSize > i) {
					memmove(m_pData+i,m_pData+i+1,(m_nSize - i)*sizeof(TYPE));
				}
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL	RemoveAt(DWORD nIndex,BOOL bFree = TRUE)
	{
		ASSERT(nIndex < m_nSize);
		if(nIndex >= m_nSize) return FALSE;
		if(bFree) delete m_pData[nIndex];
		m_nSize --;
		if(m_nSize > 0 && m_nSize > nIndex) {
			memmove(m_pData+nIndex,m_pData+nIndex+1,(m_nSize - nIndex)*sizeof(TYPE));
		}
		return TRUE;
	}
	void	Swap(DWORD i,DWORD j)
	{
		if(i == j) return ;
		TYPE t = m_pData[i];
		m_pData[i] = m_pData[j];
		m_pData[j] = t;
	}

	void	Sort(ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		DWORD nCount = GetSize();
		if(nCount < 2) return ;
		QuickSort(0,GetSize()-1,lpfnCompare,col,bDesc);
	}
	void	QuickSort(DWORD nLeft,DWORD nRight,ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		if(nLeft >= nRight) return;
		if(nRight - nLeft <= 8) {
			InsertSort(nLeft,nRight,lpfnCompare,col,bDesc);
			return ;
		}

		long i,j;
		i=nLeft; j=nRight;
		TYPE Pivot = GetAt(i);
		while(i<j)
		{
			while((*lpfnCompare)(Pivot,GetAt(j),col,bDesc)<=0 && i<j)j--;
			if(i != j) SetAt(i,GetAt(j));
			while((*lpfnCompare)(Pivot,GetAt(i),col,bDesc)>=0 && i<j)i++;
			if(i != j) SetAt(j,GetAt(i));
		}
		SetAt(i,Pivot);
		if(i-nLeft>1) QuickSort(nLeft,i-1,lpfnCompare,col,bDesc);
		if(nRight-j>1) QuickSort(j+1,nRight,lpfnCompare,col,bDesc);

	}
	void	InsertSort(DWORD nLeft,DWORD nRight,ARRAYFNCOMPARE lpfnCompare,DWORD col,BOOL bDesc)
	{
		TYPE Begin = NULL;
		for(DWORD i = nLeft+1; i <= nRight; i++) {
			Begin = GetAt(i);
			TYPE pTmp = NULL;
			DWORD index = 0;
			for(DWORD j = i; j >= nLeft+1; j--) {
				if((*lpfnCompare)(Begin,GetAt(j-1),col,bDesc) < 0) {
					if(!pTmp) pTmp = GetAt(j);
					index = j-1;
					SetAt(j,GetAt(index));
				}
			}
			if(pTmp) SetAt(index,pTmp);
		}
	}
};

typedef Array<LONG>		LArray;
typedef Array<DWORD>	DArray;
typedef Array<WORD>		WArray;
typedef Array<BYTE>		BArray;

#endif
