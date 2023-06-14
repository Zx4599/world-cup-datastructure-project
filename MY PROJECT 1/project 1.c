#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct player {
    char name[20];
    int number;
    struct player *next;
};
typedef struct player *player_Node;

int MinQueueSize=1;
struct Queue{
    int Capacity;
    int Front;
    int Rear;
    int Size;
    player_Node *array;
};

typedef struct Queue *playerQueue;

struct team {
    char name[20];
    char code[5];
    int wins;
    int draws;
    int loses;
    int goalDifferences;
    int points;
    player_Node elementary;
    playerQueue spare;
    int playersNo;
    struct team *next;
};

typedef struct team *team_Node;

team_Node teams;

int isTeamsEmpty(team_Node list);
team_Node isFoundTeam(char code[]);
void readTeams();
void addTeam();
void insertTeam(team_Node list, team_Node t);
void modifyTeam();
void deleteTeam();
void printTeams(team_Node list);
void printTeamsFile(team_Node list);

char * getNameFromNumber(player_Node p, int playerNO);
void readPlayers();
void insertPlayerIntoTeams(char *code, player_Node p);
void printElementary(team_Node list);
void printTeamsWithPlayers(team_Node list);
void printTeamsWithPlayersFile(team_Node list);
void printPlayersElementary(player_Node p);
void printPlayersSpare(playerQueue p);
int isFoundPlayerElementary(player_Node p,int playerNO);
void deletePlayerElementary(player_Node p, int playerNO);
void insertPlayer(player_Node temp, player_Node p);
void createMatch();

int IsEmpty( playerQueue Q ){
    return Q->Size == 0;
}

int IsFull( playerQueue Q ){
    return Q->Size == Q->Capacity;
}

