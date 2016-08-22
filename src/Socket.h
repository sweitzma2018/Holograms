#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <opencv2/core.hpp>

#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <stdint.h>
	#include <ws2tcpip.h>
	#pragma comment (lib, "Ws2_32.lib")
	#pragma comment (lib, "Mswsock.lib")
	#pragma comment (lib, "AdvApi32.lib")
#else
	#define SOCKET int
	#include "stdint.h"
	#include <unistd.h>
	#include <errno.h>
	#include <string.h>
	#include <netdb.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
#endif


class Socket {
public:

	Socket(const std::string &serverIP, const std::string &serverPort);
	~Socket();

	bool isConnected();
	void sendMessage(std::string);
	std::string receiveMessage();
	cv::Mat receiveImage();
	

private:

	SOCKET _socketFD;

};


#endif 
