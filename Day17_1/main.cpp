#include <iostream>
#include <assert.h>

struct targetType {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

int maxHeight(int dx, int dy, targetType target){
	int x {0};
	int y {0};
	int maxY {0};
	int steps {0};
	int maxSteps {1000};
	do {
		x += dx;
		y += dy;
		maxY = (y > maxY) ? y : maxY;
		if (x >= target.minX && x <= target.maxX && y >= target.minY && y <= target.maxY)
			return maxY;
		if (x > target.maxX || y < target.minY)
			return 0;
		if (dx > 0) dx--;
		dy--;
		steps++;
	} while (steps < 1000);
	assert(steps < 1000);
}

int main() {
	targetType target;
	// Puzzle data:
	target.minX = 57;
	target.maxX = 116;
	target.minY = -198;
	target.maxY = -148;
		
	std::cout << "Max height = " << maxHeight(11, 197, target) << std::endl;
	
	return 0;
}
