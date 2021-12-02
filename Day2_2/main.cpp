#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct position {
	int horizontal;
	int depth;
	int aim;
};

class Command 
{
private:
public:
	std::string direction;
	int distance;
	Command(std::string direction, int distance){
		this->direction = direction;
		this->distance = distance;
	}
	
	Command() {
		direction = "forward";
		distance = 0;
	}
	
	void displayCommand() {
		std::cout << "Direction = " << direction << ", Distance = " << distance << std::endl;
	}
	
};

std::vector<Command> getCommandsFromFile(std::string dataFile) {
	std::vector<Command> commands;
	std::cout << dataFile << std::endl;
	std::ifstream infile(dataFile);
	std::string direction;
	int distance;
	while (infile >> direction >> distance) {
		Command command {direction, distance};
		commands.push_back(command);
	}
	return commands;
}

position getFinalPosition(std::vector<Command> commands){
	position currentPos;
	currentPos.horizontal = 0;
	currentPos.depth = 0;
	currentPos.aim = 0;
	for (auto command: commands) {
		if (command.direction == "forward"){
			currentPos.horizontal += command.distance;
			currentPos.depth += currentPos.aim * command.distance;
		}
		else if (command.direction == "up"){
			currentPos.aim -= command.distance;
		}
		else if (command.direction == "down"){
			currentPos.aim += command.distance;
		}
	}
	return currentPos;
}

int main(){
    std::vector<Command> commands = getCommandsFromFile("../data.dat");
	position finalPos = getFinalPosition(commands);
	std::cout << "The final position of the submarine is " << finalPos.horizontal << ", " << finalPos.depth << std::endl;
	std::cout << "Horizontal X Depth = " << finalPos.horizontal * finalPos.depth << std::endl;
}