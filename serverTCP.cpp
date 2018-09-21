#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <iostream>
#include <string>
#include "connection.h"
 
 
#define MAX 515 
#define PORT 8080 
#define SA struct sockaddr 
#define CHUNK_LEN 512
  
 using namespace std; 
  
// Function designed for chat between client and server. 
void serverChat(int connfd) { 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    while (true) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
       
        // copy server message in the buffer 
        
		/*while ((buff[n++] = getchar()) != '\n') 
		;
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); */
		string ack = "received at server\n";
		bzero(buff, MAX); 
        n = 0; 
		write(connfd, (char *)ack.c_str(), ack.length()); 
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

int recvFileInChunks(int sockfd){
	//Receive chunks from client and write them in a file
	char buffer[MAX];
	FILE *destFile;
	string chunk;
	string filename = "/home/suraj/Desktop/OS/Assignment2/serverDataFile.txt";
	destFile = fopen((char *)filename.c_str(), "a");
	if(destFile == NULL){
		fprintf(stderr, "Unable to copy file '%s'\n", (char *)filename.c_str());
		return -1;
	}
	while(true){
		int len = read(sockfd, buffer, MAX); 
		//chunk = buffer;
		//int len = chunk.length();
		for(int i=0; i<len; i++){
			//fputc(chunk[i], destFile);
			if(buffer[i] == EOF){
				break;
			}
			fputc(buffer[i], destFile);
		}
		/*if(chunk[len-1] == EOF){
			cout<<"File Copied successfully.."<<endl;
			break;
		}*/
		if(buffer[len-1] == EOF ){
			cout<<"File Copied successfully.."<<endl;
			break;
		}
	}
	fclose(destFile);
	return 0;
}

// Driver function 
int main() { 
	int connection[2];
	getTCPWriteConnection(connection);
	int sockfd, connfd;
	sockfd = connection[0];
	connfd = connection[1];
    serverChat(connfd); 
    close(sockfd);
	return 0;
}
