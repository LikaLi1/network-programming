#pragma comment (lib,"ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>

#define PORT 8080

void RecieveLoop(SOCKET connectSocket) {
	char messageBuffer[4096];
	int bytesReceived;

	while (true) {
		bytesReceived = recv(connectSocket, messageBuffer, sizeof(messageBuffer), 0);
		if (bytesReceived > 0) {
			messageBuffer[bytesReceived] = '\0';
			std::cout << "Ответ от сервера: " << messageBuffer << std::endl;
		}
		else {
			std::cout << "Соединение закрыто на стороне сервера" << std::endl;
		}
	}
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cout << "Winsock Failed: " << result << std::endl;
		return 1;
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		std::cout << "create socket failed: " << WSAGetLastError() << std::endl;
		return 1;
	}

	sockaddr_in clienInfo;
	clienInfo.sin_family = AF_INET;
	clienInfo.sin_port = htons(PORT);
	inet_pton(AF_INET, "", &clienInfo.sin_addr);

	result = connect(listenSocket, (sockaddr*)&clienInfo, sizeof(clienInfo));

	std::string username;
	do {
		std::cout << "Введите имя: ";
		std::getline(std::cin, username);
	} while (username.empty());
	int usernameSend = send(listenSocket, username.c_str(), username.size(), 0);

	if (usernameSend == SOCKET_ERROR) {
		std::cerr << "send failed: " << WSAGetLastError() << std::endl;
	}

	std::thread recieveLoop(RecieveLoop, listenSocket);
	recieveLoop.detach();

	std::string message;
	std::cout << "Введите сообщение: ";
	while (true) {
		std::getline(std::cin, message);

		if (message == "exit") {
			break;
		}

		int resultSend = send(listenSocket, message.c_str(), message.size(), 0);

		if (resultSend == SOCKET_ERROR) {
			std::cerr << "send failed: " << WSAGetLastError() << std::endl;
		}
	}
	closesocket(listenSocket);
}
