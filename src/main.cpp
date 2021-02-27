/**
 * @brief Main function of the program. It is called like this "program <port> <encryptedMessage>",
 * where <port> means the address of the webserver and <encryptedMessage> is the message to show
 * 
 * @param argc number of arguments
 * @param argv pointer to arguments
 * @return int 0 if everything is ok
 */

#include <stdio.h>          //Standard library
#include <stdlib.h>         //Standard library
#include <sys/socket.h>     //API and definitions for the sockets
#include <sys/types.h>      //more definitions
#include <netinet/in.h>     //Structures to store address information
#include <unistd.h>         //close function
#include <string.h>         //strlen
#include <string>
#include <cstring>
#include <iostream>

int main(int argc, char const *argv[])
{

    int port = atoi(argv[1]);
    
    std::string message(argv[2]);
    std::string http1 = "HTTP/1.1 200 OK\r\nContent-Length: 217\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
    std::string http2 = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>Highly sensitive data</title></head><body><p>";
    std::string http3 = "</p></body></html>";

    std::string fullMessage = http2 + message + http3;
    int content_length = fullMessage.length();
    fullMessage = http1 + fullMessage;
    fullMessage.replace(33,3, std::to_string(content_length));

    char tcp_server_message[2048];
    strcpy(tcp_server_message, fullMessage.c_str());
    
    //printf("%s \n", tcp_server_message);

    char tcp_server_message_invalid[2048] = "HTTP/1.1 404 Object Not Found\r\nContent-Length: 210\r\nContent-Type: text/html; charset=utf-8\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>Highly sensitive data</title></head><body><p>Not found</p></body></html>";

    //printf("%s \n", tcp_server_message_invalid);

    //create the server socket
    int tcp_server_socket;                                  //variable for the socket descriptor
    tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);    //calling the socket function. Params: Domain of the socket (Internet in this case), type of socket stream (TCP), Protocol (default, 0 for TCP)

    //define the server address
    struct sockaddr_in tcp_server_address;                  //declaring a structure for the address
    tcp_server_address.sin_family = AF_INET;                //Structure Fields' definition: Sets the address family of the address the client would connect to
    tcp_server_address.sin_port = htons(port);             //Passing the port number - converting in right network byte order
    tcp_server_address.sin_addr.s_addr = INADDR_ANY;        //Connecting to 0.0.0.0

    // binding the socket to the IP address and port
    bind(tcp_server_socket, (struct sockaddr *) &tcp_server_address, sizeof(tcp_server_address));  //Params: which socket, cast for server address, its size

    //listen for simultaneous connections
    listen(tcp_server_socket, 5);  //which socket, how many connections

    int tcp_client_socket;
    tcp_client_socket = accept(tcp_server_socket, NULL, NULL);  // server socket to interact with client, structure like before - if you know - else NULL for local connection

    //recieve data stream
    char tcp_request[2048];
    recv(tcp_client_socket, &tcp_request, sizeof(tcp_request), 0);   // params: where (socket), what (string), how much - size of the server response, flags (0)

    //printf("%s \n", tcp_request);

    //check that /message exists in tcp_request to know to send a valid or invalid server response
    std::string request(tcp_request);
    std::string findMessage = "message";
    const char* found = strstr(request.c_str(),findMessage.c_str());

    //std::cout << request << std::endl;

    if(found){
        //send data stream
        send(tcp_client_socket, tcp_server_message, strlen(tcp_server_message), 0);  // send where, what, how much, flags (optional)
    }
    else{
        //send invalid data stream
        send(tcp_client_socket, tcp_server_message_invalid, strlen(tcp_server_message_invalid), 0);  // send where, what, how much, flags (optional)
    }

    //close the socket
    close(tcp_server_socket);

    return 0;
}


