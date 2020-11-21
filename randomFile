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
	 // printf("%s\n", initSplit[i]);
	  int l = strlen(token);
	  int trailing = 0;
	  if(token[0] == '/' && strlen(token) == 1){
		  continue;
	  }
	 for(int j =1; j <= l; j++){		 
		 if(token[j] == '/' && token[j-1] ==  '/'){
			if(token[j+1] != -1 && token[j+1] == '/'){
				continue;
			}
			char *sub;
			path_table[i] = (char *) malloc(sizeof(char) *(l-j));
			sub = malloc(sizeof(char) *(l-j));
			memcpy(sub, &token[j], l-j);
			//strcpy(path_table[i], sub);
			path_table[i] = strdup(sub);
//			printf("%p %s\n", path_table[i], path_table[i]);
//			free(sub);
			trailing =1;
		}
		 
		 
		 else if (trailing == 0 && j == l-1) {
			path_table[i] = malloc(sizeof(char*) *(l+1));
			//strcpy(path_table[i], token);
			path_table[i] = strdup(token);
			//printf("%p %s\n", path_table[i], path_table[i]);
		}
	
	 }
	 
//	 free(token);
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
