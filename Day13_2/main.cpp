#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

struct coordsType {
	int x;
	int y;
};

struct foldType {
	char direction;
	int distance;
};

int getMaxX (std::vector <coordsType> coords){
	int maxX = 0;
	for (size_t i {0}; i<coords.size(); i++){
		if (coords[i].x > maxX) maxX = coords[i].x;
	}
	return maxX+1;
}

int getMaxY (std::vector <coordsType> coords){
	int maxY = 0;
	for (size_t i {0}; i<coords.size(); i++){
		if (coords[i].y > maxY) maxY = coords[i].y;
	}
	return maxY+1;
}

std::string replaceChar(std::string source, char oldChar, char newChar){
	for (size_t i {0}; i<source.size(); i++)
		if (source[i] == oldChar) source[i] = newChar;
	return source;
}

void getDataFromFile(std::string fileName, std::vector<coordsType> &data, std::vector<foldType> &folds){
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	bool readFolds {false};
	while(std::getline(infile, line)) {
		if (line.empty()){
			readFolds = true;
			continue;
		}
		if (!readFolds){
			// Replace dashes.
			line = replaceChar(line, '\,', ' ');
			coordsType coords;
			std::istringstream str(&line[0]);
			str >> coords.x >> coords.y;
			data.push_back(coords);
		}
		else {
			std::regex regex("^fold along\\s*(x|y)*=([0-9]+)");
			std::smatch m;
			std::regex_match(line, m, regex);
			foldType fold;
			fold.direction = m.str(1)[0];
			fold.distance = std::stoi(m.str(2));
			folds.push_back(fold);
		}
	}
}

std::vector <coordsType> foldPaper(std::vector <coordsType> coords, foldType fold){
	for (size_t i {0}; i<coords.size(); i++){
		if (fold.direction == 'y' && coords[i].y >= fold.distance){
			coords[i].y = fold.distance - (coords[i].y-fold.distance);
		}
		if (fold.direction == 'x' && coords[i].x >= fold.distance){
			coords[i].x = fold.distance - (coords[i].x-fold.distance);
		}
	}
	return coords;
}

std::vector <coordsType> foldPaper(std::vector <coordsType> coords, std::vector<foldType> folds){
	for (size_t i {0}; i<folds.size(); i++){
		coords = foldPaper(coords, folds[i]);
	}
	return coords;
}

int countCoords(std::vector <coordsType> coords){
	std::vector <coordsType> distinctCoords;
	for (size_t i {0}; i<coords.size(); i++){
		bool found {false};
		for (size_t j{0}; j<distinctCoords.size(); j++){
			if (distinctCoords[j].x == coords[i].x && distinctCoords[j].y == coords[i].y)
				found = true;
		}
		if (!found){
			distinctCoords.push_back(coords[i]);
		}
	}
	return distinctCoords.size();
}

void displayPaper(std::vector <coordsType> coords){
	std::vector <std::vector<char>> paper;
	int maxY = getMaxY(coords);
	int maxX = getMaxX(coords);
	// Make blank paper
	for (size_t y{0}; y<maxY;y++){
		std::vector <char> row;
		for (size_t x{0}; x<maxX;x++){
			row.push_back('.');
		}
		paper.push_back(row);
	}
	// Add dots
	for (size_t i{0}; i<coords.size(); i++){
		paper[coords[i].y][coords[i].x] = '#';
	}
	// Print paper
	for (size_t y{0}; y<maxY;y++){
		std::vector <char> row;
		for (size_t x{0}; x<maxX;x++){
			std::cout << paper[y][x];
		}
		std::cout << std::endl;
	}
}


int main(){
	std::vector <coordsType> paper;
	std::vector<foldType> folds;
	getDataFromFile("../data.dat", paper, folds);
	std::vector <coordsType> foldedPaper = foldPaper(paper, folds);
	displayPaper(foldedPaper);
	return 0;
}