void MakeEmpty( playerQueue Q){
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

playerQueue CreateQueue( int MaxElements ){
    playerQueue Q;

    if( MaxElements < MinQueueSize )
        printf("Queue size is too small\n");

    Q = (playerQueue)malloc(sizeof( struct Queue ));
    if( Q == NULL)
        printf("Out of space");

    Q->array = (player_Node)malloc(sizeof(player_Node) * MaxElements);

    if( Q->array == NULL )
        printf("Out of space");

    Q->Capacity = MaxElements;
    MakeEmpty( Q );
    return Q;
}

void DisposeQueue( playerQueue Q ){
    if( Q != NULL ){
        free( Q->array );
        free( Q );
    }
}

int Succ( int Value, playerQueue Q ){
    if( ++Value == Q->Capacity )

        Value = 0;

    return Value;

}

void Enqueue( player_Node X, playerQueue Q ){
    if( IsFull( Q ) )
        printf("Full Queue");
    else{
        Q->Size++;
        Q->Rear = Succ( Q->Rear, Q );
        Q->array[ Q->Rear ] = X;
    }
}

player_Node Front( playerQueue Q ){
    if( !IsEmpty( Q ) )
        return Q->array[ Q->Front ];

    printf("Empty Queue!");
    return NULL;

}

void Dequeue( playerQueue Q ){
    if( IsEmpty( Q ) )
        printf("Empty Queue!");
    else{
        Q->Size--;
        Q->Front = Succ( Q->Front, Q );
    }
}

player_Node FrontAndDequeue(playerQueue Q ){
    player_Node X;
    if( IsEmpty( Q ) )
        printf("Empty Queue!");

    else{
        Q->Size--;
        X = Q->array[ Q->Front ];
        Q->Front = Succ( Q->Front, Q );
    }
    return X;
}

void insertSortedGoals(team_Node node1, team_Node node2){
    team_Node curr = node1;
    while(curr->next != NULL && curr->next->goalDifferences >= node2->goalDifferences){
        curr = curr->next;
    }
    node2->next = curr->next;
    curr->next = node2;
}

void GDSort(){
    team_Node temp = (team_Node) malloc(sizeof(struct team));
    temp->next = NULL;

    team_Node curr = teams->next;
    while(curr != NULL){
        team_Node new = (team_Node) malloc(sizeof(struct team));
        strcpy(new->name,curr->name);
        strcpy(new->code,curr->code);
        new->wins = curr->wins;
        new->draws = curr->draws;
        new->loses = curr->loses;
        new->goalDifferences = curr->goalDifferences;
        new->points = curr->points;
        new->playersNo = curr->playersNo;
        new->elementary = curr->elementary;
        new->spare = curr->spare;
        new->next = NULL;

        insertSortedGoals(temp, new);
        curr = curr->next;
    }
    teams = temp;
}

void radixSort(){
    int max_digits = 0;
    GDSort();
    team_Node temp = teams->next;
    while(temp != NULL){
        int points = temp->points;
        int digits = 0;
        while(points > 0){
            points /= 10;
            digits++;
        }
        if(digits > max_digits){
            max_digits = digits;
        }
        temp = temp->next;
    }
    int index[10];
    for(int i=0;i<10;i++){
        index[i]=0;
    }
    team_Node bucket[10][100];
    temp = teams->next;
    while(temp != NULL){
        bucket[temp->points][index[(temp->points)%10]] = temp;
        index[(temp->points)%10]++;
        temp = temp->next;
    }
    max_digits--;
    int div = 10;
    while(max_digits > 0){
        team_Node new_bucket[10][100];
        int newIndex[10];
        for(int i=0;i<10;i++){
            newIndex[i]=0;
        }

        for(int i = 0; i < 10; i++){
            for(int j = 0; j < index[i]; j++){
                team_Node temp = bucket[i][j];
                int val = ((temp->points)/div)%10;
                new_bucket[i][newIndex[val]] = temp;
                newIndex[val]++;
            }
        }
        for(int i = 0; i < 10; i++){
            index[i] = newIndex[i];
            for(int j = 0; j < index[i]; j++){
                bucket[i][j] = new_bucket[i][j];
            }
        }
        div *= 10;
    }
    team_Node prev = teams;
    for(int i = 9; i >= 0; i--){
        for(int j = 0; j < index[i]; j++){
            prev->next = bucket[i][j];
            prev = bucket[i][j];
        }
    }
    prev->next = NULL;
}

int main() {
    teams = (team_Node) malloc(sizeof(struct team));
    teams->next = NULL;
    readTeams();
    radixSort();
    printf("You are in phase 1, the teams file has been read and the teams linked list has been sorted using radix sort as follows\n");
    printTeams(teams);

    while (1) {
        printf("Choose the number of operation you wanna do\n1_ Add a team\n2_ Modify a team\n3_ Delete a team\n4_ Print team info\n5_ Move to phase 2 and save current teams information (sorted with radix sort) to the file teamsInfo.txt\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            addTeam();
            radixSort();
        } else if (choice == 2) {
            modifyTeam();
            radixSort();
        } else if (choice == 3) {
            deleteTeam();
        } else if (choice == 4) {
            printTeams(teams);
        } else if (choice == 5) {
            printTeamsFile(teams);
            break;
        } else {
            printf("Invalid input, please try again\n");
        }
    }
    printf("test");
    readPlayers();
    printf("You are in phase 2, the players file has been read and each player has been assigned to its team as follows\n");
    printTeamsWithPlayers(teams);


    while (1) {
        printf("Choose the number of operation you wanna do\n1_ Create a match\n2_ Print elementary players info for a specific team\n3_ Save current teams with players information to the file playersInfo.txt and Exit the program\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1) {
            createMatch();
        } else if (choice == 2) {
            printElementary(teams);
        } else if (choice == 3) {
            printTeamsWithPlayersFile(teams);
            break;
        } else {
            printf("Invalid input, please try again\n");
        }
    }
    return 0;
}

int isTeamsEmpty(team_Node list) {
    return list->next == NULL;
}

team_Node isFoundTeam(char code[]){
    if (!isTeamsEmpty(teams)) {
        team_Node t = teams->next;
        while (t != NULL) {
            if (strcmp(t->code, code) == 0) {
                return t;
            }
            t = t->next;
        }
        return NULL;
    } else
        printf("Empty list\n");
}

void readTeams() {
    FILE *f = fopen("teams.txt", "r");
    char line[100];
    char *token;
    while (fgets(line, sizeof(line), f)) {
        token = strtok(line, ",");
        team_Node t = (team_Node) malloc(sizeof(struct team));
        t->next = NULL;
        t->elementary = (player_Node) malloc(sizeof(struct player));
        t->elementary->next = NULL;
        t->spare = CreateQueue(100);

        strcpy(t->name, token);

        token = strtok(NULL, ",");
        strcpy(t->code, token);

        token = strtok(NULL, ",");
        t->wins = atoi(token);

        token = strtok(NULL, ",");
        t->draws = atoi(token);
        t->points = 3 * t->wins + t->draws;
        token = strtok(NULL, ",");
        t->loses = atoi(token);

        token = strtok(NULL, "\n");
        t->goalDifferences = atoi(token);

        t->playersNo = 0;

        insertTeam(teams, t);
    }
}

void addTeam() {
    team_Node t = (team_Node) malloc(sizeof(struct team));
    t->next = NULL;
    t->elementary = (player_Node) malloc(sizeof(struct player));
    t->elementary->next = NULL;
    t->spare = CreateQueue(100);

    printf("Enter the name of the team : ");
    scanf("%s", t->name);

    printf("Enter the code of the team : ");
    scanf("%s", t->code);

    printf("Enter the number of wins of the team : ");
    scanf("%d", &t->wins);

    printf("Enter the number of draws of the team : ");
    scanf("%d", &t->draws);
    t->points = 3 * t->wins + t->draws;

    printf("Enter the number of loses of the team : ");
    scanf("%d", &t->loses);

    printf("Enter the number of goad differences of the team : ");
    scanf("%d", &t->goalDifferences);
    t->playersNo = 0;

    if(t->points>9){
        printf("Error, the points of any team can't be more then 9\n");
        return;
    }
    insertTeam(teams, t);

}

void insertTeam(team_Node list, team_Node t) {
    if (list != NULL && t != NULL) {
        team_Node temp = list;
        while (temp->next != NULL)
            temp = temp->next;
        t->next = NULL;
        temp->next = t;
    } else
        printf("Error has occurred\n");
}

void modifyTeam() {
    printf("-------------------------------------------------------------\n");
    printf("Enter the team code you want to modify\n");
    char code[5];
    scanf("%s", code);

    if (!isTeamsEmpty(teams)) {
        team_Node t = teams;
        while (t->next != NULL) {
            if (strcmp(t->next->code, code) == 0) {
                char newName[20];
                char newCode[5];
                int newWins, newDraws, newLoses, newGoalDifferences;

                printf("The current name is %s, enter the new name : ", t->next->name);
                scanf("%s", newName);
                strcpy(t->next->name, newName);

                printf("The current code is %s, enter the new code : ", t->next->code);
                scanf("%s", newCode);
                strcpy(t->next->code, newCode);

                printf("The current number of wins is %d, enter the new number of wins : ", t->next->wins);
                scanf("%d", &newWins);
                t->next->wins = newWins;

                printf("The current number of draws is %d, enter the new number of draws : ", t->next->draws);
                scanf("%d", &newDraws);
                t->next->draws = newDraws;

                printf("The current number of newLoses is %d, enter the new number of newLoses : ", t->next->loses);
                scanf("%d", &newLoses);
                t->next->loses = newLoses;

                printf("The current Goals difference is %d, enter the new Goals difference : ",
                       t->next->goalDifferences);
                scanf("%d", &newGoalDifferences);
                printf("-------------------------------------------------------------\n");
                t->next->goalDifferences = newGoalDifferences;
                return;
            }
            t = t->next;
        }
    } else {
        printf("Empty list\n");
        return;
    }
    printf("The team you are trying to modify is not found\n");
    printf("-------------------------------------------------------------\n");
}

void deleteTeam() {
    printf("-------------------------------------------------------------\n");
    printf("Enter the team code you want to delete\n");
    char code[5];
    scanf("%s", code);

    if (!isTeamsEmpty(teams)) {
        team_Node t = teams;
        while (t->next != NULL) {
            if (strcmp(t->next->code, code) == 0) {
                team_Node d = t->next;
                t->next = t->next->next;
                free(d);
                printf("The team is found and deleted\n");
                printf("-------------------------------------------------------------\n");
                return;
            }
            t = t->next;
        }
    } else {
        printf("Empty list\n");
        return;
    }
    printf("The team you are trying to delete is not found\n");
    printf("-------------------------------------------------------------\n");
}

void printTeams(team_Node list) {
    if (!isTeamsEmpty(list)) {
        team_Node t = list->next;
        printf("-------------------------------------------------------------");
        while (t != NULL) {
            printf("\nName:%s\nCode:%s\nWins:%d\nDraws:%d\nLoses:%d\nGoal differences:%d\nPoints:%d\n", t->name,
                   t->code, t->wins, t->draws, t->loses, t->goalDifferences, t->points);
            t = t->next;
        }
        printf("-------------------------------------------------------------\n");
    } else
        printf("Empty list\n");
}

void printTeamsFile(team_Node list) {
    FILE *out = fopen("teamsInfo.txt", "w");
    if (!isTeamsEmpty(list)) {
        team_Node t = list->next;
        fprintf(out, "Teams Information :-\n-------------------------------------------------------------");
        while (t != NULL) {
            fprintf(out, "\nName:%s\nCode:%s\nWins:%d\nDraws:%d\nLoses:%d\nGoal differences:%d\nPoints:%d\n", t->name,
                    t->code, t->wins, t->draws, t->loses, t->goalDifferences, t->points);
            t = t->next;
        }
        fprintf(out, "-------------------------------------------------------------\n");
    } else
        fprintf(out, "Empty list\n");
    fclose(out);
}

char * getNameFromNumber(player_Node p, int playerNO){
    p = p->next;
    while (p != NULL) {
        if (p->number==playerNO){
            return p->name;
        }
        p = p->next;
    }
}

void readPlayers() {
    FILE *f = fopen("players.txt", "r");
    char line[100];
    char *token;
    char *code;
    char targetCode[20];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '*') {
            token = strtok(line, "\n");
            code = token + 1;
            strcpy(targetCode, code);
        } else {
            player_Node p = (player_Node) malloc(sizeof(struct player));
            p->next = NULL;

            token = strtok(line, "-");
            p->number = atoi(token);

            token = strtok(NULL, "\n");
            strcpy(p->name, token);

            insertPlayerIntoTeams(targetCode, p);
        }
    }
}

