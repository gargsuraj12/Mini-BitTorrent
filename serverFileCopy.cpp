#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
 
#define MAX 524289
#define PORT 8080 
#define SA struct sockaddr 
#define CHUNK_LEN 524288
  
 using namespace std; 
  
// Function designed for chat between client and server. 
void chatFunc(int connfd) { 
    char buff[MAX]; 
    int n; 
    // infinite loop for chat 
    while (true) { 
        bzero(buff, MAX); 
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
		;
		//cin>>msg;
  
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        } 
    } 
} 

 int recvFileInChunks(int connfd, string filename){
	 cout<<"Desti socket is:"<<connfd<<endl;
	//Receive chunks from client and write them in a file
	char buffer[MAX];
	int chunkNum = 0,status;
	FILE *destFile;
	string chunk,ack;
	
	destFile = fopen((char *)filename.c_str(), "wb");
	if(destFile == NULL){
		fprintf(stderr, "Unable to copy file '%s'\n", (char *)filename.c_str());
		return -1;
	}
	int len = 0;
	while(true){
		bzero(buffer, MAX); 
		len = read(connfd, buffer, CHUNK_LEN); 
		
		if(len > 0){
			cout<<"Current chunk num is: "<<chunkNum<<" and size is:"<<len<<endl;
			status = fwrite(buffer, 1, len, destFile);
			if(status > 0){
				ack = "Chunk "+to_string(chunkNum)+" received successfully at destinatoin..";
			}else{
				ack = "Error while writing chunk "+to_string(chunkNum)+" at destination..";
			}
		}
		if(len == 0){
			cout<<"File Copied successfully..with last chunk: "<<chunkNum<<endl;
			break;
		}
		chunkNum++;
	}
	fclose(destFile);
	return 0;
}
 // Driver function 
int main() 
{ 
    int sockfd, connfd, len; 
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
  
    // Function for chatting between client and server 
    //chatFunc(connfd); 
	string filename = "/home/suraj/Desktop/OS/MysticMovers.mp4";
	recvFileInChunks(connfd, filename);
	//sleep(10);
    close(sockfd); 
}