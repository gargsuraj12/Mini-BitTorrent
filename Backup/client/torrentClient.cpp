#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <iostream>
#include <vector>
#include <string>

#include "utilities.h"
#include "connection.h"
#include "mtorrentCreator.h"
#include "computeSHA.h"

#define MAX 524289
#define CHUNK_LEN 524288

using namespace std;
 
static string myIP;
static int myPort;
static string tracker1IP;
static int tracker1Port; 
static string tracker2IP;
static int tracker2Port; 
static string myURL;

 
int recvFileInChunks(string filename){//int connfd, 
	 int connection[2];
	getTCPServerConnection(connection);
	int sockfd, connfd;
	sockfd = connection[0];
	connfd = connection[1];
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
	close(sockfd);
	return 0;
}
   
int sendFileInChunks(string filename){//int sockfd, 
	int sockfd = getTCPClientConnection(); 
   	string chunk;
	int len,chunkNum=0;
	char buffer[MAX];
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
		if(len < CHUNK_LEN){	// || buffer[len-1] == EOF
			cout<<"File sent to client.."<<endl;
			break;
		}
		chunkNum++;
	}
	fclose(srcFile);
	close(sockfd);
	return 0;	
}  

int shareFileDetailsToTracker(string filePath, string hashOfHash){
	vector<string> mv;
	mv = splitLine(filePath, "/");
	string fileName = mv[mv.size()-1];
	int sockfd = getTCPClientConnection();
	char buffer[1000];
	//addfile file7.mkv 9ahna3ee5e6bfb0d325566579520b2c124894edf 1.2.3.50:7842
	string command = "addfile "+fileName+" "+hashOfHash+" "+myIP+":"+to_string(myPort);
	write(sockfd, (char *)command.c_str(), command.length()); 
    bzero(buffer, sizeof(buffer)); 
    read(sockfd, buffer, sizeof(buffer));
	cout<<"Reply from tracker: "<<buffer<<endl;
	close(sockfd);
	return 0;
}

int processCommand(){
	string cStr,command,filePath;
	vector<string> cVec;
	int status;
	while(true){
		cout<<"Enter the command: ";
		getline(cin, cStr);
		cVec = splitLine(cStr, " ");
		if(cVec.empty()){
			cout<<"Unable to process the command"<<endl;
			return -1;	
		}
		command = cVec[0];
		if(command == "share"){
			filePath = cVec[1];
			//first create .mtorrent file for the given file
			string totalHash = prepareMTorrent(tracker1IP,  tracker1Port,  tracker2IP,  tracker2Port,  filePath);
			if(totalHash.empty()){
				cout<<"Unable to create .mtorrent file for the given file.."<<endl;
				return -1;
			}
			string hashOfHash = computeSHAforChunk((char *)totalHash.c_str());
			status = shareFileDetailsToTracker(filePath, hashOfHash);
		} else if(command == ""){
			
		} else if(command == ""){
			
		} else if(command ==""){
			
		} else if(command == ""){
			
		} else if(command == ""){
			
		} else {
			cout<<"Invalid command.."<<endl;
		}
	}
}

int initializeClient(char **argv){
	vector<string> mv;
	//myURL = argv[1];
	mv = splitLine(argv[1], ":");
	myIP  = mv[0];
	myPort = stoi(mv[1]);
	
	mv = splitLine(argv[2], ":");
	tracker1IP  = mv[0];
	tracker1Port = stoi(mv[1]);
	
	mv = splitLine(argv[3], ":");
	tracker2IP  = mv[0];
	tracker1Port = stoi(mv[1]);
	return 0;
}
  
int main(int argc, char **argv) { 
	if(argc < 5){
		cout<<"Insufficient arguements to initiate the client.."<<endl;
		exit(1);
	}
	initializeClient(argv);
	processCommand();
	return 0;
}