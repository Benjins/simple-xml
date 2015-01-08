#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using std::string; using std::vector; using std::ifstream;
using std::cout; using std::endl; using std::ofstream;

#define MAX_STACK_SIZE 2048

template<typename T>
struct Stack{
	T values[MAX_STACK_SIZE];
	int count;

	Stack(){
		count = 0;
	}

	void Push(const T& newValue){ 
		values[count] = newValue;
		count++;
	}

	T Pop(){
		count--;
		return values[count];
	}

	T& Top(){
		return values[count-1];
	}
};

struct XMLAttribute{
	string name;
	string data;

	void Print() const{
		cout << name << ":" << data;
	}

	string ToString(){
		return name + "='" + data + "'";
	}
};

struct XMLElement{
	string name;
	vector<XMLAttribute> attributes;
	vector<XMLElement> children;

	void Print() const{
		cout << name << " ";
		for(auto iter = attributes.begin(); iter != attributes.end(); iter++){
			iter->Print();
			cout << " ";
		}
		for(auto iter = children.begin(); iter != children.end(); iter++){
			cout << endl;
			iter->Print();
		}
	}

	string SaveElement(){
		string save = "<" + name;
		for(auto iter = attributes.begin(); iter != attributes.end(); iter++){
			save = save + " " + iter->ToString(); 
		}

		if(children.size() == 0){
			save += "/>";
			return save;
		}
		else{
			save += ">\n";
			
			for(auto iter = children.begin(); iter != children.end(); iter++){
				save += iter->SaveElement();
				save += "\n";
			}

			save = save + "</" + name + ">";
			return save;
		}
	}
};

struct XMLDocument{
	string name;
	vector<XMLElement> contents;

	void Print() const{
		cout << "Document: " << name << endl;
		for(auto iter = contents.begin(); iter != contents.end(); iter++){
			iter->Print();
			cout << endl;
		}
	}
};

vector<string> Tokenize(const string& document);

XMLDocument ParseTokens(vector<string> tokens);

void SaveXMLDoc(XMLDocument& doc, string fileName);

int main(){
	string fileName = "test3.xml";
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

	//cout << "File Contents: \n" << fileContents << endl;
	//return 0;

	vector<string> tokens = Tokenize(fileContents);

	/*
	for(auto iter = tokens.begin(); iter != tokens.end(); iter++){
		cout << "|" << (*iter) << "|\n";
	}
	*/

	cout << "About to parse" << endl;
	XMLDocument doc = ParseTokens(tokens);

	cout << "Parsed" << endl;

	doc.Print();

	SaveXMLDoc(doc, "test3_save.xml");

	return 0;
}

vector<string> Tokenize(const string& document){
	vector<string> tokens;
	string memoryString = "";
	bool inString = false;

	for(int i = 0; i < document.size(); i++){
		char character = document[i];

		if(character == '<' || character == '>' || character == '/' || character == '='){
			if(inString){
				memoryString += character;
			}
			else{
				if(memoryString != ""){
					tokens.push_back(memoryString);
					memoryString = "";
				}
				string str = "-";
				str[0] = character;
				tokens.push_back(str);
			}
		}
		else if(character == '\''){
			if(memoryString != ""){
				tokens.push_back(memoryString);
				memoryString = "";
			}
			string str = "-";
			str[0] = character;
			//tokens.push_back(str);

			inString = !inString;
		}
		else if(character == ' ' || character == '\n' ||  character == '\t' || character == '\r'){
			if(memoryString != ""){
				tokens.push_back(memoryString);
				memoryString = "";
			}
		}
		else{
			memoryString += character;
		}
	}

	return tokens;
}

XMLDocument ParseTokens(vector<string> tokens){
	XMLDocument doc;

	Stack<string> tokenStack;
	Stack<XMLElement> elementStack;

	for(auto iter = tokens.begin(); iter != tokens.end(); iter++){
		string token = *iter;
		//cout << "Parsing token: " << token << endl;
		if(token == "<"){
			tokenStack.Push(token);
		}
		else if(token == "/"){
			if(tokenStack.Top() == "<"){
				//Add the element to the doc
				XMLElement elem = elementStack.Pop();
				if(elementStack.count == 0){
					doc.contents.push_back(elem);
				}
				else{
					elementStack.Top().children.push_back(elem);
				}
				
				iter++;
				iter++;
			}
			else{
				tokenStack.Push(token);
			}
		}
		else if(token == ">"){
			bool startList = true;
			if(tokenStack.Top() == "/"){
				startList = false;
				tokenStack.Pop();
			}

			string currToken = tokenStack.Pop();
			XMLAttribute attr;
			attr.data = attr.name = "";
			while(currToken != "<"){
				if(currToken == "="){
					//do nothing
				}
				else if(attr.data == ""){
					attr.data = currToken;
				}
				else{
					attr.name = currToken;
					elementStack.Top().attributes.push_back(attr);
					attr.data = attr.name = "";
				}
				currToken = tokenStack.Pop();
			}

			if(!startList){
				XMLElement elem = elementStack.Pop();
				if(elementStack.count == 0){
					doc.contents.push_back(elem);
				}
				else{
					elementStack.Top().children.push_back(elem);
				}
			}
		}
		else{
			if(tokenStack.Top() == "<"){
				XMLElement elem;
				elem.name = token;
				//cout << "Pushing to element stack: " << token << endl;
				elementStack.Push(elem);
			}
			//cout << "Pushing token: "<< token << endl;
			tokenStack.Push(token);
		}
	}

	return doc;
}

void SaveXMLDoc(XMLDocument& doc, string fileName){
	ofstream fileOut;
	fileOut.open(fileName.c_str());

	if(!fileOut.good()){
		cout << "Failed to open file " << fileName << " for saving.\n";
		return;
	}

	string docContents = "";

	for(auto iter = doc.contents.begin(); iter != doc.contents.end(); iter++){
		docContents += iter->SaveElement();
	}

	fileOut << docContents;
}