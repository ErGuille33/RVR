#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>



#include <iostream>
using namespace std;

int main(int argc, char **argv){
    struct addrinfo hints;
    struct addrinfo *res;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;

    hints.ai_protocol = 0;
  

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);


    if(rc != 0)
    {
        std::cerr << gai_strerror(rc) << std::endl;
        return -1;
    }
    struct addrinfo *rp;
    int aux;
    for (rp = res; rp != NULL; rp = rp->ai_next){
        
       aux = getnameinfo(rp->ai_addr, rp->ai_addrlen,host,NI_MAXHOST,service,NI_MAXSERV,NI_NUMERICHOST | NI_NUMERICSERV);
       if (aux != 0){
           std::cerr << "Error: Name or service not known" << std::endl;
       }
       else { 
           std::cerr << host << "  " << rp->ai_family << "  " << rp->ai_socktype << endl;
       }
        
    }


    return 0;
}