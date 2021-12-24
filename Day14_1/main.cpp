#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>

struct ruleType {
	std::string pair;
	std::string insertion;
	long int count;
};

struct charCountType {
	char letter;
	long int count;
};

std::string replaceChar(std::string source, char oldChar, char newChar){
	for (size_t i {0}; i<source.size(); i++)
		if (source[i] == oldChar) source[i] = newChar;
	return source;
}

void getDataFromFile(std::string fileName, std::vector<ruleType> &rules, std::string &polymer){
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	std::getline(infile, line);
	polymer = line;
	std::getline(infile, line);
	while(std::getline(infile, line)) {
		// Replace delimiter
		line = replaceChar(line, '-', ' ');
		line = replaceChar(line, '>', ' ');
		ruleType rule;
		std::istringstream str(&line[0]);
		str >> rule.pair >> rule.insertion;
		rule.count = 0;
		rules.push_back(rule);
	}
}

std::string getInsertion(std::string pair, std::vector<ruleType> rules){
	for (size_t i {0}; i<rules.size(); i++){
		if (rules[i].pair == pair)
			return rules[i].insertion;
	}
	std::cout << "ERROR - Pair Not Found!" << std::endl;
	return "";
}

std::vector <charCountType> incrementCharCounts(std::vector <charCountType> &charCounts, char letter, long int inc = 1){
	bool found = false;
	for (size_t i{0}; i<charCounts.size(); i++){
		if (charCounts[i].letter == letter){
			charCounts[i].count += inc;
			found = true;
			break;
		}
	}
	// If the letter is not found add it to the array of letters.
	if (!found){
		charCountType polyChar;
		polyChar.letter = letter;
		polyChar.count = inc;
		charCounts.push_back(polyChar);
	}
	return charCounts;
}

void incrementPairCounts(std::vector<ruleType> &rules, std::string pair, long int inc){
	for (size_t i{0}; i<rules.size(); i++){
		if (rules[i].pair == pair){
			rules[i].count += inc;
			break;
		}
	}
}

std::vector <charCountType> getInitCounts(std::vector<ruleType> &rules, std::string polymer){
	std::vector <charCountType> charCounts;
	// Add the letters from the rules.
	for (size_t i{0}; i<rules.size(); i++){
		// Add each unique letter to the charCount array.
		charCounts = incrementCharCounts(charCounts, rules[i].pair[0], 0);
		charCounts = incrementCharCounts(charCounts, rules[i].pair[1], 0);
		charCounts = incrementCharCounts(charCounts, rules[i].insertion[0], 0);
	}
	// Add the initial pair counts.
	for (size_t i {0}; i<polymer.size()-1; i++){
		std::string pair;
		pair.push_back(polymer[i]);
		pair.push_back(polymer[i+1]);
		incrementPairCounts(rules, pair, 1);
	}
	// Add the initial polymer letters.
	for (size_t i{0}; i<polymer.size(); i++){
		charCounts = incrementCharCounts(charCounts, polymer[i]);
	}
	return charCounts;
} 

std::vector<ruleType> executeStep(std::vector<ruleType> rules, std::vector <charCountType> &charCounts){
	std::vector<ruleType> result = rules;
	for (size_t i{0}; i<rules.size(); i++){
		if (rules[i].count > 0){
			incrementCharCounts(charCounts, rules[i].insertion[0], rules[i].count);
			incrementPairCounts(result, rules[i].pair, (-1*rules[i].count));
			std::string pair1 = rules[i].pair[0] + rules[i].insertion;
			incrementPairCounts(result, pair1, rules[i].count);
			std::string pair2 = rules[i].insertion + rules[i].pair[1];
			incrementPairCounts(result, pair2, rules[i].count);
		}
	}
	return result;
}

void executeSteps(std::vector<ruleType> rules, std::vector <charCountType> &charCounts, int maxSteps){
	for (int i {0}; i<maxSteps; i++){
		rules = executeStep(rules, charCounts);
	}
}

long int findMostCommon(std::vector <charCountType> charCounts){
	long int max {charCounts[0].count};
	for (size_t i {0}; i<charCounts.size(); i++){
		long int num = charCounts[i].count;
		if (num > max) max = num;
	}
	return max;
}	

long int findLeastCommon(std::vector <charCountType> charCounts){
	long int min {charCounts[0].count};
	for (size_t i {0}; i<charCounts.size(); i++){
		long int num = charCounts[i].count;
		if (num < min) min = num;
	}
	return min;
}	
	
int main(){
	std::vector<ruleType> rules;
	std::string initPolymer;
	getDataFromFile("../data.dat", rules, initPolymer);
	int numSteps = 10;
	std::vector <charCountType> charCounts = getInitCounts(rules, initPolymer);
	executeSteps(rules, charCounts, numSteps);
	long int most = findMostCommon(charCounts);
	long int least = findLeastCommon(charCounts);
	std::cout << "Least common from most common = " << (most - least) << std::endl;
	return 0;
}