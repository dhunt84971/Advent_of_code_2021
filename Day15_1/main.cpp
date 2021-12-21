#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>


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

void traverseGrid(std::vector<std::vector <int>> grid, std::vector<std::vector <int>> &risks){
	for (size_t y {0}; y<grid.size(); y++){
		int risk = risks[y][0];
		for (size_t x {0}; x<grid[0].size(); x++){
			bool start = (x == 0);
			int newRisk = start ? risk : risk + grid[y][x];
			// Update risk at the current location.
			if ((risks[y][x] > newRisk) || (risks[y][x] == 0))
				risks[y][x] = newRisk;
			else
				newRisk = risks[y][x];
			// Update the risk above.
			if (y>0)
				if (risks[y-1][x] > newRisk + grid[y-1][x])
					risks[y-1][x] = newRisk + grid[y-1][x]; 
			// Update the risk below.
			if (y<grid.size()-1)
				risks[y+1][x] = newRisk + grid[y+1][x];
			risk = newRisk;
		}
	}
}

int main(){
	std::vector<std::vector <int>> data = getDataFromFile("../data.dat");
	std::vector <int> zeroes(data[0].size(), 0);
	std::vector<std::vector <int>> risks(data.size(), zeroes);
	traverseGrid(data, risks);
	std::cout << "The total risk = " << risks[data.size()-1][data[0].size()-1] << std::endl;
	return 0;
}
