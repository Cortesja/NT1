// NT1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib") // Link with Winsock library? <-必要なのかわからない。

void ChatServer() {
    //bind >> listen >> accept
    //chat, sendとreceive

    SOCKET listen_s;
    SOCKET s;
    SOCKADDR_IN serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    u_short uport;
   
    std::cout << "使用するポート番号--> ";
    std::cin >> uport;
    fflush(stdin);
    
    //listenソケット作成
    listen_s = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_s == INVALID_SOCKET) {
        std::cerr << "listenソケット作成失敗" << std::endl;
        closesocket(listen_s);
        WSACleanup();
        return;
    }

    std::cout << "リ寸ソケットオープンしました" << std::endl;

    //サーバー地所を解決
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(uport);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //bindをする
    if (bind(listen_s, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind失敗" << std::endl;
        closesocket(listen_s);
        WSACleanup();
        return;
    }
    std::cout << "Bind成功です" << std::endl;
    
    //listen開始
    if (listen(listen_s, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen失敗" << std::endl;
        closesocket(listen_s);
        WSACleanup();
        return;
    }

    std::cout << "clientから接続をお待ちしております。" << std::endl;

    //clientの要望を受け入れる
    s = accept(listen_s, (struct sockaddr*)&serverAddr, &clientAddrSize);
    if (s == INVALID_SOCKET) {
        std::cerr << "受け入れ失敗" << std::endl;
        closesocket(s);
        WSACleanup();
        return;
    }

    std::cout << "clientと接続は完了いたします。" << std::endl;
    closesocket(listen_s);
    std::cout << "会話開始" << std::endl;
    while (1) {
        int nRcv;
        char szBuf[1024];

        nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
        szBuf[nRcv] = '\0';

        std::cout << "受信--> szBuf\n" << std::endl;
        std::cout << "受信--> " << std::endl;

        std::cin >> szBuf;
        fflush(stdin);

        send(s, szBuf, (int)strlen(szBuf), 0);
    }

    //socketsを閉じる
    closesocket(s);
    WSACleanup();
}

void ChatClient() {
    SOCKET clientSocket;
    SOCKADDR_IN serverAddr;
    u_short uport;
    char serverName[1024] = { 0 };

    std::cout << "IPアドレスを入力してください: " << std::endl;
    std::cin >> serverName;

    std::cout << "Port番号を入力してください：　" << std::endl;
    std::cin >> uport;

    HOSTENT* IpHost;
    IpHost = gethostbyname(serverName);
    memset(&serverAddr, 0, sizeof(SOCKADDR_IN));
    serverAddr.sin_family = IpHost->h_addrtype;
    serverAddr.sin_port = htons(uport);
    serverAddr.sin_addr.s_addr = *((u_long*)IpHost->h_addr_list);

    //socketを制作
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket制作失敗" << std::endl;
        WSACleanup();
        return;
    }

    //サーバーに接続
    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("サーバーと接続できませんでした\n");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "サーバーに接続完了" << std::endl;

    while (1) {
        char szBuf[1024];
        std::string input;

        int nRcv = recv(clientSocket, szBuf, sizeof(szBuf) - 1, 0);
        szBuf[nRcv] = '\0';

        std::cout << "受信--> szBuf\n" << std::endl;
        std::cout << "受信--> " << std::endl;

        std::cin >> szBuf;
        fflush(stdin);

        send(clientSocket, szBuf, (int)strlen(szBuf), 0);
    }
    //Socketを閉じる
    closesocket(clientSocket);
    WSACleanup();
}

int main()
{
    int mode;

    std::cout << "サーバーなら'0'を入力してください" << std::endl << "Clientなら'1'を入力してください" << std::endl;
    std::cin >> mode;
    fflush(stdin);

    if (mode == 0) {
        ChatServer();
    }
    else {
        ChatClient();
    }
    WSACleanup();
    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
