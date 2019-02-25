//
//  main.cpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/19.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

#include <iostream>

/*socket tcp服务器端*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <map>
#include <time.h>
#include "SimpleDataBase.hpp"
#include "User.hpp"
int SERVER_PORT=12345;
int MAX_USERNAME_AND_PASS_LENGTH=20;
//
///*
// 监听后，一直处于accept阻塞状态，
// 直到有客户端连接，
// 当客户端如数quit后，断开与客户端的连接
// */
//
Database database;
vector<User*> onlineUsers;
map<string,string> friendRequestMap;//hash map for resolveing thread communication friend invatation check
pthread_mutex_t hashLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t DBLock = PTHREAD_MUTEX_INITIALIZER;//database lock for thread
pthread_mutex_t userList = PTHREAD_MUTEX_INITIALIZER;//thread lock
bool killUserIfOnline(string username);
bool afterLoginMsgHandler(User* user);
User* isUserOnline(string username);
string getFriendListMsg(vector<string> friends);
using namespace std;


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    
    return buf;
}

string receiveStringData(int client){
    string data;
    try{
    char buffer[200];
    int iDataNum;
    iDataNum =(int) recv(client, buffer, 1024, 0);
    
    if(iDataNum < 0)
    {
        perror("recv");
        return "";
    }
    if(iDataNum>=2){//telnet filter
        if(buffer[iDataNum-1]=='\n'){
            iDataNum--;
        }
        if(buffer[iDataNum-1]=='\r'){
            iDataNum--;
        }
    }
    buffer[iDataNum] = '\0';
    data=buffer;
    }catch(int e){
        data="ReadERROR";
    }
    //-------------------logPrint----
    cout<<currentDateTime()<<"   Socket: "<<client<<"   Thread: "<<(long)pthread_self()<<"  Data: "<<data<<endl;
    cout<<"OnlineUser: "<<endl;
    pthread_mutex_lock(&userList);
    for (int i=0; i<onlineUsers.size(); i++) {
        cout<<"name: "<<onlineUsers[i]->getUsername()<<"  Socket: "<<onlineUsers[i]->getSocket()<<"  Thread: "<<(long)onlineUsers[i]->getThread()<<endl;
    }
    
    pthread_mutex_unlock(&userList);
    //-----------------------------
    return data;
}
string getFriendListMsg(vector<string> friends){
    if (friends.size()==0) {
        return "USER[]";
    }
    string msg="USER[";
    for (int i=0;i<friends.size() ; i++) {
        msg+="NAME="+friends[i]+"&STAT=";
        if(isUserOnline(friends[i])==NULL){
            msg+="OFLN,";
        }else{
            msg+="ONLN,";
        }
        
    }
    msg[msg.size()-1]=']';
    return msg;
}
bool registerUser(string username,string pass){
    bool success=false;
    pthread_mutex_lock(&DBLock);
    success=database.createUser(username, pass);
    pthread_mutex_unlock(&DBLock);

    return success;
}

bool checkUserNamePassFormat(string txt){//[0-9][a-z][A-Z]
    if (txt.length()>MAX_USERNAME_AND_PASS_LENGTH){
        return false;
    }
    for(int i=0;i<txt.size();i++){
        if(!isalpha(txt[i])&&!isdigit(txt[i])){
            return false;
        }
    }
    return true;
}
User* isUserOnline(string username){
    pthread_mutex_lock(&userList);
    for (int i=0; i<onlineUsers.size(); i++) {
        if (onlineUsers[i]->getUsername()==username) {
              pthread_mutex_unlock(&userList);
            return onlineUsers[i];
        }
    }
    
    pthread_mutex_unlock(&userList);
    return NULL;
}
void extractParameter(string request,string *first,string *second){
    try{
    for (int i=10; i<request.size(); i++) {
        if(request[i]=='&'){
            *first=request.substr(10,i-10);
            i+=6;
            *second=request.substr(i,request.size()-i);
            break;
        }
        
    }
    }catch(int e){
        cout<<"error in extract parameter"<<endl;
        *first="";
        *second="";
    }
}

bool login(string username,string pass){
    return database.checkUser(username, pass);
}

