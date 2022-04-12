#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){

    int fd_from, rbytes;
	char buff[512], path[100]="./",*ptr,str[100];
    // argc check, also combines argv2 and argv3 if argc==4
    if (argc<3) {
      printf("ERROR: Incorrect argument count!\n");
      return(-1);
    }
    strcpy(str,argv[2]);
    strcat(str," ");
    if (argc==4) {
       strcat(str,argv[3]);
       strcat(str," .");
    }
    // adds '.' to string so the found needle string in menu is exactly the requested dish
    else 
       strcat(str,".");
    // builds path string
    strcat(path,argv[1]);
    strcat(path,".txt");
    // opens and reads from path file
	if( ( fd_from = open( path, O_RDONLY ) ) == -1 )
		{ printf("Restaurant Not Found!\n"); return( - 1 ); }
    if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
		 { perror( "read file" ); return( -1 ); }
    // while bytes were read to buffer, searches for the input dish in buffer
    while( rbytes > 0 )    {
	if ((ptr=strstr(buff,str))!=NULL &&*(ptr-3)=='\n') {
        // if the dish was found
        // moves ptr to price location
        while (ptr!=NULL) {
           if (isdigit(*ptr)) break;
           ptr++;
        }
        // if price was found, returns the price and closes file
        if (atoi(ptr)!=0) {
           printf("%d NIS\n",atoi(ptr));
           close( fd_from ); return(atoi(ptr));
        }  
        // if price was NOT found, continues reading
        if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
	 	{ perror( "read 2" ); return( -1 ); }
        if (rbytes!=0&&atoi(buff)!=0) {
           printf("%d NIS\n",atoi(ptr));
           close( fd_from ); return( atoi(ptr) );
        } 
        // if the prices is still not found, something went wrong or there is no price in menu
        printf("Price not found, error");
        close( fd_from ); return(0);
	}    
    // if the dish was not found, continues reading
	if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
	 	{ perror( "read 2" ); return( -1 ); }
	}
    // if the file has been completley read and the dish was not found
    printf("Dish not found!\n");
    // closes file and returns 0
	close( fd_from ); return( 0 );
}