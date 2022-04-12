#include <stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){

    int fd_from, rbytes;
	char buff[513]={'\0'}, path[100]="./";
    // argc check
    if (argc != 2) {
      perror("ERROR: Incorrect argument count!\n");
      return(-1);
    }
    // builds path string
    strcat(path,argv[1]);
    strcat(path,".txt");
    // open path file and read contents to buffer (reads 512 bytes 1 lower than the initial set size)
	if( ( fd_from = open( path, O_RDONLY ) ) == -1 )
		{ printf("Restaurant Not Found!"); return( - 1 ); }
    if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
		 { perror( "read file" ); return( -1 ); }
    // while bytes were read prints to screen
    while( rbytes > 0 )    {
	printf("%s",buff);
	if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
	 	{ perror( "read 2" ); return( -1 ); }
	}
    printf("\n");
    // closes relevent files
	close( fd_from ); return( 0 );
}