void *receiveData(void* arg){
    int client=*((int*)arg);
    while(1)
    {
        string data=receiveStringData(client);
        if (data.size()<4) {
            send(client,"ERRO", 4, 0);
            continue;
        }

        string code=data.substr(0,4);
        if(code=="BBYE"){
            send(client, "BBYE", 4, 0);
            
            shutdown(client, SHUT_RDWR);
            return 0;
        }
        if(code!="REGI"&&code!="LOGN"){
            send(client,"ERRO", 4, 0);
            continue;
        }
        //-----login and register Process---
        string username;
        string password;
        extractParameter(data, &username, &password);
        cout<<"login "<<username<<" "<<password<<endl;
        if(!checkUserNamePassFormat(username)||!checkUserNamePassFormat(password)){
            if(code=="LOGN"){//login
                 send(client,"INLN", 4, 0);
            }else{//register
                 send(client,"INRG", 4, 0);
            }
            continue;
        }
        if(code=="LOGN"){//login
            if(login(username, password)){
                 send(client,"OKLN", 4, 0);
                //check online user list, if user already online, substitute new user 挤下线
                killUserIfOnline(username);
                User *user=new User(username,pthread_self(),client);
                pthread_mutex_lock(&userList);
                onlineUsers.push_back(user);
                pthread_mutex_unlock(&userList);
                afterLoginMsgHandler(user);
                shutdown(client, SHUT_RDWR);
                return 0;
            }else{
                send(client,"WPAS", 4, 0);
            }
        }else{//register
            if(registerUser(username, password)){
                 send(client,"OKRG", 4, 0);
            }else{
                 send(client,"ALEX", 4, 0);
            }
        }

    }
  //  shutdown(client, SHUT_RDWR);
    return NULL;
};

bool killUserIfOnline(string username){
    pthread_mutex_lock(&userList);
    for (int i=0; i<onlineUsers.size(); i++) {
        if (onlineUsers[i]->getUsername()==username) {
            delete onlineUsers[i];
            onlineUsers.erase(onlineUsers.begin()+i);
        }
    }

    pthread_mutex_unlock(&userList);
    return true;
}

bool afterLoginMsgHandler(User* user){
    
    while (1) {
        //client To server===========
        string data=receiveStringData(user->getSocket());
        if (data.size()<4) {
            send(user->getSocket(),"ERRO", 4, 0);
            continue;
        }
        
        string code=data.substr(0,4);
        if(code=="BBYE"){
            send(user->getSocket(), "BBYE", 4, 0);
            killUserIfOnline(user->getUsername());
            return 0;
        }
        
        if(code=="LSUS"){
            pthread_mutex_lock(&DBLock);
            vector<string> friendList=database.getAllFriends(user->getUsername());
            pthread_mutex_unlock(&DBLock);
            string message=getFriendListMsg(friendList);
            send(user->getSocket(),message.c_str(),message.size(), 0);
            continue;
        }
        
        if(code=="ADDF"){
            try {
                string friendUserName=data.substr(10,data.size()-10);
                pthread_mutex_lock(&DBLock);
                if(database.isFriend(user->getUsername(), friendUserName)){
                    send(user->getSocket(), "ISFR", 4, 0);
                    pthread_mutex_unlock(&DBLock);
                    continue;
                }
                if(!database.isUserNameExists(friendUserName)){
                    send(user->getSocket(), "NUSR", 4, 0);
                    pthread_mutex_unlock(&DBLock);
                    continue;
                }
                pthread_mutex_unlock(&DBLock);
                User *myFriend=isUserOnline(friendUserName);
                if(myFriend==NULL){
                    send(user->getSocket(), "NOLN", 4, 0);
                    continue;
                }
                
                
                string friendRequest="FRIE?"+user->getUsername();
                send(myFriend->getSocket(),friendRequest.c_str(),friendRequest.length(), 0);
                pthread_mutex_lock(&hashLock);
                friendRequestMap[user->getUsername()]=friendUserName;
                pthread_mutex_unlock(&hashLock);
                
                
            } catch (int e) {
                cout<<"user"<<user->getUsername()<<"add friend Fail"<<endl;
                send(user->getSocket(), "ERRO", 4, 0);
            }
            
            continue;
        }
        
        if(code=="DELF"){
            string friendUserName=data.substr(10,data.size()-10);
            pthread_mutex_lock(&DBLock);
            if(!database.isFriend(user->getUsername(), friendUserName)){
                send(user->getSocket(), "NUSR", 4, 0);
                pthread_mutex_unlock(&DBLock);
                continue;
            }
            database.delFriend(user->getUsername(),friendUserName);
            pthread_mutex_unlock(&DBLock);
            send(user->getSocket(), "DESU", 4, 0);
            continue;
        }
        if(code=="MSSG"){
            string messageContent;
            string deliverTo;
            extractParameter(data, &messageContent, &deliverTo);
            pthread_mutex_lock(&DBLock);
            if(!database.isFriend(user->getUsername(), deliverTo)){
                send(user->getSocket(), "NFRI", 4, 0);
                pthread_mutex_unlock(&DBLock);
                continue;
            }
            pthread_mutex_unlock(&DBLock);
            User *userToSend=isUserOnline(deliverTo);
            if (userToSend==NULL) {
                send(user->getSocket(), "NOLN", 4, 0);
                continue;
            }
            string message="RMSG?FROM="+user->getUsername()+"&MTXT="+messageContent;
            send(userToSend->getSocket(), message.c_str(), message.size(), 0);
            continue;
        }
        
        
        //------------------Response after Server To Client---------------------------------
        if (code=="OKLV") {
            //heartbeatresponse to be continued
            continue;
        }
        
        if(code=="ACFR"||code=="REFR"){//accept or deny the friend invatation
            string friendUserName=data.substr(10,data.size()-10);
            pthread_mutex_lock(&DBLock);
            if(!database.isUserNameExists(friendUserName)){
                send(user->getSocket(), "NUSR", 4, 0);
                pthread_mutex_unlock(&DBLock);
                continue;
            }
            pthread_mutex_unlock(&DBLock);
            pthread_mutex_lock(&hashLock);
            if(friendRequestMap[friendUserName]!=user->getUsername()){
                
                pthread_mutex_unlock(&hashLock);
                continue;
            }
            friendRequestMap[friendUserName]="";
            pthread_mutex_unlock(&hashLock);
            User *userWhoSendMeTheRequest=isUserOnline(friendUserName);
            if(code=="ACFR"){
                pthread_mutex_lock(&DBLock);
                database.addFriend(friendUserName, user->getUsername());
                pthread_mutex_unlock(&DBLock);
            }
            if(userWhoSendMeTheRequest!=NULL){//send accept message
                if(code=="REFR"){
                    string message="FRRE?FROM="+user->getUsername();
                    send(userWhoSendMeTheRequest->getSocket(), message.c_str(), message.size(), 0);
                }else{
                    string message="FRAC?FROM="+user->getUsername();
                    send(userWhoSendMeTheRequest->getSocket(), message.c_str(), message.size(), 0);
                }
            }
            
            continue;
        }
        
        send(user->getSocket(),"ERRO", 4, 0);//not valied code
        
    }
    return false;
}

