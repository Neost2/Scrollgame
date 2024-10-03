#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WORLD_WIDTH 40     // Width of the game world
#define SCREEN_HEIGHT 10   // Height of the visible screen
#define PLAYER_CHAR '@'    // Player character
#define OBSTACLE_CHAR '#'  // Obstacle character
#define EMPTY_CHAR ' '     // Empty space

// Function to initialize ncurses
void initNcurses() {
    initscr();             // Start ncurses mode
    cbreak();              // Disable line buffering
    noecho();              // Don't show input characters
    curs_set(FALSE);       // Hide the cursor
    keypad(stdscr, TRUE);  // Enable special keys like arrows
    timeout(100);          // Set input timeout for non-blocking input
}

// Function to draw the game screen
void drawGame(int playerPos, int obstacles[]) {
    clear();  // Clear the screen before drawing
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < WORLD_WIDTH; x++) {
            if (y == SCREEN_HEIGHT - 1 && x == playerPos) {
                mvaddch(y, x, PLAYER_CHAR);  // Draw the player
            }
            else if (obstacles[x] == y) {
                mvaddch(y, x, OBSTACLE_CHAR);  // Draw the obstacle
            }
            else {
                mvaddch(y, x, EMPTY_CHAR);  // Draw empty space
            }
        }
    }
    refresh();  // Refresh the screen to display the changes
}

// Function to move the obstacles left
void moveObstacles(int obstacles[]) {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        // Randomly decide to move the obstacle up or down
        if (obstacles[i] != -1) {
           /* if (rand() % 2 == 0 && obstacles[i] > 0) {
                obstacles[i]++;  // Move down
            }
            else*/ if (obstacles[i] < SCREEN_HEIGHT - 1) {
                obstacles[i]++;  // Move down
            }
            if (obsticals[i] = SCREEN_HEIGHT - 10) {
                obsticals[i] = NULL;
            }
        }

        // Randomly place a new obstacle
        if (rand() % 100 == 0) {  // Adjust frequency of new obstacles
            obstacles[i] = rand() % SCREEN_HEIGHT; // Random position
        }
    }
}

// Function to check if the player hit an obstacle
int checkCollision(int playerPos, int obstacles[]) {
    return obstacles[playerPos] == SCREEN_HEIGHT - 1;  // Check if obstacle is in the same place as the player
}

int main() {
    int playerPos = WORLD_WIDTH / 2;  // Start player in the middle of the screen
    int obstacles[WORLD_WIDTH];       // Array to store obstacle positions
    int running = 1;                  // Game running flag

    // Initialize random number generator and ncurses
    srand(time(NULL));
    initNcurses();

    // Initialize the obstacles array
    for (int i = 0; i < WORLD_WIDTH; i++) {
        obstacles[i] = -1;  // No obstacles at the start
    }

    // Main game loop
    while (running) {
        // Draw the game world
        drawGame(playerPos, obstacles);

        // Get user input (non-blocking)
        int ch = getch();
        switch (ch) {
        case KEY_LEFT:
            if (playerPos > 0) playerPos--;  // Move player left
            break;
        case KEY_RIGHT:
            if (playerPos < WORLD_WIDTH - 1) playerPos++;  // Move player right
            break;
        case 'q':  // Quit the game
            running = 0;
            break;
        }

        // Move the obstacles
        moveObstacles(obstacles);

        // Check for collisions
        if (checkCollision(playerPos, obstacles)) {
            running = 0;  // End the game if there's a collision
        }

        // Delay to control game speed (lower is faster)
        usleep(100000);  // 100ms delay
    }

    // Game over message
    clear();
    mvprintw(SCREEN_HEIGHT / 2, WORLD_WIDTH / 2 - 5, "Game Over!");
    refresh();
    getch();  // Wait for a key press

    // End ncurses mode
    endwin();

    return 0;
}