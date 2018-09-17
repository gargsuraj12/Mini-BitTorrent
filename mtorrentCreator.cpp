#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/stat.h> 
#include <openssl/sha.h>
 
using namespace std;
 
string computeSHAforChunk(string str){
	unsigned char digest[SHA_DIGEST_LENGTH];
	string retHash;
	SHA1((unsigned char*)str.c_str(), str.length(), (unsigned char*)&digest);
	if(strlen((char *)digest) == 0){
		cout<<"Unable to calculate hash"<<endl;
		return NULL;
	}
   char mdString[SHA_DIGEST_LENGTH*2+1];

   for(int i = 0; i < SHA_DIGEST_LENGTH; i++){
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	}
	retHash = mdString;
	
	/*for(int i=0; i<20; i++){
		retHash += mdString[i];
	}*/
	//cout<<retHash<<endl;
	return retHash;
}
 
 string computeSHAforFile(string filename){
	string chunk, totalHash, chunkHash;
	int len,i=1;
	char ch;//, buffer[MAX];
	FILE *srcFile = fopen((char *)filename.c_str(), "r");
	if(srcFile == NULL){
		cout<<"Unable to open file "<<filename<<endl;
		return NULL;
	}
	len = 0;
	while ((ch = fgetc(srcFile)) != EOF){
		if(len < 524288){//
			chunk += ch;
			//buffer[len] = ch;
			len++;
		}else{
			chunkHash = computeSHAforChunk(chunk);
			chunkHash = chunkHash.substr(0,20);
			totalHash.append(chunkHash);
			len = 0;
			i++;
			chunk.clear();
		}	
	}
	if(len>0 && ch == EOF){
		chunkHash = computeSHAforChunk(chunk);
		chunkHash = chunkHash.substr(0,20);
		totalHash.append(chunkHash);
		chunk.clear();
	}
	cout<<"Total Chunks are: "<<i<<endl;
	return totalHash;
 }
 
 int prepareMTorrent(string ip1, int port1, string ip2, int port2, string dataFilePath){ //, string mtFilePath
	string mtFilePath = dataFilePath;
	mtFilePath.append(".mtorrent");
	FILE *tFile = fopen((char *)mtFilePath.c_str(), "w");
	if(tFile == NULL){
		cout<<"Unable to open file "<<mtFilePath<<endl;
		return -1;
	}
	struct stat filestat;
	stat((char *)dataFilePath.c_str(), &filestat);
	long double fileSize = filestat.st_size;
	
	string totalHash;
	totalHash = computeSHAforFile(dataFilePath);
	/*Writing values in .mtorrent file*/
	fprintf(tFile, "%s:%d\n",(char *)ip1.c_str(), port1);
	//fprintf(tFile, "%s:%d\n",(char *)ip2.c_str(), port2);
	fprintf(tFile, "%s\n",(char *)dataFilePath.c_str());
	fprintf(tFile, "%.0Lf\n", fileSize);
	fprintf(tFile, "%s\n", (char *)totalHash.c_str());
	
	fclose(tFile);
	return 0;
 }
 
 int main(){
	string dataFilePath = "/home/suraj/Desktop/OS/Assignment2/BloodDonationChitta.png";	
	//string mtFilePath = "/home/suraj/Desktop/OS/Assignment2/datafile1.mtorrent";
	/*struct stat filestat;
	stat((char *)dataFilePath.c_str(), &filestat);
	long double fileSize = filestat.st_size;*/
	int rv = prepareMTorrent("127.0.0.1", 80, "127.0.0.2", 80, dataFilePath);//, mtFilePath);
	if(rv == 0){
		cout<<".mtorrent file successfully prepared"<<endl;
	}else{
		cout<<"Error while preparing.mtorrent file"<<endl;
	}
	//cout<<"File size is: "<<fileSize<<endl;
	return 0;
}