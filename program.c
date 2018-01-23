//#define NDEBUG

#include "dbg.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>



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
};

void die(const char *message)
{
    if(errno){
        perror(message);
    } else {
        printf("ERROR: %s \n", message);
    }
    exit(1);

}

void print_ch(struct ch_file *db, int num)
{
    printf("Character: %s   Rating: %i \n", db->character[num], db->rating[num]);
}

void print_all(struct ch_file *db)
{
    int i = 0;

    for(i = 0; i<MAX_C; i++){
        if(db->character[i] != NULL){
            print_ch(db,i);
        }
    }
}

void add_ch(struct ch_file *db, int num, int rating, char *name)
{

    db->character[num] = name;
    db->rating[num] = rating;

}

void fill_ch_file(char *filename, struct ch_file *db)
{
    debug("fill_ch_file start");

    db->file = fopen(filename, "r+");
    char temp_name[50];
    int temp_rating =0;
    int db_num = 0;
    int num_c = 0;
    bool next = false;
    char *temp = malloc(50);


    if(db->file ==NULL){
        die("File Failed to open");
    }


    while(1){

	num_c = fgetc(db->file);
        printf("Letter read: %c\n", num_c);
        switch(num_c){

            case ':' :
                if(next){
                   temp_rating = num_c-49;
                   next = false;
                }
                next = true;
		break;
            case '\n' :
                debug("Here is the next line: db_num = %i",db_num);
                add_ch(db,db_num,temp_rating,temp_name);
                db_num++;
		break;
            default:
                temp_name[strlen(temp_name)] = num_c;
        }


        if(feof(db->file)){
            break;
        }

    }




    debug("Fill_ch_file end");
}

void test_print(struct ch_file *db)
{
    add_ch(db,0,7,"Kirby");
    add_ch(db,1,5,"Fox");
    add_ch(db,2,3,"Ness");

    print_all(db);
}


int main(int argc, char *argv[])
{

  //  if(argc != 3){
  //      die("It should be [identifier:ie -g] [filename]");
  //   }

    struct ch_file *db = malloc(sizeof(struct ch_file));
    const char *filename = "characters.txt";

    debug("Main area");

    fill_ch_file(filename, db);


    print_all(db);

    return 0;
}
