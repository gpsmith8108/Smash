#define NDEBUG

#include "dbg.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>


//This program will chose fair super smash brother teams

#define MAX_C 512
typedef int bool;
#define true 1
#define false 0

struct ch_file
{
//This file holds the main character file, and a datbase with the characters
//and their rating, like slot character[0] corresponds with rating[0]
    FILE *file;
    int *rating[MAX_C];
    char *character[MAX_C];
    int num_characters;
};

struct ft
{
    int players[4];
};

void die(const char *message)    //this is standard error message to abort the program
{
    if(errno){
        perror(message);
    } else {
        printf("ERROR: %s \n", message);
    }
    exit(1);

}

void print_ch(struct ch_file *db, int num)  //This will print a character given the file structure pointer and the number in the list
{
    printf("Character: %s   Rating: %i \n", db->character[num], db->rating[num]);
}

void print_all(struct ch_file *db)  //This prints all of the characters in the File structure
{
    int i = 0;

    for(i = 0; i<MAX_C; i++){
        if(db->character[i] != NULL){
            printf("%i)",i);
            print_ch(db,i);
        }
    }
}

void add_ch(struct ch_file *db, int num, int rating, char *name)  //given the file structure, character number, rating, character name it adds it to the structure
{

    db->character[num] = name;
    db->rating[num] = rating;

}

void switch_places(struct ch_file *db, int A, int B){


     printf("--------------------------------------\n");
     printf("I am switching position:%i with position: %i \n", A, B);
     printf("Current: \n");
     print_all(db);
     if(A == B)
        return;

     int t_ch = db->rating[A];
     char *t_name = db->character[A];

     db->rating[A] = db->rating[B];
     db->character[A] = db->character[B];


     db->rating[B] = t_ch;
     db->character[B] = t_name;

     printf("--------------------------------------\n");
     print_all(db);

}


void fill_ch_file(char *filename, struct ch_file *db) // given a file name and a struct it fills it
{
    debug("fill_ch_file start");

    db->file = fopen(filename, "r+");   //opens the file

    char *temp_name = malloc(50);
    int temp_rating = 0;
    int db_num = 0;
    int num_c = 0;
    bool next = false;
    int i = 0;

    if(db->file ==NULL){                  //if the file cant open, abort
        die("File Failed to open");
    }


    while(1){                                                        //Reads in the chars one by one from the text file

	num_c = fgetc(db->file);
//        debug("Letter read: %c\n", num_c);
        switch(num_c){

            case ':' :
                next = true;
		break;
            case '\n' :
                debug("Here is the next line: db_num = %i",db_num);
                debug("Temp_rating = %i, temp_name = %s",temp_rating, temp_name);
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

    fclose(db->file);            //Frees up resources when finished
    free(temp_name);

    debug("Fill_ch_file end");
}

void total_random(struct ch_file *db, struct ft *final_teams)
{
    int i = 0;
    srand(time(NULL));   // should only be called once
    int choices = db->num_characters;
    int r = 0;
    for(i = 0; i<4; i++){
        r = rand() % choices;      // returns a pseudo-random integer between 0 and RAND_MAX
        printf("Number: %i \n",r);
        final_teams->players[i] = r;
        switch_places(db, r, choices);
        choices --;
    }

}


void print_teams(struct ch_file *db, struct ft *final_teams)
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
    struct ft* final_teams = malloc(sizeof(struct ft));
    const char *filename = "characters.txt";

    debug("Main area");

    fill_ch_file(filename, db);
    printf("There are %i characters. \n", db->num_characters);
//    print_all(db);

//     test_print(db);

    total_random(db,final_teams);
    print_teams(db,final_teams);

    return 0;
}
