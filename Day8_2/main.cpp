#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>

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

std::string getNumFromSize(dataType data, size_t numSegs){
	for (size_t i {0}; i < 10; i++){
		if (data.in[i].size() == numSegs){
			return data.in[i];
		}
	}
	return "";
}

size_t andNumbers(std::string num1, std::string num2){
	std::string andResult {};
	for (size_t i {0}; i<num1.size(); i++){
		if (num2.find(num1[i]) != std::string::npos)
			andResult.push_back(num1[i]);
	}
	return andResult.size();
}

bool isSameNumber(std::string num1, std::string num2){
	return (num1.size() == num2.size()) && (andNumbers(num1, num2) == num1.size());
}

int decodeData(dataType data){
	int result {0};
	// Find unique numbers
	std::string one = getNumFromSize(data, 2);
	std::string four = getNumFromSize(data, 4);
	std::string seven = getNumFromSize(data, 3);
	std::string eight = getNumFromSize(data, 7);
	
	for (size_t i {0}; i<data.out.size(); i++){
		int num {0};
		// Find the number
		if (isSameNumber(one, data.out[i]))
			num = 1;
		else if (isSameNumber(four, data.out[i]))
			num = 4;
		else if (isSameNumber(seven, data.out[i]))
			num = 7;
		else if (isSameNumber(eight, data.out[i]))
			num = 8;
		else if (data.out[i].size() == 6){ // 0, 6 or 9
			if (andNumbers(one, data.out[i]) == 1)
				num = 6;
			else if (andNumbers(four, data.out[i]) == 3)
				num = 0;
			else
				num = 9;
		}
		else if (data.out[i].size() == 5){ // 2, 3 or 5
			if (andNumbers(one, data.out[i]) == 2)
				num = 3;
			else if (andNumbers(four, data.out[i]) == 2)
				num = 2;
			else
				num = 5;
		}
		result += num * pow(10, 3-i);
	}
	return result; 
}

int getSum(std::vector<dataType> data){
	int sum {0};
	for (size_t i {0}; i<data.size(); i++){
		sum += decodeData(data[i]);
	}
	return sum;
}

int main(){
	size_t maxIn {10};
	size_t maxOut {4};
	std::vector <dataType> data = getData("../data.dat", maxIn, maxOut);
	std::cout << "Sum = " << getSum(data) << std::endl;
	return 0;
}