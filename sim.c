#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};


/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

// Define the winning conditions as combinations of line indices.
int winningConditions[20][3] = {
    {0, 1, 5}, {0, 2, 6}, {0, 3, 7}, {0, 4, 8},
    {1, 2, 9}, {1, 3, 10}, {1, 4, 11},
    {2, 3, 12}, {2, 4, 13},
    {3, 4, 14},
    {5, 6, 9}, {5, 7, 10}, {5, 8, 11},
    {6, 7, 12}, {6, 8, 13},
    {7, 8, 14},
    {9, 10, 12}, {9, 11, 13},
    {10, 11, 14},
    {12, 13, 14}
};

int has_won(board_t board, player_t player) {
    player_t opponent = (player == RED) ? BLUE : RED;

    for (int i = 0; i < 20; ++i) {
        int index1 = winningConditions[i][0];
        int index2 = winningConditions[i][1];
        int index3 = winningConditions[i][2];

        // Check if these three lines form a triangle with the opponent's color.
        if (board[index1] == opponent && board[index2] == opponent && board[index3] == opponent) {
            return 1; // The opponent has formed a triangle of their color, so the current player wins.
        }
    }

    return 0; // No winning condition has been met.
}

int is_full(board_t board) {
    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            return 0;
        }
    }
    return 1;
}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

int evaluate(board_t board, player_t player) {
    if (has_won(board, player)) {
        return 1;
    } else if (has_won(board, (player == RED) ? BLUE : RED)) {
        return -1;
    } else {
        return 0;
    }
}

void print_board(board_t board) {
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case RED: printf("R  "); break;
        case BLUE: printf("B  "); break;
        case NO: printf(".  "); break;
        }
    }
    printf("\n");
}

move_t best_move(board_t board, player_t player, int depth, int alpha, int beta) {
    move_t bestMove;
    bestMove.line = -1;
    bestMove.score = (player == RED) ? INT_MIN : INT_MAX;

    if (depth == 0 || is_full(board)) {
        bestMove.score = evaluate(board, player);
        return bestMove;
    }

    player_t opponent = (player == RED) ? BLUE : RED;

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            move_t opponentResponse = best_move(board, opponent, depth - 1, alpha, beta);

            if (player == RED && opponentResponse.score > bestMove.score) {
                bestMove.line = i;
                bestMove.score = opponentResponse.score;
                alpha = opponentResponse.score;
            } else if (player == BLUE && opponentResponse.score < bestMove.score) {
                bestMove.line = i;
                bestMove.score = opponentResponse.score;
                beta = opponentResponse.score;
            }

            board[i] = NO;

            if (alpha >= beta) {
                break; // Alpha-beta pruning
            }
        }
    }

    return bestMove;
}

int main() {
    board_t board = {NO}; // Initialize the board with all lines uncolored (NO).
    player_t player = RED; // User plays first
    player_t computer = BLUE; // Computer plays second

    printf("Welcome to the Triangle Game!\n");
    printf("You are playing as RED (R).\n");

    while (!is_full(board)) {
        printf("\nCurrent board:\n");
        print_board(board);

        if (player == RED) {
            int userMove;
            do {
                printf("Enter the line number (0-14) to color: ");
                scanf("%d", &userMove);
            } while (userMove < 0 || userMove > 14 || board[userMove] != NO);

            board[userMove] = player;
        } else {
            // Computer's turn
            move_t computerMove = best_move(board, computer, 3, INT_MIN, INT_MAX);
            printf("Computer chose line %d.\n", computerMove.line);
            board[computerMove.line] = computer;
        }

        if (has_won(board, player)) {
            printf("\nCurrent board:\n");
            print_board(board);
            printf("Congratulations! You have won!\n");
            break;
        } else if (has_won(board, computer)) {
            printf("\nCurrent board:\n");
            print_board(board);
            printf("Computer wins! Better luck next time.\n");
            break;
        }

        // Switch players
        player = (player == RED) ? BLUE : RED;
    }

    if (!has_won(board, RED) && !has_won(board, BLUE) && !is_full(board)) {
        printf("\nCurrent board:\n");
        print_board(board);
        printf("The game is a draw.\n");
    }

    return 0;
}
