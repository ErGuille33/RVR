#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>


#include <iostream>


int main(int argc, char **argv){
    struct addrinfo hints;
    struct addrinfo *res;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];


    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(struct sockaddr_in);

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;

    hints.ai_protocol = 0;

    hints.ai_socktype = SOCK_STREAM;


    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if(rc != 0)
    {
        std::cerr << gai_strerror(rc) << std::endl;
        return -1;
    }

    int sd = socket(res->ai_family, res->ai_socktype,res->ai_protocol);

    server_addr.sin_family = AF_UNSPEC;

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[2]));

    int connected = 3;
    connected = connect(sd, (struct sockaddr )&server_addr, server_len);
    std::cout << connected << std::endl;

    char buffer[80];
    ssize_t bytes = 0 ;

    int c = 1;

    while(c == 1){

        std::cin >> buffer;

        if (buffer[0] == 'q' && buffer[1] == '\0'){
            c = 0;
        }

        else{

            sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr )&server_addr, server_len);
            bytes = recv(sd, (void*) buffer, sizeof(char)*79,0);
            buffer[bytes]= '\0';
        }

    }

    freeaddrinfo(res);
    return 0;
}