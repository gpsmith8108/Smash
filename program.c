/*
* Super Smash Brothers Fair teams selector
* -----------------------------------------
* This Program will select fair Super Smash Brothers
* teams.
*
* Detailed Description:
* The program draws the names and ratings out of the characters.txt file and
* fills the struct ch_file with their names and ratings.  Then the numbers
* are randomly selected, to determine the characters for each team.  The
* characters are then moved to the last 4 places in the struct ch_file.
* Then they can be used as is, or they can be filtered more using the ratings
*
*/

/*Comment out the line below to receive debug messages*/
#define NDEBUG

#include "dbg.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define MAX_C 512
#define TEAM_SIZE 2
#define NUM_PICKED (TEAM_SIZE*2)
#define MAX_LINE 256


/*
*This struct holds a database with the character and their rating,
*like slot character[0] corresponds with rating[0].
*last is the index of the last character in the list.
*/
struct ch_file
{
    int rating[MAX_C];
    char *character[MAX_C];
    int last;
};


/*This is the standard error message to abort the program*/
void die(const char *message)
{
    if(errno){
        perror(message);
    } else {
        fprintf(stderr, "ERROR: %s \n", message);
    }
    exit(1);
}

/*
*This will print a character given the file structure pointer and the number
*in the list.
*/
void print_ch(const struct ch_file *db, int num)
{
    printf("Character: %s   Rating: %i \n", db->character[num],
         db->rating[num]);
}

/*This prints all of the characters in the File structure*/
void print_all(const struct ch_file *db)
{
    int i = 0;

    for(i = 0; i <= db->last; i++){
        printf("%i)",i);
        print_ch(db,i);
    }
}

/*
*Switches the positions of character A and character B in the file
*structure db
*/
void switch_places(struct ch_file *db, int A, int B)
{
    debug("I am switching position:%i with position: %i \n", A, B);
    if(A == B)
        return;

    int t_rate = db->rating[A];
    char *t_name = db->character[A];

    db->rating[A] = db->rating[B];
    db->character[A] = db->character[B];

    db->rating[B] = t_rate;
    db->character[B] = t_name;
}


/*
*Given a file name and a struct it fills it.
*Each line is "[Name]:[rating]"; the name keeps any padding before the ':'
*so the file is written back in the same layout.
*/
void fill_ch_file(const char *filename, struct ch_file *db)
{
    debug("fill_ch_file start");

    FILE *fp = fopen(filename, "r");
    char line[MAX_LINE];
    int count = 0;

    if(fp == NULL){                       /*if the file cant open, abort*/
        die("File Failed to open");
    }

    while(fgets(line, sizeof line, fp) != NULL){
        line[strcspn(line, "\r\n")] = '\0';
        if(line[0] == '\0')               /*skip blank lines*/
            continue;

        char *colon = strchr(line, ':');
        if(colon == NULL)
            die("Each line must be in the format [Name]:[rating]");
        if(count >= MAX_C)
            die("Too many characters in the file");

        *colon = '\0';
        db->character[count] = strdup(line);
        if(db->character[count] == NULL)
            die("Out of memory");
        db->rating[count] = atoi(colon + 1);
        debug("Rating = %i, name = %s", db->rating[count], line);
        count++;
    }

    fclose(fp);
    db->last = count - 1;

    debug("Fill_ch_file end");
}

/*
*Writes to the filename given in the order it is received
*/
void write_to_file(const struct ch_file *db, const char *filename)
{
    int i = 0;
    FILE *fp = fopen(filename,"w");

    if(fp == NULL){
        die("File Failed to open for writing");
    }

    for(i = 0; i <= db->last; i++){
        fprintf(fp, "%s:%i\n",db->character[i],db->rating[i]);
    }

    if(fclose(fp) != 0){
        die("File Failed to save");
    }
}

/*
*Randomly picks the characters for the teams, never picking any of the
*last NUM_PICKED characters (the teams from the previous run), then moves
*the picked characters to the last NUM_PICKED places in the list.
*/
void total_random_no_repeat(struct ch_file *db)
{
    int i = 0;
    int choices = db->last - NUM_PICKED;
    int r = 0;

    if(choices < 1){
        die("Not enough characters in the file, at least 9 are needed");
    }

    srand(time(NULL));
    for(i = 0; i < NUM_PICKED; i++){
        r = rand() % choices;
        debug("Number: %i \n",r);
        switch_places(db, r, choices);
        choices--;
    }
    /*Moves the selected teams to the last 4 places in the chart*/
    for(i = 0; i < NUM_PICKED; i++){
        switch_places(db, db->last - NUM_PICKED - i, db->last - i);
    }
}

/*
*Orders the last four characters according to their ratings, the highest
*rating ends up last
*/
void order(struct ch_file *db)
{
    int i = 0;
    int j = 0;
    int temp_rate = 0;
    int n = db->last;

    for(i = 0; i < NUM_PICKED-1; i++){

        temp_rate = 0;

        for(j = 0; j < NUM_PICKED-i; j++){
            debug("The first person's rating is %i, and it is compared with %i"
                , db->rating[n-j], temp_rate);
            if(db->rating[n-j] > temp_rate){
                debug("I am switching position %i, with position %i",n-j, n);
                temp_rate = db->rating[n-j];
                switch_places(db,n-j,n);
            }
        }
        n--;
    }
}

/*
*Prints out last four characters in the list as the teams
*/
void print_teams(const struct ch_file *db)
{
    int num_high = db->last;

    printf("Team one: \n");
    print_ch(db, num_high);
    print_ch(db, num_high-3);
    printf("Team two: \n");
    print_ch(db, num_high-1);
    print_ch(db, num_high-2);
}

void free_ch_file(struct ch_file *db)
{
    int i = 0;

    for(i = 0; i <= db->last; i++){
        free(db->character[i]);
    }
    free(db);
}

int main(void)
{
    struct ch_file *db = calloc(1, sizeof(struct ch_file));
    const char *filename = "characters.txt";

    if(db == NULL){
        die("Out of memory");
    }

    fill_ch_file(filename, db);    /*Reads the characters stored in filename*/

    total_random_no_repeat(db);    /*Chooses 4 random chars/moves Position*/
    order(db);                     /*Orders the chosen chars according to rate*/
    print_teams(db);               /*prints teams*/
    write_to_file(db, filename);   /*Writes the new order to filename*/

    free_ch_file(db);
    return 0;
}
