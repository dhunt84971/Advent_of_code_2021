#include <iostream>
#include <assert.h>

struct targetType {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

bool isValidTrajectory(int dx, int dy, targetType target){
	int x {0};
	int y {0};
	int steps {0};
	int maxSteps {1000};
	do {
		x += dx;
		y += dy;
		if (x >= target.minX && x <= target.maxX && y >= target.minY && y <= target.maxY)
			return true;
		if (x > target.maxX || y < target.minY)
			return false;
		if (dx > 0) dx--;
		dy--;
		steps++;
	} while (steps < 1000);
	assert(steps < 1000);
}

int findValidTrajectories(targetType target){
	int sumTrajectories {0};
	int startY = -1*target.minY;  // Invert target as starting Y velocity.
	for (int dx {1}; dx <= target.maxX; dx++){
		for (int dy {startY}; dy >= target.minY; dy--){
			if (isValidTrajectory(dx, dy, target))
				sumTrajectories++;
		}
	}
	return sumTrajectories;
}

int main() {
	targetType target;
	// Puzzle data:
	target.minX = 57;
	target.maxX = 116;
	target.minY = -198;
	target.maxY = -148;
		
	std::cout << "Valid trajectories = " << findValidTrajectories(target) << std::endl;
	
	return 0;
}
