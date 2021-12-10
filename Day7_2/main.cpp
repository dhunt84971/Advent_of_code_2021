#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>


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

int distanceToFuelCost(int distance){
	int fuel {0};
	for (int i {0}; i<distance;i++)
		fuel += (i+1);
	return fuel;
}

int getFuelRequired(std::vector <int> data, int position){
	int fuel {0};
	for (size_t i {0}; i<data.size(); i++){
		fuel += distanceToFuelCost(std::abs(data[i]-position));
	}
	return fuel;
}

int getMax (std::vector <int> data){
	int max = 0;
	for (size_t i {0}; i<data.size(); i++){
		if (data[i] > max) max = data[i];
	}
	return max;
}

int getMinIndex (std::vector <int> data){
	int min = 0;
	int minIndex = 0;
	for (size_t i {0}; i<data.size(); i++){
		if (i==0) min = data[i];
		if (data[i] < min){
			min = data[i];
			minIndex = i;
		}
	}
	return minIndex;
}

std::vector <int> findFuels (std::vector <int> data, int max){
	std::vector <int> fuels;
	for (int i {0}; i<max; i++){
		fuels.push_back(getFuelRequired(data, i));
	}
	return fuels;
}

int main(){
	std::vector <int> data = getData("../data.dat");
	int max = getMax(data);
	std::vector <int> fuels = findFuels(data, max);
	int targetPosition = getMinIndex(fuels);
	int fuelExpended = fuels[targetPosition];
	std::cout << "Position with least fuel consumption = " << targetPosition << std::endl;
	std::cout << "The total fuel required is = " << fuelExpended << std::endl;
	return 0;
}