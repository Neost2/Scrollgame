
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define PADDLE_HEIGHT 4
#define PADDLE_WIDTH 1
#define BALL_SPEED 1
#define PADDLE_SPEED 1
#define DELAY 30000

// Paddle structure
typedef struct {
    int x, y;
    int height, width;
} Paddle;

// Ball structure
typedef struct {
    int x, y;
    int vel_x, vel_y;
} Ball;

// Game field
int field_width, field_height;

// Function to draw a paddle
void draw_paddle(Paddle* paddle) {
    for (int i = 0; i < paddle->height; i++) {
        mvprintw(paddle->y + i, paddle->x, "|");
    }
}

// Function to draw the ball
void draw_ball(Ball* ball) {
    mvprintw(ball->y, ball->x, "O");
}

// Initialize the game
void init_game(Paddle* left_paddle, Paddle* right_paddle, Ball* ball) {
    getmaxyx(stdscr, field_height, field_width);

    // Set paddle positions
    left_paddle->x = 2;
    left_paddle->y = field_height / 2 - PADDLE_HEIGHT / 2;
    left_paddle->height = PADDLE_HEIGHT;
    left_paddle->width = PADDLE_WIDTH;

    right_paddle->x = field_width - 3;
    right_paddle->y = field_height / 2 - PADDLE_HEIGHT / 2;
    right_paddle->height = PADDLE_HEIGHT;
    right_paddle->width = PADDLE_WIDTH;

    // Set ball initial position and velocity
    ball->x = field_width / 2;
    ball->y = field_height / 2;
    ball->vel_x = BALL_SPEED;
    ball->vel_y = BALL_SPEED;
}

// Update ball movement
void update_ball(Ball* ball, Paddle* left_paddle, Paddle* right_paddle) {
    ball->x += ball->vel_x;
    ball->y += ball->vel_y;

    // Ball collision with top or bottom
    if (ball->y <= 0 || ball->y >= field_height - 1) {
        ball->vel_y = -ball->vel_y;
    }

    // Ball collision with paddles
    if (ball->x == left_paddle->x + PADDLE_WIDTH &&
        ball->y >= left_paddle->y &&
        ball->y < left_paddle->y + PADDLE_HEIGHT) {
        ball->vel_x = -ball->vel_x;
    }

    if (ball->x == right_paddle->x - 1 &&
        ball->y >= right_paddle->y &&
        ball->y < right_paddle->y + PADDLE_HEIGHT) {
        ball->vel_x = -ball->vel_x;
    }

    // Ball out of bounds
    if (ball->x <= 0 || ball->x >= field_width - 1) {
        ball->x = field_width / 2;
        ball->y = field_height / 2;
    }
}

// Move paddle based on input
void move_paddle(Paddle* paddle, int direction) {
    paddle->y += direction * PADDLE_SPEED;
    if (paddle->y < 0) {
        paddle->y = 0;
    }
    if (paddle->y + paddle->height > field_height) {
        paddle->y = field_height - paddle->height;
    }
}

// Main game loop
int main() {
    Paddle left_paddle, right_paddle;
    Ball ball;
    int ch;

    initscr();            // Start ncurses mode
    noecho();             // Don't echo input
    cbreak();             // Disable line buffering
    timeout(0);           // Non-blocking input
    curs_set(FALSE);      // Hide cursor

    init_game(&left_paddle, &right_paddle, &ball);

    while (1) {
        // Clear screen
        clear();

        // Input handling
        ch = getch();
        if (ch == 'q') break; // Quit game
        if (ch == 'w') move_paddle(&left_paddle, -1);
        if (ch == 's') move_paddle(&left_paddle, 1);
        if (ch == 'i') move_paddle(&right_paddle, -1);
        if (ch == 'k') move_paddle(&right_paddle, 1);

        // Update game state
        update_ball(&ball, &left_paddle, &right_paddle);

        // Draw paddles and ball
        draw_paddle(&left_paddle);
        draw_paddle(&right_paddle);
        draw_ball(&ball);

        // Refresh screen
        refresh();

        // Control speed
        usleep(DELAY);
    }

    // End ncurses mode
    endwin();

    return 0;
}
