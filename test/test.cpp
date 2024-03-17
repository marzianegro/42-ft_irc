#include <iostream>
#include <sstream>
#include <string>
#include <map>

int main() {
	std::map<std::string, char *> test;
	char *str = "test";

	test["test"] = str;

	if (test["testone"]) {
		std::cout << "testone exists\n";
	} else {
		std::cout << "testone does not exist\n";
	}
}
