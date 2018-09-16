#include "computeSHA.h"
 
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
	
	for(int i=0; i<20; i++){
		retHash += mdString[i];
	}
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
		if(len < 512){
			chunk += ch;
			//buffer[len] = ch;
			len++;
		}else{
			chunkHash = computeSHAforChunk(chunk);
			totalHash.append(chunkHash);
			len = 0;
			i++;
			chunk.clear();
		}	
	}
	if(len>0 && ch == EOF){
		chunkHash = computeSHAforChunk(chunk);
		totalHash.append(chunkHash);
		chunk.clear();
	}
	//cout<<"Total Chunks are: "<<i<<endl;
	return totalHash;
 }
 
int main(){
	string totalHash;
    totalHash = computeSHAforFile("/home/suraj/Desktop/OS/Assignment2/datafile1.txt");
	cout<<"Total hash of the file is: "<<totalHash<<endl;
	cout<<"Hash of the file hash is: "<<computeSHAforChunk(totalHash)<<endl;
    return 0;
}