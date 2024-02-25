#include <iostream>
#include <cstring>
#include <vector>

class HTTPParser{
    private:
        std::vector<std::string> essentials ;
        std::string extension, method, file_path ;
        std::string http_header = "HTTP/1.1 200 Ok\r\n";

        std::vector<std::string> tokenize(char* text, const char* delimiter){
            std::vector<std::string> tokens ;
            char *copy = (char*) std::malloc(std::strlen(text)+1);
            std::strcpy(copy, text);
            char *token = std::strtok(copy, delimiter) ;
            while(token != nullptr){
                tokens.push_back(std::string(token)) ;
                token = std::strtok(nullptr, delimiter);
            }
            delete token ;
            delete copy ;
            return tokens ;
        }

    public:
        HTTPParser(){}

        void parseRequest(std::string buffer){
            essentials = this->tokenize(buffer.data(), " ") ;
            method = essentials[0] ;  
            file_path = essentials[1] ;
            std::cout << "Parsing HTTP Request ---------> " << std::endl ;
            std::cout << "Request type: " << method << std::endl ; 
            std::cout << "File Path: " << file_path << std::endl ; 
            
            if(method == "GET"){
                if(file_path.length() <= 1){
                    //char path_head[500] = ".";
                    file_path = "./index.html" ;
                    http_header = http_header + "Content-Type: text/html\r\n\r\n" ;
                }
                else{
                    extension = this->tokenize(file_path.data(), ".")[1] ;  
                    std::cout << "File type: " << extension << std::endl ; 
                }
            }
            else if(method == "POST"){

            } 
        }

        std::string getHeader(){
            return http_header ;
        }

        std::string getPath(){
            return file_path ;
        }
} ;


int main(int argc, char *argv[]){ 
    HTTPParser *parser = new HTTPParser() ;
    std::string request = "GET / HTTP/1.1" ;
    parser->parseRequest(request) ;
    std::cout << parser->getHeader() << std::endl ;
    std::cout << parser->getPath() << std::endl ;
    delete parser ;
    return 0 ;
}
