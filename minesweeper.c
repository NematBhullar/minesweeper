// Assignment 1 21T2 COMP1511: Minesweeper
// minesweeper.c

// This program was written by Nemat Bhullar (z5360388)
// on 28/6/21

// Version 1.0.0 (2021-06-19): Assignment released.

#include <stdio.h>
#include <stdlib.h>

// Square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE            8

// Command codes.
#define DETECT_ROW      1
#define DETECT_SQUARE   2
#define REVEAL_CROSS    3
#define GAME_MODE       4
#define FLAG_MINE       5
#define DEFUSE          6

// Helper functions 
void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void check_win(int minefield[SIZE][SIZE]);
void gameplay_mode(int minefield[SIZE][SIZE]);
void gameplay_lost(int minefield[SIZE][SIZE]);
int square(int minefield[SIZE][SIZE], int row_input, int column_input, int size);
int adj_mines(int minefield[SIZE][SIZE], int row_input, int column_input);
void output_debug(int count, int minefield[SIZE][SIZE]);
void vertical(int row_input, int column_input, int minefield[SIZE][SIZE]);
void horizontal(int row_input, int column_input, int minefield[SIZE][SIZE]);

int main(void) {

    int num_rows;
    int num_columns = 2;
    
    printf("Welcome to Minesweeper!\n");
    printf("How many mines? ");
    scanf("%d", &num_rows);
    
    int coord[num_rows][num_columns];
    int minefield[SIZE][SIZE];
    
    //Initialising the board
    initialise_field(minefield);
   
    //Input of mines
    printf("Enter pairs:\n");
    
    int i = 0;
    while (i < num_rows) {
        
        int j = 0;
        while (j < num_columns) {
            
            scanf("%d", &coord[i][j]);
            if (j == 1 && coord[i][j - 1] >= 0 && coord[i][j - 1] < 8) {
                
                if (coord[i][j] >= 0 && coord[i][j] < 8) {
                    minefield[(coord[i][j - 1])] [(coord[i][j])] = HIDDEN_MINE;
                }
            
            }  
            j++;
        
        }
        i++;
    
    }
    
    printf("Game Started\n");
    //int num_input = 0;
    print_debug_minefield(minefield);
    
    // Analysing Input of Functions
    int input;
    int count_hints = 0;
    int count = 0;

    while (scanf("%d", &input) == 1) {
    
        // Function 1 - Row Check
        if (input == DETECT_ROW) {
            
            int row_input, column_input, length;
            
            scanf("%d", &row_input);
            scanf("%d", &column_input);
            scanf("%d", &length);
         
            if (count_hints < 3) {
            
                // Debugging - Row Check     
                if (column_input + length > 8 || column_input + length < 0) {
                    output_debug (count, minefield); 
                    
                } else if (row_input < 0 || row_input > 8) {
                    output_debug (count, minefield); 
                
                } else if (column_input < 0 || column_input > 8) {
                    output_debug (count, minefield); 
                
                } else if (length < 0 || length > 8) {
                    output_debug (count, minefield); 
                
                } else {
                    // Row Output
                    int num_mines = 0;
                    int column_count = column_input;
                    
                    while (column_count < (column_input + length)) {
                        
                        if (minefield[row_input][column_count] == HIDDEN_MINE) {
                            num_mines++;
                        } 
                        column_count++;
                    }
                        
                    // Printing Row Output    
                    printf("There are %d mine(s) in row ", num_mines); 
                    printf("%d, from column %d ", row_input, column_input);
                    printf("to %d\n", (column_input + length - 1));
                    
                    if (count == 0) {
                        print_debug_minefield(minefield);
                    } else if (count == 1) {
                        gameplay_mode(minefield);
                    }  
                    count_hints++;
                }
            
                // If all three hints are used
            } else {
                
                printf("Help already used\n");
                if (count == 0) {
                    print_debug_minefield(minefield);
                } else if (count == 1) {
                    gameplay_mode(minefield);
                } 
                
            }
        } 
        
        // Function 2 - Square 
        else if (input == DETECT_SQUARE) {
        
            int row_input, column_input, size;
            
            scanf("%d", &row_input);
            scanf("%d", &column_input);
            scanf("%d", &size);

            if (count_hints < 3) {
            
                // Debugging - Sqaure
                if ((column_input + size / 2) > 8) {
                    output_debug (count, minefield); 
                
                } else if ((column_input + size / 2) < 0) {
                    output_debug (count, minefield);
                
                } else if (row_input < 0 || row_input > 7) {
                    output_debug (count, minefield); 
                
                } else if ((row_input + size / 2) > 8) {
                    output_debug (count, minefield); 
                
                } else if ((row_input + size / 2) < 0) {
                    output_debug (count, minefield); 
                
                } else if (column_input < 0 || column_input > 7) {
                    output_debug (count, minefield); 
                
                } else if (size < 0 || size > 8) {
                    output_debug (count, minefield);  
                
                } else {
                    
                    // Square Output
                    int num_mine = square(minefield, row_input, column_input, size);
            
                    // Printing Square Output    
                    printf("There are %d mine(s) in the square ", num_mine);
                    printf("centered at row %d, column ", row_input);
                    printf("%d of size %d\n", column_input, size);
                    
                    if (count == 0) {
                        print_debug_minefield(minefield);
                    } else if (count == 1) {
                        gameplay_mode(minefield);
                    } 
                    count_hints++;
                }
            
                // If all three hints are used
            } else {
                
                printf("Help already used\n");
                if (count == 0) {
                    print_debug_minefield(minefield);
                } else if (count == 1) {
                    gameplay_mode(minefield);
                } 
                
            }   
        }
        
        // Function 3 - Reveal Cross    
        else if (input == REVEAL_CROSS) {
            
            int row_input, column_input;
            
            scanf("%d", &row_input);
            scanf("%d", &column_input);
                    
            // Debugging - Reveal Cross
            if (row_input < 0 || row_input > 7) {
                output_debug (count, minefield); 
            
            } else if (column_input < 0 || column_input > 7) {
                output_debug (count, minefield); 
            
            } else {
                
                // Function Output
                if (minefield[row_input][column_input] == HIDDEN_SAFE) {
                    minefield[row_input][column_input] = VISIBLE_SAFE;
                    
                    // Checking Vertically
                    vertical(row_input, column_input, minefield);
                           
                    // Checking Horizontally
                    horizontal(row_input, column_input, minefield);
                        
                    // Check If Player Won
                    check_win(minefield);
                    
                    // Printing Reveal Cross Output
                    if (count == 0) {
                        print_debug_minefield(minefield);
                    } else if (count == 1) {
                        gameplay_mode(minefield);
                    }  
                    
                    // If Input Coordinate is a Mine
                } else if (minefield[row_input][column_input] == HIDDEN_MINE) {
                    
                    printf("Game over\n");
                    if (count == 0) {
                        print_debug_minefield(minefield);
                    } else if (count == 1) {
                        gameplay_lost(minefield);
                    } 
                    return 0;
                
                } else {
                    
                    if (count == 0) {
                        print_debug_minefield(minefield);
                    } else if (count == 1) {
                        gameplay_mode(minefield);
                    } 
                    
                }
            }
        }
        
        // Function 4 - Gameplay Mode
        else if (input == GAME_MODE) {
            
            if (count == 0) {
                printf("Gameplay mode activated\n");
                gameplay_mode(minefield);
                count++;
            
            } else if (count == 1) {
                printf("Debug mode activated\n");
                print_debug_minefield(minefield);
                count--;
            }
            
        }   
    }
} 



// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int x = 0;
    while (x < SIZE) {
        int y = 0;
        while (y < SIZE) {
            minefield[x][y] = HIDDEN_SAFE;
            y++;
        }
        x++;
    }
}



// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}



// Scan to see if player won
void check_win(int minefield[SIZE][SIZE]) {
    int win_count = 0;
    int win_row = 0;
    while (win_row < SIZE) {
        int win_column = 0;
        while (win_column < SIZE) {
            if (minefield[win_row][win_column] == HIDDEN_MINE) {
                win_count++;
            } else if (minefield[win_row][win_column] == VISIBLE_SAFE) {
                win_count++;
            }
            win_column++;
        }
        win_row++;
    }  
    // If all squares are either a mine or revealed, the game is won
    if (win_count == (SIZE*SIZE)) {
        printf("Game Won!\n");
    } 
}



// Gameplay Mode Activated 
void gameplay_mode(int minefield[SIZE][SIZE]) {
    printf("..\n\\/\n");
    int i = 0;
    while (i <= (SIZE + 2)) {
        int j = 0;
        while (j < (SIZE + 1)) {
            // First row of array coordinates
            if ((i <= 1) && (j == 0)) {
                printf("   ");
            } else if ((i == 0) && (j > 0) && (j <= SIZE)) {
                printf(" 0%d", (j - 1));
                // First column of array coordinates
            } else if ((i > 1) && (j == 0)) {
                if (i == SIZE + 2) {
                    printf("   ");
                } else {
                    printf("0%d ", (i - 2));
                }
                // Row of top border
            } else if ((i == 1) && (j > 0)) {
                if (j == SIZE) {
                    printf("----");
                } else {
                    printf("---");
                }
                // Row of bottom border
            } else if ((i == (SIZE + 2)) && (j > 0)) {
                if (j == SIZE) {
                    printf("----");
                } else {
                    printf("---");
                }
                // First column of minefield
            } else if ((i > 1) && (i < SIZE + 2) && (j == 1)) {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("|   ");
                } else {
                    printf("|## ");
                } 
                // Last column of minefield
            } else if ((i > 1) && (i < SIZE + 2) && (j == SIZE)) {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("  |");
                } else {
                    printf("##|");
                } 
                // Rest of the minefield
            } else {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("   ");
                } else {
                    printf("## ");
                } 
            }
            j++;
        }
        printf("\n");
        i++;
    }
}



