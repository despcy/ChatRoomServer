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

**Username must be at most 20 Characters, only [1-9][a-z][A-Z] allowed**

**Password must be at most 20 Characters, only [1-9][a-z][A-Z] allowed**

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

## Get Online User List:

`LSUS`

### Return format:

`USER[Username1,Username2,Username3,...]`


## Add friend:

`ADDF?NAME=username`

这里记得设置防止多人同时添加好友造成混乱

|Code|Meaning|
|----|-------|
|FBZY|friend is chating with others|
|FRFU|friend refused connection|
|TOUT|request time out, friend no response for 60 second|
|CONN|friend add successfully|

## Del friend:

`DELF`

|Code|Meaning|
|----|-------|
|YRFR|Delete Friend Successfully,you are free|

## Send Message To Friend Added:

`MSSG=yourmessagehere `

|Code|Meaning|
|----|-------|
|MACK|Server received the message|
|NCON|you must add a friend before sending|

# Server To Client:

## HeartBeat

To keep alive, **Server will send client** the following Code every 30 seconds, if no response, the client will be set offline and socket will close

`LIVE`

**Client should send back:**

|Code|Meaning|
|----|-------|
|OKLV|tell server you are online|

## Online User List Update:

`NEWU[Username1,Username2,Username3,...]`

## New Friend Request:

`FRIE?Username`

|Code|Meaning|
|----|-------|
|ACFR|ACCEPT|
|REFR|REJECT|

## New Message:

RMSG?MSID=12345&MTXT=yournewmessagehere

|Code|Meaning|
|----|-------|
|ACKM?MSID=12345|message id 12345 received|

## You are deleted by friend:

`YDEL`
