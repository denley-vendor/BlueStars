#pragma once
#ifndef THREADLOCK_H
#define THREADLOCK_H
enum THREAD_LOCK{
	THREAD_LOCKREFRESHPREVIOUSCCONTENT1		= 0x00000001,
	THREAD_LOCK_REFRESH_CCONTENT1			= 0x00000002,
	THREAD_LOCK_TODAYLIGON					= 0x00000004,
	THREAD_LOCK_SETHTMLSTR					= 0x00000008,
	THREAD_LOCK_REFRESH_CONTENT3			= 0x00000010,
	THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS	= 0x00000020,
	THREAD_LOCK_REFRESH_CCONTENT2			= 0x00000040,
	THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS	= 0x00000080,

	THREAD_LOCK_COUNT						= 0x00000008,
};
struct THREAD_TIME
{
	LONG id;
	LONG time;
	LONG limitTime;
};
class TodayThreadLock
{
public:
			TodayThreadLock(void);
			~TodayThreadLock(void);
	void	SetBStart(LONG value,LONG limit = 10);
	BOOL	IsBStart(LONG value);
	void	SetBStop(LONG value);
	void	Remove(LONG value);
private:
	LONG	m_value;
	CArray<LONG> m_arrComp;
	CArray<THREAD_TIME> m_arrTheadTime;

};
extern TodayThreadLock g_TodayThreadLock;
#endif
