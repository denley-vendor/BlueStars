#ifndef BLOCKSOCKET_H
#define BLOCKSOCKET_H

#define RECV_BUFFER_LEN 256

class BlockSocket
{
public:
		SOCKET		m_hSocket;

		char		m_RecvBuf[RECV_BUFFER_LEN];

protected:
public:
		InStreamA	m_SendStream;
		InStreamA	m_RecvStream;

					BlockSocket();
virtual				~BlockSocket();

		void		OnEnum(DWORD param1,DWORD param2);
inline  SOCKET		GetSocket(){return m_hSocket;}
		BOOL		IsConnect(){return (m_hSocket != INVALID_SOCKET);}
		void		Disconnect();

		BOOL		Connect(DWORD ip,WORD port);				
		LONG		Send();
		LONG		Recv();

		BOOL		Connect_UnBlock(DWORD ip,WORD port);
};

#endif