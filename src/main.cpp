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

int main(int argc, char const *argv[])
{

    int port = atoi(argv[1]);

    char tcp_server_message[400] = "HTTP/1.1 200 OK\r\nContent-Length: 217 Content-Type: text/html; charset=utf-8\r\n\r\n<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"/><title>Highly sensitive data</title></head><body><p> Hello, I am the TCP Server you successfully connected to!! \n\n Bye Bye!!!\n\n</p></body></html>";

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
    char tcp_server_response[256] = "GET / HTTP/1.1\r\nHost: 127.0.0.1:5678\r\nUser-Agent: Go-http-client/1.1\r\nAccept-Encoding: gzip";
    recv(tcp_client_socket, &tcp_server_response, sizeof(tcp_server_response), 0);   // params: where (socket), what (string), how much - size of the server response, flags (0)

    //send data stream
    send(tcp_client_socket, tcp_server_message, strlen(tcp_server_message), 0);  // send where, what, how much, flags (optional)

    //close the socket
    close(tcp_server_socket);

    return 0;
}


