//
//  SimpleDataBase.hpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/24.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

//#ifndef SimpleDataBase_hpp
//#define SimpleDataBase_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class Database{
public:
    Database();//check if two table already exist
    ~Database();
    bool createUser(string userName,string password);
    bool checkUser(string userName,string password);
    bool addFriend(string userName,string friendName);
    bool delFriend(string userName,string friendName);
    bool isFriend(string userName,string friendName);
    bool isUserNameExists(string userName);
    vector<string> getAllFriends(string userName);
private:
    string UserNameAndPassTable;
    string UserFriendTable;
    ofstream UserNameAndPassFile;
    ofstream UserFriendFile;
};
//#endif /* SimpleDataBase_hpp */
