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
    int fd_to, wbytes,count[100]={0},j=0,i,dir,x; // unnamed variables are used for counting
	char path[100]="./",*type[atoi(argv[2])],**dish,str[100],*ptr;
    // argc check
    if (argc != 3) {
      perror("ERROR: Incorrect argument count!\n");
      return(-1);
    }
    // builds path string
    strcat(path,argv[1]);
    strcat(path,".txt");
    // malloc initial dish list of 100 items, realloced if input supercedes 100
    if ((dish=(char**)(malloc(100*sizeof(char*))))==NULL)
       { perror( "memaloc fail" ); return( - 1 ); }
    // this loop runs based on input number of dish types, inputs dishes for each type based on guidelines
    for (i=0;i<atoi(argv[2]);i++) {
       printf("Insert type dish %c:\n", (char)('a'+i));
       // each use of fgets is followed with simple if check to eliminate the newline char
       fgets(str,100,stdin);
       if ((ptr=strchr(str, '\n')) != NULL)
          *ptr = '\0';
       // the relevent cell in type array is malloced based on input length
       if ((type[i]=(char*)(malloc(strlen(str)*sizeof(char))))==NULL)
          { perror( "memaloc fail" ); return( - 1 ); }
       strcpy(type[i],str);

       printf("Insert dish name %d:\n", count[i]+1);
       fgets(str,100,stdin);
       if ((ptr=strchr(str, '\n')) != NULL)
          *ptr = '\0';
       // while loop for dish names and prices input
       while (strcmp(str,"Stop")!=0) { 
          // j represents overall number of dishes, if exceeds 100 needs to realloc 
          if (j>99) {
             if ((dish=(char**)(realloc(dish,sizeof(char*)*(j+1))))==NULL)
             { perror( "memaloc fail" ); return( - 1 ); }  
          }
          // this is the malloc of the dish string itself    
          if ((dish[j]=(char*)(malloc(strlen(str)*sizeof(char))))==NULL)
          { perror( "memaloc fail" ); return( - 1 ); }
          strcpy(dish[j],str);
          // every addition to dish array j and count[i] go up by 1 (where i represents the type num)
          j++;
          count[i]++;
          // inputs again to restart loop
          printf("Insert dish name %d:\n",count[i]+1);
          fgets(str,100,stdin);
          if ((ptr=strchr(str, '\n')) != NULL)
             *ptr = '\0';
       }
    }  
    // input proccessing is done, creates write file and writes contents based on guidelines
    // builds initial write string
    strcpy(str,argv[1]);
    strcat(str," Menu");
    // create path file with read/write permission
    if( ( fd_to = open(path,O_RDWR | O_CREAT, 0664  ) ) == -1 )
		{ perror( "open write file" ); return( - 1 ); }
    // writes the initial string 
    if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
		{ perror( "write" ); return( -1 ); }
    // resets j for further use with dish array
    j=0;
    // for loop based on number of dish types, for each one writes the requested formatting to file
    for (i=0;i<atoi(argv[2]);i++) {
        if( ( wbytes = write( fd_to, "\n\n", strlen("\n\n")) ) == -1 )
		{ perror( "write" ); return( -1 ); }
        strcpy(str," . ");
        str[0]=(char)('a'+i);
        strcat(str,type[i]);
        if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
		{ perror( "write" ); return( -1 ); }
        // while count[i] is higher than 0, means there are still dishes to write in the current type
        while (count[i]>0) {
           if( ( wbytes = write( fd_to, "\n  ", strlen("\n  ")) ) == -1 )
		   { perror( "write" ); return( -1 ); }
           strcpy(str,dish[j]);
           // this while loop adds '.' to the string to match the requested guidelines 
           while(strlen(str)<21) {
              str[strlen(str)+1]=str[strlen(str)];  
              str[strlen(str)]=str[strlen(str)-1]; 
              x=2;
              while (isdigit(str[strlen(str)-x])) {
                 str[strlen(str)-x]=str[strlen(str)-(x+1)];
                 x++;
              }
              str[strlen(str)-x]='.';
           }
           // some final changes to the dish write string, adding spaces where needed and NIS
           ptr=strchr(str,'.');
           *ptr=' ';
           strcat(str,"NIS");
           if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
	       { perror( "write" ); return( -1 ); }
           // dish has been written, count is lowered and j is added 1
           count[i]--;
           j++;
       }          
    }
    // dish info is done writing, writes final requested lines to the file
    if( ( wbytes = write( fd_to, "\n         Bon Appetit", strlen("\n         Bon Appetit")) ) == -1 )
    { perror( "write" ); return( -1 ); }
    // builds directory path string
    strcpy(str,argv[1]);
    strcat(str,"_Order");
    if( ( dir = mkdir( str, S_IRWXU | S_IRWXG | S_IRWXO ) )== -1 )
		{ perror( "open directory" ); return( - 1 ); }
    // builds and prints the last string signifying the program was successful and the relevnt files were created
    strcpy(str,argv[1]);
    strcat(str,".txt");
    printf("Successfully created // %s",str);
    printf(" created , ");
    strcpy(str,argv[1]);
    strcat(str,"_Order dir created.");
    printf("%s\n",str);
    // free everything that was malloced
    while (j>0) {
       free(dish[j-1]);
       j--;
    }
    for (j=0;j<atoi(argv[2]);j++) {
       free(type[j]);
    }
    free(dish);
    // closes file
    close(fd_to);
    return( 0 );

}