// ==== ������ ���� Earthworm Game by jhjang, 2021.09. ====
// 
// �л����� ���������� ������ �� �ֵ��� �������� ��°�� ����� ����� ����ߴ�.
// ����� �� ������Ʈ(������, +, -)�� ��ǥ�� ���¸� ����ϴ� ���� �� ���� ������ �� �ִµ�,
// �ð��� ���� ����� �ѹ� �õ��غ��� �ٶ�

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define WIDTH   18
#define HEIGHT  10

// �迭 ���� �Լ��� �����ϴ� �͵� ������ �� ���� ���� ������ �θ� �������� ���� �ۼ��ϱ⵵ ���ϴ�.
// You can deliver array('board') through function parameters. 
// However, in this case, it's much readable to define 'board' and the other variables as global variables.
char board[HEIGHT][WIDTH];

// ������ ���� ��ġ
// the current position of the worm
int x, y;

// ���� ����(worm_x, worm_y�� ����/���ҷ�)
// the current direction. i.e. increment/decrement values for 'worm_x' and 'worm_y'
int dx, dy;

int dx_add, dy_add;

int point_change = 0;

//'+', '-'���� ��ǥ����
int point_x, point_y;

//'+', '-'����
int NUM_pos = 5, NUM_neg = 5;

void initialize(int, int);
void display();
bool is_blocked();
void turn(int);
void move();

void make_tail(int);
void point_loc();
int count_remain(char);
void reloc_point(char);
void add_move(int);
void rebuild_walls();

// �����ǰ� ������ �ʱ�ȭ initialize game baord & earthworm
void initialize(int start_x, int start_y) {

    // ��, �Ʒ� ���κ� horizontal walls
    for (int i = 0; i < WIDTH; i++) {
        board[0][i] = board[HEIGHT - 1][i] = '#';
    }

    // ������ ���� inner space
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            board[i][j] = ' ';
        }
    }

    // ��, �� ���κ� vertical walls
    for (int i = 0; i < HEIGHT; i++) {
        board[i][0] = board[i][WIDTH - 1] = '#';
    }

    // ������ �ʱ� ��ġ�� ����
    // initial position & direction of earthworm
    x = start_x;
    y = start_y;
    dx = dy = 1;
    board[x][y] = '@';
}

// ������ ȭ�� ��� display the game board
void display() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

// ���� �̵��� ��ġ�� �����Ѵ�.
// Investigate the next position in the current direction
bool is_blocked() {
    return board[x + dx][y + dy] == '#';
}



void turn(int length) {
    for (int i = 0; i < length; i++) {
        board[x - i * dx][y - i * dy] = ' ';
    }
    if (board[x + dx][y] == '#')
        dx = -dx;
    else
        dy = -dy;
}

void make_tail(int length) {
    if (board[x - 2 * dx][y] == '#') {
        for (int i = 2; i < length; i++) {
            board[x - (i - 2) * -dx][(y - 2 * dy) - (i - 2) * dy] = '@';
        }
    }
    else if (board[x][y - 2 * dy] == '#') {
        for (int i = 2; i < length; i++) {
            board[(x - 2 * dx) - (i - 2) * dx][y + (i - 2) * dy] = '@';
        }
    }
}

void move() {
    board[x][y] = ' ';
    x += dx;
    y += dy;
    board[x][y] = '@';
}

//'+', '-'��ġ
void point_loc() {
    //'+'
    for (int i = 0; i < NUM_pos; i++) {
        point_x = rand() % (HEIGHT - 2) + 1;
        point_y = rand() % (WIDTH - 2) + 1;
        board[point_x][point_y] = '+';
    }
    //'-'
    for (int i = 0; i < NUM_neg; i++) {
        point_x = rand() % (HEIGHT - 2) + 1;
        point_y = rand() % (WIDTH - 2) + 1;
        board[point_x][point_y] = '-';
    }
}

//'+','-' ������ Ž��
int count_remain(char type) {
    int remain_pos = 0, remain_neg = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int k = 0; k < WIDTH; k++) {
            if (board[i][k] == '+') {
                remain_pos++;
            }
            else if (board[i][k] == '-') {
                remain_neg++;
            }
        }
    }
    if (type == '+') {
        return remain_pos;
    }
    if (type == '-') {
        return remain_neg;
    }
}

//����Ʈ ���ġ
void reloc_point(char type_point) {
    int reloc_x, reloc_y;

    reloc_x = rand() % (HEIGHT - 2) + 1;
    reloc_y = rand() % (WIDTH - 2) + 1;

    if (board[reloc_x][reloc_y] == '@' || board[reloc_x][reloc_y] == '+' || board[reloc_x][reloc_y] == '-') {
        while (board[reloc_x][reloc_y] == '@' && board[reloc_x][reloc_y] == '+' && board[reloc_x][reloc_y] == '-') {
            reloc_x = rand() % (HEIGHT - 2) + 1;
            reloc_y = rand() % (WIDTH - 2) + 1;
        }
    }
    board[reloc_x][reloc_y] = type_point;
}

//�߰��� �������� ������
void add_move(int length) {
    for (int i = 0; i < length; i++) {
        board[x - (i + 1) * dx][y - (i + 1) * dy] = ' ';
        board[x - i * dx][y - i * dy] = '@';
    }
}

void rebuild_walls() {
    for (int i = 0; i < WIDTH; i++) {
        board[0][i] = board[HEIGHT - 1][i] = '#';
    }

    for (int i = 0; i < HEIGHT; i++) {
        board[i][0] = board[i][WIDTH - 1] = '#';
    }
}

int main(void)
{
    int length = 1;

    srand(time(0));
    initialize(1, 1);
    point_loc();
    while (1) {
        while (is_blocked()) {
            turn(length);
        }
        if (count_remain('+') != NUM_pos) {
            length++;
            reloc_point('+');
        }
        else if (count_remain('-') != NUM_neg) {
            if (length > 1) {
                length--;
                board[x - length * dx][y - length * dy] = ' ';
            }
            reloc_point('-');
        }
        move();
        add_move(length);
        make_tail(length);
        rebuild_walls();
        printf("length = %d\n", length);
        display();
        Sleep(300);
        system("cls");
    }
    return 0;
}