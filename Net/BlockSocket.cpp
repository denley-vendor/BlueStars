#define BLOCKSOCKET_C

#include "..//stdafx.h"
#include "BlockSocket.h"

BlockSocket::BlockSocket()
{
		m_hSocket = INVALID_SOCKET;
}

BlockSocket::~BlockSocket()
{

}

void	BlockSocket::Disconnect()
{
		if(m_hSocket == INVALID_SOCKET) return ;
		shutdown(m_hSocket,SD_BOTH);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
}

BOOL	BlockSocket::Connect(DWORD ip,WORD port)
{
		SOCKET hNewSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(hNewSocket == INVALID_SOCKET) {
			Disconnect();
			return FALSE;
		}
		Disconnect();
		
		int timeout = 2000;
		setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));

		m_hSocket = hNewSocket;
		struct sockaddr_in inAddr;
		memset(&inAddr,0,sizeof(sockaddr_in));
		inAddr.sin_addr.S_un.S_addr = ip;
		inAddr.sin_family = AF_INET;
		inAddr.sin_port = htons(port);
		if(connect(m_hSocket,(sockaddr*)&inAddr,sizeof(inAddr)) == SOCKET_ERROR) {
			DWORD error = GetLastError();
			CString str;
			str.Format(_T("%d"),error);
			AfxMessageBox(str);
			return FALSE;
		}

		return TRUE;
}

BOOL    BlockSocket::Connect_UnBlock(DWORD ip,WORD port)
{
		SOCKET hNewSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if(hNewSocket == INVALID_SOCKET) {
			Disconnect();
			return FALSE;
		}		
		Disconnect();
		m_hSocket = hNewSocket;

		int timeout = 2000;
		setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));

		int ret = 0;
		unsigned long ul = 1; 
		ret = ioctlsocket(m_hSocket,FIONBIO,(unsigned long*)&ul); 
		if(ret == SOCKET_ERROR) { 
			Disconnect();
			return FALSE; 
		}

		struct sockaddr_in inAddr;
		inAddr.sin_addr.S_un.S_addr = ip;
		inAddr.sin_family = AF_INET;
		inAddr.sin_port = htons(port);
		ret = connect(m_hSocket,(PSOCKADDR)&inAddr,sizeof(inAddr));

		unsigned long ul1= 0;
		ret = ioctlsocket(m_hSocket,FIONBIO,(unsigned long*)&ul1);
		if(ret == SOCKET_ERROR) { 
			Disconnect();
			return FALSE; 
		} 
		return TRUE;
}

LONG	BlockSocket::Send()
{
		if(m_SendStream.IsEmpty()) return 0;

		BYTE *pBuf = (BYTE*)m_SendStream.GetBuf();
		DWORD msglen = m_SendStream.GetBufLen();

		int sendlen = 0,wantsendlen=msglen;
		DWORD pos = 0;
		while(1) {
			wantsendlen = msglen-pos;
			sendlen = send(m_hSocket,(char*)pBuf+pos,wantsendlen,0);
			if(sendlen > 0) {
				pos += sendlen;
				if(pos == msglen) {
					return pos;
				}
			}
			else {
				Disconnect();
				return sendlen;
			}
		}
		return sendlen;
}

LONG	BlockSocket::Recv()
{
		int recvlen = 0;
		m_RecvStream.Empty();
		while(1) {
			//if (g_Mail.GetMailNeedClose()) return 0;
			recvlen = recv(m_hSocket,m_RecvBuf,RECV_BUFFER_LEN-1,0);
			if(recvlen > 0) {
				m_RecvBuf[recvlen] = 0;
				m_RecvStream << m_RecvBuf;
				if(m_RecvStream.IsLineEnd()) {
					return m_RecvStream.GetBufLen();
				}
			}
			else {
				Disconnect();
				return recvlen;
			}
		}
		return recvlen;
}

void	BlockSocket::OnEnum(DWORD param1,DWORD param2)
{
		if(param1 == 1) delete this;
}