// Gameplay Mode - Lost
void gameplay_lost(int minefield[SIZE][SIZE]) {
    printf("xx\n/\\ \n");
    int i = 0;
    while (i <= (SIZE + 2)) {
        int j = 0;
        while (j < (SIZE + 1)) {
            // First row of array coordinates
            if ((i <= 1) && (j == 0)) {
                printf("   ");
            } else if ((i == 0) && (j > 0) && (j <= SIZE)) {
                printf(" 0%d", (j - 1));
                // First column of array coordinates
            } else if ((i > 1) && (j == 0)) {
                if (i == SIZE + 2) {
                    printf("   ");
                } else {
                    printf("0%d ", (i - 2));
                }
                // Row of top border                
            } else if ((i == 1) && (j > 0)) { 
                if (j == SIZE) {
                    printf("----");
                } else {
                    printf("---");
                }
                // Row of bottom border               
            } else if ((i == (SIZE + 2)) && (j > 0)) {
                if (j == SIZE) {
                    printf("----");
                } else {
                    printf("---");
                }
                // First column of minefield
            } else if ((i > 1) && (i < SIZE + 2) && (j == 1)) {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("|   ");
                } else if (minefield[i - 2][j - 1] == HIDDEN_MINE) {
                    printf("|() ");
                } else {
                    printf("|## ");
                }
                // Last column of minefield
            } else if ((i > 1) && (i < SIZE + 2) && (j == SIZE)) {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("  |");
                } else if (minefield[i - 2][j - 1] == HIDDEN_MINE) {
                    printf("()|");
                } else {
                    printf("##|");
                }
                // Rest of the minefield
            } else {
                if (minefield[i - 2][j - 1] == VISIBLE_SAFE) {
                    printf("   ");
                } else if (minefield[i - 2][j - 1] == HIDDEN_MINE) {
                    printf("() ");
                } else {
                    printf("## ");
                }
            }
            j++;
        }
        printf("\n");
        i++;
    }
}



