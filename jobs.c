/* COMP 530: Tar Heel SHell
 * This file implements functions related to launching
 * jobs and job control.
 */

#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "thsh.h"
#include <string.h>
#include <stdio.h> 

static char ** path_table;

/* Initialize the table of PATH prefixes.
 *
 * Split the result on the parenteses, and
 * remove any trailing '/' characters.
 * The last entry should be a NULL character.
 *
 * For instance, if one's PATH environment variable is:
 *  /bin:/sbin///
 *
 * Then path_table should be:
 *  path_table[0] = "/bin"
 *  path_table[1] = "/sbin"
 *  path_table[2] = '\0'
 *
 * Hint: take a look at getenv().  If you use getenv, do NOT
 *       modify the resulting string directly, but use
 *       malloc() or another function to allocate space and copy.
 *
 * Returns 0 on success, -errno on failure.
 */
int  init_path(void) {
  /* Lab 0: Your code here */ 
  char *path = getenv("PATH"); // will hold addy to  path looking  /usr/lib/lightdm/lightdm:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/home/porter/bin
  //char *path =(char*) "/foo:/bar////";
  char *fullPath =strdup(path); // copy path so we don't get an uh oh 
  char **initSplit;
  if(fullPath == NULL){
	  return -errno;
  }

//  printf("%s\n", fullPath);


  int pathCount = 0;
  //int pathLength = strlen(fullPath);
  int i = 0;
  char *token;

  while ( (token = strsep(&fullPath, ":") ) != NULL){
	  pathCount++;	  
  }
  path_table = malloc((pathCount+1)*sizeof(char*));
  initSplit = malloc((pathCount+1)*sizeof(char*));
  free(fullPath);
  fullPath = strdup(path);

  for(i = 0; i <pathCount; i++){
	 token = strsep(&fullPath, ":");
	 //printf("%s\n", token);
	 int length = strlen(token);
	 initSplit[i] = malloc((length+1)*sizeof(char*));
	 if(initSplit[i] == NULL){
		 return -errno; 
	 }else{
		 strcpy(initSplit[i], token);
//		 printf("%s\n", initSplit[i]);
	 }
  }

 // free(token);
  free(fullPath);

  //strcpy(token, initSplit[0]);
  //path_table[0] = token;
  //printf("%s\n", token);


  for(i =0; i< pathCount; i++){
	  token = initSplit[i];
	  int l = strlen(token);
	  if(l ==0){
		  continue;
	  }

	 for(int j =l-1; j >=0; j--){		 
		 if(token[j] == '/' ){		 
			 token[j] ='\0';
		 }else{break;}
	 }

 
	path_table[i] = (char *) malloc(sizeof(char) *(strlen(token)+1));
		
	strcpy(path_table[i], token);
	 }

  path_table[pathCount +1 -1] = '\0';
  //printf("%s\n", path_table[0]);
 return 0;
}

/* Debug helper function that just prints
 * the path table out.
 */
void print_path_table() {
  if (path_table == NULL) {
    printf("XXXXXXX Path Table Not Initialized XXXXX\n");
    return;
  }

  printf("===== Begin Path Table =====\n");
  for (int i = 0; path_table[i]; i++) {
    printf("Prefix %2d: [%s]\n", i, path_table[i]);
  }
  printf("===== End Path Table =====\n");
}

/* Given the command listed in args,
 * try to execute it.
 *
 * If the first argument starts with a '.'
 * or a '/', it is an absolute path and can
 * execute as-is.
 *
 * Otherwise, search each prefix in the path_table
 * in order to find the path to the binary.
 *
 * Then fork a child and pass the path and the additional arguments
 * to execve() in the child.  Wait for exeuction to complete
 * before returning.
 *
 * stdin is a file handle to be used for standard in.
 * stdout is a file handle to be used for standard out.
 *
 * If stdin and stdout are not 0 and 1, respectively, they will be
 * closed in the parent process before this function returns.
 *
 * wait, if true, indicates that the parent should wait on the child to finish
 *
 * Returns 0 on success, -errno on failure
 */
int run_command(char *args[MAX_ARGS], int stdin, int stdout, bool wait) {
  /* Lab 1: Your code here */
  int rv = 0;
  struct stat buf;
  int status= -1;
  char *path;

  // check type of path
  if(*args[0] == '.' || *args[0] == '/'){
	  path = malloc(sizeof(args[0]));
	  strcpy(path, args[0]);
	  status =0;
  }
  else{
	  for (int i = 0; path_table[i]; i++) {
//	    printf("Prefix %2d: [%s]\n", i, path_table[i]);
		 path = malloc(sizeof(path_table[i]) +  sizeof(args[0]) +1);
                 path =strcat(path, path_table[i]);
		 path = strcat(path, "/"); 
		 path = strcat(path, args[0]);
		 //printf("%s\n", path);
		 if(stat(path, &buf) == 0){
			status = 0;
			break;	
		}
	}
  }

  if(status < 0) return -errno; 

  // fork and return pid of child
  pid_t childFork = fork();
 
  if(childFork < 0){
	  return -errno;
  }
  else if (childFork == 0){
	  	execvp(path, args);
  }else {
	  // wait for child
	  if(wait) waitpid(childFork, &rv, 0);
  }

  return rv;
}
