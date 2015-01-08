#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using std::string; using std::vector; using std::ifstream;
using std::cout; using std::endl; using std::ofstream;

#define MAX_STACK_SIZE 8192

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

	void Print() const;

	string SaveElement();
};

struct XMLDocument{
	string name;
	vector<XMLElement> contents;

	void Print() const;
};

vector<string> Tokenize(const string& document);

XMLDocument ParseTokens(vector<string> tokens);

void SaveXMLDoc(XMLDocument& doc, string fileName);

void LoadXMLDoc(XMLDocument& doc, string fileName);