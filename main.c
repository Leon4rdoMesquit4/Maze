#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define selected_width 100
#define selected_height 100
#define WIDTH (selected_width % 2 == 0 ? selected_width + 1 : selected_width + 2)
#define HEIGHT (selected_height % 2 == 0 ? selected_height + 1 : selected_height + 2)

char maze[HEIGHT][WIDTH];
int visited[HEIGHT][WIDTH];

int dx[] = { 0, 1, 0, -1 };
int dy[] = { -1, 0, 1, 0 };

int startX = 1, startY = 1;
int endX = WIDTH - 2, endY = HEIGHT - 2;
int found = 0;

void initMaze() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            maze[y][x] = '#';
}

int isInBounds(int x, int y) {
    return x > 0 && x < WIDTH-1 && y > 0 && y < HEIGHT-1;
}

void shuffle(int *array, int n) {
    for (int i = 0; i < n - 1; i++) {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[i];
        array[i] = array[j];
        array[j] = t;
    }
}

void generateMaze(int x, int y) {
    maze[y][x] = ' ';
    int dirs[] = {0, 1, 2, 3};
    shuffle(dirs, 4);

    for (int i = 0; i < 4; i++) {
        int dir = dirs[i];
        int nx = x + dx[dir]*2;
        int ny = y + dy[dir]*2;

        if (isInBounds(nx, ny) && maze[ny][nx] == '#') {
            maze[y + dy[dir]][x + dx[dir]] = ' ';
            generateMaze(nx, ny);
        }
    }
}

void writeMazeToFile(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Erro ao abrir o arquivo para escrita");
        exit(1);
    }
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            fputc(maze[y][x], fp);
        }
        fputc('\n', fp);
    }
    fclose(fp);
}

void readMazeFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo para leitura");
        exit(1);
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char c = fgetc(fp);
            if (c == EOF) break;
            if (c == '\n') c = fgetc(fp);  
            maze[y][x] = c;
        }
    }

    fclose(fp);
}

void solveMaze(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    if (maze[y][x] == '#' || visited[y][x]) return;

    visited[y][x] = 1;

    if (x == endX && y == endY) {
        found = 1;
        maze[y][x] = '.';
        return;
    }

    solveMaze(x + 1, y);
    if (found) { maze[y][x] = '.'; return; }

    solveMaze(x - 1, y);
    if (found) { maze[y][x] = '.'; return; }

    solveMaze(x, y + 1);
    if (found) { maze[y][x] = '.'; return; }

    solveMaze(x, y - 1);
    if (found) { maze[y][x] = '.'; return; }
}

int main() {
    srand(time(NULL));

    initMaze();
    generateMaze(startX, startY);

    maze[startY][startX] = ' ';
    maze[endY][endX] = ' ';

    writeMazeToFile("labirinto.txt");

    readMazeFromFile("labirinto.txt");

    solveMaze(startX, startY);

    writeMazeToFile("labirinto_resolvido.txt");

    if (found)
        printf("Caminho encontrado com DFS e salvo em 'labirinto_resolvido.txt'\n");
    else
        printf("Nenhum caminho encontrado.\n");

    return 0;
}
