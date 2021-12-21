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

int debugTime = 100;
bool showGrid = false;

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

std::vector<std::vector <int>> expandDataSet(std::vector<std::vector <int>> data){
	std::vector<std::vector <int>> expandedData;
	for (size_t ty {0}; ty<5; ty++){
		for (size_t y {0}; y<data.size(); y++){
			std::vector <int> exDataRow;
			for (size_t tx {0}; tx<5; tx++){
				for (size_t x {0}; x<data[0].size(); x++){
					int newNum = (data[y][x]+tx) + ty;
					newNum = (newNum > 9) ? (newNum-10 + 1) : newNum;
					exDataRow.push_back(newNum);
				}
			}
			expandedData.push_back(exDataRow);
		}
	}
	return expandedData;
}


void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void displayGrid(std::vector<std::vector <int>> grid, std::string delimeter){
	clear();
	for (size_t y {0}; y<grid.size(); y++){
		for (size_t x {0}; x<grid[0].size(); x++){
			std::cout << grid[y][x];
			if (x<grid[0].size()-1)
				std::cout << delimeter;
		}
		std::cout << std::endl;
	}
}

void traverseGrid(std::vector<std::vector <int>> grid, std::vector<std::vector <int>> &risks, int traverse){
	bool foundBetter = true;
	while (foundBetter){
		foundBetter = false;
		for (size_t y {0}; y<grid.size(); y++){
			int risk = risks[y][0];
			for (size_t x {0}; x<grid[0].size(); x++){
				bool start = (x == 0);
				int newRisk = start ? risk : risk + grid[y][x];
				// Update risk at the current location.
				if ((risks[y][x] > newRisk) || (risks[y][x] == 0)){
					risks[y][x] = newRisk;
					if (!start) foundBetter = true;
					if (showGrid){
						displayGrid(risks, ", ");
						std::this_thread::sleep_for(std::chrono::milliseconds(debugTime));
					}
				}
				else
					newRisk = risks[y][x];
				// Update the risk above.
				int yupRisk = newRisk;
				for (int ty {0};ty<traverse; ty++){
					int yup = y-ty;
					if (yup>0){
						if (risks[yup-1][x] > yupRisk + grid[yup-1][x]){
							risks[yup-1][x] = yupRisk + grid[yup-1][x]; 
							foundBetter = true;
							if (showGrid){
								displayGrid(risks, ", ");
								std::this_thread::sleep_for(std::chrono::milliseconds(debugTime));
							}
						}
						else yupRisk = risks[yup-1][x];
						// Update the risk to the left
						int lftRisk = yupRisk;
						for (int tx {0}; tx<traverse; tx++){
							int xlft = x-tx;
							if (xlft>0)
								if (risks[yup-1][xlft-1] > lftRisk + grid[yup-1][xlft-1]){
									risks[yup-1][xlft-1] = lftRisk + grid[yup-1][xlft-1];
									lftRisk = risks[yup-1][xlft-1];
									foundBetter = true;
									if (showGrid){
										displayGrid(risks, ", ");
										std::this_thread::sleep_for(std::chrono::milliseconds(debugTime));
									}
								}
								else break;
						}
					}
					else break;
				}
				// Update the risk below.
				int ydnRisk = newRisk;
				for (int ty {0};ty<traverse; ty++){
					int ydn = y+ty;
					if (ydn<grid.size()-1){
						if (risks[ydn+1][x] > ydnRisk + grid[ydn+1][x] || risks[ydn+1][x] == 0){
							risks[ydn+1][x] = ydnRisk + grid[ydn+1][x];
						}
						ydnRisk = risks[ydn+1][x];
					}
					else break;
					// Update the risk to the left
					int lftRisk = ydnRisk;
					for (int tx {0}; tx<traverse; tx++){
						int xlft = x-tx;
						if (xlft>0)
							if (risks[ydn+1][xlft-1] > lftRisk + grid[ydn+1][xlft-1]){
								risks[ydn+1][xlft-1] = lftRisk + grid[ydn+1][xlft-1];
								lftRisk = risks[ydn+1][xlft-1];
								foundBetter = true;
								if (showGrid){
									displayGrid(risks, ", ");
									std::this_thread::sleep_for(std::chrono::milliseconds(debugTime));
								}
							}
							else break;
					}
				}
				risk = newRisk;
			}
		}
		if (showGrid){
			displayGrid(risks, ", ");
			std::this_thread::sleep_for(std::chrono::milliseconds(debugTime));
		}
	}
}

int main(){
	std::vector<std::vector <int>> data = getDataFromFile("../data.dat");
	data = expandDataSet(data);
	std::vector <int> zeroes(data[0].size(), 0);
	std::vector<std::vector <int>> risks(data.size(), zeroes);
	traverseGrid(data, risks, 1);
	std::cout << "The total risk = " << risks[data.size()-1][data[0].size()-1] << std::endl;
	return 0;
}
