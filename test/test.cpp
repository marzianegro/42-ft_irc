#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int main() {
	std::istringstream ssmsg("#target +k test +i");
	std::string target, arg;
	std::vector<std::string> args;

	std::getline(ssmsg, target, ' ');
	while (std::getline(ssmsg, arg, ' ')) {
		args.push_back(arg);
	}

    std::cout << "Target:  " << target << std::endl;
    std::cout << "ArgSize: " << args.size() << std::endl;
    return 0;
}
