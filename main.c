#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(void) {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int s, newfd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    getaddrinfo(NULL, "80", &hints, &res);

    s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(s, res->ai_addr, res->ai_addrlen);

    listen(s, 5);

    printf("%s\n", "Listening:");


    FILE *INFD = fopen("index.html", "r"); 
    char message[1024];
    fseek(INFD, 0, SEEK_END);
    long MSGLEN = ftell(INFD);
    fseek(INFD, 0, SEEK_SET);

    fread(message, 1, MSGLEN, INFD);

    while(1) {
    addr_size = sizeof(their_addr);
    newfd = accept(s, (struct sockaddr *)&their_addr, &addr_size);

    //char *sendthis = "Benis.";
    int len, bytes_sent;

    len = strlen(message);
    bytes_sent = send(newfd, message, len, 0);
    printf("Sent %d bytes\n", bytes_sent); 
    shutdown(newfd, 2);
    }
}
