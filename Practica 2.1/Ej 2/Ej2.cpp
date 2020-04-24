#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <time.h>



#include <iostream>
using namespace std;

int main(int argc, char **argv){
    struct addrinfo hints;
    struct addrinfo *res;

    char buffer[80];


    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);
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

    if(bind(sd,(struct sockaddr *)res->ai_addr,res->ai_addrlen) != 0)
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    //Recepcion mensaje cliente
    int c = 1;

    time_t rawtime;
    struct tm * timeinfo;


    char message[80];
    while(c == 1){

        bytes = recvfrom(sd,buffer,79 * sizeof(char), 0,(struct sockaddr *) &client_addr, &client_len);
        buffer[bytes]= '\0';
        if(bytes != -1){

            getnameinfo(res->ai_addr, res->ai_addrlen,host,NI_MAXHOST,service,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
            std::cerr << bytes << " bytes de " << inet_ntoa(client_addr.sin_addr)<< ":" << ntohs(client_addr.sin_port) << endl;

        }

    
        switch(buffer[0])
        {
            case 'q': 
            c = 0;
            break;
            case 't':
            time(&rawtime);
            timeinfo = localtime (&rawtime);
            strftime(message,80, "%r",timeinfo);
            sendto(sd, message, strlen(message), 0, (struct sockaddr *)&client_addr, client_len);
            break;
            case 'd':
            time(&rawtime);
            timeinfo = localtime (&rawtime);
            strftime(message,80, "%F",timeinfo);
            sendto(sd, message, strlen(message), 0, (struct sockaddr *)&client_addr, client_len);
            break;
            default:
            memset(message,0,80);
            std::cerr << "Comando no soportado X" << endl;
            break;
        }

        
      
    }
    freeaddrinfo(res);
    std::cerr << "Saliendo... " << endl;
    return 0;
}