#include <stdio.h>
#include <stdlib.h>

char** maze;
int** visited;
int rows;
int cols;
int start_row;
int start_col;

enum terrain {
    empty,
    wall,
    finish,
    twig
};

void alloc_maze() {
    maze = malloc(rows * sizeof(char*));
    int i;
    for (i = 0; i < rows; i++) {
        maze[i] = malloc(cols * sizeof(char*));
    }
}

void alloc_visited() {
	visited = malloc(rows * sizeof(int*));
	int i;
	for (i = 0; i < rows; ++i){
		visited[i] = malloc(cols * sizeof(int*));
	}
}

void get_maze(char* file_name) {
    char c;
    char rows_s[3] = { '\0' };
    char cols_s[3] = { '\0' };
    int rows_i = 0;
    int cols_i = 0;
    int swap = 0;

    FILE* file = fopen(file_name, "r");

    if (file) {
        while ((c = getc(file)) != EOF) { // while c is not the end of the file
            if (c == '\n') {
                break;
            } else if (c == ',') {
                swap = 1;
            } else if (!swap) {
                rows_s[rows_i] = c;
                rows_i++;
            } else {
                cols_s[cols_i] = c;
                cols_i++;
            }
        }
    }

    rows = atoi(rows_s);
    cols = atoi(cols_s);

    // mem alloc for the maze
    alloc_maze();
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            c = getc(file);

            if (c == '\n') {
                c = getc(file);
            }

            maze[i][j] = c;

            if (c == 's') {
                start_row = i;
                start_col = j;
            }
        }
    }
    fclose(file);
}

void init_visited() {
	alloc_visited();

	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			if (maze[i][j] == '.') {
				visited[i][j] = wall;
			} else if (maze[i][j] == 'f') {
				visited[i][j] = finish;
			} else {
				visited[i][j] = empty;
			}
		}
	}
}

int dfs(int row, int col) {
    int* current = &visited[row][col];

    if (*current == finish) {
        return 1;
    }

    if (*current == empty) {
        *current = twig;
        if (dfs(row, col - 1)) {
            *current = twig;
            return 1;
        }
        if (dfs(row + 1, col)) {
            *current = twig;
            return 1;
        }
        if (dfs(row, col + 1)) {
            *current = twig;
            return 1;
        }
        if (dfs(row - 1, col)) {
            *current = twig;
            return 1;
        }
    }

    return 0;
}

void add_twigs() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] != 's') {
                if (visited[i][j] == twig) {
                    maze[i][j] = '%';
                }
            }
        }
    }
}

void print_visited() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", visited[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_maze() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    get_maze("../maze.txt");

    init_visited();
    //print_visited();
    dfs(start_row, start_col);
    add_twigs();

    print_maze();

    return 0;
}