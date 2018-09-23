#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct entry{
  string hash;
  string fileName;
  string clientIP;
  int clientPort;
}tEntry;

void runServer();
vector<string> getSeederListForHash(string);
int addFileIntoList(string , string , int , string);
int removeSeederFromList(string , int );
int removeFileFromList(string , string, int );
tEntry prepareStructFromLine(char *);
vector<string> splitLine(string, string);//char *, char *);
int loadInitialSeeders(string );
int printDataVec();
void printVec(vector<string>);
