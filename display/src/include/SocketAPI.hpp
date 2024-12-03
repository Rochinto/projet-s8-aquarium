
#ifndef __SOCKET_API_HPP__
#define __SOCKET_API_HPP__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#include "ReplyManager.hpp"

#define h_addr  h_addr_list[0]
#define BUFSIZE 16384

class SocketAPI{

private:
    int sockfd, port, n;
    sockaddr_in serv_addr;
    hostent *server;
    list<string> msg_list;
    ReplyManager parser;
    Aquarium* aquarium;
    bool running = false;

    void setup(string host_name);
    //Get all messages from msg_list with prefix as command, and remove/consume them from msg_list
    list<string> getReplies();
    list<string> getListReplies();

public:

    SocketAPI(string host_name, int port, Aquarium* aquarium);
    SocketAPI(char* host_name, int port, Aquarium* aquarium);
    SocketAPI();
    
    //int send(const char* data, unsigned int len);
    int send(string data);
    void readAll();
    void loopRead();
    void close(thread* loop);

    //Ask controler for a view, and return the view number (0 if no view, -1 if error)
    bool sendCommand(string command, string expected_reply);
    void sendCommand(string command);
};







#endif