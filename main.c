#include "main.h"
#include "./headers/queue.h"
#include "./headers/BSTandAVL.h"

#define numOfTasks 5
#define lastEight 8

int main(int argc, char *argv[])
{
    FILE *tasks = fopen(argv[1], "rt");
    if(tasks == NULL){
        printf("Error! The file cannot be opened."); exit(1);
    }
    int Tasks[5] = {0};
    for(int i = 0; i < numOfTasks; i++)
        fscanf(tasks, "%d", &Tasks[i]);

    FILE *fileRead = fopen(argv[2], "rt");
    if(fileRead == NULL){
        printf("Error! The file cannot be opened."); exit(1);
    }

    FILE *filePrint = fopen(argv[3], "wt");
    if(filePrint == NULL){
        printf("Error! The file cannot be opened."); exit(1);
    }

    int numOfTeams, teamMates;
    char teamName[50], stringHelp[50], spaceDistroyer;

    fscanf(fileRead, "%d", &numOfTeams);
    Team **teamList = (Team **)malloc(numOfTeams * sizeof(Team *));
    if(teamList == NULL){
        printf("Allocation error!"); exit(1);
    }

    for(int i = 0; i < numOfTeams; i++){

        teamList[i] = 0;
        fscanf(fileRead, "%d%c", &teamMates, &spaceDistroyer);
        fgets(teamName, 50, fileRead);

        Player *p = malloc(teamMates * sizeof(Player));
        if(p == NULL){
            printf("Allocation error!"); exit(1);
        }

        for(int j = 0; j < teamMates; j++){
            fscanf(fileRead, "%s%c", stringHelp, &spaceDistroyer);
            p[j].firstName = malloc(strlen(stringHelp) + 1);
            strcpy(p[j].firstName, stringHelp);

            fscanf(fileRead, "%s%c", stringHelp, &spaceDistroyer);
            p[j].secondName = malloc(strlen(stringHelp) + 1);
            strcpy(p[j].secondName, stringHelp);

            fscanf(fileRead, "%d", &p[j].points);

            addAtBeginning(&(teamList[i]), teamName, teamMates, 0.0, p[j]);
        }

        free(p);
    }

    if(Tasks[0] == 1){
        for(int i = numOfTeams - 1; i >= 0; i--){
            (teamList[i]->teamName)[strlen(teamList[i]->teamName) - 2] = '\0';
            fprintf(filePrint, "%s\n", teamList[i]->teamName);
        }
        fclose(filePrint);
    }

    if(Tasks[1] == 1){
        filePrint = fopen(argv[3], "wt");

        averagePoints(teamList, numOfTeams);

        int totalTeamsAfterElimination = 2;
        while(totalTeamsAfterElimination * 2 <= numOfTeams)
            totalTeamsAfterElimination *= 2;

        eliminationTeams(teamList, &numOfTeams, totalTeamsAfterElimination);

        for(int i = numOfTeams - 1; i >= 0; i--)
            fprintf(filePrint, "%s\n", teamList[i]->teamName);
        fclose(filePrint);
    }

    BST *BSTree = NULL;
    if(Tasks[2] == 1){
        Queue *teamListQueue = createQueue();
        int roundContor = 1;

        for(int i = numOfTeams - 1; i >= 0; i--)
            enQueue(teamListQueue, teamList[i]);

        Stack *winnerTeam, *defeatedTeam;
        while(numOfTeams > 1){
            filePrint = fopen(argv[3], "at");
            fprintf(filePrint, "\n--- ROUND NO:%d\n", roundContor);
            fclose(filePrint);

            for(int i = 0; i < numOfTeams; i += 2){
                filePrint = fopen(argv[3], "at");
                Team *firstTeam = deQueue(teamListQueue);
                Team *secondTeam = deQueue(teamListQueue);

                if((secondTeam->teamName)[strlen(secondTeam->teamName) - 1] == ' ')
                    (secondTeam->teamName)[strlen(secondTeam->teamName) - 1] = '\0';
                fprintf(filePrint, "%-33s-%33s\n", firstTeam->teamName, secondTeam->teamName);

                if(firstTeam->totalPoints > secondTeam->totalPoints){
                    (firstTeam->totalPoints) = firstTeam->totalPoints + 1.0;
                    createStack(&winnerTeam, firstTeam);
                    createStack(&defeatedTeam, secondTeam);
                }
                else{
                    (secondTeam->totalPoints) = secondTeam->totalPoints + 1.0;
                    createStack(&winnerTeam, secondTeam);
                    createStack(&defeatedTeam, firstTeam);
                }

                free(firstTeam); free(secondTeam);

                fclose(filePrint);
            }

            filePrint = fopen(argv[3], "at");
            fprintf(filePrint, "\nWINNERS OF ROUND NO:%d\n", roundContor);
            fclose(filePrint);

            numOfTeams = numOfTeams / 2;
            roundContor++;

            int indexWinners = 0;
            Stack *winners = winnerTeam;
            while(indexWinners < numOfTeams){
                filePrint = fopen(argv[3], "at");
                fprintf(filePrint, "%-34s-  %.2f\n", winners->val.teamName, winners->val.totalPoints);

                if(numOfTeams == lastEight)
                    BSTree = insertBST(BSTree, winners);

                winners = winners->next;
                indexWinners++;

                fclose(filePrint);
            }

            free(teamListQueue);
            teamListQueue = createQueue();

            for(int j = numOfTeams - 1; j >= 0; j--){
                enQueueStack(teamListQueue, winnerTeam);
                winnerTeam = winnerTeam->next;
            }
        }
    }

    BST *AVLTree = NULL;
    if(Tasks[3] == 1){
        filePrint = fopen(argv[3], "at");
        fprintf(filePrint, "\nTOP 8 TEAMS:\n");
        fclose(filePrint);

        filePrint = fopen(argv[3], "at");
        preorder(filePrint, BSTree);
        fclose(filePrint);

        transformAVL(&AVLTree, BSTree);
    }

    if(Tasks[4] == 1){
        filePrint = fopen(argv[3], "at");

        fprintf(filePrint, "\nTHE LEVEL 2 TEAMS ARE: \n");
        preorderAVL(filePrint, AVLTree);

        fclose(filePrint);
    }

    free(BSTree); free(AVLTree);

    for(int i = 0; i < numOfTeams; i++)
        free(teamList[i]);
    free(teamList);

    return 0;
}