void insertPlayerIntoTeams(char *code, player_Node p) {
    if (!isTeamsEmpty(teams)) {
        team_Node t = teams->next;
        while (t != NULL) {
            if (strcmp(t->code, code) == 0) {
                if (t->playersNo < 11) {
                    if (t->elementary != NULL && p != NULL) {
                        player_Node temp = t->elementary;
                        while (temp->next != NULL)
                            temp = temp->next;
                        p->next = NULL;
                        temp->next = p;
                    } else
                        printf("Error has occurred\n");
                } else {
                    Enqueue(p,t->spare);
                }
                t->playersNo++;
                break;
            }
            t = t->next;
        }
    } else
        printf("Empty list\n");
}

void printTeamsWithPlayers(team_Node list) {
    if (!isTeamsEmpty(list)) {
        team_Node t = list->next;
        printf("-------------------------------------------------------------");
        while (t != NULL) {
            printf("\nName:%s\nCode:%s\nWins:%d\nDraws:%d\nLoses:%d\nGoal differences:%d\nPoints:%d\nPlayers NO. :%d\n",t->name, t->code, t->wins, t->draws, t->loses, t->goalDifferences, t->points, t->playersNo);
            printf("Elementary Players:\n");
            if (t->elementary->next != NULL) {
                player_Node p = t->elementary->next;
                while (p != NULL) {
                    printf("%d-%s\n", p->number, p->name);
                    p = p->next;
                }
            } else {
                printf("No players in the team\n\n");
            }
            printf("\nSpare Players:\n");
            if (!IsEmpty(t->spare)) {
                for(int i=t->spare->Front;i<t->spare->Size+t->spare->Front;i++){
                    printf("%d-%s\n", t->spare->array[i]->number, t->spare->array[i]->name);
                }
            } else {
                printf("No players in the team\n\n");
            }
            t = t->next;
        }
        printf("-------------------------------------------------------------\n");
    } else
        printf("Empty list\n");
}

