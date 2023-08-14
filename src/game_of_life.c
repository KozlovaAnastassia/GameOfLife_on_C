#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80

void read_matrix(char *filename, int matrix[ROWS][COLS]);
void rewrite_matrix(int matrix[ROWS][COLS]);
void print_matrix(int matrix[ROWS][COLS]);

int main() {
    int fileNumb;
    int flag = 0;
    int matrix[ROWS][COLS];
    printf("Choose the file number from 1 to 5\n\nIf you want change the speed press '+' or '-'\n");
    if (scanf("%d", &fileNumb) != 1 || fileNumb < 1 || fileNumb > 5) flag += 1;
    if (flag == 0) {
        int speed = 150000;
        initscr();

        nodelay(stdscr, 1);
        keypad(stdscr, 1);

        switch (fileNumb) {
            case 1:
                read_matrix("cycle_dynamic.txt", matrix);
                break;
            case 2:
                read_matrix("unlimited_rain.txt", matrix);
                break;
            case 3:
                read_matrix("generation_blinders.txt", matrix);
                break;
            case 4:
                read_matrix("cow.txt", matrix);
                break;
            case 5:
                read_matrix("ping_pong.txt", matrix);
                break;
            default:
                break;
        }

        while (1) {
            usleep(speed);
            int speedNano = getch();
            if (speedNano == '-')
                speed += 10000;
            else if (speedNano == '+' && speed - 10000 > 0)
                speed -= 10000;
            else if (speedNano == 'q' || speedNano == 'w' || speedNano == 'W' || speedNano == 'Q')
                break;

            print_matrix(matrix);
            rewrite_matrix(matrix);
            move(0, 0);
            refresh();
            timeout(1);
        }
        clear();
        refresh();
        endwin();
    } else
        printf("n/a");
    return 0;
}

void read_matrix(char *filename, int matrix[ROWS][COLS]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printw("Ошибка открытия файла.\n");
        return;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
}

void rewrite_matrix(int matrix[ROWS][COLS]) {
    int count = 0;
    int matrix2[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[(ROWS + i - 1) % ROWS][(COLS + j - 1) % COLS] == 1) count += 1;
            if (matrix[(ROWS + i - 1) % ROWS][j] == 1) count += 1;
            if (matrix[(ROWS + i - 1) % ROWS][(COLS + j + 1) % COLS] == 1) count += 1;
            if (matrix[i][(COLS + j - 1) % COLS] == 1) count += 1;
            if (matrix[i][(COLS + j + 1) % COLS] == 1) count += 1;
            if (matrix[(ROWS + i + 1) % ROWS][(COLS + j - 1) % COLS] == 1) count += 1;
            if (matrix[(ROWS + i + 1) % ROWS][j] == 1) count += 1;
            if (matrix[(ROWS + i + 1) % ROWS][(COLS + j + 1) % COLS] == 1) count += 1;

            if (matrix[i][j] == 1) {
                if (count < 2 || count > 3) {
                    matrix2[i][j] = 0;
                } else {
                    matrix2[i][j] = 1;
                }
            } else {
                if (count == 3) {
                    matrix2[i][j] = 1;
                } else {
                    matrix2[i][j] = 0;
                }
            }
            count = 0;
        }
    }
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) matrix[i][j] = matrix2[i][j];
}

void print_matrix(int matrix[ROWS][COLS]) {
    int i, j;
    printf("\33[0d\33[2J");
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (matrix[i][j] == 1) {
                printw("*");
            } else {
                printw(" ");
            }
        }
        printw("\n");
    }
}
