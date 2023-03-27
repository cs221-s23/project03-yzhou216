#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define USAGE "Usage: ./project03 [-s board size] [initial board state]"

void init_board(int board_sz, char board[][board_sz], char board_val[][2])
{
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			board[i][j] = board_val[i * board_sz + j][0];
		}
	}
}

void print_board(int board_sz, char board[][board_sz])
{
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			printf(" %c ", board[i][j]);
			if (j < board_sz - 1)
				printf("|");
		}
		if (i < board_sz - 1) {
			printf("\n");
			for (int k = 0; k < board_sz; k++) {
				if (k == board_sz - 1)
					printf("---");
				else
					printf("---+");
			}
		}
		printf("\n");
	}
}

int check_win(char ref) {
	switch (ref) {
		case 'X':
			return 1;
		case 'O':
			return -1;
	}
}

int check_board(int board_sz, char board[][board_sz])
{
	char ref;
	int repeated = 0;

	for (int i = 0; i < board_sz; i++) {
		/* horizontal */
		ref = board[i][0];
		/*
		 * If ref is '_', escape to the next iteration because it
		 * indicates that the current coordinates don't determine the
		 * winner.
		 */
		if (ref != '_') {
			for (int j = 1; j < board_sz; j++) {
				if (board[i][j] != ref) {
					repeated = 1;
					break;
				}
			}
			if (!repeated)
				return check_win(ref);
			repeated = 0;
		}

		/* vertical */
		ref = board[0][i];
		if (ref != '_') {
			for (int j = 0; j < board_sz; j++) {
				if (board[j][i] != ref) {
					repeated = 1;
					break;
				}
			}
			if (!repeated)
				return check_win(ref);
			repeated = 0;
		}
	}

	/* backward diagonal */
	ref = board[0][0];
	if (ref == '_')
		goto for_diag;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (i == j) {
				if (board[i][j] != ref) {
					repeated = 1;
					break;
				}
			}
		}
	}
	if (!repeated)
		return check_win(ref);
	repeated = 0;

for_diag:
	/* forward diagonal */
	ref = board[0][board_sz - 1];
	if (ref == '_')
		goto term_state;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (i + j == board_sz - 1) {
				if (board[i][j] != ref) {
					repeated = 1;
					break;
				}
			}
		}
	}
	if (!repeated)
		return check_win(ref);
	repeated = 0;

term_state:
	/* termination state */
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (board[i][j] == '_')
				return 2; /* unfinished */
		}
	}

	/* draw */
	return 0;
}

int mm(int board_sz, char board[][board_sz], int depth, int is_max)
{
	if (check_board(board_sz, board) == 1)
		return -10;
	else if (check_board(board_sz, board) == -1)
		return 10;
	else if (check_board(board_sz, board) == 0)
		return 0;

	if (is_max == 0) {
		int best_score = INT_MIN;
		for (int i = 0; i < board_sz; i++) {
			for (int j = 0; j < board_sz; j++) {
				if (board[i][j] == '_') {
					board[i][j] = 'O'; /* program */
					int score = mm(board_sz, board,
						       depth + 1, 1);
					board[i][j] = '_'; /* reset */
					best_score = (int) fmax(score,
								best_score);
				}
			}
		}
		return best_score;
	} else {
		int best_score = INT_MAX;
		for (int i = 0; i < board_sz; i++) {
			for (int j = 0; j < board_sz; j++) {
				if (board[i][j] == '_') {
					board[i][j] = 'X'; /* human */
					int score = mm(board_sz, board,
						       depth + 1, 0);
					board[i][j] = '_'; /* reset */
					best_score = (int) fmin(score,
								best_score);
				}
			}
		}
		return best_score;
	}
}

void best_move(int board_sz, char board[][board_sz],
	       int *move_r, int *move_c)
{
	/* check if the board is empty */
	int repeated = 0;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (board[i][j] != '_') {
				repeated = 1;
				break;
			}
		}
	}
	if (!repeated) {
		*move_r = 0;
		*move_c = 0;
		return;
	}

	int best_score = INT_MIN;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (board[i][j] == '_') {
				board[i][j] = 'O'; /* program */
				if (check_board(board_sz, board) == 1 ||
				    check_board(board_sz, board) == -1 ||
				    check_board(board_sz, board) == 0) {
					*move_r = i;
					*move_c = j;
					board[i][j] = '_';
					return;
				}
				int score = mm(board_sz, board, 0, 1);
				board[i][j] = '_';
				if (score > best_score) {
					best_score = score;
					*move_r = i;
					*move_c = j;
				}
			}
		}
	}
}