void printTeamsWithPlayersFile(team_Node list) {
    FILE *out = fopen("playersInfo.txt", "w");
    if (!isTeamsEmpty(list)) {
        team_Node t = list->next;
        fprintf(out, "Teams With Players Information :-\n-------------------------------------------------------------");
        while (t != NULL) {
            fprintf(out,"\nName:%s\nCode:%s\nWins:%d\nDraws:%d\nLoses:%d\nGoal differences:%d\nPoints:%d\nPlayers NO. :%d\n",t->name, t->code, t->wins, t->draws, t->loses, t->goalDifferences, t->points, t->playersNo);
            fprintf(out, "Elementary Players:\n");
            if (t->elementary->next != NULL) {
                player_Node p = t->elementary->next;
                while (p != NULL) {
                    fprintf(out, "%d-%s\n", p->number, p->name);
                    p = p->next;
                }
            } else {
                fprintf(out, "No players in the team\n\n");
            }

            fprintf(out, "\nSpare Players:\n");
            if (!IsEmpty(t->spare)) {
                for(int i=t->spare->Front;i<t->spare->Size+t->spare->Front;i++){
                    fprintf(out,"%d-%s\n", t->spare->array[i]->number, t->spare->array[i]->name);
                }
            } else {
                fprintf(out,"No players in the team\n\n");
            }
            t = t->next;
        }
        fprintf(out, "-------------------------------------------------------------\n");
    } else
        printf("Empty list\n");
}

