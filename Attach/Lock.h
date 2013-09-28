#pragma once
#ifndef LOCK_H
#define LOCK_H

class ICommon
{
protected:
	LONG volatile	m_ReferenceCount;
public:
	ICommon();
	virtual					~ICommon();

	virtual	LONG			AddRef();
	virtual	LONG			Release();
	inline	LONG			GetRef(){return m_ReferenceCount;}

};

class LockKernel : public ICommon
{
protected:
		DWORD		m_Tick;
		LONG		m_LockCount;

		BOOL		m_bTop;
		LONG		m_SN;
public:
					LockKernel();
virtual				~LockKernel();


virtual	LONG		Release();
virtual	void		EndRead(DWORD msg,BOOL bException){};
virtual	void		EndWrite(DWORD msg,BOOL bException){};

inline	void		BeginLock()
{
	if(m_LockCount == 0) {
		m_Tick = GetTickCount();
	}
	m_LockCount ++;
}

inline	BOOL		IsTop(){return m_bTop;}
inline	void		SetTop(BOOL b){m_bTop = b;}

inline	LONG		GetSN(){return m_SN;}
inline	void		SetSN(LONG sn){m_SN = sn;}

};

class CSLockKernel : public LockKernel
{
private:
		CRITICAL_SECTION	m_CS;
public:
					CSLockKernel();
virtual				~CSLockKernel();
		void		LockWrite();
		BOOL		TryLockWrite();
		void		UnlockWrite();

virtual	void		EndWrite(DWORD msg,BOOL bException);
};

// class RWLockKernel : public LockKernel
// {
// private:
// 		CRITICAL_SECTION	m_CS;
// 		LONG		m_ReadCount;
// 
// 		HANDLE		m_hWriteLock;
// public:
// 					RWLockKernel();
// virtual				~RWLockKernel();
// 		BOOL		LockRead();
// 		BOOL		UnlockRead();
// 
// 		BOOL		LockWrite();
// 		BOOL		UnlockWrite();
// 
// virtual	void		EndRead(DWORD msg,BOOL bException);
// virtual	void		EndWrite(DWORD msg,BOOL bException);
// };


class MutexLockKernel : public LockKernel
{
private:
		HANDLE		m_hMutexLock;
public:
					MutexLockKernel();
virtual				~MutexLockKernel();
		BOOL		LockWrite();
		BOOL		UnlockWrite();
virtual	void		EndWrite(DWORD msg,BOOL bException);
};

class CSLock
{
private:
		CSLockKernel	*m_pLockKernel;
public:
					CSLock();
					~CSLock();
inline	CSLockKernel*GetKernel(){return m_pLockKernel;}
inline	void		LockWrite(){m_pLockKernel->LockWrite();}
inline	BOOL		TryLockWrite(){return m_pLockKernel->TryLockWrite();}
inline	void		UnlockWrite(){m_pLockKernel->UnlockWrite();}
inline	void		LockRead(){m_pLockKernel->LockWrite();}
inline	void		UnlockRead(){m_pLockKernel->UnlockWrite();}


inline	void		SetTop(BOOL b){m_pLockKernel->SetTop(b);}
inline	void		SetSN(LONG sn){m_pLockKernel->SetSN(sn);}
};

//////////////////////////////////////////////////////////////////////////

// class RWLock
// {
// private:
// 		RWLockKernel	*m_pLockKernel;
// public:
// 					RWLock();
// 					~RWLock();
// 
// inline	BOOL		LockRead(){return m_pLockKernel->LockRead();}
// inline	BOOL		UnlockRead(){return m_pLockKernel->UnlockRead();}
// 
// inline	BOOL		LockWrite(){return m_pLockKernel->LockWrite();}
// inline	BOOL		UnlockWrite(){return m_pLockKernel->UnlockWrite();}
// };


//////////////////////////////////////////////////////////////////////////

class MutexLock
{
private:
		MutexLockKernel	*m_pLockKernel;
public:
					MutexLock();
					~MutexLock();

inline	BOOL		LockRead(){return m_pLockKernel->LockWrite();}
inline	BOOL		UnlockRead(){return m_pLockKernel->UnlockWrite();}

inline	BOOL		LockWrite(){return m_pLockKernel->LockWrite();}
inline	BOOL		UnlockWrite(){return m_pLockKernel->UnlockWrite();}

inline	void		SetTop(BOOL b){m_pLockKernel->SetTop(b);}
inline	void		SetSN(LONG sn){m_pLockKernel->SetSN(sn);}

};

#endif