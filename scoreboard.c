//
// Created by marosm on 3/27/19.
//

#include "scoreboard.h"

int initializeScoreboard(void)
{
    FILE *fp;
    fp = fopen("scoreboard", "a");
    return fclose(fp);
}

inline void printBoard(struct dln_board * board, int n)
{
    printf("\nRank\t| Level\t| Score\t| \t  Name\t\t| \t   Date\t\t|\n");
    for(int j=0; j<73; j++) printf("-");
    printf("\n");

    int i=1;
    for( ; n>0; n--)
    {
        printf("%d:\t| %u\t| %lu\t| %20s\t| %s\t|\n", i++, board->level, board->score, board->name, board->time);
        if(i==4)
        {
            for(int j=0; j<73; j++) printf("-");
            printf("\n");
        }
        board = board->next;
        if(!board) return;
    }
}

int displayScoreBoard(int size)
{
    FILE *fp;

    if( !(fp = fopen("scoreboard", "r")) )  //Does not exist
    {
        if( !(fp = fopen("scoreboard", "a")) ) return -1; //Can't create either
        fclose(fp);
        return 1; //No score
    }

    char buf[150];

    struct dln_board tmp; //DoublyLinkedNode
    tmp.next = NULL; //default values for first member
    tmp.prev = NULL;

    struct dln_board * first = NULL;

    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        if(sscanf(buf, "%19[^;]; %u; %19[^;]; %lu", tmp.time, &tmp.level, tmp.name, &tmp.score) == 4)
        {
            struct dln_board * it = first;
            struct dln_board * newNode = calloc(1, sizeof(struct dln_board));
            if(newNode == NULL) continue;
            memcpy(newNode, &tmp, sizeof(struct dln_board));

            if(!first) //first datapoint insertion
            {
                first = newNode;
                continue;
            }

            while(it->next && (it->score >= newNode->score)) it = it->next;
            if(it->prev == NULL)  //Add before first
            {
                first = newNode;
                newNode->next = it;
                it->prev = newNode;
            }
            else if (!(it->next) && (it->score > newNode->score)) //Add after last
            {
                it->next = newNode;
                newNode->prev = it;
                newNode->next = NULL;
            }
            else  //Insert between two
            {
                newNode->prev = it->prev;
                it->prev->next = newNode;
                newNode->next = it;
                it->prev = newNode;
            }
        }
    }

    #if !defined(NDEBUG) && 0
    struct dln_board * iterator = first;
    for(unsigned i=0; i<2*size; i++)
    {
        if(!iterator) continue;
        printf("I am: %p\tprev: %p\tnext: %p value: %lu\n", iterator, iterator->prev, iterator->next, iterator->score);
        iterator = iterator->next;
    }
    #endif
    
    printBoard(first, size);

    //free (yeah exiting the program anyway, but :) )
    struct dln_board * it = first;
    while(it != NULL)
    {
        struct dln_board * next = it->next;
        free(it);
        it = next;
    }

    return fclose(fp);
}

int addScore(const int score, const unsigned level, const char* name)
{
    if(initializeScoreboard() == -1) return -1;

    FILE *fp;
    time_t now;
    time(&now);
    struct tm *t = localtime(&now);
    char time[25];
    sprintf(time, "%02d/%02d/%d %02d:%02d", t->tm_mday, t->tm_mon+1, 1900+t->tm_year, t->tm_hour, t->tm_min);

    if( !(fp = fopen("scoreboard", "a")) ) return -1;

    fprintf(fp,"%s; %u; %s; %d;\n", time, level, name, score);

    return fclose(fp);
}
