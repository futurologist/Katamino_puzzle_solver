#include <iostream>
#include <time.h>

const int n_squares = 5;
const int n_pieces = 10;

void E(const int x[2], const int &k, const int &m, const int& t_row, const int& t_col, int y[2] ) {
	if (m == 1) { y[0] = -x[1]; y[1] = x[0]; }
	else if (m == 2) { y[0] = -x[0]; y[1] = -x[1]; }
	else if (m == 3) { y[0] = x[1]; y[1] = -x[0]; }
	else { y[0] = x[0]; y[1] = x[1]; }
	if (k == 1) { y[0] = -y[0]; }
	y[0] = y[0] + t_row;
	y[1] = y[1] + t_col;
	return;
}

bool available_position(const int Board[n_squares][n_pieces], const int Piece[n_squares+1][2], const int &k, const int &m, const int &t_row, const int &t_col, int position_of_Piece[n_squares][2]) {
	int Square[2];
	for (int i = 0; i < n_squares; i = i + 1) {
		E(Piece[i], k, m, t_row, t_col, Square);
		if (!(0 <= Square[0] && Square[0] < n_squares && 0 <= Square[1] && Square[1] < n_pieces && Board[Square[0]][Square[1]] == 0)){
			return false;
		}
		position_of_Piece[i][0] = Square[0];
		position_of_Piece[i][1] = Square[1];
	}
	return true;
}

void put_piece_on_Board(int Board[n_squares][n_pieces], const int position_of_Piece[n_squares][2], const int &colour){
	for (int i = 0; i < n_squares; i = i + 1) {
		Board[position_of_Piece[i][0]][position_of_Piece[i][1]] = colour;
	}
	return;
}

void remove_piece_from_Board(int Board[n_squares][n_pieces], const int position_of_Piece[n_squares][2]) {
	for (int i = 0; i < n_squares; i = i + 1) {
		Board[position_of_Piece[i][0]][position_of_Piece[i][1]] = 0;
	}
	return;
}

void copy_position(int position[n_squares][2], int position_copy[n_squares][2]){
	for (int i = 0; i < n_squares; i = i + 1) {
		position_copy[i][0] = position[i][0];
		position_copy[i][1] = position[i][1];
	}
	return;
}


bool solve_puzzle(int Board[n_squares][n_pieces], int set_of_Peices[n_pieces][n_squares+1][2], int n_pieces_left_1, int position_of_Piece[n_squares][2]) {
	if(n_pieces_left_1 == -1){
		return true;
	 }
	else {
		int colour = set_of_Peices[n_pieces_left_1][n_squares][0]; // colour [][5][0]
		int Group = set_of_Peices[n_pieces_left_1][n_squares][1]; // group [][5][1]
		bool there_is_room;
		bool solved;
		int position_of_Piece_copy[n_squares][2];
		for (int i = 0; i < n_squares; i = i + 1) {
			for (int j = 0; j < n_pieces; j = j + 1) {
				if (Board[i][j] == 0) {
					for (int k = 0; k <= Group; k = k + 1) {
						for (int m = 0; m <= 3; m = m + 1) {
							there_is_room = available_position(Board, set_of_Peices[n_pieces_left_1], k, m, i, j, position_of_Piece);
							if (there_is_room) {
								copy_position(position_of_Piece, position_of_Piece_copy);
								put_piece_on_Board(Board, position_of_Piece, colour);
								solved = solve_puzzle(Board, set_of_Peices, n_pieces_left_1 - 1, position_of_Piece);
								if (solved) { return true; }
								remove_piece_from_Board(Board, position_of_Piece_copy);
							}
						}
					}
				}
			}
		}
		return false;
	}
}


int main()
{
	bool solved;
	int g[2] = { 0, 1 };
	//int Piece[5][2] = { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 2, 1 } };
	int position_of_Piece[5][2];
	int Board[n_squares][n_pieces];
	for (int i = 0; i < n_squares; i = i + 1) { for (int j = 0; j < n_pieces; j = j + 1) {
		Board[i][j] = 0;
	}}
	int set_of_Pieces[n_pieces][n_squares + 1][2] = { { {0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 1}, { 8, 1} },  //grey
						          { {0, 0}, {1, 0}, {2, 0}, {1, 1}, {2, 1}, { 5, 1} },  //pink 
							  { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {3, 1}, { 3, 1} },  //violet (asymmetric zigzag)
						          { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {2, 1}, { 2, 1} },  //brown
							  { {0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}, {10, 1} },  //regular blue (Z, symmetric zigzag)
							  { {0, 0}, {1, 0}, {2, 0}, {0, 1}, {2, 1}, { 9, 0} },  //yellow 
							  { {0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, { 7, 0} },  //light blue (corner, symmetric L)
							  { {0, 0}, {1, 0}, {2, 0}, {1, 1}, {1, 2}, { 6, 0} },  //dark green (T)
						          { {0, 0}, {1, 0}, {1, 1}, {2, 1}, {2, 2}, { 4, 0} },  //light green (W)
							  { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {3, 1}, { 1, 0} }   //orange (asymetric L)
													  };


	std::cout << std::endl;
	std::cout << "Solution of Katamino puzzle with " << n_pieces << " pieces:" << std::endl;
	
	clock_t t;
	t = clock();
	solved = solve_puzzle(Board, set_of_Pieces, n_pieces - 1, position_of_Piece);
	t = clock() - t;
	double cpu_time_used = ((double)t) / CLOCKS_PER_SEC;

	std::cout << std::endl;
	for (int i = 0; i < n_squares; i = i + 1) {
		for (int j = 0; j < n_pieces; j = j + 1) {
			if (Board[i][j] <= 9) {
				std::cout << "   " << Board[i][j] << "  ";
			}
			else {
				std::cout << "  " << Board[i][j] << "  ";
			}
		} std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Calculation time " << cpu_time_used << "  sec" << std::endl;
	return 0;
}
