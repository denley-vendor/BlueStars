#define LOCK_CPP
#include "../stdafx.h"
#include "Lock.h"
#ifdef _DEBUG
#define LOCK_TIME	INFINITE
#else
#define LOCK_TIME	60000
#endif

ICommon::ICommon()
{
	m_ReferenceCount = 0;
}

ICommon::~ICommon()
{

}

LONG	ICommon::AddRef()
{
	return InterlockedIncrement(&m_ReferenceCount);
}

LONG	ICommon::Release()
{
	return InterlockedDecrement(&m_ReferenceCount);
}
//////////////////////////////////////////////////////////////////////////
LockKernel::LockKernel()
{
		AddRef();
		m_LockCount = 0;
		m_Tick = 0;

		m_bTop = FALSE;
		m_SN = 0;
}

LockKernel::~LockKernel()
{

}

LONG	LockKernel::Release()
{
		LONG r = ICommon::Release();
		if(r <= 0) {
			delete this;
		}
		return r;
}

//////////////////////////////////////////////////////////////////////////
CSLockKernel::CSLockKernel()
{
		InitializeCriticalSection(&m_CS);
		SetCriticalSectionSpinCount(&m_CS,5000);
}

CSLockKernel::~CSLockKernel()
{
		DeleteCriticalSection(&m_CS);
}

void	CSLockKernel::LockWrite()
{
		EnterCriticalSection(&m_CS);

		BeginLock();
}

BOOL	CSLockKernel::TryLockWrite()
{
		if(!TryEnterCriticalSection(&m_CS)) return FALSE;
		BeginLock();
		return TRUE;
}



void	CSLockKernel::UnlockWrite()
{
		m_LockCount --;
		LONG lockcnt = m_LockCount;
		DWORD tick = m_Tick;

		LeaveCriticalSection(&m_CS);

		if(lockcnt == 0) {
			DWORD t = GetTickCount() - tick;
			if(t > 1000) {
				InStreamW err;
				err << _T("cslock time too long ") << t << _T("ms");
			}
		}
		else if(lockcnt < 0) {
			InStreamW err;
			err << _T("cslock count=") << lockcnt;
		}
}

void	CSLockKernel::EndWrite(DWORD msg,BOOL bException)
{
		UnlockWrite();

		if(!bException) {
			InStreamW s;
			s << _T("Not_UnLock msg=") << msg;
		}
}

