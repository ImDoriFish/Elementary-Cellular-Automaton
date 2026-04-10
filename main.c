


//Elementary Cellular Automaton

#include <stdio.h>
#include <stdbool.h> 



const int WORLD_SIZE = 65;
//Represent a single cell in the automaton
typedef struct cell_struct{
    bool state[3]; //active status for [left, me, right] cells 
    bool active; //current status for me
    int total; //running accumulated count of this cell's active status for all generations
} cell;



//Purpose: Convert a rule number (0-255) into an 8 bit array
//Parameters: bitArray: array to store the binary rule
//            rule - int rule number entered by the user
//Returns: true if rule is valid, false otherwise.
bool setBitArray(bool bitArray[8], int rule) {
    if(rule < 0 || rule > 255) return false;
    
    //Extract binary digits of the rule number
    for(int i = 0; i < 8; i++){
        bitArray[i] = rule % 2;
        rule /=2;
    }
    return true;    
}



//Purpose: Convert a 3-cell neighboorhood into a rule table index.
//Parameters: state - boolean array [left, middle, right].
//Returns: Int index from 0 to 7.
int stateToIndex(bool state[3]) {
    int left = state[0] ? 1 : 0;
    int middle = state[1] ? 1 : 0;
    int right = state[2] ? 1 : 0;

    int index = left * 4 + middle * 2 + right;
    return index;
}




//Purpose: Update each cell's neighboorhood state based on current active values
//Parameters: world - array of cells representing the automaton world.
//Return: None
void setStates(cell world[WORLD_SIZE]) {
    //Compute neighboorhood for each cell using wrap-around indexing
    for(int i = 0; i < WORLD_SIZE; i++){
        //For checking the edge cases
        if (i == 0 || i == WORLD_SIZE - 1){
            int leftIndex = (i - 1 + WORLD_SIZE) % WORLD_SIZE;
            int rightIndex = (i + 1) % WORLD_SIZE;

            world[i].state[0] = world[leftIndex].active;
            world[i].state[1] = world[i].active;
            world[i].state[2] = world[rightIndex].active;
        }else{
            world[i].state[0] = world[i - 1].active;
            world[i].state[1] = world[i].active;
            world[i].state[2] = world[i + 1].active;
        }
        
    }
    return;
}



//Purpose: Evolve the world forward by one generation.
//Parameters: wolrd - array of cells
//            ruleBitArray - lookup table for the rule.
//Returns: Number of acticve cells in the new generation.
int evolveWorld(cell world[WORLD_SIZE], bool ruleBitArray[8]) {
    int bitArrayindex = 0;
    int rowActiveCount = 0;
    //Determine next active state for each cell
    for(int i = 0; i < WORLD_SIZE; i++){
        bitArrayindex = stateToIndex(world[i].state);
        world[i].active = ruleBitArray[bitArrayindex];
        //Update total count and row count
        if(world[i].active){
            world[i].total++;
            rowActiveCount++;
        }
    }
    //Refresh neighboorhood states for the next generation
    setStates(world);
    return rowActiveCount;
}

//Purpose: Check whether the active-cell count is a single digit
//Parameters: rowActiveCount - the number of active cells in the current generation.
//Returns: true if rowActiveCount is less than 10, false otherwise.
bool rowCountflag(int rowActiveCount){
    if (rowActiveCount < 10) return true;
    return false;
}


int main() {
    cell world[WORLD_SIZE];
    
    //Display program welcome message
    printf("Welcome to the Elementary Cellular Automaton!\n");
    
   
   //Read a valid rule number, generate its 8-bit lookup table and display the rule in binary form.
    int rule;
    bool bitArray[8];

    printf("Enter the rule # (0-255): ");
    scanf("%d", &rule);

    bool bitArrayReturn = setBitArray(bitArray, rule);

    while(!bitArrayReturn){
        printf("Enter the rule # (0-255): ");
        scanf("%d", &rule);
        bitArrayReturn = setBitArray (bitArray, rule);
    }

    printf("The bit array for rule #%d is ", rule);
    for(int i = 7; i >= 0; i--){
        printf("%d", bitArray[i]);
    }
    printf("\n");





    // Display how the rule affects all 8 possible neighboorhood patterns.
    printf("\nThe evolution of all possible states are as follows:\n");

    for(int i = 7; i >= 0; i--){
        int temp = i;

        bool right = temp % 2; 
        temp /= 2;

        bool middle = temp % 2;
        temp /= 2;

        bool left = temp % 2;

        printf("|%c%c%c|", left ? 'o' : '\'', middle ? 'o' : '\'', right ? 'o' : '\'');      
        if(i != 0) printf("   ");
        

    }
    printf("\n ");

    for(int i = 7; i >= 0; i--){
        bitArray[i] ? printf("|o|") : printf("|\'|");
        if(i > 0){
            printf("     ");
        }else{
            printf("    ");
        }
    }
    printf("\n");




    //Read number of generation and initialize the world with only the middle cell active.
    int generations;
    printf("\n");
    printf("Enter the number of generations (1-99): ");
    scanf("%d", &generations);
    printf("\n");
    while(generations < 1 || generations > 99){
        printf("Enter the number of generations (1-99): ");
        scanf("%d", &generations);
        printf("\n");
    }

    for(int i = 0; i < WORLD_SIZE; i++){
        if(i == WORLD_SIZE/2){
            world[i].active = 1;
            world[i].total = 1;
        }else{
            world[i].active = 0;
            world[i].total = 0;
        }
    }

    setStates(world);
    printf("\n");

    //Display initial world generation 0
    printf("Initializing world & evolving...\n");

    int rowActiveCount = 0;
    for(int i = 0; i < WORLD_SIZE; i++){
        printf("%c", world[i].state[1] ? 'o' : '\'');
        if (world[i].state[1]){
            rowActiveCount++;
        }
        
    }
    printf("  %d\n", rowActiveCount);
    
        



    
   

    
    //Evolve the world for the specified number of generations and print each generation and its active cell count.
    for(int i = 0; i < generations -1; i++){
        int rowActiveCount = evolveWorld(world, bitArray);
        for(int j = 0; j < WORLD_SIZE; j++){
            printf("%c", world[j].state[1] ? 'o' : '\'');
            
        }
        if(rowCountflag(rowActiveCount)){
            printf("  %d\n", rowActiveCount);
        }else{
            printf(" %d\n", rowActiveCount);
        }
        

        
    }
    printf("_________________________________________________________________\n");



    
    //Print total active counts for each cell vertically using two rows.
    int topRow[65];
    int bottowmRow[65];

    for(int i = 0; i < WORLD_SIZE; i++){
        if(world[i].total == 0){
            topRow[i] = -1;
            bottowmRow[i] = -1;
        }else if(world[i].total < 10){
            topRow[i] = -1;
            bottowmRow[i] = world[i].total;
        }else{
            topRow[i] = (world[i].total/10);
            bottowmRow[i] = (world[i].total%10);
        }

    }

    for(int i = 0; i < WORLD_SIZE; i++){
        if(topRow[i] == -1){
            printf(" ");
        }else{
            printf("%d", topRow[i]);
        }
    }
    printf("\n");

    for(int i = 0; i < WORLD_SIZE; i++){
        if(bottowmRow[i] == -1){
            printf(" ");
        }else{
            printf("%d", bottowmRow[i]);
        }
    }
    printf("\n");


    return 0;
}
