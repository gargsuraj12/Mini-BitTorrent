// Write CPP code here 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/types.h>
 #include "connection.h"
 
#define PORT 8080 
#define MAX 515 
#define CHUNK_LEN 512

using namespace std;

void clientChat(int sockfd) {
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

void serverChat(int sockfd) { 
    char buff1[MAX]; 
    int n; 
    // infinite loop for chat 
    while (true) { 
        bzero(buff1, MAX); 
  
        // read the message from client and copy it in buffer 
        read(sockfd, buff1, sizeof(buff1)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff1); 
        bzero(buff1, MAX); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff1[n++] = getchar()) != '\n') 
		;
		//cin>>msg;
  
        // and send that buffer to client 
        write(sockfd, buff1, sizeof(buff1)); 
  
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff1, 4) == 0) { 
            printf("Server Exit...\n"); 
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
		if(buffer[len-1] == EOF ){
			cout<<"File Copied successfully.."<<endl;
			break;
		}
	}
	fclose(destFile);
	return 0;
}
 
int main() { 
	int sockfd = getTCPReadConnection();
	cout<<"Client 1 socket is: "<<sockfd<<endl;
    clientChat(sockfd); 
	//string filename = "/home/suraj/Desktop/OS/Assignment2/datafile1.txt";
	//sendFileInChunks(sockfd, filename);
    // close the socket 
    close(sockfd); 
	return 0;
}