#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct square {
	int number;
	bool marked;
};

class BingoBoard {
private:
	std::vector <std::vector<square>> board;
	
	void initializeBoard(){
		std::vector<square> row;
		for (size_t r{0}; r<MAXROWS; r++){
			for (size_t c{0}; c<MAXCOLS; c++){
				square sq;
				sq.number = 0;
				sq.marked = false;
				row.push_back(sq);
			}
			board.push_back(row);
			row.clear();
		}
	}
	
public:
	static const int MAXROWS = 5;
	static const int MAXCOLS = 5;
	BingoBoard(){
		initializeBoard();
	}

	void setSquare(int row, int col, int number) {
		board[row][col].number = number;
	}
	
	void markSquares(int number){
		for (size_t r{0}; r<MAXROWS; r++){
			for (size_t c{0}; c<MAXCOLS; c++){
				if (board[r][c].number == number){
					board[r][c].marked = true;
				}
			}
		}
	}
	
	bool isWinner() {
		// Check for winner on row;
		for (size_t r{0}; r<MAXROWS; r++){
			bool marked = true;
			for (size_t c{0}; c<MAXCOLS; c++){
				marked = marked && board[r][c].marked;
			}
			if (marked) return true;
		}
		
		// Check for winner on col;
		for (size_t c{0}; c<MAXCOLS; c++){
			bool marked = true;
			for (size_t r{0}; r<MAXCOLS; r++){
				marked = marked && board[r][c].marked;
			}
			if (marked) return true;
		}
		return false;
	}
	
	int getScore(int lastNumber){
		int score {0};
		for (size_t c{0}; c<MAXCOLS; c++)
			for (size_t r{0}; r<MAXCOLS; r++)
				score += (board[r][c].marked)? 0 : board[r][c].number;
		return score * lastNumber;
	}
	
	void displayBoard(){
		for (int r {0}; r<BingoBoard::MAXROWS; r++){
				for (int c {0}; c<BingoBoard::MAXCOLS; c++)
					std::cout << board[r][c].number << " ";
				std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	
	void displayMarks(){
			for (int r {0}; r<BingoBoard::MAXROWS; r++){
				for (int c {0}; c<BingoBoard::MAXCOLS; c++){
					char marked = (board[r][c].marked)? 'X' : 'O';
					std::cout << marked << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
	}
};


class BingoBoards {
private:
	std::vector<BingoBoard> boards;
	std::string csBingoNumbers;
	int lastDrawnNumber;
	size_t drawNumberPos {0};
public:

	void loadBoardsFromFile(std::string fileName){
		std::cout << "Retrieved boards from " << fileName << std::endl;
		std::ifstream infile(fileName);
		std::getline(infile, csBingoNumbers);
		std::string line;
		int row {0};
		BingoBoard board;
		while(std::getline(infile, line)) {
			if (line.empty()) continue;
			std::istringstream str(&line[0]);
			int number;
			for(int col{0}; col < BingoBoard::MAXCOLS; col++){
				str >> number;
				board.setSquare(row,col,number);
			}
			row++;
			if (row == BingoBoard::MAXROWS){
				boards.push_back(board);
				row = 0;
			}
		}
	}
	
	void displayBoards(){
		for (auto board: boards){
			board.displayBoard();
		}
	}
	
	void displayMarks(){
		for (auto board: boards){
			board.displayMarks();
		}
	}
	
	bool drawNumber(){
		size_t pos = csBingoNumbers.find(",", drawNumberPos);
		if (pos == std::string::npos) return false;
		lastDrawnNumber = std::stoi(csBingoNumbers.substr(drawNumberPos, pos - drawNumberPos));
		drawNumberPos = pos + 1;
		//for (auto board:boards){
		//	board.markSquares(lastDrawnNumber);
		//}
		for (size_t i {0}; i < boards.size(); i++)
			boards[i].markSquares(lastDrawnNumber);
		return true;
	}
	
	int checkForWinner(){
		for (auto board: boards){
			if (board.isWinner()){
				return board.getScore(lastDrawnNumber);
			}
		}
		return 0;
	}
	
	void displayDrawnNumber(){
		std::cout << "The number is " << lastDrawnNumber << std::endl;
	}
	
};

int main(){
	BingoBoards boards;
	boards.loadBoardsFromFile("../data.dat");
	int score {0};
	while (score == 0 && boards.drawNumber()){
		boards.displayDrawnNumber();
		score = boards.checkForWinner();
	}
	if (score == 0 ){
		std::cout << "Sadly no one got bingo. The giant squid eats the submarine." << std::endl;
	}
	else {
		std::cout << "We have a winner!" << std::endl;
		std::cout << "The winner's score was " << score << std::endl;
	}
	return 0;
}