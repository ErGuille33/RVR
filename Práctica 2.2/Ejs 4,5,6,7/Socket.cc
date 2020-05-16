#include <string.h>


#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase

    struct addrinfo *res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 17;
  

    int rc = getaddrinfo(address, port, &hints, &res);
    
    sa.sin_family =res->ai_family;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa_len = sizeof(struct sockaddr_in);
    sa.sin_port = htons(atoi(port));

    //inet_aton(address,&sa.sin_addr);

    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);



}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr_in sa;
    socklen_t sa_len = sizeof(struct sockaddr_in);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, (struct sockaddr *)&sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }
    
    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    //Serializar el objeto
    //Enviar el objeto binario a sock usando el socket sd
    int aux;
    obj.to_bin();
    
    char *charData = obj.data();

    aux = sendto(sd, charData, obj.size(), 0, (struct sockaddr *)&sock.sa, sock.sa_len);
    std::cout << aux << " Mensaje enviado" << std::endl;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere

    sockaddr_in* sock1 = (struct sockaddr_in *)&s1.sa;
    sockaddr_in* sock2 = (struct sockaddr_in *)&s2.sa;

    //std::cout << sock1->sin_family << std::endl;

    if(sock1->sin_family != sock2->sin_family) return false;

    if(sock1->sin_port != sock2->sin_port) return false;
    
    return true;
    
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};