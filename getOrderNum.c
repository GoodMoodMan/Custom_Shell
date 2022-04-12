#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char* argv[]){

   int count=0;
   char path[100]="./";
   // uses dirent.h and DIR type to count files in directory
   DIR *d;
   struct dirent *dir;
   // argc check
   if (argc != 2) {
      perror("ERROR: Incorrect argument count!\n");
      exit(EXIT_FAILURE);
   }
   // builds path string
   strcat(path,argv[1]);
   strcat(path,"_Order");
   // if the folder is not found
   if( ( d = opendir(path) ) == NULL)
		{ printf("Restaurant not found!\n"); return( - 1 ); }
   // while there are still files to read in folder count goes up
   while ((dir = readdir(d)) != NULL) {
        count++;
   }
   // initial file count for empty folder is 2, dont know why
   printf("%d Orders\n",count-2);
   // closes directory
   closedir(d);
   return 0;
}