////////////////////////////////////////////////客户端代码
#include <WINSOCK2.H>  
#include <iostream>  
#include <string> 
#include <cstdlib> 
#include <iomanip>

#pragma comment(lib, "ws2_32.lib ")  
using namespace std;  

#define BUFSIZE 1024

int main(){  

/*Winsocket注册过程*/
    WORD wVersionRequested = MAKEWORD( 2, 0 );  // 请求WinSock库，高字节指明副版本，低字节指明主版本
    WSADATA wData;    // 这结构是用于接收Wjndows Socket的结构信息的（版本信息）
    int err;
    err = WSAStartup(wVersionRequested, &wData); //Winsock服务初始化
    if ( err != 0 ) {
        cout << "Initialize failed."<<endl;
        return -1; // 返回值为零时表示成功WSAStartup
    } 


/*创建Socket*/
    SOCKET sockCli = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockCli < 0){
        cout << "Failed." << endl;
        return -1;
    }
    cout << "Create socket successfully." << endl;
    SOCKADDR_IN addr_client; //服务器的地址数据结构
    addr_client.sin_family = AF_INET;
    addr_client.sin_port = htons(6789);                        //端口号为6789
    addr_client.sin_addr.S_un.S_addr=inet_addr("172.19.33.77");   //127.0.0.1为本电脑IP地址
    if (bind(sockCli, (SOCKADDR *)&addr_client, sizeof(addr_client)) == SOCKET_ERROR)
	{
		cout<<"Failed to bind."<< endl;
		closesocket(sockCli);
        WSACleanup(); 
		return 0;
    }
    else
        cout << "Bind successfully." << endl;

/*向指定地址和端口收发数据*/
    string sendBuf= "Hello server! There is a packet.";      //发送数据的缓冲区

//互联网环境
    char recvBuf[BUFSIZE];          //接受数据的缓冲区
    SOCKADDR_IN addr_server;        //服务器的地址数据结构
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(6789);                       
    addr_server.sin_addr.S_un.S_addr=inet_addr("8.129.101.161");   
    SOCKADDR_IN addr_recv;
    int recv_len = sizeof(SOCKADDR_IN);
    int count = 0;
    for (int i = 1; i <= 100; i++){
        err = sendto(sockCli, sendBuf.data(), sendBuf.size(), 0, (SOCKADDR *)&addr_server, sizeof(SOCKADDR)); //发送
        if (err <= 0){
            cout << "Sendto failed."<< endl;
            cout<<WSAGetLastError()<<endl;
            return -1;
        }
        else{
            cout << "Packet " << i << " has been sent." << endl;
        }
    //接收echo的数据
        int last = recvfrom(sockCli, recvBuf, BUFSIZE, 0, (SOCKADDR *)&addr_recv, &recv_len);
        if (last <= 0)
        {
            cout << "Recvfrom Error!" << endl;
            continue;
        }
        else{
            cout << "Recvfrom:" << setw(7) << recvBuf;
            cout << "   Count:" << ++count << endl;
        }
    }

/*关闭套接字*/
    closesocket(sockCli); 

    WSACleanup();
    system("pause");
}   
