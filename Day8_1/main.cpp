#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

struct dataType {
	std::vector<std::string> in;
	std::vector<std::string> out;
};

std::vector <dataType> getData(std::string fileName, size_t dataInSize, size_t dataOutSize){
	std::vector <dataType> datas;
	
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	while(std::getline(infile, line))
	{
		dataType data;
		std::string segments;
		std::istringstream str(&line[0]);
		for (size_t i {0}; i < dataInSize; i++){
			str >> segments;
			data.in.push_back(segments);
		}
		// Throw away the delimiter.
		str >> segments;
		for (size_t i {0}; i < dataOutSize; i++){
			str >> segments;
			data.out.push_back(segments);
		}
		datas.push_back(data);
	}
	return datas;	
}

int getNumOccurrences(std::vector <dataType> data, size_t numSegs, size_t dataInSize, size_t dataOutSize){
	int sum {0};
	for (size_t d {0}; d < data.size(); d++){
		for (size_t i {0}; i < dataOutSize; i++){
			if (data[d].out[i].size() == numSegs){
				sum++;
			}
		}
	}
	return sum;
}

int main(){
	size_t maxIn {10};
	size_t maxOut {4};
	std::vector <dataType> data = getData("../data.dat", maxIn, maxOut);
	int dig1478 = getNumOccurrences(data, 7, maxIn, maxOut);
	dig1478 += getNumOccurrences(data, 4, maxIn, maxOut);
	dig1478 += getNumOccurrences(data, 3, maxIn, maxOut);
	dig1478 += getNumOccurrences(data, 2, maxIn, maxOut);
	std::cout << "Digits 1, 4, 7, 8 appear " << dig1478 << " times." << std::endl;
	return 0;
}