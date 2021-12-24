#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct pathType{
	std::string start;
	std::string finish;
};

std::string replaceChar(std::string source, char oldChar, char newChar){
	for (size_t i {0}; i<source.size(); i++)
		if (source[i] == oldChar) source[i] = newChar;
	return source;
}

std::vector<pathType> getDataFromFile(std::string fileName){
	std::vector<pathType> data;
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::string line;
	while(std::getline(infile, line)) {
		// Replace dashes.
		line = replaceChar(line, '-', ' ');
		pathType path;
		std::istringstream str(&line[0]);
		str >> path.start >> path.finish;
		data.push_back(path);
	}
	return data;  
}

bool stringIsLower(std::string value){
	bool lower = true;
	for (size_t i {0}; i<value.size(); i++){
		lower = lower && islower(value[i]);
	}
	return lower;
}

bool pathContains(std::vector<std::string> route, std::string location){
	for (size_t i {0}; i<route.size(); i++){
		if (route[i] == location)
			return true;
	}
	return false;
}

bool pathHasTwo(std::vector<std::string> route){
	int count {0};
	for (size_t i{0}; i<route.size(); i++){
		for (size_t j{0}; j<route.size(); j++){
			if (i!=j){
				if (stringIsLower(route[i])){
					if (route[i] == route[j]) return true;
				}
			}
		}
	}
	return false;
}

bool pathAlreadyRun(std::vector <std::vector<std::string>> routes, std::vector<std::string> route){
	for (size_t i {0}; i<routes.size(); i++){
		if (routes[i] == route)
			return true;
	}
	return false;
}

void displayRoute(std::vector<std::string> route){
	for (size_t i {0}; i<route.size(); i++){
		std::cout << route[i] << ", ";
	}
	std::cout << std::endl; 
}

int findNextPath(std::vector<pathType> data, std::vector <std::vector<std::string>> &routes, std::vector<std::string> route){
	int numRoutes {0};
	// If the end has been found add this route.
	if (route.back() == "end"){
		routes.push_back(route);
		return 1;
	}
	// If the end has not been found keep searching.
	for (size_t i {0}; i<data.size(); i++){
		std::vector<std::string> routeSearchForward = route;
		if (routeSearchForward.back() == data[i].start && data[i].start != "start"){
			if (!stringIsLower(data[i].finish)){
				routeSearchForward.push_back(data[i].finish);
				numRoutes += findNextPath(data, routes, routeSearchForward);
			}
			else if (!pathContains(routeSearchForward, data[i].finish) || !pathHasTwo(routeSearchForward)){
				routeSearchForward.push_back(data[i].finish);
				numRoutes += findNextPath(data, routes, routeSearchForward);
			}
		}
		std::vector<std::string> routeSearchBackward = route;
		if (routeSearchBackward.back() == data[i].finish && data[i].start != "start"){
			if (!stringIsLower(data[i].start)){
				routeSearchBackward.push_back(data[i].start);
				numRoutes += findNextPath(data, routes, routeSearchBackward);
			}
			else if (!pathContains(routeSearchBackward, data[i].start) || !pathHasTwo(routeSearchBackward)){
				routeSearchBackward.push_back(data[i].start);
				numRoutes += findNextPath(data, routes, routeSearchBackward);
			}
		}
	}
	// If this path did not pan out drop it.
	return 0;
}

std::vector <std::vector<std::string>> getRoutes(std::vector<pathType> data){
	std::vector <std::vector<std::string>> routes;
	int numRoutes {0};
	for (size_t i {0}; i<data.size(); i++){
		// Find the starts
		if (data[i].start == "start"){
			std::vector<std::string> route;
			route.push_back(data[i].start);
			route.push_back(data[i].finish);
			numRoutes += findNextPath(data, routes, route);
		}
	}
	return routes;
}


int main(){
	std::vector <std::vector<std::string>> routes;
	std::vector <pathType> data = getDataFromFile("../data.dat");
	routes = getRoutes(data);
	std::cout << "Possible routes found = " << routes.size() << std::endl;
	return 0;
}
