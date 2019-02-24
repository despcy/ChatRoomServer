//
//  User.cpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/24.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

#include "User.hpp"
using namespace std;

User::User(string name){
    username=name;
    isFree=true;
    socket=-1;
    nowChatingWith=NULL;
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
        User *partner=getPartner();
        partner->setChatPartner(NULL);
        partner->setisFree(true);
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

