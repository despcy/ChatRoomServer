//
//  User.cpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/24.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

#include "User.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
using namespace std;

User::User(string name,pthread_t self,int sock){
    username=name;
    socket=sock;
    threadId=self;
    //update the online list, and all user who isFriend send userlist notification
}

User::~User(){
    //close socket,clean thread
    shutdown(getSocket(), SHUT_RDWR);
    pthread_cancel(getThread());
    //update the online list, and all user who isFriend send userlist notification
    cout<<"User:"<<getUsername()<<" R.I.P"<<endl;
}

pthread_t User::getThread(){
    return threadId;
}

string User::getUsername(){
    return username;
}


int User::getSocket(){
    return socket;
}

bool User::setSocket(int sock){
    socket=sock;
    return true;
}

