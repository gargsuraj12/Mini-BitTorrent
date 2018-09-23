#include "computeSHA.h"
 
#define CHUNK_SIZE 524288 
 
 static int cnt = 0; 
 
string computeSHAforChunk(char *chunk){//string str){
	unsigned char digest[SHA_DIGEST_LENGTH];
	string retHash;
	SHA1((unsigned char*)chunk, strlen(chunk), (unsigned char*)&digest);
	if(strlen((char *)digest) == 0){
		cout<<"Unable to calculate hash"<<endl;
		return NULL;
	}
   char mdString[SHA_DIGEST_LENGTH*2+1];
	cnt++;
   for(int i = 0; i < SHA_DIGEST_LENGTH; i++){
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
	}
	retHash = mdString;
	//cout<<"Hash for "<<cnt<<" is:"<<retHash<<endl;
	return retHash;
}
 
string computeSHAforFile(string filename){
	string totalHash, chunkHash;
	char chunk[CHUNK_SIZE+1];
	int i=0;
	FILE *srcFile = fopen((char *)filename.c_str(), "rb");
	if(srcFile == NULL){
		cout<<"Unable to open file "<<filename<<endl;
		return NULL;
	}
	while(fgets(chunk , CHUNK_SIZE , srcFile)){
   	fseek ( srcFile, CHUNK_SIZE, SEEK_CUR );	
		chunkHash = computeSHAforChunk(chunk);
		chunkHash = chunkHash.substr(0,20);
		totalHash.append(chunkHash);
		i++;
		memset(chunk, 0, CHUNK_SIZE);
	}		
	cout<<"Total Chunks are: "<<i<<endl;
	return totalHash;
}
 
/*int main(){
	string totalHash;
    //totalHash = computeSHAforFile("/home/suraj/Desktop/OS/Assignment2/BeginningLinuxProgramming3rdEd.pdf");
	totalHash = computeSHAforFile("/home/suraj/Desktop/Study/Books/Algo&DS/horowitz-and-sahani-fundamentals-of-computer-algorithms-2nd-edition.pdf");
	cout<<"Total hash of the file is: "<<totalHash<<endl;
	cout<<"Hash of the file hash is: "<<computeSHAforChunk((char *)totalHash.c_str())<<endl;
    return 0;
}*/