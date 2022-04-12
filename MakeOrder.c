#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

int getprice(char*,char*); // getprice func declaration

int main(int argc, char* argv[]){
    int fd_from,fd_to, wbytes,count=0,cost=0,j=0,i,quan,price,status; // unnamed variables are for counting
	char path[100]="./",**dish,str[100],str2[100],*ptr;
    // time.h usage to calculate current date
    time_t t;  
    time(&t);
    struct tm *local = localtime(&t);
    // argc check
    if (argc != 3) {
      perror("ERROR: Incorrect argument count!\n");
      return(-1);
    }
    // build path strings
    strcat(path,argv[1]);
    strcpy(str,path);
    strcat(path,"_Order/");
    strcat(path,argv[2]);
    strcat(path,".txt");
    strcat(str,".txt");
    // checks if menu exists
    if( ( fd_from = open( str, O_RDONLY ) ) == -1 )
	    { printf("Restaurant Not Found!\n"); return( - 1 ); }
    // if it does, menu interaction will be used through getprice func, can close in main
    close(fd_from);
    if ((dish=(char**)(malloc(100*sizeof(char*))))==NULL) // malloc initial dish list of 100
       { perror( "memaloc fail" ); return( - 1 ); }
    printf("Insert your order (Finish to finish):\n");
    // every fgets usage is followed by if condition to eliminate newspace char
    fgets(str,100,stdin);
    if ((ptr=strchr(str, '\n')) != NULL)
       *ptr = '\0';
    // while finish was not input
    while (strcmp(str,"Finish")) {
       strcpy(str2,str);
       if (count>99) { // if there are more than 99 items in order
             if ((dish=(char**)(realloc(dish,sizeof(char*)*(j+1))))==NULL)
             { perror( "memaloc fail" ); return( - 1 ); }  
       }
       // running getPrice on input item, also checks if input string is viable
       ptr=str;
       // moves ptr to quantity in input
       while (*ptr!='\0') {
           if (isdigit(*ptr)) break;
           ptr++;
       }
       if (*ptr=='\0') {
          // if no quantity was found, restarts loop with new input
          printf("Quantity Missing! try again\n");
          fgets(str,100,stdin);
          if ((ptr=strchr(str, '\n')) != NULL)
            *ptr = '\0'; 
          continue; 
       }
       quan=atoi(ptr--);
       *(ptr--)='\0';
       while (*(ptr)=='\b') 
          *(ptr--)='\0';
       // calls getprice func with input string
       if((price = getprice(argv[1],str)) == -1) {
          printf("\ngetprice() failed.");
          return(-1);
       }
       if (price==0) { // dish was not found
         printf("\nDish not found! try again\n");
         fgets(str,100,stdin);
         if ((ptr=strchr(str, '\n')) != NULL)
            *ptr = '\0'; 
         continue; 
       }
       // cost calculation
       cost=cost+(price*quan);
       // malloc string for current dish to write in order file later
       if ((dish[count]=(char*)(malloc(strlen(str2)*sizeof(char))))==NULL)
          { perror( "memaloc fail" ); return( - 1 ); }
       strcpy(dish[count],str2);
       count++;
       fgets(str,100,stdin);
       if ((ptr=strchr(str, '\n')) != NULL)
          *ptr = '\0'; 

    }
    // prints total price and waits for confirmation
    printf("Total Price: %d NIS (Confirm to approve/else cancle)\n",cost);
    fgets(str,100,stdin);
    if ((ptr=strchr(str, '\n')) != NULL)
       *ptr = '\0'; 
    if (strcmp(str,"Confirm")) { // cancel
       printf("\n Order canceled, exiting\n");
       return(0);
    }
    // order confirmed, creates write file   
    if( ( fd_to = open(path,O_RDWR | O_CREAT, 0664  ) ) == -1 )
		{ perror( "open write file" ); return( - 1 ); }   
    strcpy(str,argv[1]);
    strcat(str," Order\n\n");
    if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
		{ perror( "write" ); return( -1 ); }
    // for each dish, writes to order file
    for (i=0;i<count;i++) {
        if( ( wbytes = write( fd_to, dish[i], strlen(dish[i])) ) == -1 )
		{ perror( "write" ); return( -1 ); }
        if( ( wbytes = write( fd_to, "\n", strlen("\n")) ) == -1 )
	    { perror( "write" ); return( -1 ); }      
    }
    // further writings with total price etc
    if( ( wbytes = write( fd_to, "Total Price is: ", strlen("Total Price is: ")) ) == -1 )
    { perror( "write" ); return( -1 ); }
    sprintf(str,"%d",cost);
    if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
    { perror( "write" ); return( -1 ); }
    if( ( wbytes = write( fd_to, "NIS\n\n", strlen("NIS\n\n")) ) == -1 )
    { perror( "write" ); return( -1 ); }
    // proccessing current time info and writing the current date to file
    sprintf(str2,"%d",local->tm_mday);
    if (local->tm_mday<10) 
       strcpy(str,"0");
    else 
       strcpy(str,"");
    strcat(str,str2);
    sprintf(str2,"%d",local->tm_mon+1);
    if (local->tm_mon+1<10) 
       strcat(str,"/0");
    else 
       strcat(str,"/");
    strcat(str,str2);
    strcat(str,"/");
    sprintf(str2,"%d",local->tm_year+1900);
    strcat(str,str2);
    if( ( wbytes = write( fd_to, str, strlen(str)) ) == -1 )
    { perror( "write" ); return( -1 ); }
    // after writing has finished, changes permission to read only
    if( ( status = fchmod(fd_to,S_IRUSR|S_IRGRP|S_IROTH) ) == -1 )
 	{ perror( "chmod error:" ); return( -1 ); }
	// prints end string
    strcpy(str,argv[2]);
    strcat(str,".txt");
    strcpy(str2,argv[1]);
    strcat(str2,"_Order Dir with Read Mode");
    printf("Order created! // %s Created in %s\n",str,str2);
    // free malloced memory
    for (j=0;j<count;j++) {
       free(dish[j]);
    }
    free(dish);
    // closes write file
    close(fd_to);   
    return( 0 );

}


// this getprice function is pretty much identical to getPrice.c, without the printings
int getprice(char* resto,char* dish) {
    int fd_from, rbytes;
	char buff[512], path[100]="./",*ptr,str[100];
    // builds path string
    strcpy(str,dish);
    strcat(str," .");
    strcat(path,resto);
    strcat(path,".txt");
	if( ( fd_from = open( path, O_RDONLY ) ) == -1 )
		{ printf("Restaurant Not Found!"); return( - 1 ); }
    if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
		 { perror( "read file" ); return( -1 ); }
    while( rbytes > 0 )    {
	if ((ptr=strstr(buff,str))!=NULL &&*(ptr-3)=='\n') {
        while (ptr!=NULL) {
           if (isdigit(*ptr)) break;
           ptr++;
        }
        if (atoi(ptr)!=0) {
           close( fd_from ); return(atoi(ptr));
        }  
        if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
	 	{ perror( "read 2" ); return( -1 ); }
        if (rbytes!=0&&atoi(buff)!=0) {
           close( fd_from ); return( atoi(ptr) );
        } 
        printf("Price not found, error");
        close( fd_from ); return(0);
	}    
	if( ( rbytes = read( fd_from, buff, 512 ) ) == -1 )
	 	{ perror( "read 2" ); return( -1 ); }
	}
    // closes relevent files
	close( fd_from ); return( 0 );
}