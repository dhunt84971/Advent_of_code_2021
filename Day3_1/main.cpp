#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

std::vector<std::string> getData (std::string fileName);
int getIntFromBits(std::string bits);
char getMostCommonBit(std::vector<std::string> bits, int bitPos);
std::string getDiagValue(std::vector<std::string> bits, std::string bit);


std::vector<std::string> getData (std::string fileName){
	std::vector<std::string> data;
	std::cout << "Retrieved bits from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string a;
	while (infile >> a) {
		data.push_back(a);
		//std::cout << a << std::endl;
	}
	return data;
}

int getIntFromBits(std::string bits) {
	int retVal {0};
	int bitPos {0};
	for (size_t i {bits.length()}; i>=1; i--){
		retVal += (bits[i-1] == '1') ? pow(2, bitPos) : 0;
		bitPos++;
	}
	return retVal;
}

char getMostCommonBit(std::vector<std::string> bits, int bitPos, bool invert){
	int numZeros {0};
	int numOnes {0};
	for (auto word: bits){
		numOnes += (word[bitPos] == '1') ? 1 : 0;
		numZeros += (word[bitPos] == '0') ? 1 : 0;
	}
	if (numOnes > numZeros)
		return invert ? '0' : '1';
	else
		return invert ? '1' : '0';
}

std::string getDiagValue(std::vector<std::string> bits, bool invert){
	std::string retVal {};
	for (size_t i {0}; i < bits[0].length(); i++ ){
		
		retVal.push_back(getMostCommonBit(bits, i, invert));
	}
	return retVal;
}


int main(){
		std::vector<std::string> data = getData("../data.dat");
		int gamma = getIntFromBits(getDiagValue(data, false));
		int epsilon = getIntFromBits(getDiagValue(data, true));
		std::cout << "Gamma = " << gamma << ",  Epsilon = " << epsilon << std::endl;
		std::cout << "Product = " << gamma * epsilon << std::endl;		
}