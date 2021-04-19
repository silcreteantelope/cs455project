#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

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
		cout << "File not owned by process\n";
		system("sudo ./a.out main.cpp /media/stardisk/cs/cs455/cs455project/test");
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
