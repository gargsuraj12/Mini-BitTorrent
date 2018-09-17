#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "connection.h"


using namespace std;

typedef struct entry{
  string hash;
  string fileName;
  string clientIP;
  int clientPort;
}tEntry;

//vector <pair <string, string> > dataVec;

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

void printDataVec(){
	for(auto it : dataVec){
		cout<<it.hash<<"\t"<<it.fileName<<"\t"<<it.clientIP<<"\t"<<it.clientPort<<endl;
	}
}

void printVec(vector<string> vec){
	for(auto it : vec){
		cout<<it<<endl;
	}
}

int main(){
	
	return 0;
}

/*
dataVec.push_back(make_pair("487accf534abd342fe54", "1.2.3.14:8080"));
	dataVec.push_back(make_pair("487accf5242342fe54cb", "1.2.3.62:1979"));
	dataVec.push_back(make_pair("487accf534abd342fe54", "1.2.3.10:1500"));
	dataVec.push_back(make_pair("487accf545565d342fe5", "1.2.3.78:2045"));
	dataVec.push_back(make_pair("487accf55647bff2fe5a", "1.2.3.17:4859"));
	dataVec.push_back(make_pair("487accf5242342fe54cb", "1.2.3.15:1020"));
	dataVec.push_back(make_pair("487accf545565d342fe5", "1.2.3.60:7845"));
	dataVec.push_back(make_pair("487accf534abd342fe54", "1.2.3.48:8010"));

	cout<<"Old data vec"<<endl;
	printDataVec();
	//vector<string> seeders = getSeederListForHash("487accf534abd342fe54");
	//printVec(seeders);

	cout<<"data vec after adding a seeder"<<endl;
	addSeederIntoList("1.2.3.56", "1515", "415accf53df8d342fe5c");
	printDataVec();

	cout<<"data vec after deleting a seeder"<<endl;
	deleteSeederFromList("1.2.3.60:7845");
	printDataVec();
*/
