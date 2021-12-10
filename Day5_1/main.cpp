#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct lineType {
	int x1;
	int y1;
	int x2;
	int y2;
};

std::string replaceChar(std::string source, char oldChar, char newChar){
	for (size_t i {0}; i<source.size(); i++)
		if (source[i] == oldChar) source[i] = newChar;
	return source;
}

std::vector <lineType> getLineFromFile(std::string fileName){
	std::vector <lineType> lines;
	std::cout << "Retrieved lines from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	while(std::getline(infile, line)) {
		// Replace commas.
		line = replaceChar(line, '\,', ' ');
		// Replace dashes.
		line = replaceChar(line, '-', ' ');
		// Replace greater than.
		line = replaceChar(line, '>', ' ');
		std::istringstream str(&line[0]);
		lineType coordinates;
		str >> coordinates.x1 >> coordinates.y1 >> coordinates.x2 >> coordinates.y2;
		lines.push_back(coordinates);
	}
	return lines;
}

std::vector <std::vector<int>> initDiagram (int maxX, int maxY){
	std::vector <std::vector<int>> diagram;
	for (int x {0}; x<=maxX; x++){
		std::vector<int> col {};
		for (int y {0}; y<=maxY; y++)
			col.push_back(0);
		diagram.push_back(col);
	}
	return diagram;
}

int getMaxX (std::vector <lineType> lines){
	int maxX = 0;
	for (size_t i {0}; i<lines.size(); i++){
		if (lines[i].x1 > maxX) maxX = lines[i].x1;
		if (lines[i].x2 > maxX) maxX = lines[i].x2;
	}
	return maxX;
}

int getMaxY (std::vector <lineType> lines){
	int maxY = 0;
	for (size_t i {0}; i<lines.size(); i++){
		if (lines[i].y1 > maxY) maxY = lines[i].y1;
		if (lines[i].y2 > maxY) maxY = lines[i].y2;
	}
	return maxY;
}

bool isOnLine(lineType line, int x, int y){
	if ((line.x1 == line.x2) || (line.y1 == line.y2))
		return (
			((x >= line.x1 && x <= line.x2) || (x >= line.x2 && x <= line.x1))
			&&
			((y >= line.y1 && y <= line.y2) || (y >= line.y2 && y <= line.y1))
			);
	else return false;
}

std::vector <std::vector<int>> addVents(
	std::vector <std::vector<int>> diagram, 
	std::vector <lineType> lines,
	int maxX, int maxY)	{
	for (int y {0}; y<=maxY; y++){
		for (int x {0}; x<=maxX; x++){
			for (size_t i {0}; i<lines.size(); i++){
				if (isOnLine(lines[i], x, y)){
					diagram[x][y] += 1;
				}
			}
		}
	}
	return diagram;	
}

int getIntersectionCount(std::vector <std::vector<int>> diagram, int maxX, int maxY) {
	int count {0};
	for (int y {0}; y<=maxY; y++)
		for (int x{0}; x<=maxX; x++)
			if (diagram[x][y] > 1) count++;
	return count;
}

void displayDiagram(std::vector <std::vector<int>> diagram, int maxX, int maxY){
	for (int y {0}; y<=maxY; y++){
		for (int x{0}; x<=maxX; x++){
			std::cout << diagram[x][y] << ' ';
		}
		std::cout << std::endl;
	}
}

int main() {
	std::vector <lineType> lines = getLineFromFile("../data.dat");
	int maxX = getMaxX(lines);
	int maxY = getMaxY(lines);
	std::vector <std::vector<int>>  diagram;
	diagram = initDiagram(maxX, maxY);
	diagram = addVents(diagram, lines, maxX, maxY);
	int intersections = getIntersectionCount(diagram, maxX, maxY);
	std::cout << "Intersections = " << intersections << std::endl;
	return 0;
}