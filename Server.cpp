#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <list>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8081

std::mutex coutMutex;
std::list<SOCKET> clients;

void HandleThread(SOCKET clientSocket) {
    std::string username;
    char buffer[4096];
    int bytesReceived = 0;

    bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    buffer[bytesReceived] = '\0';
    username = buffer;
    if (bytesReceived > 0) {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Клиент: " << username << " подключился" << std::endl;
    }

    do {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "[" << username << "]: " << buffer << std::endl;
            std::string response = "Получен ответ от сервера: ";
            response += buffer;
            for (auto client : clients) {
                if (client == clientSocket) {
                    continue;
                }
                int sendResult = send(client, buffer, response.size(), 0);
                if (sendResult == SOCKET_ERROR) {
                    std::cout << "send failed" << WSAGetLastError() << std::endl;
                }
            }
        }
        else if (bytesReceived == 0) {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Клиент отключился.\n";
            clients.remove(clientSocket);
            closesocket(clientSocket);
        }
        else {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout << "Клиент отключился.\n";
            clients.remove(clientSocket);
            closesocket(clientSocket);
        }
    } while (bytesReceived > 0);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port \n" << PORT;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            continue;
        }

        clients.push_back(clientSocket);

        std::thread clientThread(HandleThread, clientSocket);
        clientThread.detach();
    }

    WSACleanup();

    std::cout << "Server closed.\n";
    return 0;
}
