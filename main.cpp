#include <fstream>
#include <iostream>
#include <string>



using namespace std;


int copy(string src,string path){
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
		cout << "File not owned by process or does not exist, please enter sudo\n";
		string strcommand = "sudo ./filecopy "+src+" "+path;
		const char * command = strcommand.c_str();
		//cout << newrun;
		system(command);
	}
	file1.close();
	file2.close();

	return 0;
}


int main(int arc, char* argv[]) {
	string src, dest, path;
	if(arc==1){
		cout << "Enter src file to copy ";
		cin >> src;
	}
	else {
		src=argv[1];
	}
	if(arc==1){
		
		cout << "Enter full destination path to copy src file to ";
		cin >> dest;
		path=dest+"/"+src;
	}
	else {
		string destpath(argv[2]);
		path=destpath+"/"+src;
	}
	copy(src,path);
}
