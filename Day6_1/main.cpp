#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tgmath.h>

long long int countPopulation(int daysLeft, int count){
	long long int population = 0;
	while (daysLeft > 0){
		daysLeft--;
		count--;
		if (count < 0){
			count = 6;
			population += 1;
			population += countPopulation(daysLeft, 8);
		}
	}
	return population;
}

std::vector <int> getData(std::string fileName){
	std::vector <int> data;
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	while(std::getline(infile, line, '\,'))
	{
		data.push_back(std::stoi(line));
	}
	return data;	
}


int main(){
	std::vector <int> data = getData("../data.dat");
	std::vector <long long int> preCounts;
	long long int subTotal = 0;
	for (int i {0}; i < 6; i++){
		if (i!=0)
			subTotal = countPopulation(80, i);
		else
			subTotal = 0;
		std::cout << i << " = " << subTotal << std::endl;
		preCounts.push_back(subTotal);
	}
	long long int total = 0;
	
	for (size_t i {0}; i<data.size(); i++){
		total += preCounts[data[i]];
	}
	total += data.size();
	std::cout << "Total population = " << total << std::endl;
	return 0;
}