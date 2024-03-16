#include <iostream>
#include <sstream>
#include <string>

int main() {
	std::istringstream ssch("this,is,a,test");
	std::istringstream ssky("less,keys");
	std::string channel, ky;
	while (std::getline(ssch, channel, ',')) {
		if (!std::getline(ssky, ky, ','))
			ky = "";
		std::cout << "channel: " << channel << " key: " << ky << std::endl;
	}
	return 0;
}
