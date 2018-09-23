#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <netinet/in.h> 
#include <sys/types.h>
 
#define PORT 8080 

int getTCPClientConnection();
int getTCPServerConnection(int []);
 