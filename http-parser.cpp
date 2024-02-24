#include <iostream>
#include <cstring>
#include <vector>

class HTTPParser{
    private:
        std::vector<std::string> essentials ;
        std::string extension, method, file_path, header ;
        std::string http_header = "HTTP/1.1 200 Ok\r\n";

        std::vector<std::string> tokenize(char* text, const char* delimiter){
            std::vector<std::string> tokens ;
            char *token = std::strtok(text, delimiter) ;
            while(token != nullptr) {
                tokens.push_back(token) ;
                token = std::strtok(nullptr, delimiter);
            }
            return tokens ;
        }

    public:
        HTTPParser(){}

        void parseRequest(std::string buffer){
            essentials = tokenize(buffer.data(), " ") ;
            method = essentials[0] ;  
            file_path = essentials[1] ;
            extension = tokenize(file_path.data(), ".")[1] ;  
            std::cout << "Request type: " << method << std::endl ; 
            std::cout << "File type: " << extension << std::endl ; 

            if(method == "GET"){
                if(file_path.length() <= 1){
                    //char path_head[500] = ".";
                    file_path = "./index.html" ;
                    header += "Content-Type: text/html\r\n\r\n" ;
                }
            }
            else if(method == "POST"){

            } 
        }

        std::string getHeader(){
            return this->header ;
        }

        std::string getPath(){
            return this->file_path ;
        }
} ;


int main(int argc, char *argv[]){ 
    HTTPParser *parser = new HTTPParser ;
    std::string request = "GET /diwas HTTP/1.1" ;
    parser->parseRequest(request) ;
    std::cout << parser->getHeader() << std::endl ;
    std::cout << parser->getPath() << std::endl ;
    delete parser ;
    return 0 ;
}