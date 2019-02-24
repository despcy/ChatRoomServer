# ChatRoomServer
Chat room server side for EECS22L project
### **[Click Here For API Document](./ChatRoomServer/API.md)**
Usage:

1. `git clone https://github.com/yangchenxi/ChatRoomServer.git`

2. `cd ./ChatRoomServer/ChatRoomServer/ChatRoomServer`

3. 
```
g++ -c main.cpp -lpthread -o main.o
g++ -c User.cpp -o User.o
g++ -c SimpleDataBase.cpp -liostream -lfstream -lstdlib -lstring -o SimpleDataBase.o
g++ main.o SimpleDataBase.o User.o -lpthread -o server
```

4. `./server`


Reference: https://blog.csdn.net/lovekun1989/article/details/41042273
