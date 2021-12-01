#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<int> getData(std::string dataFile) {
	std::vector<int> data;
	std::cout << dataFile << std::endl;
	std::ifstream infile(dataFile);
	int a;
	while (infile >> a) {
		data.push_back(a);
		std::cout << a << std::endl;
	}
	return data;
}

void displayData(std::vector<int> data) {
	for (auto dat: data){
		std::cout << dat << std::endl;
	}
}

int countIncreases(std::vector<int> data) {
	int increases {0};
	
	for (size_t i {0}; i<data.size(); i++){
		if (i > 0){
			if (data[i] > data[i-1])
				increases++;
		}
	}
		
	return increases;
}

int main(){
    //std::vector<int> data = getData("../Day1_1.dat");
	std::vector<int> data = getData("../Day1_1_sample.dat");
	displayData(data);
	int increases = countIncreases(data);
	std::cout << "There are " << increases << " measurements that are larger than the previous." << std::endl;
	return 0;
}