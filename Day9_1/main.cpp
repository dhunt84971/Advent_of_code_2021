#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

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

std::vector <int> findLowPoints(std::vector<std::vector <int>> data){
	std::vector <int> lowPoints;
	for (int y {0}; y<data.size(); y++){
		for (int x {0}; x<data[0].size(); x++){
			int u = ((y-1)>=0) ? data[y-1][x] : 10;
			int d = ((y+1)<data.size()) ? data[y+1][x] : 10;
			int l = ((x-1)>=0) ? data[y][x-1] : 10;
			int r = ((x+1)<data[0].size()) ? data[y][x+1] : 10;
			if (data[y][x] < u && data[y][x] < d && data[y][x] < l && data[y][x] < r)
				lowPoints.push_back(data[y][x]);
		}
	}
	return lowPoints;
}

int getSumOfHeights(std::vector <int> lowPoints){
	int sum {0};
	for (size_t i {0}; i<lowPoints.size(); i++){
		sum += lowPoints[i] + 1;
	}
	return sum;
}

int main(){
	std::vector<std::vector <int>> data = getDataFromFile("../data.dat");
	int sumOfRisk = getSumOfHeights(findLowPoints(data));
    std::cout << "The sum of the risk is " << sumOfRisk << std::endl;
}