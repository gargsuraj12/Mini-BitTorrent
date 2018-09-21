#include <string.h> 
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "connection.h"


#define MAX 515 
#define PORT 8080 
#define SA struct sockaddr 
#define CHUNK_LEN 512
  
using namespace std; 

typedef struct entry{
  string hash;
  string fileName;
  string clientIP;
  int clientPort;
}tEntry; 

vector <tEntry> dataVec;

vector<string> getSeederListForHash(string fileHash){
	vector<string> seederList;
	string url;
	for(auto it : dataVec){
		if(it.hash == fileHash){
			url.append(it.clientIP);
			url.append(":");
			url.append(to_string(it.clientPort));
			seederList.push_back(url);
		}
	}
	return seederList;
}

int addFileIntoList(string fileName, string seederIP, int seederPort, string fileHash){
	string url;
	tEntry entry;
	entry.fileName = fileName;
	entry.hash = fileHash;
	entry.clientIP = seederIP;
	entry.clientPort = seederPort;
	/*url.append(seederIP);
	url.append(":");
	url.append(seederPort);
	dataVec.push_back(make_pair(fileHash, url));*/
	dataVec.push_back(entry);
	return 0;
}

int removeSeederFromList(string seederIP, int seederPort){
	for(auto entry = dataVec.begin(); entry != dataVec.end(); ++entry){
		if( (*entry).clientIP == seederIP  && (*entry).clientPort == seederPort){
			dataVec.erase(entry);
		}
	}
	return 0;
}

int removeFileFromList(string fileHash, string seederIP, int seederPort){
	for(auto entry = dataVec.begin(); entry != dataVec.end(); ++entry){
		if( (*entry).clientIP == seederIP  && (*entry).clientPort == seederPort && (*entry).hash == fileHash){
			dataVec.erase(entry);
		}
	}
	return 0;
}

tEntry prepareStructFromLine(char *line){
	tEntry entry;
	char *token;
	token = strtok (line," ");
	int i=0;
	while (token != NULL && i<=3){
		i++;
		if(i ==1){
			entry.fileName = token;
		}
		if(i ==2){
			entry.hash = token;
		}
		if(i == 3){
			string url = token;
			int pos = url.find(":");
			entry.clientIP = url.substr(0, pos);
			entry.clientPort = stoi(url.substr(pos+1));
		}
		token = strtok (NULL, " ");
	}
	return entry;
}

vector<string> splitLine(string line, string delimiter){
	//cout<<"Message passed to split fun is: "<<line<<" and delimiter is: '"<<delimiter<<"'"<<endl;
	vector<string> splitV;
	string token;
	int index = line.find(delimiter);
	while(index != -1){
		token = line.substr(0, index);
		splitV.push_back(token);
		line.erase(0, index+1);
		index = line.find(delimiter);
		//cout<<"Next token is :"<<token<<endl;
	}
	if(!line.empty()){
		splitV.push_back(line);
		//cout<<"Next token is :"<<line<<endl;
	}
	return splitV;
}

int loadInitialSeeders(string filePath){
	tEntry entry;
	char * line = NULL;
    size_t len = 0;
    int size;
	FILE *srcFile = fopen((char *)filePath.c_str(), "rb");
	if(srcFile == NULL){
		cout<<"Unable to open file "<<filePath<<endl;
		return -1;
	}
	while ((size = getline(&line, &len, srcFile)) != -1) {
		entry = prepareStructFromLine(line);
		dataVec.push_back(entry);
    }
    fclose(srcFile);
	return 0;
}

int printDataVec(){
	if(dataVec.size() == 0){
		cout<<"Data vector is currently empty.."<<endl;
		return -1;
	}
	for(auto it : dataVec){
		cout<<it.hash<<"\t"<<it.fileName<<"\t"<<it.clientIP<<"\t"<<it.clientPort<<endl;
	}
}

void printVec(vector<string> vec){
	for(auto it : vec){
		cout<<it<<endl;
	}
}

string processCommand(vector<string> msgVec){
	string ack,url,ip;
	int pos;
	string command = msgVec[0];
	int status;
	// Process the command
	if(command == "addfile") {
		url = msgVec[3];
		pos = url.find(":");
		ip = url.substr(0, pos);
		int port = stoi(url.substr(pos+1));
		status = addFileIntoList(msgVec[1], ip, port, msgVec[2]);
		if(status == 0){
			ack = "File added successfully at tracker...";
		}else{
			ack = "unable to add file at tracker...";
		}
	} else if(command == "removefile"){
		url = msgVec[2];
		pos = url.find(":");
		ip = url.substr(0, pos);
		int port = stoi(url.substr(pos+1));
		cout<<"ip is:"<<ip << " and port is:"<<port<<endl;
		status = removeFileFromList(msgVec[1], ip, port);
		if(status == 0){
			//cout<<"File added successfully at tracker..."<<endl;
			ack = "File removed successfully from tracker...";
		}else{
			//cout<<"unable to add file at tracker..."<<endl;
			ack = "unable to remove file from tracker...";
		}
	} else if(command == "removeseeder"){
		
	} else if(command == "getseederlist"){
		
	} else {
		ack = "Invalid command..";
	}
	return ack;
}

// Function designed for chat between client and server. 
void serverChat(int connfd) { 
    char buff[MAX]; 
    int n; 
	string command, message,ack;
	vector<string> msgVec;	
    // infinite loop for chat 
    while (true) { 
        bzero(buff, MAX); 
  
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
		message = buff;
		msgVec = splitLine(message, " ");
		if(!msgVec.empty()){
			//call processCommand here
			//cout<<"msg vec not empty\n";
			ack = processCommand(msgVec);
			printDataVec();
		}
        // copy server message in the buffer 
        
		/*while ((buff[n++] = getchar()) != '\n') 
		;
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); */
		//ack = "received at server\n";
		bzero(buff, MAX); 
        n = 0; 
		write(connfd, (char *)ack.c_str(), ack.length()); 
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit...\n"); 
            break; 
        }
		message.clear();
		msgVec.clear();
		ack.clear();	
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
int main(int argc, char **argv) {
	if(argc < 5) {
		cout<<"Insufficient arguements..\n"<<endl;
		exit(1); 
	}
	string thisTackerURL = argv[1];
	string otherTackerURL = argv[2];
	string seederFile = argv[3];
	string logFile = argv[4];
	int status = loadInitialSeeders(seederFile);
	printDataVec();
	if(status != -1){
		cout<<"Initial seeders loaded successfully.."<<endl;
	}else{
		cout<<"Failed to load initial seeders from file.."<<endl;
		exit(1);
	}
	
	int connection[2];
	getTCPWriteConnection(connection);
	int sockfd, connfd;
	sockfd = connection[0];
	connfd = connection[1];
    serverChat(connfd); 
    close(sockfd);
	return 0;
}
