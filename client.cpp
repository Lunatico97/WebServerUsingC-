/*
    Client using POSIX sockets
    Author: Diwas Adhikari
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 256

void error(std::string message){
    std::cerr << "!!! " << message << " !!!" << std::endl ;
    exit(1) ;
}

class Client{
    private:
        struct sockaddr_in server_address ;
        struct hostent * server ;
        char buffer[MAX] ;
        int port ;

    public:
        Client(int port_num){

        }

        void connectWithServer(const char* host_name){

        }

        void sendRequest(std::string request){

        }

        void getResponse(){

        }

        void terminateClient(){

        }
} ;

int main(int argc, char *argv[]){

    return 0 ;
}