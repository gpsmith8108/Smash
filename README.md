# WARNING: Garrett probably messed something up

# Smash
Super Smash brothers Team Selector

This is a command line tool that will help decide teams in supersmash brothers, you upload the character list with the ratings
and the tool creates fair teams.

Detailed Description:
First pull down the branch from git hub and compile with your prefered C compiler

Commands:

    make program     /*Make the program*/
    ./program        /*Run the program*/

The program works by pulling in the characters listed in characters.txt, but they must be in 
the format below:

    Fox                :9
    Kirby              :8
    ect.  
    [Name]             :[rating]


The ratings must be one digit.

Then the program will randomly chose 4 and move them to the last 4 positions
of the text file.  These 4 will be the teams that are chosen.

It is designed so that no 2 characters will appear twice or that characters will 
not be repeated in 2 subsequent runnings.

Now it is currently being developed so that the ratings affect the players on each team.
The goal is:

    Team One:
    Player One:     1st Highest Rated Character
    Player Two:     4th Highest Rated Character

    Team Two:
    Player Three:   2nd Highest Rated Character
    Player Four:    3rd Highest Rated Character


However this goal has not been realized yet.
