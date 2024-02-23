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

void error(std::string message, bool force_exit=false){
    std::cerr << "ERROR - !!! " << message << " !!!" << std::endl ;
    if(!force_exit)
        return ;
    exit(1) ;
}

class Server{
    private:
        struct sockaddr_in server_address, client_address ;
        int sockfd, cli_sockfd ;
        char buffer[MAX] ;
        int status ;
        int port ;

    public:
        Server(int port_num){
            std::memset((char*) &server_address, 0, sizeof(server_address)) ;
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) // sockfd == -1
                error("Can't create socket for connection") ;
            server_address.sin_addr.s_addr = INADDR_ANY ; // Server address of machine 
            server_address.sin_family = AF_INET ; // Internet Domain Network
            server_address.sin_port = htons(port_num); // Convert to network order bytes
            this->port = port_num ;
        }

        void initialize(){
            if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) 
                error("Can't bind socket with socket address");
        }

        void getServerInfo(){
            std::cout << "Server Address = " << server_address.sin_addr.s_addr << ":" << this->port << std::endl ;
        }

        void connectWithClient(int maxQueue = 5){
            if(listen(sockfd, maxQueue) < 0)
                error("Can't find any client requests") ;
            std::cout << "Listening for connections ... " << std::endl ; 
            socklen_t length = sizeof(client_address) ;
            cli_sockfd = accept(sockfd, (struct sockaddr *) &client_address, &length) ;
            if (cli_sockfd < 0) 
                error("Can't accept connection from client at " + client_address.sin_addr.s_addr, false) ;
            std::cout << "Client [" << client_address.sin_addr.s_addr << "] connected successfully! " << std::endl ;
            this->busy = true ;
        }

        void getRequest(){
            std::memset(buffer, 0, sizeof(buffer)) ;
            status = read(cli_sockfd, buffer, sizeof(buffer)-1);
            if (status < 0)
                error("Can't fetch request from client", false) ;
            std::cout << "Client: " << buffer << std::endl ;
        }

        void sendResponse(std::string response){
            status = write(cli_sockfd, response.c_str(), response.length());
            if(status < 0){
                this->busy = false ;
                std::cout << "Client [" << client_address.sin_addr.s_addr << "] disconnected! " << std::endl ;
                error("Can't send response to client", false) ;
            }
        }

        void terminateServer(){
            this->busy = false ;
            close(cli_sockfd) ;
            close(sockfd) ;
        }

        bool busy ;
} ;

int main(int argc, char *argv[])
{
    if(argc < 2){
        error("No port assigned") ;
    }
    int port = std::atoi(argv[1]) ;
    Server *server = new Server(port) ;
    server->initialize() ;
    server->getServerInfo() ;
    std::string msg ;

    while(1){
        server->connectWithClient(1) ;
        while(server->busy){
            server->getRequest() ;
            std::cout << "You: " ;
            std::getline(std::cin, msg, '\n') ;
            server->sendResponse(msg) ;
        }
    }

    server->terminateServer() ;
    delete server ;
    return 0; 
}
