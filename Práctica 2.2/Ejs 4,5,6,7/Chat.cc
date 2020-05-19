#include "Chat.h"

void ChatMessage::to_bin()
{
    //Serializar los campos type, nick y message en el buffer _data

    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    char * tmp = _data;

    memcpy(tmp, &type , sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, &nick, sizeof(char) * 8);
    tmp += sizeof(char) * 8;

    memcpy(tmp, &message, 80 * sizeof(char) );
    tmp += 80 *sizeof(char);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data

    char * tmp = _data;

    memcpy(&type, tmp , sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(&nick, tmp, sizeof(char) * 8);
    tmp += sizeof(char) * 8;
    
    memcpy(&message, tmp, sizeof(char) * 80);
    tmp += sizeof(char) * 80;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
        ChatMessage obj;
        Socket* outSocket; 
    while (true)
    {

        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        socket.recv(obj, outSocket);
        switch(obj.type){
            case 0:
            clients.push_back(outSocket);
            break;
            case 1:
            for(Socket* i: clients){
                if (!(*i == *outSocket)){
                    socket.send(obj, *i );
                }
            }
            break;
            case 2:
            int cont = 0;
            int aux;
            for(auto i = clients.begin(); i != clients.end(); i++){
                if (*i == outSocket){
                    aux = cont;
                }
                cont++;
            }
            clients.erase(clients.begin() + aux);
            break;
            
        }
    }
}

void ChatClient::login()
{
    std::string msg;
    msg = "Login";
    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;
    socket.send(em, socket);
    std::cout << "LOGIN" << std::endl;

    //em.type = ChatMessage::MESSAGE;
    //socket.send(em, socket);
}

void ChatClient::logout()
{
  std::string msg;

  ChatMessage em(nick, msg);
  em.type = ChatMessage::LOGOUT;
  socket.send(em, socket);

  std::cout << "LOGOUT" << std::endl;
}

void ChatClient::input_thread()
{
    while (true)
    {
        std::string msg; 

        std::getline(std::cin, msg);

        ChatMessage em(nick, msg);
        em.type = ChatMessage::MESSAGE;
        
        socket.send(em, socket);
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        ChatMessage em;
         
        socket.recv(em);

        std::cout << em.nick << ": " << em.message << std::endl;
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
    }
}

