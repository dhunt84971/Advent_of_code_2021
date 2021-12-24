#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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



int findBasinSize(std::vector<std::vector <int>> &data, int x, int y){
	if (data[y][x] == 9) return 0;
	int sum = 0;
	sum += 1;
	data[y][x] = 9; // Set the current position to a height of 9 to prevent revisting this position.
	// Up
	if (y-1>=0) sum += findBasinSize(data, x, y-1); 
	// Down
	if ((y+1)<data.size()) sum += findBasinSize(data, x, y+1);
	// Left
	if ((x-1)>=0) sum += findBasinSize(data, x-1, y);
	// Right
	if ((x+1)<data[0].size()) sum += findBasinSize(data, x+1, y);
	return sum;
}

std::vector <int> findLowPoints(std::vector<std::vector <int>> data){
	std::vector <int> lowPoints;
	std::vector <int> basinSizes;
	for (int y {0}; y<data.size(); y++){
		for (int x {0}; x<data[0].size(); x++){
			int u = ((y-1)>=0) ? data[y-1][x] : 10;
			int d = ((y+1)<data.size()) ? data[y+1][x] : 10;
			int l = ((x-1)>=0) ? data[y][x-1] : 10;
			int r = ((x+1)<data[0].size()) ? data[y][x+1] : 10;
			if (data[y][x] < u && data[y][x] < d && data[y][x]< l && data[y][x]< r){
				lowPoints.push_back(data[y][x]);
				basinSizes.push_back(findBasinSize(data, x, y));
			}
		}
	}
	std::sort (basinSizes.begin(), basinSizes.end());
	int basin1, basin2, basin3;
	basin1 = *(basinSizes.end()-3);
	basin2 = *(basinSizes.end()-2);
	basin3 = *(basinSizes.end()-1);
	
	std::cout << "Product of top 3 basins = " << basin1 * basin2 * basin3 << std::endl;
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