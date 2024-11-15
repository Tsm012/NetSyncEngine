#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#pragma comment(lib, "Ws2_32.lib")

void handle_client(SOCKET client_socket) {
    char buffer[256];
    int result;
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        result = recv(client_socket, buffer, sizeof(buffer), 0);
        if (result <= 0) {
            std::cerr << "Error reading from socket or client disconnected" << std::endl;
            break;
        }
        std::cout << "Received message: " << buffer << std::endl;

        result = send(client_socket, buffer, strlen(buffer), 0);
        if (result == SOCKET_ERROR) {
            std::cerr << "Error writing to socket" << std::endl;
            break;
        }
    }
    closesocket(client_socket);
}


int run() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return EXIT_FAILURE;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    std::cout << "Server listening on port 12345" << std::endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
            closesocket(server_socket);
            WSACleanup();
            return EXIT_FAILURE;
        }
        std::thread(handle_client, client_socket).detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}