int main()
{
    //调用socket函数返回的文件描述符
    int serverSocket;
    //声明两个套接字sockaddr_in结构体变量，分别表示客户端和服务器
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len = sizeof(clientAddr);
    int client;



    //socket函数，失败返回-1
    //int socket(int domain, int type, int protocol);
    //第一个参数表示使用的地址类型，一般都是ipv4，AF_INET
    //第二个参数表示套接字类型：tcp：面向连接的稳定数据传输SOCK_STREAM
    //第三个参数设置为0
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        return 1;
    }

    bzero(&server_addr, sizeof(server_addr));
    //初始化服务器端的套接字，并用htons和htonl将端口和地址转成网络字节序
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    //ip可是是本服务器的ip，也可以用宏INADDR_ANY代替，代表0.0.0.0，表明所有地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //对于bind，accept之类的函数，里面套接字参数都是需要强制转换成(struct sockaddr *)
    //bind三个参数：服务器端的套接字的文件描述符，
    if(::bind(serverSocket,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect");
        return 1;
    }
    //设置服务器上的socket为监听状态
    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }

    while(1)
    {
        printf("Listening on port: %d\n", SERVER_PORT);
        //调用accept函数后，会进入阻塞状态
        //accept返回一个套接字的文件描述符，这样服务器端便有两个套接字的文件描述符，
        //serverSocket和client。
        //serverSocket仍然继续在监听状态，client则负责接收和发送数据
        //clientAddr是一个传出参数，accept返回时，传出客户端的地址和端口号
        //addr_len是一个传入-传出参数，传入的是调用者提供的缓冲区的clientAddr的长度，以避免缓冲区溢出。
        //传出的是客户端地址结构体的实际长度。
        //出错返回-1
        client = accept(serverSocket, (struct sockaddr*)&clientAddr, (socklen_t*)&addr_len);
        if(client < 0)
        {
            perror("accept");
            continue;
        }
        printf("\nrecv client data...n");
        //inet_ntoa   ip地址转换函数，将网络字节序IP转换为点分十进制IP
        //表达式：char *inet_ntoa (struct in_addr);
        printf("IP is %s\n", inet_ntoa(clientAddr.sin_addr));
        printf("Port is %d\n", htons(clientAddr.sin_port));

        pthread_t thread_id;
        int clientNum=client;
        pthread_create(&thread_id, NULL, receiveData, (void*)&clientNum);
        printf("Thread ID %ld for Client %d IP %s:%d\n",(long)thread_id,client,inet_ntoa(clientAddr.sin_addr),htons(clientAddr.sin_port));

    }
    

    return 0;
}

