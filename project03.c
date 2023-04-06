#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#define USAGE "Usage: ./project03" \
	      " [-s board size / -t TUI interactive mode] [initial board state]"

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
	bool repeated = true;

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
					repeated = false;
					break;
				}
			}
			if (repeated)
				return check_win(ref);
			repeated = true;
		}

		/* vertical */
		ref = board[0][i];
		if (ref != '_') {
			for (int j = 0; j < board_sz; j++) {
				if (board[j][i] != ref) {
					repeated = false;
					break;
				}
			}
			if (repeated)
				return check_win(ref);
			repeated = true;
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
					repeated = false;
					break;
				}
			}
		}
	}
	if (repeated)
		return check_win(ref);
	repeated = true;

for_diag:
	/* forward diagonal */
	ref = board[0][board_sz - 1];
	if (ref == '_')
		goto term_state;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (i + j == board_sz - 1) {
				if (board[i][j] != ref) {
					repeated = false;
					break;
				}
			}
		}
	}
	if (repeated)
		return check_win(ref);
	repeated = true;

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

int mm(int board_sz, char board[][board_sz], int depth, bool is_max)
{
	if (check_board(board_sz, board) == 1)
		return -10;
	else if (check_board(board_sz, board) == -1)
		return 10;
	else if (check_board(board_sz, board) == 0)
		return 0;

	if (is_max) {
		int best_score = INT_MIN;
		for (int i = 0; i < board_sz; i++) {
			for (int j = 0; j < board_sz; j++) {
				if (board[i][j] == '_') {
					board[i][j] = 'O'; /* program */
					int score = mm(board_sz, board,
						       depth + 1, false);
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
						       depth + 1, true);
					board[i][j] = '_'; /* reset */
					best_score = (int) fmin(score,
								best_score);
				}
			}
		}
		return best_score;
	}
}

void best_mv(int board_sz, char board[][board_sz], int *mv_r, int *mv_c)
{
	/* check if the board is empty */
	bool repeated = true;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (board[i][j] != '_') {
				repeated = false;
				break;
			}
		}
	}
	if (repeated) {
		*mv_r = 0;
		*mv_c = 0;
		return;
	}

	int best_score = INT_MIN;
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			if (board[i][j] == '_') {
				board[i][j] = 'O'; /* program */
				/*
				 * check if the game is terminated after the
				 * first move
				 */
				if (check_board(board_sz, board) <= 1) {
					*mv_r = i;
					*mv_c = j;
					board[i][j] = '_';
					return;
				}
				int score = mm(board_sz, board, 0, false);
				board[i][j] = '_';
				if (score > best_score) {
					best_score = score;
					*mv_r = i;
					*mv_c = j;
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
	bool sflag = false;
	bool interact = false;
	int board_sz;
	int board_arg_index;
	/* start loop from argv[1] */
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			sflag = true;

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
	if (!sflag) {
		board_sz = 3;
		board_arg_index = 1;
	}

	if (argc < 4)
		interact = true;

	/*
	 * check if the `size` flag value matches the initial board input
	 * size
	 */
	if (!interact) {
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

		int mv_r;
		int mv_c;
		best_mv(board_sz, board, &mv_r, &mv_c);
		printf("O: %d %d\n", mv_r, mv_c);

		return 0;
	}

	/* interactive when argc < 4 */
	bool tflag = false;
	if (argc >= 2) {
		if (!strcmp(argv[1], "-t") || !strcmp(argv[1], "--tui")) {
			tflag = true;
		} else {
			printf("invalid option: %s\n%s\n", argv[1], USAGE);
			exit(-1);
		}
	}

	board_sz = 3;
	char board[board_sz][board_sz];
	for (int i = 0; i < board_sz; i++) {
		for (int j = 0; j < board_sz; j++) {
			board[i][j] = '_';
		}
	}

	if (tflag)
		system("clear");
	print_board(board_sz, board);

	bool game_over = false;
	while (!game_over) {
		int r;
		int c;

		printf("row: ");
		if (!scanf("%d", &r) || r < 0 || r > 2)
			goto scanf_err;

		printf("col: ");
		if (!scanf("%d", &c) || c < 0 || c > 2)
			goto scanf_err;

		if (board[r][c] != '_')
			goto mv_err;

		if (tflag)
			system("clear");
		board[r][c] = 'X';

		int mv_r;
		int mv_c;
		best_mv(board_sz, board, &mv_r, &mv_c);
		board[mv_r][mv_c] = 'O';
		if (!tflag)
			printf("\n");
		print_board(board_sz, board);

		if (check_board(board_sz, board) < 1) {
			print_res(check_board(board_sz, board));
			game_over = true;
		}
	}

	return 0;

scanf_err:
	printf("scanf failed: illegal coordinate input\n");
	exit(-1);

mv_err:
	printf("illegal move: coordinate already occupied\n");
	exit(-1);
}
