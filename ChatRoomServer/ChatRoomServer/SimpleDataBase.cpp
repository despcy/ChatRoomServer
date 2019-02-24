//
//  SimpleDataBase.cpp
//  ChatRoomServer
//
//  Created by ycx on 2019/2/24.
//  Copyright © 2019 chenxiy.net. All rights reserved.
//

#include "SimpleDataBase.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
Database::Database(){
    UserFriendTable="UserFriendTable.txt";
    UserNameAndPassTable="UserNameAndPassTable.txt";
    try{
    UserFriendFile.open(UserFriendTable.c_str(),std::ios_base::app);
    UserNameAndPassFile.open(UserNameAndPassTable.c_str(),std::ios_base::app);
    }catch(int e){
        cout<<"An exception occurred In open database file Exception Nr. " << e << endl;
    }

}
Database::~Database(){
    UserFriendFile.close();
    UserNameAndPassFile.close();
}

bool Database::createUser(string userName, string password){
    try{
        if(isUserNameExists(userName)){
            return false;
        }
    UserNameAndPassFile<<userName<<','<<password<<endl;
        return true;
    }catch(int e){
        return false;
    }

}

bool Database::addFriend(string userName, string friendName){
    if(!isFriend(userName, friendName)){
        try {
            UserFriendFile<<userName<<','<<friendName<<endl;
            return true;
        } catch (int e) {
            return false;
        }

    }
    return false;
}

bool Database::isFriend(string userName, string friendName){
    ifstream readFile(UserFriendTable.c_str());
    if(!readFile) {
        cout << "Database Cannot open input file.\n";
        return false;
    }
    string compare1=userName+","+friendName;
    string compare2=friendName+","+userName;
    string substr;
    while(readFile) {
        getline(readFile,substr);  // delim defaults to '\n'
        if(readFile){
            if (substr==compare1||substr==compare2) {
                readFile.close();
                return true;
            }
            
        }
        
    }
    
    readFile.close();
    return false;
}


bool Database::delFriend(string userName,string friendName){
    if(isFriend(userName, friendName)){
        fstream ioFile(UserFriendTable.c_str());
        if(!ioFile) {
            cout << "Database Cannot open input file.\n";
            return false;
        }
        string compare1=userName+","+friendName;
        string compare2=friendName+","+userName;
        string substr;
        int i=0;
        while(ioFile) {
            getline(ioFile,substr);  // delim defaults to '\n'
            i++;
            if(ioFile){
                if (substr==compare1||substr==compare2) {
                   ioFile.seekp(-std::ios::off_type(compare1.size())-1, std::ios_base::cur);
                    ioFile<<"@";//set invalied
                }
                
            }
            
        }
        
        ioFile.close();
        return true;
    }
    return false;
}

bool Database::isUserNameExists(string userName){
    ifstream readFile(UserNameAndPassTable.c_str());
    if(!readFile) {
        cout << "Database Cannot open input file.\n";
        return false;
    }
    
    string substr;
    while(readFile) {
        getline(readFile,substr,',');  // delim defaults to '\n'
        if(readFile){
            if (substr==userName) {
                readFile.close();
                return true;
            }
            
        }
        getline(readFile,substr);
        
    }
    
    readFile.close();
    
    return false;
}

bool Database::checkUser(string userName, string password){
    ifstream readFile(UserNameAndPassTable.c_str());
    if(!readFile) {
        cout << "Database Cannot open input file.\n";
        return false;
    }
    
    string substr1;
    string substr2;
    while(readFile) {
        
        getline(readFile,substr1,',');  // delim defaults to '\n'
        getline(readFile,substr2,'\n');
        if(readFile){
            if (substr1==userName&&substr2==password) {
                readFile.close();
                return true;
            }
            
        }
        
        
    }
    
    readFile.close();
    
    return false;
}

vector<string> Database::getAllFriends(string userName){
    vector<string> friends;
    ifstream readFile(UserFriendTable.c_str());
    if(!readFile) {
        cout << "Database Cannot open input file.\n";
        return friends;
    }
    string substr1;
    string substr2;
    while(readFile) {
        getline(readFile,substr1,',');  // delim defaults to '\n'
        getline(readFile,substr2,'\n');
        if(readFile){
            if(substr1==userName&&substr2[0]!='@'){
                friends.push_back(substr2);
            }else if(substr2==userName&&substr1[0]!='@'){
                friends.push_back(substr1);
            }
        }
       
        
    }
    
    readFile.close();
    return friends;
}
