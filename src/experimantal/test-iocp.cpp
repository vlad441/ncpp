#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

struct PER_IO_OPERATION_DATA {
    OVERLAPPED overlapped;
    SOCKET clientSocket;
    char buffer[BUFFER_SIZE];
};

void handleClient(SOCKET client) {
    std::cout << "Обработка клиента: " << client << std::endl;
    // Обработка данных клиента...
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Ошибка привязки сокета." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    listen(listenSocket, SOMAXCONN);

    // Создание порта завершения ввода-вывода (IOCP)
    HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (iocpHandle == NULL) {
        std::cerr << "Ошибка создания IOCP." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Привязка listen-сокета к IOCP
    CreateIoCompletionPort((HANDLE)listenSocket, iocpHandle, 0, 0);

    // Создание сокета для принятия соединений
    SOCKET clientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    PER_IO_OPERATION_DATA* ioData = new PER_IO_OPERATION_DATA();
    ZeroMemory(&ioData->overlapped, sizeof(OVERLAPPED));
    ioData->clientSocket = clientSocket;
	
	LPFN_ACCEPTEX AcceptEx = NULL; GUID GuidAcceptEx = WSAID_ACCEPTEX;

	AcceptEx = WSAIoctl(
		listenSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&GuidAcceptEx,
		sizeof(GuidAcceptEx),
		&AcceptEx,
		sizeof(AcceptEx),
		NULL,
		NULL,
		NULL);


    // Буфер для приема соединений
    DWORD bytesReceived = 0;
    BOOL result = AcceptEx(
        listenSocket,
        clientSocket,
        ioData->buffer,
        0,  // Дополнительные данные
        sizeof(sockaddr_in) + 16,
        sizeof(sockaddr_in) + 16,
        &bytesReceived,
        &ioData->overlapped
    );

    if (result == FALSE && WSAGetLastError() != ERROR_IO_PENDING) {
        std::cerr << "Ошибка AcceptEx: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        delete ioData;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Ожидание соединений..." << std::endl;

    // Обработка завершённых операций
    while (true) {
        DWORD bytesTransferred = 0;
        ULONG_PTR completionKey = 0;
        LPOVERLAPPED overlapped = NULL;

        BOOL success = GetQueuedCompletionStatus(
            iocpHandle,
            &bytesTransferred,
            &completionKey,
            &overlapped,
            INFINITE
        );

        if (!success) {
            std::cerr << "Ошибка GetQueuedCompletionStatus: " << GetLastError() << std::endl;
            continue;
        }

        PER_IO_OPERATION_DATA* ioData = (PER_IO_OPERATION_DATA*)overlapped;
        handleClient(ioData->clientSocket);

        // Освобождаем ресурсы и подготавливаем следующий приём
        closesocket(ioData->clientSocket);
        delete ioData;

        // Создаем новый сокет для приёма следующего соединения
        clientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        ioData = new PER_IO_OPERATION_DATA();
        ZeroMemory(&ioData->overlapped, sizeof(OVERLAPPED));
        ioData->clientSocket = clientSocket;

        result = AcceptEx(
            listenSocket,
            clientSocket,
            ioData->buffer,
            0,
            sizeof(sockaddr_in) + 16,
            sizeof(sockaddr_in) + 16,
            &bytesReceived,
            &ioData->overlapped
        );

        if (result == FALSE && WSAGetLastError() != ERROR_IO_PENDING) {
            std::cerr << "Ошибка AcceptEx: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            delete ioData;
        }
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
