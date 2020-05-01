#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

char *splitted[200];
int counter;
//this function updates the files when a child is terminated

void terminatorr(int value)
{
    FILE* t=fopen("logg.log", "a+");
    fprintf(t,"Child process was terminated");
    fprintf(t," signal %d \n",value);
    fclose(t);
}
// this function takes a line into a string array and splits it into words using pointers
void split()
{
    char input[500];
    counter=0;
    char *point;
    gets(input);
    point= strtok(input," ");
    while (point!= NULL)
    {
        splitted[counter]= point;
        point= strtok(NULL," ");
        counter++;
    }
}

//this function uses the splitted array of words and executes according to the process ID ( whether its a child or a parent process)


void processsID()
{

    int flag=1;
    int exitFlag =1 ;
    flag = strcmp(splitted[counter-1],"&");
//printf("%d flag",flag);
    pid_t processIDD;
    processIDD= fork();
    if(processIDD == 0)
        execvp(splitted[0],splitted);
    else if (processIDD < 0)
        printf("Failure\n");
    else
    {
        if (flag ==0)
        {
            signal(SIGCHLD, terminatorr);
        }
        else
        {
            signal(SIGCHLD, terminatorr);
            pause();
            wait(NULL);
        }

    }
}

int main()
{
    while(1)
    {
        split();
        if(strcmp(splitted[0], "cd")==0)
        {
            int chdirr=chdir(splitted[1]);
            if(chdirr<0)
            {
                printf("directory doesnt exist");
            }
        }
        if(strcmp(splitted[0], "exit")==0)
            exit(0);
        processsID();
    }
    return 0;
}
