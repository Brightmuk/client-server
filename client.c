#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
# include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if(argc <3){
        fprintf(stderr,"Usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <0)
        error("Error opening socket");
        server = gethostbyname(argv[1]);
        if(server == NULL){
            fprintf(stderr, "Error, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server ->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server ->h_length);
        serv_addr.sin_port = htons(portno);
        if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
            error("Error connecting");
        printf("Client: ");
        while(1)
        {
            bzero(buffer, 256);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));
            if(n < 0)
                error("Error writing to socket");
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);
            if(n < 0)
                error("Error reading from socket");
            printf("Server: %s\n", buffer);

            int i = strncmp("Bye", buffer, 3);
            int j = strncmp("Send file", buffer, 9);

            if(j==0){
               printf("File transfer....");
               FILE * f;
               int words = 0;
               char c;

               f = fopen("glad.txt", "r");
               while((c = getc(f)) != EOF)
               {
                   fscanf(f, "%s", buffer);
                   if(isspace(c) || c=='\t')
                    words++;
               }
               write(sockfd, &words, sizeof(int));
               rewind(f);
               char ch;
               while(ch != EOF)
               {
                   fscanf(f, "%s", buffer);
                   write(sockfd, buffer, 255);
                   ch = fgetc(f);
               }
               printf("The file has been sent successfully");
            }

            if(i==0)
                break;
        }
        close(sockfd);
        return 0;
}