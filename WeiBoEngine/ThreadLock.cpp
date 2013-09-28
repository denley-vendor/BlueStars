/* 作者:王家帅
   个人博客:http://lanxingxing.net/
   谢谢您能来关注我的网站:http://shanpao.info/ 山炮网,写下你最讨厌的人
*/
#define THREADLOACK_CPP
#include "..//stdafx.h"
#include "ThreadLock.h"

TodayThreadLock::TodayThreadLock(void)
{
		m_value = 0;
		m_arrComp.RemoveAll();
		m_arrComp.Add(THREAD_LOCKREFRESHPREVIOUSCCONTENT1);
		m_arrComp.Add(THREAD_LOCK_REFRESH_CCONTENT1);
		m_arrComp.Add(THREAD_LOCK_TODAYLIGON);

		m_arrComp.Add(THREAD_LOCK_SETHTMLSTR);
		m_arrComp.Add(THREAD_LOCK_REFRESH_CONTENT3);
		m_arrComp.Add(THREAD_LOCK_REFRESH_CONTENT3_PREVIOUS);
		m_arrComp.Add(THREAD_LOCK_REFRESH_CCONTENT2);
		m_arrComp.Add(THREAD_LOCK_REFRESH_CONTENT2_PREVIOUS);
}

TodayThreadLock::~TodayThreadLock(void)
{
		m_arrComp.RemoveAll();
		m_arrTheadTime.RemoveAll();
}

void	TodayThreadLock::SetBStart(LONG value,LONG limit /* = 10 */)
{
		if(!(m_value & value)){
			m_value |= value;

			CTime time = CTime::GetCurrentTime();
			THREAD_TIME sTime;
			sTime.id = value;
			sTime.limitTime = limit;
			sTime.time = time.GetTime();
			m_arrTheadTime.Add(sTime);
		}
}

BOOL	TodayThreadLock::IsBStart(LONG value)
{
		DWORD count = m_arrTheadTime.GetSize();
		THREAD_TIME sTime;
		LONG lTime = 0;
		for(DWORD i = 0; i < count; i ++){
			sTime = m_arrTheadTime.GetAt(i);
			if(sTime.id & value){
				CTime time = CTime::GetCurrentTime();
				lTime = time.GetTime() - sTime.time;
				if(lTime < sTime.limitTime) return TRUE;

				m_arrTheadTime.RemoveAt(i);
				Remove(value);
				break;
			}
		}

		BOOL b = FALSE;
		if(m_value & value){
			b = TRUE;
		}
		return b;
}

void	TodayThreadLock::SetBStop(LONG value)
{
		DWORD count = m_arrTheadTime.GetSize();
		THREAD_TIME sTime;
		LONG lTime = 0;
		for(DWORD i = 0; i < count; i ++){
			sTime = m_arrTheadTime.GetAt(i);
			if(sTime.id & value){
				CTime time = CTime::GetCurrentTime();
				lTime = time.GetTime() - sTime.time;
				if(lTime < sTime.limitTime) return;

				m_arrTheadTime.RemoveAt(i);
				break;
			}
		}
		if(m_value & value){
			//TRACE("Thread Info: Thread Lock:SetBStop ~ \n");
			Remove(value);
		}
}

void	TodayThreadLock::Remove(LONG value)
{
		LONG tempValue = 0;
		LONG value1 = 0;
		DWORD count = m_arrComp.GetSize();
		for(DWORD i = 0; i < count; i ++){
			tempValue = m_arrComp.GetAt(i);
			if(tempValue & value) continue;

			if(tempValue & m_value){
				value1 |= tempValue;
			}
		}

		//TRACE("Thread Info: Thread Lock:Remove(value)  \n");
		m_value = value1;
}

TodayThreadLock g_TodayThreadLock;