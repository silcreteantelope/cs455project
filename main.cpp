#include <fstream>
#include <iostream>
#include <string>

using namespace std;


const int maxFilenameLength = 254;
const string illegalChars = "\\:*?\"<>|"; // The char / was removed to allow for paths to be entered

string getUserFile(){
	string filename;
	bool hasIllegal = false;
	int i=0;
	int filenameLength;

    	do {    //cin a filename from user
		getline(cin,filename);
		filenameLength = filename.length();	//record its length
		for(i=0; i<filenameLength; i++) {	//make sure it doesn't contain any illegal characters
			if(illegalChars.find_first_of(filename[i]) != string::npos){ hasIllegal = true; cout<<"This char is illegal"<<filename[i]<<"\n";}
			//else if(!isalnum(filename[i])) hasIllegal = true; 	//if it does, ask for a new one
		}
	
        	if(hasIllegal){
			cout << "The filename you entered was not valid. Enter new filename or ctrl-c" << endl; //repeats the process of prompting for a name until the user enters a valid one
        		i=0;	
		}
		cin.clear();    //clears the previous input in the case it wasn't a valid filename
        	cin.ignore(256,'\n');   //ignores the previous enter
	} while((cin.fail()) || hasIllegal || (filenameLength >= maxFilenameLength));

    	//cin.ignore(); //prevents the last enter from being counted as an empty Movie


	return filename;
}

//Int copy() takes src, path, and flags and copies the file to the path
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
		cout << "File not owned by process or does not exist, if the file is owned by root";		//4-1  error message doesn't expose too much information
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
		cout << "Would you like to change the owner and group permissions of file(y/N) ";
		cin >> input;
		if(input=="Y"||input=="y"){
			cout << "permission flag active\n";
			permission=1;
		}
		cout << "Would you like their to be a .txt file in the destination path with";
		cout << "time, date, and original file path of the file transfer for future notice(y/N) ";
		cin >> input;
		if(input=="Y"||input=="y"){
			cout << "info text file flag active\n";
			info=1;
		}
		flagquestion=1;
	}
	copy(src,path,permission,info);
}