#define MAIL_C

#include "..//stdafx.h"
#include "Mail.h"

const char BOUNDARY_TEXT[] = "__MESSAGE__ID__54yg6f6h6y456345";

DWORD   CALLBACK_GetCurrentThreadID()
{
		return ::GetCurrentThreadId();
}

MailClient::MailClient()
{
		ResetData();
}

MailClient::~MailClient()
{

}

void	MailClient::Destroy()
{
}

void	MailClient::ResetData()
{
		m_bLog = FALSE;
		memset(&m_SMTPInfo,0,sizeof(m_SMTPInfo));
		m_MailData.Reset();
		m_bSSL = FALSE;
		m_MailDataCount = 0;
		m_NeedClose = 0;
}

BOOL	MailClient::InitData(SMTP_INFO* pSMTPInfo,MAIL_DATA* pData)
{
	    InitData(pSMTPInfo,"410929706@qq.com",pData);
		return TRUE;
}
BOOL    MailClient::InitData(SMTP_INFO* pSMTPInfo,char* pTarget,MAIL_DATA* pData)
{
	ResetData();
	SMTP_INFO info;
	if (!pSMTPInfo) {
		strncpy_s(info.SMTP_HOST,"smtp.sina.com",sizeof(info.SMTP_HOST));
		strncpy_s(info.username,"aknnn",sizeof(info.username));
		strncpy_s(info.pwd,"4563882",sizeof(info.pwd));
		strncpy_s(info.mailaddr,"aknnn@sina.com",sizeof(info.mailaddr));
		pSMTPInfo = &info;
	}
	memcpy(&m_SMTPInfo,pSMTPInfo,sizeof(SMTP_INFO));

	m_MailData.Copy(pData);	
	if (!m_MailData.aAddr.GetSize()) {
		MAIL_ADDR addr;
		memset(&addr,0,sizeof(MAIL_ADDR));
		strncpy_s(addr.E_Mail,pTarget,sizeof(addr.E_Mail));
		strncpy_s(addr.nickname,"-_-",sizeof(addr.nickname));
		m_MailData.aAddr.Add(&addr,1);
	}
	return TRUE;
}
void	MailClient::WriteLog(TCHAR *pErr)
{
		if (!m_bLog) return ;
}
BOOL	MailClient::ConnectSMTP(char* pHostName)
{
		InStreamW isa;
		isa.PutChar(pHostName);
		InStream isErr;
		DWORD dwip = 0;		
		isErr.Empty();
		isErr << _T("域名解析: ") << isa;
		WriteLog((WCHAR*)isErr.GetBuf());
		HOSTENT *pHost = gethostbyname(pHostName);
		if(!pHost) {
			isErr << _T("失败！");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		else {
			memcpy(&dwip,pHost->h_addr_list[0],pHost->h_length);
		}
		isErr.Empty();
		isErr << _T("Connect SMTP: ") << SMTP_PORT;
		WriteLog((WCHAR*)isErr.GetBuf());
		if (!Connect(dwip,SMTP_PORT)) {
			isErr << _T("Failed!");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		LONG recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			QuitSMTP();
			isErr << _T(" | recvLen <= 0 | QUIT");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		if (strncmp(m_RecvStream.GetBuf(),"220",3)) {
			QuitSMTP();
			return FALSE;
		}

		return TRUE;
}

LONG	MailClient::SendMessage()
{
		return Send();
		return 0;
}

LONG    MailClient::RecvMessage()
{		
		LONG recvlen = 0;
		m_RecvStream.Empty();
		recvlen = Recv();
		return recvlen;
}

void	MailClient::CloseAll()
{
		QuitSMTP();
}

void	MailClient::QuitSMTP()
{
		m_SendStream.Empty();
		m_SendStream << "QUIT\r\n";
		SendMessage();
		InStream isErr;
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
}

BOOL    MailClient::IsRecvKey(char* pKey)
{
		if (!pKey || strlen(pKey) != 3) return FALSE;
		if (strncmp(m_RecvStream.GetBuf(),pKey,3)) return FALSE;
		return TRUE;
}

LONG    MailClient::AuthLogin()
{
		InStream isErr;
		LONG sendLen = 0;
		LONG recvLen = 0;
		if (!ConnectSMTP(m_SMTPInfo.SMTP_HOST)) return 0;
		//EHLO 
		m_SendStream.Empty();
		m_SendStream << "EHLO " << m_SMTPInfo.SMTP_HOST << "\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		if (!IsRecvKey("250")) {
			isErr << _T(" | Not 250");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;			
		}
		//TEST TLS/SSL
		BOOL bSSL = FALSE;
		m_SendStream.Empty();
		m_SendStream << "AUTH LOGIN" << "\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		if (IsRecvKey("250")) {
			isErr << _T(" | Not 250");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		if (IsRecvKey("530")) {
			bSSL = TRUE;
			isErr << _T(" | 530 | Use SSL");
			WriteLog((WCHAR*)isErr.GetBuf());
		}
		else if (IsRecvKey("334")) {
			isErr << _T(" | 334 | OK");
			WriteLog((WCHAR*)isErr.GetBuf());
		}
		else {
			QuitSMTP();
			return 0;
		}
		if (bSSL) {
			m_SendStream.Empty();
			m_SendStream << "STARTTLS" << "\r\n";
			sendLen = SendMessage();
			isErr.Empty();
			isErr.PutChar(m_SendStream.GetBuf());
			WriteLog((WCHAR*)isErr.GetBuf());
			if (sendLen <= 0) {
				isErr << _T(" | sendLen <= 0");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
			recvLen = RecvMessage();
			isErr.Empty();
			isErr.PutChar(m_RecvStream.GetBuf());
			WriteLog((WCHAR*)isErr.GetBuf());
			if (recvLen <= 0) {
				isErr << _T(" | recvLen <= 0");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
			if (!IsRecvKey("220")) {
				isErr << _T(" | Not 220");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
			//AUTH LOGON
			m_SendStream.Empty();
			m_SendStream << "AUTH LOGIN" << "\r\n";
			sendLen = SendMessage();
			isErr.Empty();
			isErr.PutChar(m_SendStream.GetBuf());
			WriteLog((WCHAR*)isErr.GetBuf());
			if (sendLen <= 0) {
				isErr << _T(" | sendLen <= 0");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
			recvLen = RecvMessage();
			isErr.Empty();
			isErr.PutChar(m_RecvStream.GetBuf());
			WriteLog((WCHAR*)isErr.GetBuf());
			if (recvLen <= 0) {
				isErr << _T(" | recvLen <= 0");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
			if (!IsRecvKey("334")) {
				isErr << _T(" | Not 334");
				WriteLog((WCHAR*)isErr.GetBuf());
				QuitSMTP();
				return 0;
			}
		}
		char destUsername[64];
		char destPwd[64];
		memset(destUsername,0,sizeof(destUsername));
		memset(destPwd,0,sizeof(destPwd));
		//USER
		int destUsername_len = 64;
		Base64Encode((BYTE*)m_SMTPInfo.username,strlen(m_SMTPInfo.username),destUsername,&destUsername_len);
		m_SendStream.Empty();
		m_SendStream << destUsername << "\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		//PWD
		int destPwd_len = 64;
		Base64Encode((BYTE*)m_SMTPInfo.pwd,strlen(m_SMTPInfo.pwd),destPwd,&destPwd_len);
		m_SendStream.Empty();
		m_SendStream << destPwd << "\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;
		}
		//LOGON RESULT
		if (!IsRecvKey("235")) {
			isErr << _T(" | Not 235");
			WriteLog((WCHAR*)isErr.GetBuf());
			QuitSMTP();
			return 0;			
		}
		WriteLog(_T("AUTH LOGON OK"));
		return 1;
}

LONG	MailClient::MailSend(BOOL bLog)
{
		if (!AuthLogin()) {
			CloseAll();
			return 0;
		}
		DWORD count = 0;
		MAIL_ADDR* pAddr = NULL;
		DWORD num = m_MailData.aAddr.GetSize();
		for (DWORD i = 0; i < num; i++) {
			pAddr = m_MailData.aAddr.GetPoint(i);
			if (!DoSendMail(pAddr)) continue;
			count++;
		}
		CloseAll();
		return count;		
}

BOOL	MailClient::DoSendMail(MAIL_ADDR* pMailInfo)
{
		WriteLog(_T("MAIL START."));
		InStream isErr;
		//SINGLE MAIL START
		LONG sendLen = 0;
		LONG recvLen = 0;
		//MAIL FROM
		m_SendStream.Empty();
		m_SendStream << "MAIL FROM:<";
		m_SendStream << m_SMTPInfo.mailaddr;
		m_SendStream << ">\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		if (!IsRecvKey("250")) {
			isErr << _T(" | Not 250");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		//RCPT TO
		m_SendStream.Empty();
		m_SendStream << "RCPT TO:<";
		m_SendStream << pMailInfo->E_Mail;
		m_SendStream << ">\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		if (!IsRecvKey("250")) {
			isErr << _T(" | Not 250");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		//DATA
		m_SendStream.Empty();
		m_SendStream << "DATA\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (sendLen <= 0) {
			isErr << _T(" | sendLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		if (!IsRecvKey("354")) {
			isErr << _T(" | Not 354");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}		
		//HEADER
		m_SendStream.Empty();
		InStreamA tmpStream;
		char szHeader[500];
		char szDate[500];
		char sztTime[500];
		SYSTEMTIME st={0};
		::GetSystemTime(&st);
		::GetDateFormatA(MAKELCID(0x0409,SORT_DEFAULT),0,&st,"ddd\',\' dd MMM yyyy",szDate,sizeof(szDate));
		::GetTimeFormatA(MAKELCID(0x0409,SORT_DEFAULT),TIME_FORCE24HOURFORMAT,&st,"HH\':\'mm\':\'ss",sztTime,sizeof(sztTime));
		sprintf(szHeader,"Date: %s %s\r\n", szDate, sztTime); 
		m_SendStream << szHeader;
		m_SendStream << "From:" << m_SMTPInfo.username << "<";
		m_SendStream << m_SMTPInfo.mailaddr;
		m_SendStream << ">\r\n";
		m_SendStream << "To:";
		m_SendStream << pMailInfo->E_Mail;
		m_SendStream << "\r\n";
		m_SendStream << "Subject:";  //  SUBJECT
		tmpStream.Empty();
		tmpStream.PutWChar(m_MailData.sSubject.GetData());
		m_SendStream << tmpStream;
		m_SendStream << "\r\n";
		m_SendStream << "MIME_Version:1.0\r\n";
		m_SendStream << "Content-Type: multipart/mixed;";
		m_SendStream << "boundary=\"" << BOUNDARY_TEXT << "\"\r\n";
		m_SendStream << "\r\n";
		m_SendStream << "--" << BOUNDARY_TEXT << "\r\n";
		m_SendStream << "Content-Type: text/plain;charset=utf-8\r\n";
		m_SendStream << "Content-Transfer-Encoding:8bit\r\n";
		m_SendStream << "\r\n";
		sendLen = SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		WriteLog(_T("Start Trans."));

		m_SendStream.Empty();
		tmpStream.Empty();
		tmpStream.PutWCharToUTF8(m_MailData.sMessage.GetData());
		m_SendStream << tmpStream << "\r\n";
		sendLen = SendMessage();

		WCHAR filename[FILENAME_MAX];
		memset(filename,0,sizeof(filename));
		InStreamA InSFileName;
		WCHAR* p = NULL;			
		char* pBuf = NULL;
		char* pNewBuf = NULL;

		int filepathlen = m_MailData.sMailFilePath.GetLength();
		if (filepathlen) {	
			//single attachment
			CString csTemp = m_MailData.sMailFilePath;
			int cspos = csTemp.ReverseFind(_T('\\'));
			csTemp = csTemp.Right(csTemp.GetLength()-cspos);
			p = wcschr(csTemp.GetBuffer(0),_T('\\'));
			if (!p) {
				isErr.Empty();
				isErr << m_MailData.sMailFilePath.GetData();
				isErr << _T(" | Can't Find '\\'");
				WriteLog((WCHAR*)isErr.GetBuf());
				return FALSE;
			}
			p++;
			wcsncpy_array(filename,p,FILENAME_MAX-1);
			InSFileName.Empty();
			InSFileName.PutWCharToUTF8(filename);

			m_SendStream.Empty();
			m_SendStream << "--" << BOUNDARY_TEXT << "\r\n";
			m_SendStream << "Content-Type: application/x-msdownload; name=\"" << InSFileName << "\"\r\n";
			m_SendStream << "Content-Transfer-Encoding: base64\r\n";
			m_SendStream << "Content-Disposition: attachment; filename=\"" << InSFileName << "\"\r\n";
			m_SendStream << "\r\n";
			sendLen = SendMessage();

			int readpartlen = 54;
			int partlen = 5120;	

			CFile file;
			CFileException e;
			if(!file.Open(m_MailData.sMailFilePath.GetData(),CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary,&e)) {
				e.ReportError();
				isErr.Empty();
				isErr << m_MailData.sMailFilePath.GetData();
				isErr << _T(" | Can't Open File.");
				WriteLog((WCHAR*)isErr.GetBuf());
				return FALSE;
			}
			pBuf = new char[readpartlen+1];			
			pNewBuf = new char[readpartlen*2+1];
			UINT64 fileLen = file.GetLength();
			UINT64 lastLen = 0;
			int destlen = 0;
			int res = 0;
			for (UINT64 j = 0; j < fileLen/readpartlen+1; j++) {
				memset(pBuf,0,readpartlen+1);
				res = file.Read(pBuf,readpartlen);
				memset(pNewBuf,0,readpartlen*2+1);
				destlen = readpartlen*2;
				Base64Encode((BYTE*)pBuf,readpartlen,pNewBuf,&destlen);
				if (!lastLen) {
					m_SendStream.Empty();
				}
				m_SendStream.Put(pNewBuf,destlen);
				m_SendStream << "\r\n";
				lastLen += res + 2;
				if(lastLen >= partlen) {
					lastLen = 0;
					sendLen = SendMessage();
				}
			}
			if (lastLen) {
				sendLen = SendMessage();
			}
			file.Close();

			SAFE_DELETE_ARRAY(pBuf);
			SAFE_DELETE_ARRAY(pNewBuf);

// 			ifstream handle;
// 			handle.open(m_MailData.sMailFilePath.GetData(),ios::in|ios::binary);
// 			if(!handle.good()) return FALSE;
// 			handle.seekg(0,ios::end);
// 			int len = handle.tellg();
// 			handle.seekg(0,ios::beg);
// 			pBuf = new char[len];
// 			pNewBuf = new char[len*2];
// 			handle.read(pBuf,len);
// 			handle.close();
// 			int destlen = g_Base64.EncodeBase64((const unsigned char*)(LPCTSTR)pBuf,pNewBuf,len,len * 2);
// 
// 			char *pPos = NULL;
// 			int partcount = 0;
// 			int tmplen = 0;
// 			int tempdata = 0;
// 			int partlen = 5120;			
// 			BOOL bBreak = FALSE;
// 			if (partlen > destlen) {
// 				partlen = destlen;
// 				bBreak = TRUE;
// 			}
// 			while (1) {
// 				tmplen = partlen;
// 				tempdata = partcount * partlen;
// 				if (tempdata >= destlen) {
// 					tmplen = tempdata - destlen;
// 					bBreak = TRUE;
// 				}
// 				pPos = pNewBuf + partcount * partlen;
// 				m_SendStream.Empty();
// 				m_SendStream.Put(pPos,tmplen);
// 				m_SendStream << _T("\r\n");
// 				sendLen = SendMessage();
// 				partcount++;
// 				if (bBreak) break;
// 			}			

			m_SendStream.Empty();
			m_SendStream << "\r\n--" << BOUNDARY_TEXT << "--\r\n";
			sendLen = SendMessage();
		}

		m_SendStream.Empty();
		m_SendStream << "\r\n.\r\n";
		SendMessage();
		isErr.Empty();
		isErr.PutChar(m_SendStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		WriteLog(_T("End Trans."));
		recvLen = RecvMessage();
		isErr.Empty();
		isErr.PutChar(m_RecvStream.GetBuf());
		WriteLog((WCHAR*)isErr.GetBuf());
		if (recvLen <= 0) {
//			AfxMessageBox(m_RecvStream.GetBuf());
			isErr << _T(" | recvLen <= 0");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		if (!IsRecvKey("250")) {
			isErr << _T(" | Not 250");
			WriteLog((WCHAR*)isErr.GetBuf());
			return FALSE;
		}
		WriteLog(_T("MAIL FINISH."));
		Sleep(500);
		return TRUE;
}
