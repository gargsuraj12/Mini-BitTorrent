// Write CPP code here 
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define MAX 515 
#define PORT 8080 
#define SA struct sockaddr 
#define CHUNK_LEN 512

using namespace std;

void chatFunc(int sockfd) {
    char buff[MAX]; 
    int n; 
    while (true) { 
        bzero(buff, sizeof(buff)); 
        printf("Enter the string : "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n')
			;
		
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 
  
int sendFileInChunks(int sockfd, string filename){
   	string chunk;
	int len,i=1;
	char ch, buffer[MAX];
	FILE *srcFile = fopen((char *)filename.c_str(), "r");
	if(srcFile == NULL){
		cout<<"Unable to open file "<<filename<<endl;
		return -1;
	}
	len = 0;
	while ((ch = fgetc(srcFile)) != EOF){
		if(len < 512){
			//chunk += ch;
			buffer[len] = ch;
			len++;
		}else{
			//cout<<"Chunk "<<i<<" is:"<<chunk<<"\n\n\n";
			//write(sockfd, chunk, chunk.length()); 
			write(sockfd, buffer, CHUNK_LEN); 
			len = 0;
			i++;
			chunk.clear();
		}	
	}
	if(ch == EOF){
		if(len <  MAX-1){
			buffer[len] = EOF;
			write(sockfd, buffer, len+1); 
		}
	}
	fclose(srcFile);
	return 0;	
}  
  
int main() { 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    }  else{
		printf("connected to the server..\n"); 
	}
    // function for chat 
    //chatFunc(sockfd); 
	string filename = "/home/suraj/Desktop/OS/Assignment2/datafile1.txt";
	sendFileInChunks(sockfd, filename);
    // close the socket 
    close(sockfd); 
}