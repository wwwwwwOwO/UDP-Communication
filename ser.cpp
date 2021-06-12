////////////////////////////////////////////////服务器代码
#include <WINSOCK2.H>  
#include <iostream>  
#include <iomanip>  
#include <string> 

#pragma comment(lib, "ws2_32.lib")
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
    int sockSev = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockSev < 0)
    {
        cout << "Failed to create socket." << endl;
        return -1;
    }
    cout << "Create socket successfully." << endl;



/*绑定socket和端口号*/
    SOCKADDR_IN addr_server;                                    //服务器的地址数据结构
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(6666);                        //端口号为6666
    addr_server.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   //172.19.1.207为本电脑IP地址

	if (bind(sockSev, (SOCKADDR *)&addr_server, sizeof(addr_server)) == SOCKET_ERROR)
	{
		cout<<"Failed to bind."<< endl;
		closesocket(sockSev);
        WSACleanup(); 
		return 0;
    }
    else
        cout << "Bind successfully." << endl;

/*向指定地址和端口收发数据*/
    char recvBuf[BUFSIZE];  //接受数据的缓冲区
    
    SOCKADDR_IN addr_client;            //用于接收用户的ip地址和端口号等信息
    int client_len = sizeof(addr_client);
    int count = 0;
    while(true){
        int last = recvfrom(sockSev, recvBuf, BUFSIZE, 0, (SOCKADDR *)&addr_client, &client_len);
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
    closesocket(sockSev); 
    WSACleanup(); 
    
    return 0;
}
