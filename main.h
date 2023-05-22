#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxCharacter 50

typedef struct Player{
    char *firstName;
    char *secondName;
    int points;
}Player;

typedef struct Team{
    int teamMates;
    char *teamName;
    float totalPoints;

    Player val;
    
    struct Team *next;
}Team;