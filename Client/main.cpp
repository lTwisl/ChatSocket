#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

using namespace std;

int main()
{
    WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "Error Startup\n";
        return 1;
    }

    sockaddr_in clientSocket;
    int clientSocketLen = sizeof(clientSocket);
    clientSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientSocket.sin_port = htons(8888);
    clientSocket.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&clientSocket, clientSocketLen) != 0)
    {
        cout << "Error connet\n";
        return 1;
    }
    cout << "Connected\n";

    thread t = thread([&]()
        {
            char buf[256];
            while (true)
            {
                int a = recv(Connection, buf, sizeof(buf), NULL);
                if (a > 0)
                    cout << buf << endl;
            }
        });

    while (true)
    {
        char msg[256];
        cin >> msg;
        send(Connection, msg, sizeof(msg), NULL);
    }

    system("pause");
    WSACleanup();
    closesocket(Connection);
}

