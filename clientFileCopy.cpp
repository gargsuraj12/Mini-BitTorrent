#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
 #define MAX 524289
#define PORT 8080 
#define SA struct sockaddr 
#define CHUNK_LEN 524288
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
	cout<<"Source socket is: "<<sockfd<<endl;
   	string chunk;
	int len,chunkNum=0;
	char ch, buffer[MAX];
	FILE *srcFile = fopen((char *)filename.c_str(), "rb");
	if(srcFile == NULL){
		cout<<"Unable to open file "<<filename<<endl;
		return -1;
	}
	
	while (true){	
		
		bzero(buffer, sizeof(buffer)); 
		len = fread(buffer,1,CHUNK_LEN,srcFile);
		/*if(len < 0){
			cout<<"Error while sending the chunk: "<<chunkNum<<endl;
			fclose(srcFile);
			return -1;
		}*/	
		if(len > 0){
			cout<<"Sent chunk size is: "<<len<<" & num is: "<<chunkNum<<endl;
			int status= write(sockfd, buffer, len); 
			cout<<"bytes written on socket are: "<< status << " " << strerror(errno) << endl;
		}
		if(len < CHUNK_LEN){
			cout<<"File sent to client.."<<endl;
			break;
		}
		chunkNum++;
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
	//string filename = "/home/suraj/Desktop/OS/t.mp4";
	string filename = "/home/suraj/Desktop/Movies/MysticMovers.mp4";
	sendFileInChunks(sockfd, filename);
    // close the socket 
    close(sockfd); 
}