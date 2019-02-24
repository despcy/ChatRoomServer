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
 User(string name);
    string getUsername();
    bool getisFree();
    User* getPartner();
    int getSocket();
    bool setisFree(bool isfree);
    bool setChatPartner(User* partner);
    bool setSocket(int sock);
private:
    string username;
    bool isFree;
    User *nowChatingWith;
    int socket;
    
};

#endif /* User_hpp */