void printElementary(team_Node list){
    char code[5];
    printf("Enter the team code you want to show their elementary players :");
    scanf("%s",code);
    if (!isTeamsEmpty(list)) {
        team_Node t = list->next;
        while (t != NULL) {
            if (strcmp(t->code,code)==0){
                printf("Elementary Players in %s team:\n",t->name);
                if (t->elementary->next != NULL) {

                    player_Node p = t->elementary->next;
                    while (p != NULL) {

                        printf("%d-%s\n", p->number, p->name);
                        p = p->next;

                    }
                } else {
                    printf("No players in the team\n");
                }
                printf("\n");
                return;
            }
            t = t->next;
        }
    } else{
        printf("Empty list\n");
        return;
    }
    printf("The team you are searching for is not found\n\n");
}

void printPlayersElementary(player_Node p){
    p = p->next;
    while (p != NULL) {
        printf("%d-%s\n", p->number, p->name);
        p = p->next;
    }
}

void printPlayersSpare(playerQueue p){
    for(int i=p->Front;i<p->Size+p->Front;i++){
        printf("%d-%s\n", p->array[i]->number, p->array[i]->name);
    }
}

int isFoundPlayerElementary(player_Node p,int playerNO){
    p = p->next;
    while (p != NULL) {
        if (p->number==playerNO){
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void deletePlayerElementary(player_Node p, int playerNO) {
    while (p->next != NULL) {
        if (p->next->number==playerNO){
            player_Node p2 = p->next;
            p->next = p->next->next;
            free(p2);
            return;
        }
        p = p->next;
    }
}

void insertPlayer(player_Node temp, player_Node p) {
    p->next = NULL;
    while (temp->next != NULL)
        temp = temp->next;
    p->next = NULL;
    temp->next = p;
}

void createMatch() {
    char code1[5],code2[5];
    char name[20];
    printf("Enter the code of team 1 : ");
    scanf("%s",code1);
    printf("Enter the code of team 2 : ");
    scanf("%s",code2);

    if (strcmp(code1,code2)==0){
        printf("The match can't be between the same team\n");
        return;
    }

    while (isFoundTeam(code1)&&isFoundTeam(code2)){
        team_Node t1,t2;
        t1=isFoundTeam(code1);
        t2=isFoundTeam(code2);
        printf("The match has been started\nChoose action : \n1-Change players for team %s\n2-Change players for team %s\n3-Give red card for a player from team %s\n4-Give red card for a player from team %s\n5-End the match\n",t1->name,t2->name,t1->name,t2->name);
        int choice;
        scanf("%d",&choice);
        if (choice==1){
            printf("Elementary Players in %s team:\n",t1->name);
            printPlayersElementary(t1->elementary);
            printf("Enter the number of player from elementary players you want to get him out\n");
            int playerNO;
            scanf("%d",&playerNO);

            if (isFoundPlayerElementary(t1->elementary,playerNO)){
                strcpy(name, getNameFromNumber(t1->elementary,playerNO));

                deletePlayerElementary(t1->elementary,playerNO);
                insertPlayer(t1->elementary, FrontAndDequeue(t1->spare));
                player_Node p = (player_Node) malloc(sizeof(struct player));
                p->next = NULL;
                p->number=playerNO;
                strcpy(p->name,name);
                Enqueue(p,t1->spare);
                printf("Elementary Players in %s team:\n",t1->name);
                printPlayersElementary(t1->elementary);
                printf("Spare Players in %s team:\n",t1->name);
                printPlayersSpare(t1->spare);
            } else {
                printf("There is an invalid input\n\n");
            }
        }
        else if (choice==2){
            printf("Elementary Players in %s team:\n",t2->name);
            printPlayersElementary(t2->elementary);
            printf("Enter the number of player from elementary players you want to get him out\n");
            int playerNO;
            scanf("%d",&playerNO);

            if (isFoundPlayerElementary(t2->elementary,playerNO)){
                strcpy(name, getNameFromNumber(t2->elementary,playerNO));

                deletePlayerElementary(t2->elementary,playerNO);
                insertPlayer(t2->elementary, FrontAndDequeue(t2->spare));
                player_Node p = (player_Node) malloc(sizeof(struct player));
                p->next = NULL;
                p->number=playerNO;
                strcpy(p->name,name);
                Enqueue(p,t2->spare);

                printf("Elementary Players in %s team:\n",t2->name);
                printPlayersElementary(t2->elementary);
                printf("Spare Players in %s team:\n",t2->name);
                printPlayersSpare(t2->spare);
            } else {
                printf("There is an invalid input\n\n");
            }
        }
        else if (choice==3){
            printf("Enter the number of player -from the list below- you wanna give him a red card\n");
            printf("Elementary Players in %s team:\n",t1->name);
            printPlayersElementary(t1->elementary);
            int playerNO;
            scanf("%d",&playerNO);
            if (isFoundPlayerElementary(t1->elementary,playerNO)){
                deletePlayerElementary(t1->elementary,playerNO);
                printf("Player with number %d has been given a red card and sent out\n",playerNO);
            }
            else {
                printf("The player you have tried to giva a red card doesn't exist\n");
            }
        }
        else if (choice==4){
            printf("Enter the number of player -from the list below- you wanna give him a red card\n");
            printf("Elementary Players in %s team:\n",t2->name);
            printPlayersElementary(t2->elementary);
            int playerNO;
            scanf("%d",&playerNO);
            if (isFoundPlayerElementary(t2->elementary,playerNO)){
                deletePlayerElementary(t2->elementary,playerNO);
                printf("Player with number %d has been given a red card and sent out\n",playerNO);
            }
            else {
                printf("The player you have tried to give a red card doesn't exist\n");
            }
        }
        else if (choice==5){
            return;
        }
        else {
            printf("Invalid input, please try again\n");
        }
    }
    printf("Invalid input teams codes\n");

}
