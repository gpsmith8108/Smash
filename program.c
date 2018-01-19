//#define NDEBUG

#include "dbg.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>



//This program will chose fair super smash brother teams

#define MAX_C 512

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

int interperate_line(char *str)
{
    char *ptr = str;
    int c_pos = 0;

    while(*ptr != EOF){

        if(*ptr == ','){
            return c_pos;
        }

    ptr = ptr+1;
    c_pos = c_pos +1;

    }

    die("There was no comma in one of the names in the characters sheet");

    return -1;
}
void fill_ch_file(char *filename, struct ch_file *db)
{
    debug("fill_ch_file start");

    db->file = fopen(filename, "r+");
    char *temp = malloc(500);
    int c_pos = 0;

    if(db->file ==NULL){
        die("File Failed to open");
    }

    debug("Middle of fill_ch_file start");




   // while(1){

        fread(db->character, sizeof(db->character),1,db->file);
       // printf("%s \n", );
         print_all(db);

//        c_pos = interperate_line(temp);

  //      printf("the comma is at: %i \n", c_pos);

     //   if(feof(db->file)){
       //     break;
        //}
   // }




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
    char * filename = "characters.txt";

    debug("Main area");

    fill_ch_file(filename, db);


    return 0;
}