// Output for values outside of the minefield
void output_debug(int count, int minefield[SIZE][SIZE]) {
    printf("Coordinates not on map\n");
    if (count == 0) {
        print_debug_minefield(minefield);
    } else if (count == 1) {
        gameplay_mode(minefield);
    }  
}



// Function 2 - Detect Square Output
int square(int minefield[SIZE][SIZE], int row_input, int column_input, int size) {   
    int num_mines = 0;
    int i = (row_input - (size / 2));
    if (row_input == 0) {
        i = 0;
    }
    while (i <= (row_input + (size / 2))) {  
        if (i >= 0 && i < 8) {    
            int j = (column_input - (size / 2));
            if (column_input == 0) {
                j = 0;
            }
            while (j <= (column_input + (size / 2))) {
                if (j >= 0 && j < 8) {
                    if (minefield[i][j] == HIDDEN_MINE) {
                        num_mines++;         
                    }             
                }
                j++;               
            }             
        }
        i++;              
    }
    return num_mines;
}



// Function 3 - Checking Adjacent Mines
int adj_mines(int minefield[SIZE][SIZE], int row_input, int column_input) {
    int mine_count = 0;
    int x = row_input - 1;
    if (row_input <= 0) {
        x = 0;
    }
    while (x <= row_input + 1 && x < SIZE) {
        int y = column_input - 1;
        if (column_input <= 0) {
            y = 0;
        }
        while (y <= column_input + 1 && y < SIZE) {
            if (minefield[x][y] == HIDDEN_MINE) {
                mine_count++;
            }
            y++;
        }
        x++;
    }
    return mine_count;
}



// Function 3 - Checking Reveal Cross Vertically
void vertical(int row_input, int column_input, int minefield[SIZE][SIZE]) {
    int row_count = row_input - 1;
    if (row_input <= 0) {
        row_count = 0;
    }
    while (row_count <= (row_input + 1) && row_count >= 0 && row_count < 8) {
        if (minefield[row_count][column_input] == HIDDEN_SAFE) {
            int row_adj = row_count - 1;
            if (row_count <= 0) {
                row_adj = 0;
            }   
            int num_mines = 0;
            while (row_adj <= (row_count + 1) && row_adj >= 0 && row_adj < 8) {
                int column_adj = column_input - 1;
                if (column_input <= 0) {
                    column_adj = 0;
                }
                if (column_adj >= 0 && column_adj < 8) {
                    while (column_adj <= (column_input + 1)) {
                        if (minefield[row_adj][column_adj] == HIDDEN_MINE) {
                            num_mines++;                    
                        }
                        column_adj++;                         
                    }                       
                }
                row_adj++;
            }
            if (num_mines > 0) {
                minefield[row_count][column_input] = HIDDEN_SAFE;
            } else {
                minefield[row_count][column_input] = VISIBLE_SAFE;
            }
        }
        row_count++;                 
    }  
}



// Function 3 - Checking Reveal Cross Horizontally
void horizontal(int row_input, int column_input, int minefield[SIZE][SIZE]) {
    int column_count = column_input - 1;
    if (column_input <= 0) {
        column_count = 0;
    }
    while (column_count <= (column_input + 1) && column_count >= 0 && column_count < 8) {
        if (minefield[row_input][column_count] == HIDDEN_SAFE) {
            int row_adj = row_input - 1;
            if (row_input <= 0) {
                row_adj = 0;
            }
            int num_mines = 0;
            while (row_adj <= (row_input + 1) && row_adj >= 0 && row_adj < 8) {
                int column_adj = column_count - 1;
                if (column_count <= 0) {
                    column_adj = 0;
                }
                if (column_adj >= 0 && column_adj < 8) {
                    while (column_adj <= (column_count + 1)) {
                        if (minefield[row_adj][column_adj] == HIDDEN_MINE) {
                            num_mines++;
                        } 
                        column_adj++;
                    }
                }
                row_adj++;
            }
            if (num_mines > 0) {
                minefield[row_input][column_count] = HIDDEN_SAFE;
            } else {
                minefield[row_input][column_count] = VISIBLE_SAFE;
            }
        }
        column_count++;
    }     
}    
    
    
    
    
    


