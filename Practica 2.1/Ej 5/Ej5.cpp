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
    struct addrinfo res;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;

    hints.ai_protocol = 6;


    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);


    if(rc != 0)
    {
        std::cerr << gai_strerror(rc) << std::endl;
        return -1;
    }

   int sd = socket(res->ai_family, res->ai_socktype,res->ai_protocol);

    int connected = 3;
    connected = connect(sd,  res->ai_addr, res->ai_addrlen);

    if(connected != 0)
    {
        std::cerr << strerror(connected) << std::endl;
    }

    char buffer[80];
    ssize_t bytes = 0 ;

    int c = 1;

    while(c == 1){

        std::cin >> buffer;
 
        if (buffer[0] == 'q' && buffer[1] == '\0'){
            c = 0;
        }

        else{
        sendto(sd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
        bytes = recv(sd, (void) buffer, sizeof(char)*79,0);
        buffer[bytes]= '\0';
        std::cout << buffer<< std::endl;
        }

    }

    freeaddrinfo(res);
    return 0;
}