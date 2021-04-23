#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;


const int maxFilenameLength = 254;
const string illegalChars = "\\:*?\"<>|"; // 6-1		verificationcomment

//Function getUserFile gets called by int main() and asks the user for filename then checks the input for danger		verificationcomment
//Primary Author:Sean Oushana with work additions from Charles and Jack		verificationcomment
string getUserFile(){
	string filename;
	bool hasIllegal = false;
	int i=0;
	int filenameLength;

    	do {    //cin a filename from user
		getline(cin,filename);
		filenameLength = filename.length();	//record its length
		for(i=0; i<filenameLength; i++) {	//make sure it doesn't contain any illegal characters
			if(illegalChars.find_first_of(filename[i]) != string::npos){ hasIllegal = true; cout<<"This char is illegal"<<filename[i]<<"\n";}	//8-1 Prints statements when invalid input is entered		verificationcomment
			//else if(!isalnum(filename[i])) hasIllegal = true; 	//if it does, ask for a new one
		}
	
        	if(hasIllegal){
			cout << "The filename you entered was not valid. Enter new filename or ctrl-c" << endl; //repeats the process of prompting for a name until the user enters a valid one  8-1   verificationcomment
        		i=0;	
		}
		cin.clear();    //clears the previous input in the case it wasn't a valid filename
        	cin.ignore(256,'\n');   //ignores the previous enter
	} while((cin.fail()) || hasIllegal || (filenameLength >= maxFilenameLength));		//6-1 Reiterates through program till safe input is entered or process exited	1-1 BUFFER OVERRUN		verificationcomment
																					//2-1		verificationcomment
    	//cin.ignore(); //prevents the last enter from being counted as an empty Movie


	return filename;
}

//Function copy() takes src, path, and flags and copies the file to the path then does flag functionality if requested		verificationcomment
//Primary Author:Charles & Jack with additions from Sean		verificationcomment
int copy(string src,string path,int pflag,int iflag){
	ifstream file1(src);
	ofstream file2(path);
	if(file1.good()){
		string line;
		if (file1.good() && file2.good()) {
			while (getline(file1, line)) {
				file2 << line;
				file2 << '\n';
			}
		}
	}
	else {
		cout << "File not owned by process or does not exist, if the file is owned by root";		//4-1  error message doesn't expose too much information		verificationcomment
		cout << "or another user complete the sudo prompt, else check if file exists\n";			
		string strcommand = "sudo ./filecopy "+src+" "+path;
		const char * command = strcommand.c_str();
		system(command);
	}
	file1.close();
	file2.close();
	if(pflag==1){
		string user,group;
		cout << "Enter unix user to own file: ";
		cin >> user;
		cout << "Enter unix group to own file: ";
		cin >> group;
		string permissioncommand = "chown "+user+":"+group+" "+path;
		const char * command = permissioncommand.c_str();
		system(command);
	}
	if(iflag==1){
		//txt file creation here
	}
	return 0;
}

//Int main takes runtime arguments and calls getUserFile if data is missing then calls copy() to copy the files		verificationcomment
int main(int arc, char* argv[]) {
	
	
	cout << "\nFileCopy started: \n";
	
	string src, dest, path, input;
	int checksum=0,permission=0,info=0;
	int flagquestion=0; //Check if user has been asked if flags shall be enabled
	for(int i=1;i<=arc-1;i++){
		//cout << "argv["<< i << "]= " << argv[i] << "\n";
		string temp(argv[i]);
		if(temp.find('-') != std::string::npos){
			flagquestion=1;
			if(temp.find('c') != std::string::npos){
				cout << "checksum flag active\n";
				checksum=1;
			}
			if(temp.find('p') != std::string::npos){
				cout << "permission flag active\n";
				permission=1;
			}
			if(temp.find('i') != std::string::npos){
				cout << "info text file flag active\n";
				info=1;
			}
		}
		else {
			if(src.empty())
				src=argv[i];
			else if(path.empty()){
				string destpath(argv[i]);
				path=destpath+"/"+src;
			}
		}
	}
	if(src.empty()){
		cout << "Enter src file to copy: ";
		src = getUserFile();
	}
	if(path.empty()){
		cout << "Enter full destination path to copy src file to: ";
		dest = getUserFile();
		if(dest.back()=='/')//check if path has a slash at end
			path=dest+src;
		else
			path=dest+"/"+src;
	}
	if(flagquestion==0){
		cout << "Would you like to change the owner and group permissions of file(y/n) ";
		cin >> input;
		if(input=="Y"||input=="y"){
			cout << "permission flag active\n";
			permission=1;
		}
		cout << "Would you like their to be a .txt file in the destination path with";
		cout << "time, date, and original file path of the file transfer for future notice(y/n) ";
		cin >> input;
		if(input=="Y"||input=="y"){
			cout << "info text file flag active\n";
			info=1;

			//.txt file maker with current date and time
			std::ofstream outfile ("summary.txt");
			auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
			outfile << "time of transfer: " <<  ctime(&timenow) << std::endl;
			outfile.close();

		}
		flagquestion=1;
	}
	copy(src,path,permission,info);
}