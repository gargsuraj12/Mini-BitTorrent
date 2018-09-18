#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "connection.h"
#define MAX 515 

using namespace std;

typedef struct entry{
  string hash;
  string fileName;
  string clientIP;
  int clientPort;
}tEntry;

//vector <pair <string, string> > dataVec;

vector <tEntry> dataVec;

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

int addSeederIntoList(string fileName, string seederIP, int seederPort, string fileHash){
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

int deleteSeederFromList(string seederIP, int seederPort){
	for(auto entry = dataVec.begin(); entry != dataVec.end(); ++entry){
		if( (*entry).clientIP == seederIP  && (*entry).clientPort == seederPort){
			dataVec.erase(entry);
		}
	}
	return 0;
}

tEntry prepareStructFromLine(char * line){
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

int loadSeederList(string filePath){
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

void clientHandler(){
	int connect[2];
	getTCPWriteConnection(connect);
	int sockfd, connfd;
	sockfd = connect[0];
	connfd = connect[1];
    serverChat(connfd); 
    close(sockfd);
}

int main(int argc, char **argv){
	if(argc < 5) {
		cout<<"Insufficient arguements..\n"<<endl;
		exit(1); 
	}
	string thisTackerURL = argv[1];
	string otherTackerURL = argv[2];
	string seederFile = argv[3];
	string logFile = argv[4];
	loadSeederList(seederFile);
	
	printDataVec();
	
	return 0;
}
