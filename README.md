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
    Captain Falcon     :4
    ect.  
    [Name]             :[rating]


The ratings can be one to five digits.

Then the program will randomly chose 4 and move them to the last 4 positions
of the text file.  These 4 will be the teams that are chosen.

It is designed so that no 2 characters will appear twice or that characters will 
not be repeated in 2 subsequent runnings.

The ratings affect the teams in the way described below:

    Team 1:
    character 1 <rank 1>
    character 2 <rank 4>

    Team 2:
    character 3 <rank 2>
    character 4 <rank 3>