//////////////////////////////////////////////////////////////////////////
// RWLockKernel::RWLockKernel()
// {
// 		InitializeCriticalSection(&m_CS);
// 		m_ReadCount = 0;
// 		m_hWriteLock = CreateSemaphore(NULL,1,1,NULL);
// 		if(!m_hWriteLock) {
// 			InStreamW s;
// 			s << "CreateSemaphore Failed err =" << GetLastError();
// 			g_Exception.WriteStackLog(s.GetBuf(),LOG_TYPE_SERVER,"RWLock");
// 			RAISEEXCEPTION
// 		}
// }
// 
// RWLockKernel::~RWLockKernel()
// {
// 		DeleteCriticalSection(&m_CS);
// 		CLOSEHANDLE(m_hWriteLock);
// 
// }
// 
// BOOL	RWLockKernel::LockRead()
// {
// 		BOOL bOK = TRUE;
// 		EnterCriticalSection(&m_CS);
// 		m_ReadCount ++;
// 
// 		if(m_ReadCount == 1) {
// 			if(WaitForSingleObject(m_hWriteLock,LOCK_TIME) != WAIT_OBJECT_0) {
// 				bOK = FALSE;
// 				g_Exception.WriteStackLog("ReadLock Failed",LOG_TYPE_SERVER,"RWLock");
// 				RAISEEXCEPTION
// //				RaiseException(EXCEPTION_READ_LOCK_TIME_OUT,EXCEPTION_NONCONTINUABLE,0,NULL);
// 			}
// 		}
// 
// 		LeaveCriticalSection(&m_CS);
// 
// 		if(bOK) {
// 			g_Exception.AddReadLock(this);
// 		}
// 
// 		return bOK;
// }
// 
// BOOL	RWLockKernel::UnlockRead()
// {
// 		BOOL bOK = TRUE;
// 		EnterCriticalSection(&m_CS);
// 		m_ReadCount --;
// 
// 		if(m_ReadCount == 0) {
// 			if(!ReleaseSemaphore(m_hWriteLock,1,NULL)) {
// 				bOK = FALSE;
// 				g_Exception.WriteStackLog("UnReadLock Failed",LOG_TYPE_SERVER,"RWLock");
// 			}
// 		}
// 
// 		LeaveCriticalSection(&m_CS);
// 
// 		if(bOK) {
// 			g_Exception.RemoveReadLock(this);
// 		}
// 
// 		return bOK;
// }
// 
// BOOL	RWLockKernel::LockWrite()
// {
// 
// 		if(WaitForSingleObject(m_hWriteLock,LOCK_TIME) != WAIT_OBJECT_0) {
// 			g_Exception.WriteStackLog("WriteLock Failed",LOG_TYPE_SERVER,"RWLock");
// 			RAISEEXCEPTION
// //			RaiseException(EXCEPTION_WRITE_LOCK_TIME_OUT,EXCEPTION_NONCONTINUABLE,0,NULL);
// 			return FALSE;
// 		}
// 		g_Exception.AddWriteLock(this);
// 		return TRUE;
// }
// 
// BOOL	RWLockKernel::UnlockWrite()
// {
// 		if(!ReleaseSemaphore(m_hWriteLock,1,NULL)) {
// 			g_Exception.WriteStackLog("UnWriteLock Failed",LOG_TYPE_SERVER,"RWLock");
// 			return FALSE;
// 		}
// 		g_Exception.RemoveWriteLock(this);
// 		return TRUE;
// }
// 
// void	RWLockKernel::EndRead(DWORD msg,BOOL bException)
// {
// 		UnlockRead();
// 		if(!bException) {
// 			InStreamW s;
// 			s << "Read_Not_UnLock msg=" << msg;
// 			g_LogManager.WriteLog(s.GetBuf(),LOG_TYPE_SERVER,"RWLock");
// 		}
// }
// 
// void	RWLockKernel::EndWrite(DWORD msg,BOOL bException)
// {
// 		UnlockWrite();
// 		if(!bException) {
// 			InStreamW s;
// 			s << "Write_Not_UnLock msg=" << msg;
// 			g_LogManager.WriteLog(s.GetBuf(),LOG_TYPE_SERVER,"RWLock");
// 		}
// }

//////////////////////////////////////////////////////////////////////////
MutexLockKernel::MutexLockKernel()
{
		m_hMutexLock = CreateMutex(NULL,FALSE,NULL);
		if(!m_hMutexLock) {
			InStreamW s;
			s << _T("CreateMutex Failed err =") << GetLastError();
		}
}

MutexLockKernel::~MutexLockKernel()
{
		CLOSEHANDLE(m_hMutexLock);
}

BOOL	MutexLockKernel::LockWrite()
{

		if(WaitForSingleObject(m_hMutexLock,LOCK_TIME) != WAIT_OBJECT_0) {
			return FALSE;
		}
		BeginLock();

		return TRUE;
}

BOOL	MutexLockKernel::UnlockWrite()
{
		m_LockCount --;
		LONG lockcnt = m_LockCount;
		DWORD tick = m_Tick;

		if(!ReleaseMutex(m_hMutexLock)) {
			return FALSE;
		}

		if(lockcnt == 0) {
			DWORD t = GetTickCount() - tick;
			if(t > 1000) {
				InStreamW err;
				err << _T("mutex lock time too long ") << t << _T("ms");
			}
		}
		else if(lockcnt < 0) {
			InStreamW err;
			err << _T("mutex count=") << lockcnt;
		}


		return TRUE;
}

void	MutexLockKernel::EndWrite(DWORD msg,BOOL bException)
{
		UnlockWrite();
		if(!bException) {
			InStreamW s;
			s << _T("Write_Not_UnLock msg=") << msg;
		}
}
//////////////////////////////////////////////////////////////////////////


CSLock::CSLock()
{
		m_pLockKernel = new CSLockKernel;
}

CSLock::~CSLock()
{
		SAFE_RELEASE(m_pLockKernel);
}

//////////////////////////////////////////////////////////////////////////

// RWLock::RWLock()
// {
// 		m_pLockKernel = new RWLockKernel;
// }
// 
// RWLock::~RWLock()
// {
// 		SAFE_RELEASE(m_pLockKernel);
// }


//////////////////////////////////////////////////////////////////////////

MutexLock::MutexLock()
{
	m_pLockKernel = new MutexLockKernel;
}

MutexLock::~MutexLock()
{
	SAFE_RELEASE(m_pLockKernel);
}

