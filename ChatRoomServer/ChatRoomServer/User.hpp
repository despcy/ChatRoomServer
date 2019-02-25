//
//  User.hpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/24.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

#ifndef User_hpp
#define User_hpp
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
class User{
public:
    User(string name,pthread_t self,int sock);
    ~User();
    string getUsername();
    bool getisFree();
    int getSocket();
    pthread_t getThread();
    bool setSocket(int sock);
private:
    string username;
    int socket;
    pthread_t threadId;
};

#endif /* User_hpp */
