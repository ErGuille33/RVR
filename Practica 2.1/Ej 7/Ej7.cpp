#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string>
#include <thread>
#include <vector>



class ThreadTCP
{
    public:
    ThreadTCP(int _sd) {
        sd = _sd;
    }
    ~ThreadTCP(){};
    void haz_mensaje(){

        //Rececpcion mensaje cliente

        char buffer[80];
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        ssize_t bytes;

        while(bytes > 0){

            bytes = recv(sd,(void*)buffer,79 * sizeof(char), 0);
            buffer[bytes]= '\0';
            if(bytes >0){
            std::cout << "Thread número: " <<std::this_thread::get_id() << " recibiendo mensaje" << std::endl;
            }
            sleep(1);

            send(sd, (void*)buffer, strlen(buffer), 0);

        }
        std::cout << "Cerrando el thread número: " << std::this_thread::get_id() << std::endl;
        close(sd);
    }
    
    bool active = true;
    private:
    socklen_t client_len = sizeof(struct sockaddr_in);
    int sd;
}
;

void createThread(){

}

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo *res;

    //INICIALIZACION SOCKET Y BIND 

    memset(&hints, 0 ,sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if(rc != 0){
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    //res contiene la representacion como sockaddr de dirección + PUERTO

    int sd =  socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if(bind(sd,res->ai_addr,res->ai_addrlen) != 0)
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }
    freeaddrinfo(res);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);

    listen(sd,16);
    while(true)
    {
        int sd_client = accept(sd, (struct sockaddr *) &client_addr, &client_len); 
        ThreadTCP tcp(sd_client);
        std::cout << "Conexion desde IP: " << inet_ntoa(client_addr.sin_addr) << " PUERTO: " << ntohs(client_addr.sin_port) << std::endl;
        std::thread t(&ThreadTCP::haz_mensaje,tcp);
        t.detach();
    }
    
    return 0;
}