#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
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

long int getIncompleteScore(std::string openChunks){
	long int score {0};
	for (size_t i {openChunks.size()}; i>0; i--){
		score = score * 5;
		if (openChunks[i-1] == '(')
			score += 1;
		else if (openChunks[i-1] == '[')
			score += 2;
		else if (openChunks[i-1] == '{')
			score += 3;
		else if (openChunks[i-1] == '<')
			score += 4;
	}
	return score;
}

long int tallyIncompleteScore(std::vector <std::string> data){
	std::vector <long int> scores;
	for (size_t i {0}; i<data.size(); i++){
		std::string corrupt = isCorrupted(data[i]);
		if (isClose(corrupt[0]) == std::string::npos){  // Incomplete won't contain close characters.
			scores.push_back(getIncompleteScore(corrupt));
		}
	}
	std::sort (scores.begin(), scores.end());
	return scores[scores.size()/2];
}

int main(){
	std::vector<std::string> data = getDataFromFile("../data.dat");
	std::cout << "Total incomplete syntax error score = " << tallyIncompleteScore(data) << std::endl;
}