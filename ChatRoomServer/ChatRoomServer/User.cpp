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
using namespace std;

User::User(string name,pthread_t self,int sock){
    username=name;
    isFree=true;
    socket=sock;
    nowChatingWith=NULL;
    threadId=self;
}

User::~User(){
    //close socket,clean thread
    shutdown(getSocket(), SHUT_RDWR);
    pthread_cancel(getThread());
    cout<<"User:"<<getUsername()<<"R.I.P"<<endl;
}

pthread_t User::getThread(){
    return threadId;
}

string User::getUsername(){
    return username;
}

bool User::getisFree(){
    return isFree;
}

int User::getSocket(){
    return socket;
}

User * User::getPartner(){
    return nowChatingWith;
}

bool User::setisFree(bool isfree){
    isFree=isfree;
    return true;
}

//execute this function when the partner agree to connect
bool User::setChatPartner(User* partner){
      setisFree(true);
    if(getPartner()!=NULL){//now is chating
        User *oldpartner=getPartner();
        oldpartner->setChatPartner(NULL);
        oldpartner->setisFree(true);
    }
    
    if(partner!=NULL&&!partner->isFree){//the partner is now busy
        return false;
    }else{
        nowChatingWith=partner;
        setisFree(false);
        partner->setisFree(false);
        partner->setChatPartner(this);
        return true;
    }

}

bool User::setSocket(int sock){
    socket=sock;
    return true;
}

