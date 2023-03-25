#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	/* start loop from argv[1]*/
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
	if ((argc - board_arg_index) != (board_sz * board_sz)) {
		printf("invalid arguments: board size mismatch\n"
		       "%s\n", USAGE);
		exit(-1);
	}

	char board[board_sz][board_sz];

	/* string array containing the initial board state */
	char board_val[board_sz * board_sz][2];
	memset(board_val, sizeof(board_val), 0);

	for (int i = board_arg_index, j = 0;
	     i < argc && j < board_sz * board_sz;
	     i++, j++) {
		strncpy(board_val[j], argv[i], 2);
	}

	init_board(board_sz, board, board_val);
	print_board(board_sz, board);

	return 0;
}
