#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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
	printf("\n");
}

/* known bug: check_board function only checks 3 * 3 instead n * n */
int check_board(int board_sz, char board[][board_sz])
{
	char ref;
	int repeated = 0;
	for (int i = 0; i < board_sz; i++) {
		/* horizontal */
		ref = board[i][0];
		for (int j = 1; j < board_sz; j ++) {
			if (board[i][j] != ref) {
				repeated = 1;
				break;
			}
		}
		if (!repeated) {
			switch (ref) {
				case 'X':
					return 1;
				case 'O':
					return -1;
			}
		}
		repeated = 0;

		/* vertical */
		ref = board[0][i];
		for (int j = 0; j < board_sz; j ++) {
			if (board[j][i] != ref) {
				repeated = 1;
				break;
			}
		}
		if (!repeated) {
			switch (ref) {
				case 'X':
					return 1;
				case 'O':
					return -1;
			}
		}
		repeated = 0;
	}

	/* backward diagnol */
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
		if (board[0][0] == 'X')
			return 1;
		else if (board[0][0] == 'O')
			return -1;
	}

	/* forward diagnol */
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
		if (board[0][2] == 'X')
			return 1; /* X wins */
		else if (board[0][2] == 'O')
			return -1; /* O wins */
	}

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

			/* initial boar states starts at
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

	/* check if the `size` flag value matches the initial board input
	 * size
	 */
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

	init_board(board_sz, board, board_val);
	print_board(board_sz, board);

	print_res(check_board(board_sz, board));

	return 0;
}
