#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 10

void clear_screen() {
    system("clear");  // Use "cls" instead of "clear" on Windows
}

void draw_game(int player_pos, int obstacle_pos) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == HEIGHT - 1 && x == player_pos)
                printf("P");
            else if (y == obstacle_pos && x == WIDTH - 1)
                printf("O");
            else
                printf(".");
        }
        printf("\n");
    }
}

int main() {
    int player_pos = WIDTH / 2;
    int obstacle_pos = 0;
    int score = 0;

    srand(time(NULL));

    while (1) {
        clear_screen();
        draw_game(player_pos, obstacle_pos);
        printf("Score: %d\n", score);

        // Move player
        char input;
        if (scanf(" %c", &input) == 1) {
            if (input == 'a' && player_pos > 0)
                player_pos--;
            else if (input == 'd' && player_pos < WIDTH - 1)
                player_pos++;
        }
        int x;
        srand(time(NULL));
        x = (rand()%(20 -1));
        // Move obstacle
        obstacle_pos++;
        if (obstacle_pos >= HEIGHT) {
            obstacle_pos = 0;
            score++;
        }

        // Check collision
        if (obstacle_pos == HEIGHT - 1 && player_pos == WIDTH - 1) {
            printf("Game Over! Final Score: %d\n", score);
            break;
        }

        usleep(100000);  // Delay for smoother gameplay
    }

    return 0;
}