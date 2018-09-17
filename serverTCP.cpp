//#include <netdb.h> 
//#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
//#include <sys/socket.h> 
//#include <sys/types.h>
//#include <arpa/inet.h>
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
void serverChat(int sockfd) { 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    while (true) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
		;
		//cin>>msg;
  
        // and send that buffer to client 
        write(sockfd, buff, sizeof(buff)); 
  
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

/*
struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
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
	//servaddr.sin_addr.s_addr = htonl("192.168.0.11"); 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded..\n"); 
  
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
        exit(0); 
    } 
    else
        printf("Server listening..\n"); 
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, (socklen_t*) &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
    } 
    else
        printf("server acccept the client...\n"); 
*/