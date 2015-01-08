#include "simple-xml.h"

int main(){

	XMLDocument doc;
	LoadXMLDoc(doc, "test5.xml");
	SaveXMLDoc(doc, "test5_save.xml");

	return 0;
}
