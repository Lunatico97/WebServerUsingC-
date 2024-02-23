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
        int sockfd ;
        int status ;
        int port ;

    public:
        Client(int port_num){
            std::memset((char*) &server_address, 0, sizeof(server_address)) ;
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) // sockfd == -1
                error("Can't create socket for connection") ;
            server_address.sin_family = AF_INET ; // Internet Domain Network
            server_address.sin_port = htons(port_num); // Convert to network order bytes
            this->port = port_num ;
        }

        void connectWithServer(const char* host_name){
            server = gethostbyname(host_name) ;
            if(server == NULL)
                error("Can't find the host - Host doesn't exist") ;
            std::memcpy((struct sockaddr_in*) &server_address.sin_addr.s_addr, server->h_addr, server->h_length) ;
        }

        void sendRequest(std::string request){
            status = write(sockfd, request.c_str(), request.length());
            if(status < 0)
                error("Can't send request to the server");
        }

        void getResponse(){
            std::memset(buffer, 0, sizeof(buffer)) ;
            status = read(sockfd, buffer, sizeof(buffer)-1);
            if (status < 0)
                error("Can't fetch response from the server");
            std::cout << "Server: " << buffer << std::endl ;
        }

        void terminateClient(){
            close(sockfd) ;
        }
} ;

int main(int argc, char *argv[]){

    return 0 ;
}