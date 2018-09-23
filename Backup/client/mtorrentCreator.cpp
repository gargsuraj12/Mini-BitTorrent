#include "mtorrentCreator.h"
#include "computeSHA.h"
 
using namespace std;
 
string prepareMTorrent(string ip1, int port1, string ip2, int port2, string dataFilePath){ //, string mtFilePath
	string mtFilePath = dataFilePath;
	mtFilePath.append(".mtorrent");
	FILE *tFile = fopen((char *)mtFilePath.c_str(), "w");
	if(tFile == NULL){
		cout<<"Unable to open file "<<mtFilePath<<endl;
		return "";
	}
	struct stat filestat;
	stat((char *)dataFilePath.c_str(), &filestat);
	long double fileSize = filestat.st_size;
	
	string totalHash;
	totalHash = computeSHAforFile(dataFilePath);
	/*Writing values in .mtorrent file*/
	fprintf(tFile, "%s:%d\n",(char *)ip1.c_str(), port1);
	fprintf(tFile, "%s:%d\n",(char *)ip2.c_str(), port2);
	fprintf(tFile, "%s\n",(char *)dataFilePath.c_str());
	fprintf(tFile, "%.0Lf\n", fileSize);
	fprintf(tFile, "%s\n", (char *)totalHash.c_str());
	
	fclose(tFile);
	return totalHash;
 }
 
/* 
 int main(){
	string dataFilePath = "/home/suraj/Desktop/OS/Assignment2/MereDholna.mp3";	
	//string mtFilePath = "/home/suraj/Desktop/OS/Assignment2/datafile1.mtorrent";
	//struct stat filestat;
	//stat((char *)dataFilePath.c_str(), &filestat);
	//long double fileSize = filestat.st_size;
	int rv = prepareMTorrent("127.0.0.1", 80, "127.0.0.2", 80, dataFilePath);//, mtFilePath);
	if(rv == 0){
		cout<<".mtorrent file successfully prepared"<<endl;
	}else{
		cout<<"Error while preparing.mtorrent file"<<endl;
	}
	//cout<<"File size is: "<<fileSize<<endl;
	return 0;
}*/