#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

// Prototypes
std::vector<std::string> getData (std::string fileName);
int getIntFromBits(std::string bits);
char getMostCommonBit(std::vector<std::string> bits, int bitPos);
std::string getDiagValue(std::vector<std::string> bits, bool invert);
std::vector<std::string> filterOnBit (std::vector <std::string> bits,  int bitPos, char bit);
std::string getDiagValue2(std::vector<std::string> bits, bool invert);


std::vector<std::string> getData (std::string fileName){
	std::vector<std::string> data;
	std::cout << "Retrieved bits from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string a;
	while (infile >> a) {
		data.push_back(a);
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
	if (numOnes >= numZeros)
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

std::vector<std::string> filterOnBit (std::vector <std::string> bits,  int bitPos, char bit){
	std::vector<std::string>  retVal {};
	for (auto word: bits){
		if (word[bitPos]  == bit)
			retVal.push_back(word);
	}
	return retVal;
}

std::string getDiagValue2(std::vector<std::string> bits, bool invert){
	size_t i {0};
	std::vector<std::string> bitsFiltered = bits;
	char mcBit;
	do {
		mcBit = getMostCommonBit(bitsFiltered, i, invert);
		bitsFiltered = filterOnBit(bitsFiltered, i, mcBit);
		i++;
	} while (i<bits[0].length() && bitsFiltered.size() > 1);
	return bitsFiltered[0];
}

int main(){
		std::vector<std::string> data = getData("../data.dat");
		int gamma = getIntFromBits(getDiagValue(data, false));
		int epsilon = getIntFromBits(getDiagValue(data, true));
		std::cout << "Gamma = " << gamma << ",  Epsilon = " << epsilon << std::endl;
		std::cout << "Product = " << gamma * epsilon << std::endl;		
		std::cout << std::endl;
		int oxygen = getIntFromBits(getDiagValue2(data, false));
		int co2Scrubber = getIntFromBits(getDiagValue2(data, true));
		std::cout << "Oxygen = " << oxygen << ",  CO2 Scrubber = " << co2Scrubber << std::endl;
		std::cout << "Product = " << oxygen * co2Scrubber << std::endl;		
		std::cout << std::endl;
}