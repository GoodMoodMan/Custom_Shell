#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){

    int i=0,j=0,status; // unnamed variables are used for loop countings
    pid_t pid;
	char str[100], *ptr,*ptr2,*argv2[5]={NULL};
    // infinite shell loop, runs based on input (ends when exit is input)
    while (1) {    
    printf("AdvShell>");
    fgets(str,100,stdin);
    if ((ptr2=strchr(str, '\n')) != NULL)
       *ptr2 = '\0';
    if (strcmp(str,"exit")==0) {
       printf("Goodbye...\n");
       return 0;
    }
    // builds args string array (every string between backspace goes to different cell)
    ptr=strtok(str," ");
    while (ptr != NULL && i<5) {
       argv2[i]=(char*)(malloc(sizeof(char)*(strlen(ptr)+1)));
       if (argv2[i]==NULL) {
          perror("mem alloc failed.");
          exit(1);
       }

       strcpy(argv2[i], ptr);
       ptr = strtok (NULL, " ");
       i++;
    }
    // forks to child proccess in order to run the input commands
    if((pid = fork()) == -1) {
       perror("fork() failed.");
       exit(1);
    }

	if (pid==0) { // child proccess if condition, relevent command for each program
       if(strcmp(argv2[0],"CreateMenu")==0) {
          execvp("./CreateMenu",argv2);
          perror("CreateMenu error");
       }
       if(strcmp(argv2[0],"getMenu")==0) {
          execvp("./getMenu",argv2);
          perror("getMenu error");
       }
       if(strcmp(argv2[0],"MakeOrder")==0) {
          execvp("./MakeOrder",argv2);
          perror("MakeOrder error");
       }
       if(strcmp(argv2[0],"getPrice")==0) {
          execvp("./getPrice",argv2);
          perror("getPrice error");
       }
       if(strcmp(argv2[0],"getOrderNum")==0) {
          execvp("./getOrderNum",argv2);
          perror("getOrderNum error");
       }
       // if the requested operation is not one of the custom built ones
       execvp(argv2[0],argv2);
       printf("Not Supported\n");
       return(0);
    }
    // waits for child proccess to end
    pid = wait(&status);
    // frees malloced memory (for each loop memory is allocated and freed)
    for(i=0;i<5;i++) {
       free(argv2[i]);
       argv2[i]=NULL;
    }
    // resets i
    i=0;
    }
    return( 0 );
}