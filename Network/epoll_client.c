#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>

int main() 
{ 
    int client_sockfd; 
    int len; 
    struct sockaddr_in address;//Server-side Network Address Structures 
     int result; 
    char str1[] = "ABCDE"; 
    char str2[] = "ABCDEFGHIJK"; 
    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);//Set up client socket 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(6336); 
    len = sizeof(address); 
    result = connect(client_sockfd, (struct sockaddr *)&address, len); 
    if(result == -1) 
    { 
         perror("oops: client2"); 
         exit(1); 
    } 
    //First reading and writing
    write(client_sockfd, str1, sizeof(str1)); 

    sleep(5);
    
    //Second reading and writing
    write(client_sockfd, str2, sizeof(str2)); 

    
    close(client_sockfd); 
   
    return 0; 
}
