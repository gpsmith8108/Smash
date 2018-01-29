/*
* Super Smash Brothers Fair teams selector
* -----------------------------------------
* This Program will select fair Super Smash Brothers
* teams for one, two, three, or four player player mode
*/

/*Comment out the line below to recieve debug messages*/
#define NDEBUG

#include "dbg.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#define MAX_C 512
#define TEAM_SIZE 2
typedef int bool;
#define true 1
#define false 0


/*
*This struct holds the main character file, and a datbase with the
*character and their rating, like slot character[0] corresponds with
*rating[0]
*/
struct ch_file
{
    FILE *file;
    int *rating[MAX_C];
    char *character[MAX_C];
    int num_characters;
};


/*This is the standard error message to abort the program*/
void die(const char *message)
{
    if(errno){
        perror(message);
    } else {
        printf("ERROR: %s \n", message);
    }
    exit(1);

}

/*
*This will print a character given the file structure pointer and the number
*in the list.
*/
void print_ch(struct ch_file *db, int num)
{
    printf("Character: %s   Rating: %i \n", db->character[num],
         db->rating[num]);
}

/*This prints all of the characters in the File structure*/
void print_all(struct ch_file *db)
{
    int i = 0;

    for(i = 0; i<MAX_C; i++){
        if(db->character[i] != NULL){
            printf("%i)",i);
            print_ch(db,i);
        }
    }
}

/*
*Adds a character to the structrue held in db.
*It requires the file structure, character number, rating, character name.
*/
void add_ch(struct ch_file *db, int num, int rating, char *name)
{

    db->character[num] = name;
    db->rating[num] = rating;

}
/*
*Switched the positions of character A an dcharacter B in the file
*structure db
*/
void switch_places(struct ch_file *db, int A, int B){

    debug("I am switching position:%i with position: %i \n", A, B);
    if(A == B)
        return;

    int t_ch = db->rating[A];
    char *t_name = db->character[A];

    db->rating[A] = db->rating[B];
    db->character[A] = db->character[B];

    db->rating[B] = t_ch;
    db->character[B] = t_name;

}


/* Given a file name and a struct it fills it*/
void fill_ch_file(char *filename, struct ch_file *db)
{
    debug("fill_ch_file start");

    db->file = fopen(filename, "r+");     /*opens the file*/

    char *temp_name = malloc(50);
    int temp_rating = 0;
    int db_num = 0;
    int num_c = 0;
    bool next = false;
    int i = 0;

    if(db->file ==NULL){                  /*if the file cant open, abort*/
        die("File Failed to open");
    }

    while(1){

        /*
        *Reads in each character one by one, looking for ':'
        */
        num_c = fgetc(db->file);
        switch(num_c){

            case ':' :
                next = true;
		break;
            case '\n' :
                debug("Here is the next line: db_num = %i",db_num);
                debug("Temp_rating = %i, temp_name = %s",
                    temp_rating, temp_name);
                add_ch(db,db_num,temp_rating,temp_name);
                db_num++;
                temp_name = malloc(50);
		i = 0;
                break;
            default:
                if(next){
                   temp_rating = num_c-48;
                   next = false;
                   break;
                }
                temp_name[i] = num_c;
                i++;


        }
        if(feof(db->file)){
            db->num_characters = db_num-1;
            break;
        }
    }

    fclose(db->file);            /*Frees up resources when finished*/
    free(temp_name);

    debug("Fill_ch_file end");
}

/*
*Writes to the filename given in the order it is recieved
*/
void write_to_file(struct ch_file *db, const char *filename)
{

    int i = 0;
    FILE * fp;

    fp = fopen(filename,"w");

    for(i = 0; i < db->num_characters+1; i++){
        fprintf(fp, "%s:%i\n",db->character[i],db->rating[i]);
    }

}

void total_random(struct ch_file *db)
{
    int i = 0;
    srand(time(NULL));   /*should only be called once*/
    int choices = db->num_characters;
    int r = 0;
    for(i = 0; i<4; i++){
        /* Returns a pseudo-random integer between 0 and RAND_MAX*/
        r = rand() % choices;
        debug("Number: %i \n",r);
        switch_places(db, r, choices);
        choices --;
    }

}
void total_random_no_repeat(struct ch_file *db)
{
    int i = 0;
    srand(time(NULL));   /*should only be called once*/
    int choices = db->num_characters-(TEAM_SIZE*2);
    int r = 0;
    for(i = 0; i<TEAM_SIZE*2; i++){
        /* Returns a pseudo-random integer between 0 and RAND_MAX*/
        r = rand() % choices;
        debug("Number: %i \n",r);
        switch_places(db, r, choices);
        choices --;
    }

    for(i = 0; i<TEAM_SIZE*2; i++){
        switch_places(db, db->num_characters-TEAM_SIZE*2-i,
            db->num_characters-i);
    }



}

/*
*Prints out last four characters in the list as the teams
*/
void print_teams(struct ch_file *db)
{
     int num_high = db->num_characters;

     printf("Team one: \n");
     print_ch(db, num_high);
     print_ch(db, num_high-1);
     printf("Team two: \n");
     print_ch(db, num_high-2);
     print_ch(db, num_high-3);


}
void test_print(struct ch_file *db)
{
    add_ch(db,0,7,"Kirby");
    add_ch(db,1,5,"Fox");
    add_ch(db,2,3,"Ness");

    print_all(db);
    switch_places(db,1,0);

}


int main(int argc, char *argv[])
{

  //  if(argc != 3){
  //      die("It should be [identifier:ie -g] [filename]");
  //   }

    struct ch_file *db = malloc(sizeof(struct ch_file));
    const char *filename = "characters.txt";


    fill_ch_file(filename, db);
    debug("There are %i characters. \n", db->num_characters);
//    print_all(db);


    total_random_no_repeat(db);
    print_teams(db);
    write_to_file(db, filename);

    return 0;
}
