#include <iostream>
#include <WinSock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable: 4996)

using namespace std;

thread thr[100];
SOCKET ListConnected[100];
int Counter = 0;

int main()
{
    WSAData wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "Error Startup\n";
        return 1;
    }

    sockaddr_in serverSocket;
    int serverSocketLen = sizeof(serverSocket);
    serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverSocket.sin_port = htons(8888);
    serverSocket.sin_family = AF_INET;

    SOCKET socketListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(socketListen, (sockaddr*)&serverSocket, serverSocketLen);
    listen(socketListen, SOMAXCONN);

    for (int i = 0; i < 100; ++i)
    {
        SOCKET newConnnection = accept(socketListen, (sockaddr*)&serverSocket, &serverSocketLen);


        if (newConnnection == 0)
        {
            cout << "Error new connection\n";
        }
        else
        {
            cout << "Client connected\n";

            ListConnected[Counter] = newConnnection;


            thr[Counter] = thread([&]()
                {
                    char buf[256];
                    int ii = Counter;
                    while (true)
                    {
                        if (recv(ListConnected[ii], buf, sizeof(buf), NULL) > 0)
                        {
                            for (int i = 0; i < Counter; ++i)
                            {
                                if (ii == i)
                                    continue;

                                send(ListConnected[i], buf, sizeof(buf), NULL);
                            }
                        }
                    }
                });

            Sleep(10);

            ++Counter;
        }
    }

    system("pause");
    WSACleanup();
    closesocket(socketListen);
    for (int i = 0; i < Counter; ++i)
    {
        thr[i].join();
        closesocket(ListConnected[i]);
    }
}