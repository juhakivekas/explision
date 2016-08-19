#include <iostream>
#include "json/json.h"
int main(){
	Json::Value face;
	face["id"] = 2;
	std::cout << face << std::endl;
}