void print_res(int res)
{
	switch (res) {
		case 1:
			printf("X wins\n");
			break;
		case -1:
			printf("O wins\n");
			break;
		case 2:
			printf("unfinished\n");
			break;
		default:
			printf("draw\n");
	}
}

int is_int(char *str)
{
	for (int i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i]))
			return 1;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int sflag = 1;
	int interact = 1;
	int board_sz;
	int board_arg_index;
	/* start loop from argv[1] */
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			sflag = 0;

			if (i + 1 == argc) {
				printf("not enough arguments\n");
				exit(-1);
			}

			if (is_int(argv[i + 1])) {
				printf("invalid arguments: "
				       "`size` flag value must be integer\n"
				       "%s\n", USAGE);
				exit(-1);
			}

			board_sz = atoi(argv[i + 1]);

			/*
			 * initial boar states starts at
			 * argv[board_arg_index]
			 */
			board_arg_index = i + 2;
		}
	}

	/* size flag missing: default board size to 3 */
	if (sflag) {
		board_sz = 3;
		board_arg_index = 1;
	}

	if (argc < 4) {
		interact = 0;
	}

	/*
	 * check if the `size` flag value matches the initial board input
	 * size
	 */
	if (interact) {
		if (argc - board_arg_index != (int) pow(board_sz, 2)) {
			printf("invalid arguments: board size mismatch\n"
			       "%s\n", USAGE);
			exit(-1);
		}

		char board[board_sz][board_sz];

		/* string array containing the initial board state */
		char board_val[(int) pow(board_sz, 2)][2];
		memset(board_val, sizeof(board_val), 0);

		for (int i = board_arg_index, j = 0;
		     i < argc && j < (int) pow(board_sz, 2);
		     i++, j++) {
			strncpy(board_val[j], argv[i], 2);
		}

		int X_count = 0;
		int O_count = 0;
		for (int i = 0; i < (int) pow(board_sz, 2); i++) {
			if (!strcmp(board_val[i], "X"))
				X_count++;
			if (!strcmp(board_val[i], "O"))
				O_count++;

			if (strcmp(board_val[i], "X") &&
			    strcmp(board_val[i], "O") &&
			    strcmp(board_val[i], "_")) {
				printf("illegal board input\n");
				exit(-1);
			}
		}

		/*
		 * X_count must be either equal to or one more than O_count
		 * since the program only plays O
		 */
		int XO_diff = X_count - O_count;
		if (XO_diff != 1 && XO_diff != 0) {
			printf("illegal board input\n");
			exit(-1);
		}

		init_board(board_sz, board, board_val);
		print_board(board_sz, board);

		int move_r;
		int move_c;
		best_move(board_sz, board, &move_r, &move_c);
		printf("O: %d %d\n", move_r, move_c);

		return 0;
	}

	/* interactive when argc < 4 */
	board_sz = 3;
	char board[board_sz][board_sz];
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			board[i][j] = '_';
		}
	}

	system("clear");
	print_board(board_sz, board);

	int game_over = 1;
	while (game_over) {
		int r;
		int c;

		printf("row: ");
		if (!scanf("%d", &r) || r < 0 || r > 2)
			goto scanf_error;

		printf("col: ");
		if (!scanf("%d", &c) || c < 0 || c > 2)
			goto scanf_error;

		system("clear");
		board[r][c] = 'X';

		int move_r;
		int move_c;
		best_move(board_sz, board, &move_r, &move_c);
		board[move_r][move_c] = 'O';
		print_board(board_sz, board);

		if (check_board(board_sz, board) < 1) {
			print_res(check_board(board_sz, board));
			game_over = 0;
		}
	}

	return 0;

scanf_error:
	printf("scanf failed: illegal coordinate input\n");
	exit(-1);
}
