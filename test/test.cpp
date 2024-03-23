#include <iostream>
#include <string>

int main() {
    std::string array[] = {"Hellaaaaaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahaahahahahhahahaahhahaahahhaahahahhahao", "World", "from", "C++"};
	int sizeofstr = sizeof(std::string);
    int num_elements = sizeof(array) / sizeofstr;

	std::cout << "Size of string: " << sizeofstr << std::endl;
	std::cout << "Size of array: " << sizeof(array) << std::endl;
    std::cout << "Number of elements: " << num_elements << std::endl;

    return 0;
}
