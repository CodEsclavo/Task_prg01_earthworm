// ==== 지렁이 게임 Earthworm Game by jhjang, 2021.09. ====
// 
// 학생들이 직관적으로 이해할 수 있도록 게임판을 통째로 만드는 방법을 사용했다.
// 사실은 각 오브젝트(지렁이, +, -)의 좌표나 상태만 기억하는 편이 더 쉽게 구현할 수 있는데,
// 시간이 남는 사람은 한번 시도해보기 바람

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define WIDTH   18
#define HEIGHT  10

// 배열 등을 함수에 전달하는 것도 좋지만 이 경우는 전역 변수로 두면 가독성도 높고 작성하기도 편하다.
// You can deliver array('board') through function parameters. 
// However, in this case, it's much readable to define 'board' and the other variables as global variables.
char board[HEIGHT][WIDTH];

// 지렁이 현재 위치
// the current position of the worm
int x, y;

// 현재 방향(worm_x, worm_y의 증가/감소량)
// the current direction. i.e. increment/decrement values for 'worm_x' and 'worm_y'
int dx, dy;

int dx_add, dy_add;

int point_change = 0;

//'+', '-'들의 좌표변수
int point_x, point_y;

//'+', '-'갯수
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

// 게임판과 지렁이 초기화 initialize game baord & earthworm
void initialize(int start_x, int start_y) {

    // 위, 아래 가로벽 horizontal walls
    for (int i = 0; i < WIDTH; i++) {
        board[0][i] = board[HEIGHT - 1][i] = '#';
    }

    // 안쪽의 공백 inner space
    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            board[i][j] = ' ';
        }
    }

    // 좌, 우 세로벽 vertical walls
    for (int i = 0; i < HEIGHT; i++) {
        board[i][0] = board[i][WIDTH - 1] = '#';
    }

    // 지렁이 초기 위치와 방향
    // initial position & direction of earthworm
    x = start_x;
    y = start_y;
    dx = dy = 1;
    board[x][y] = '@';
}

// 게임판 화면 출력 display the game board
void display() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

// 다음 이동할 위치를 조사한다.
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

//'+', '-'배치
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

//'+','-' 개수를 탐색
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

//포인트 재배치
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

//추가된 지렁이의 움직임
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