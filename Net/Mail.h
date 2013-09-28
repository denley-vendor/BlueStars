#ifndef MAIL_H
#define MAIL_H

#define  SMTP_PORT  25
#define  SMTP_PORT_SSL  465
#define  SMTP_PORT_TLS  587

#define  WM_MAIL_NOTIFY		WM_USER + 222

#include <afxmt.h>
#include "..//WeiBoEngine/Data/Array.h"
#include "..//WeiBoEngine/Data/String.h"
#include "BlockSocket.h"

struct SMTP_INFO
{
		char SMTP_HOST[32];
		char username[32];
		char pwd[32];
		char mailaddr[32];
};

struct MAIL_ADDR
{
		char E_Mail[32];
		char nickname[32];
};

struct MAIL_DATA
{
		StringW   sMailFromNickname;
		StringW   sSubject;
		StringW   sMessage;
		StringW   sMailFilePath;
		Array<MAIL_ADDR> aAddr;
		MAIL_DATA()
		{
			Reset();
		}
		
		void	Reset()
		{
			sMailFromNickname = _T("");
			sSubject = _T("");
			sMessage = _T("");
			sMailFilePath = _T("");
			aAddr.RemoveAll();
		}
		void	Copy(MAIL_DATA* pData)
		{
			if (!pData) return ;
			Reset();
			sMailFromNickname = pData->sMailFromNickname;
			sSubject = pData->sSubject;
			sMessage = pData->sMessage;
			sMailFilePath = pData->sMailFilePath;
			if (pData->aAddr.GetSize()) {
				aAddr.Add(pData->aAddr.GetPoint(0),pData->aAddr.GetSize());
			}
		}
};

class MailClient : public BlockSocket
{
private:
		BOOL		m_bLog;
		BOOL		m_bSSL;
		
protected:
		SMTP_INFO	m_SMTPInfo;
		MAIL_DATA   m_MailData;
		LONG volatile						m_NeedClose;
		CCriticalSection					m_MailLock;
		LONG volatile						m_MailDataCount;
public:
					MailClient();
virtual				~MailClient();
inline		BOOL	IsLogging(){return m_bLog;}
inline		void	SetLogging(BOOL bLog){m_bLog = bLog;}
			BOOL	ConnectSMTP(char* pHostName);
			LONG    AuthLogin();
			BOOL    IsRecvKey(char* pKey);
			BOOL	DoSendMail(MAIL_ADDR* pMailInfo);
			void	ResetData();
			BOOL	InitData(SMTP_INFO* pSMTPInfo,MAIL_DATA* pData);
			BOOL    InitData(SMTP_INFO* pSMTPInfo,char* pTarget,MAIL_DATA* pData);
			LONG	SendMessage();
			LONG    RecvMessage();
			void	ReleasePublic();
			void	QuitSMTP();
			void	CloseAll();

			LONG	MailSend(BOOL bLog = FALSE);		//ÓÊ¼þ·¢ËÍº¯Êý		
			void	WriteLog(TCHAR *pErr);

inline		void	Lock(){m_MailLock.Lock();}
inline		void	Unlock(){m_MailLock.Unlock();}

inline		LONG	GetMailNeedClose(){return m_NeedClose;}
inline		void	SetMailNeedClose(LONG i) {InterlockedExchange(&m_NeedClose,i);}
inline		void	AddMailNeedClose(LONG i) {InterlockedExchangeAdd(&m_NeedClose,i);}

inline		LONG	GetMailDataCount(){return m_MailDataCount;}
inline		void	SetMailDataCount(LONG i) {InterlockedExchange(&m_MailDataCount,i);}
inline		void	AddMailDataCount(LONG i) {InterlockedExchangeAdd(&m_MailDataCount,i);}

			BOOL    ExitThread();
			void	Destroy();
};

#endif
