#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int copy(string src,string path,int cflag,int pflag,int iflag){
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
		cout << "File not owned by process or does not exist, if the file is owned by root";
		cout << "or another user complete the sudo prompt, else check if file exists\n";
		string strcommand = "sudo ./filecopy "+src+" "+path;
		const char * command = strcommand.c_str();
		//cout << newrun;
		system(command);
	}
	file1.close();
	file2.close();
	if(pflag=1){
		string user,group;
		cout << "Enter unix user to own file: ";
		cin >> user;
		cout << "Enter unix group to own file: ";
		cin >> group;
		string permissioncommand = "chown "+user+":"+group+" "+path;
		const char * command = permissioncommand.c_str();
		system(command);
	}
	return 0;
}


int main(int arc, char* argv[]) {
	cout << "\nFileCopy started: ";
	
	string src, dest, path, input;
	int checksum=0,permission=0,info=0;
	
	if(arc==1){ //No src file or destination or flags
		cout << "Enter src file to copy: ";
		cin >> src;
		cout << "Enter full destination path to copy src file to: ";
		cin >> dest;
		path=dest+"/"+src;
		cout << "Would you like to checksum the src and moved files to check file integrity(y/n) ";
		cin >> input;
		if(input=="Y"||input=="y"){
			cout << "checksum flag active\n";
			checksum=1;
		}
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
		}
	}
	for(int i=0;i<=arc-1;i++){
		string temp(argv[i]);
		if(temp=="checksum" || temp=="c"){
			cout << "checksum flag active\n";
			checksum=1;
		}				
		else if(temp=="permission"|| temp=="p"){
			cout << "permission flag active\n";
			permission=1;
		}
		else if(temp=="info"|| temp=="i"){
			cout << "info text file flag active\n";
			info=1;
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
	copy(src,path,checksum,permission,info);
}