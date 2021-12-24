#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

std::vector<std::string> getDataFromFile(std::string fileName){
	std::vector<std::string> data;
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::string line;
	std::ifstream infile(fileName);
	while(std::getline(infile, line)) {
		data.push_back(line);
	}
	return data; 
}

size_t isOpen(char c){
	std::string openChunk= "([{<";
	return openChunk.find(c);
}

size_t isClose(char c){
	std::string closeChunk= ")]}>";
	return closeChunk.find(c);
}

std::string isCorrupted(std::string line){
	std::string openChunks;
	std::string corruptedChar;
	for (size_t i {0}; i<line.size(); i++){
		if (isOpen(line[i]) != std::string::npos)
			openChunks.push_back(line[i]);
		else { // Assume is a close character.
			if (openChunks.size() > 0){
				if (isOpen(openChunks.back()) == isClose(line[i]))
					openChunks.pop_back();
				else{
					corruptedChar.push_back(line[i]);
					return corruptedChar;
				}
			}
		}
	}
	return openChunks;
}

int tallyCorruptScore(std::vector <std::string> data){
	int score {0};
	for (size_t i {0}; i<data.size(); i++){
		std::string corrupt = isCorrupted(data[i]);
		if (corrupt == ")")
			score += 3;
		else if (corrupt == "]")
			score += 57;
		else if (corrupt == "}")
			score += 1197;
		else if (corrupt == ">")
			score += 25137;
	}
	return score;
}

int main(){
	std::vector<std::string> data = getDataFromFile("../data.dat");
	std::cout << "Total syntax error score = " << tallyCorruptScore(data) << std::endl;
}