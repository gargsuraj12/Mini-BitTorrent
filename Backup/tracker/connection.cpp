#include "connection.h"

using namespace std;

int getTCPClientConnection(){
	int sockfd; 
    struct sockaddr_in servaddr; 
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
		return -1;
        //exit(0); 
    } 
    else{
		printf("Socket successfully created..\n"); 
	}
	bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.10"); 
	//servaddr.sin_addr.s_addr = inet_addr("127.0.1.2");
    servaddr.sin_port = htons(PORT); 
	//localaddr.sin_port = 0;  // Any local port will do
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  
    // connect the client socket to server socket 
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    }  else{
		printf("connected to the server..\n"); 
	}
    return sockfd;
}
 
int getTCPServerConnection(int connection[]){
	int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
		return -1;
        //exit(0); 
    } 
    else{
		printf("Socket successfully created..\n"); 
	}
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.10"); 
    servaddr.sin_port = htons(PORT); 
  
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed...\n"); 
		return -1;
        //exit(0); 
    } 
    else{
		printf("Socket successfully binded..\n"); 
	}
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed...\n"); 
		return -1;
        //exit(0); 
    } 
    else{
		printf("Server listening..\n"); 
	}
    len = sizeof(cli); 
  
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (struct sockaddr *)&cli, (socklen_t*) &len); 
    if (connfd < 0) { 
        printf("server acccept failed...\n"); 
		return -1;
        //exit(0); 
    } 
    else{
		printf("server acccept the client...\n"); 
	}
	connection[0] = sockfd;
	connection[1] = connfd;	
	return 0;
	//return connfd;
} 
