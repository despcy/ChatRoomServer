# ChatRoomServer Application Programming Interface

@Author: Chenxi Yang

## Universal Code:

|Code|Meaning|
|----|-------|
|ERRO|error|


# Client To Server:

## Register:

`REGI?UNAM=yourusername&PASS=yourpassword`

**Username must be at most 20 Characters, only [1-9][a-z][A-Z] allowed**

**Password must be at most 20 Characters, only [1-9][a-z][A-Z] allowed**

### Return:

|Code|Meaning|
|----|-------|
|OKRG|Register Success|
|INRG|Invalied Username or Password|
|ALEX|Username Already Exists|

## Login:

`LOGN?NAME=yourusername&PASS=yourpassword`

**Username must be at most 20 Characters, only [0-9][a-z][A-Z] allowed**

**Password must be at most 20 Characters, only [0-9][a-z][A-Z] allowed**

### Return:

|Code|Meaning|
|----|-------|
|OKLN|Login Success|
|INLN|Invalied Username or Password|
|WPAS|Wrong Password|

## Logout(Offline):

`BBYE`

### Return:

|Code|Meaning|
|----|-------|
|BBYE|Success|

## Get Online Friend List:

`LSUS`

RESPONSE: [Online Friend User List Update](#online-friend-user-list-update)



## Add friend:

`ADDF?NAME=username`

这里记得设置防止多人同时添加好友造成混乱

|Code|Meaning|
|----|-------|
|FRRE?FROM=friendUsername|friend refused request(this code may not be received immediately)|
|FRAC?FROM=friendUsername|friend add successfully(this code may not be received immediately)|
|NUSR|no such user|
|NOLN|friend not online|
|ISFR|you two are already friends|

## Del friend:

`DELF?NAME=yourfriendname`

|Code|Meaning|
|----|-------|
|DESU|Delete Friend Successfully|
|NUSR|no such user|

## Send Message To Friend Added:

`MSSG?TEXT=yourmessagehere&GOTO=username `

|Code|Meaning|
|----|-------|
|NFRI|this user is not your friend|
|NOLN|friend offline|
# Server To Client:

## HeartBeat

To keep alive, **Server will send client** the following Code every 30 seconds, if no response, the client will be set offline and socket will close

`LIVE`

**Client should send back:**

|Code|Meaning|
|----|-------|
|OKLV|tell server you are online|

## Online Friend User List Update:

`USER[NAME=Username1&STAT=ONLN,NAME=UserName2&STAT=OFLN,......]`

## New Friend Request:

`FRIE?Username`

|Code|Meaning|
|----|-------|
|ACFR?FROM=Username|ACCEPT friend request asked from username|
|REFR?FROM=Username|REJECT the request asked form username|

## New Message:

`RMSG?FROM=yourFriendUserName&MTXT=yournewmessagehere`

