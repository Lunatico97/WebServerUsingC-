/*
    Server using POSIX sockets
    Author: Diwas Adhikari
*/

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX 256

void error(std::string message){
    std::cerr << "!!! " << message << " !!!" << std::endl ;
    exit(1) ;
}

class Server{
    private:
        struct sockaddr_in server_address, client_address ;
        char buffer[MAX] ;
        int status ;
        int port ;

    public:
        Server(int port_num){

        }

        void initialize(){

        }

        void getServerInfo(){

        }

        void connectWithClient(int maxQueue = 5){

        }

        void getRequest(){

        }

        void sendResponse(std::string response){

        }

        void terminateServer(){

        }
} ;

int main(int argc, char *argv[])
{
     
    return 0; 
}
