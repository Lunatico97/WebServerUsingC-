/*
    HTTP Delivery using TCP Server using POSIX sockets
    Author: Diwas Adhikari
*/

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include "http-parser.hpp"

#define MAX 20000

void error(std::string message, bool force_exit=true){
    std::cerr << "ERROR - !!! " << message << " !!!" << std::endl ;
    if(!force_exit)
        return ;
    exit(1) ;
}

class HTTPServer{
    private:
        struct sockaddr_in server_address, client_address ; // Server address
        std::string httpHeader, file_path ; // Variables to store parsed headers
        struct stat statBuffer ;  // Buffer for file info
        HTTPParser *parser ; 
        int sockfd, cli_sockfd ;
        char buffer[MAX] ;
        int status ;
        int port ;

    public:
        HTTPServer(int port_num){
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) // sockfd == -1
                error("Can't create socket for connection") ;
            server_address.sin_addr.s_addr = INADDR_ANY ; // Server address of machine 
            server_address.sin_family = AF_INET ; // Internet Domain Network
            server_address.sin_port = htons(port_num); // Convert to network order bytes
            std::memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero)) ;
            this->port = port_num ;
        }

        void initialize(){
            parser = new HTTPParser() ; 
            if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) 
                error("Can't bind socket with server address");
        }

        void getServerInfo(){
            std::cout << "Server Address = " << server_address.sin_addr.s_addr << ":" << this->port << std::endl ;
        }

        void connectWithClient(int maxQueue = 5){
            if(listen(sockfd, maxQueue) < 0)
                error("Can't find any client requests") ;
            std::cout << "----------------------------------------------------------------------\n" ;
            std::cout << "Listening for connections ... " << std::endl ; 
            socklen_t length = sizeof(client_address) ;
            cli_sockfd = accept(sockfd, (struct sockaddr *) &client_address, &length) ;
            if (cli_sockfd < 0) 
                error("Can't accept connection from client at " + client_address.sin_addr.s_addr, false) ;
            std::cout << "Client [" << client_address.sin_addr.s_addr << "] connected successfully! " << std::endl ;
            this->busy = true ;
        }

        void getHTTPRequest(){
            std::memset(buffer, 0, sizeof(buffer)) ;
            status = read(cli_sockfd, buffer, sizeof(buffer)-1);
            if (status < 0)
                error("Can't fetch request from client", false) ;
            std::cout << "Client: " << buffer << std::endl ;
        }

        void processHTTPRequest(){
            std::string request = std::string(buffer) ;
            //std::cout << "Buffer filled: " << request.length() << std::endl ;
            if(request.length() < 1){
                this->busy = false ;
                std::cout << "[" << client_address.sin_addr.s_addr << "] disconnected ! " << std::endl ;
            }
            else
                parser->parseRequest(std::string(buffer)) ;
        }

        void sendHTTPResponse(){
            /*
                A request will be 'sort of' sent in this way by a client browser
                "HTTP/1.1 200 Ok\r\n"
                "Content-Type: image/jpeg\r\n\r\n";
            */
            std::cout << "Server Response: " << std::endl ; 
            if(this->busy){
                httpHeader = parser->getHeader() ;
                file_path = parser->getPath() ;
                // Serve the requested file from client
                int fd = open(file_path.c_str(), O_RDONLY);
                if(fd < 0){
                    error("Error fetching file: " + file_path) ;
                }
                fstat(fd, &statBuffer) ;
                int totalSize = statBuffer.st_size;
                int blockSize = statBuffer.st_blksize;
                // Write a HTTP header for response
                httpHeader = httpHeader + "Content-Length: " + std::to_string(totalSize) + "\r\n" ;
                httpHeader = httpHeader + "Connection: close \r\n" ;
                write(cli_sockfd, httpHeader.c_str(), httpHeader.length());
                std::cout << httpHeader << file_path << std::endl ;
                if(status < 0){
                    this->busy = false ;
                    std::cout << "Client [" << client_address.sin_addr.s_addr << "] disconnected! " << std::endl ;
                    error("Can't send response to client", false) ;
                }
                while(totalSize > 0){
                    int bytesSent = sendfile(cli_sockfd, fd, NULL, blockSize);
                    totalSize = totalSize - bytesSent;
                }
                close(fd);
                std::cout << "------------- Response served ----------------" << std::endl ;
            }
            else
            {
                close(cli_sockfd) ;
            }
        }

        void terminateServer(){
            this->busy = false ;
            shutdown(cli_sockfd, SHUT_RDWR);
            close(sockfd) ;
            delete parser ;
        }

        bool busy ;
} ;

int main(int argc, char *argv[])
{
    if(argc < 2){
        error("No port assigned") ;
    }
    int port = std::atoi(argv[1]) ;
    HTTPServer *server = new HTTPServer(port) ;
    server->initialize() ;
    server->getServerInfo() ;

    while(1){
        server->connectWithClient(1) ;
        int pid = fork();
        if(pid < 0){
            error("Error creating request handler thread") ;
        }
        if(pid == 0){
            while(server->busy){
                server->getHTTPRequest() ;
                server->processHTTPRequest() ;
                server->sendHTTPResponse() ;
            }
        }
    }

    server->terminateServer() ;
    delete server ;
    return 0; 
}
