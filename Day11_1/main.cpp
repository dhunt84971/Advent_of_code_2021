#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//Prototypes
int flashAdjacent(std::vector<std::vector <int>> &data, int x, int y);

std::vector<std::vector <int>> getDataFromFile(std::string fileName){
	std::vector<std::vector <int>> data;
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	while(std::getline(infile, line)) {
		std::vector <int> row;
		for (size_t i {0}; i<line.size(); i++){
			row.push_back(line[i] - '0');
		}
		data.push_back(row);
	}
	return data;  // Data will be returned as [y][x] NOT [x][y].
}

int checkForFlashed (std::vector<std::vector <int>> &data, int x, int y){
	if  (data[y][x] == 10){
		data[y][x]++;
		return flashAdjacent(data, x, y) + 1;
	}
	return 0;
}

int flashAdjacent(std::vector<std::vector <int>> &data, int x, int y){
	int flashes {0};
	if ((y-1)>=0) { // Up
		flashes += checkForFlashed(data, x, y-1);
		data[y-1][x]++;
		flashes += checkForFlashed(data, x, y-1);
		if ((x+1)<data[0].size()){
			flashes += checkForFlashed(data, x+1, y-1);
			data[y-1][x+1]++; // Up Right
			flashes += checkForFlashed(data, x+1, y-1);
		}
		if ((x-1)>=0){
			flashes += checkForFlashed(data, x-1, y-1);
			data[y-1][x-1]++;  // Up Left
			flashes += checkForFlashed(data, x-1, y-1);
		}
	}
	if ((y+1)<data.size()) {  // Down
		flashes += checkForFlashed(data, x, y+1);
		data[y+1][x]++;
		flashes += checkForFlashed(data, x, y+1);
		if ((x+1)<data[0].size()){
			flashes += checkForFlashed(data, x+1, y+1);
			data[y+1][x+1]++; // Down Right
			flashes += checkForFlashed(data, x+1, y+1);
		}
		if ((x-1)>=0){
			flashes += checkForFlashed(data, x-1, y+1);
			data[y+1][x-1]++; // Down Left
			flashes += checkForFlashed(data, x-1, y+1);
		}
	}
	if ((x-1)>=0){
		flashes += checkForFlashed(data, x-1, y);
		 data[y][x-1]++; // Left
		 flashes += checkForFlashed(data, x-1, y);
	}
	if ((x+1)<data[0].size()){
		flashes += checkForFlashed(data, x+1, y);
		data[y][x+1]++; // Right
		flashes += checkForFlashed(data, x+1, y);
	}
	return flashes;
}

void increaseByOne(std::vector<std::vector <int>> &data){
	for (int y {0}; y<10; y++){
		for (int x {0}; x<10; x++){
			data[y][x]++;
		}
	}
}

void resetFlashed(std::vector<std::vector <int>> &data){
	for (int y {0}; y<10; y++){
		for (int x {0}; x<10; x++){
			if (data[y][x] > 10){
				data[y][x] = 0;
			}
			else if (data[y][x] == 10){
				std::cout << "Count missed flashes" << std::endl;
			}
		}
	}
}

int increaseStep(std::vector<std::vector <int>> &data){
	// First increase all by one;
	increaseByOne(data);
	// Loop on each flash;
	int flashes {0};
	for (int y {0}; y<10; y++){
		for (int x {0}; x<10; x++){
			flashes += checkForFlashed(data, x, y);
		}
	}
	resetFlashed(data);
	return flashes;
}

int executeSteps(std::vector<std::vector <int>> data, int steps){
	int sumFlashes = {0};
	for (int i {0}; i < steps; i++){
		sumFlashes += increaseStep(data);
	}
	return sumFlashes;
}

int main(){
	std::vector<std::vector <int>> data = getDataFromFile("../data.dat");
	std::cout << "The total number of flashes after 100 steps = " << executeSteps(data, 100) << std::endl;
	return 0;
}
