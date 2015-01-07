#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using std::string; using std::vector; using std::ifstream;
using std::cout; using std::endl;

struct XMLAttribute{
	string name;
	string data;
};

struct XMLElement{
	string name;
	vector<XMLAttribute> attributes;
	vector<XMLElement> children;
};

struct XMLDocument{
	string name;
	vector<XMLElement> contents;
};

vector<string> ParseTokens(string document);

int main(){
	string fileName = "test.xml";
	ifstream file;
	string fileContents;

	file.open(fileName.c_str());

	if(!file.good()){
		cout << "Error opening file: " << fileName << endl;
		return -1;
	}

	while(!file.eof()){
		string line;
		std::getline(file,line,'\n');
		fileContents = fileContents + line + "\n";
	}

	cout << "File Contents: \n" << fileContents << endl;
	return 0;

	vector<string> tokens;

	return 0;
}



vector<string> ParseTokens(string document){
	return vector<string>();
}