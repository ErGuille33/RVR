#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>


#include <iostream>
using namespace std;

int main(int argc, char **argv){
    struct addrinfo hints;
    struct addrinfo *res;

    char buffer[80];

    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(struct sockaddr_in);
    ssize_t bytes;

  

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_protocol = 17;
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);


    if(rc != 0)
    {
        std::cerr << gai_strerror(rc) << std::endl;
        return -1;
    }

    
    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


    server_addr.sin_family = AF_UNSPEC;

    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[2]));

    sendto(sd, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&server_addr, server_len);
    bytes = recvfrom(sd,buffer,79 * sizeof(char), 0,(struct sockaddr *) &server_addr, &server_len);
    buffer[bytes]= '\0';
    std::cerr << buffer << endl;
    freeaddrinfo(res);
    return 0;
}