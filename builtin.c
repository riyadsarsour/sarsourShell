/* COMP 530: Tar Heel SHell
 *
 * This file implements a table of builtin commands.
 */
#include "thsh.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct builtin {
  const char * cmd;
  int (*func)(char *args[MAX_ARGS], int stdin, int stdout);
};


static char old_path[MAX_INPUT];
static char cur_path[MAX_INPUT];

/* This function needs to be called once at start-up to initialize
 * the current path.  This should populate cur_path.
 *
 * Returns zero on success, -errno on failure.
 */
int init_cwd(void) {

  // Lab 1: Your code here
  char buf[MAX_INPUT];
  if( getcwd(buf, sizeof(buf)) != NULL ){
	  strcpy(cur_path,buf);
	 // printf("%s\n", cur_path);
  }else{
	  return -errno;
  }

  return 0;
}

/* Handle a cd command.  */
int handle_cd(char *args[MAX_INPUT], int stdin, int stdout) {

  // Note that you need to handle special arguments, including:
  // "-" switch to the last directory
  // "." switch to the current directory.  This should change the
  //     behavior of a subsequent "cd -"
  // ".." go up one directory
  //
  // Hint: chdir can handle "." and "..", but saving
  //       these results may not be the desired outcome...

  // XXX: Be sure to write test cases for ., .., and weirdness
  // XXX: Test for errors in the output if a cd fails

  // Lab 1: Your code here
  //

  // Remove the following two lines once implemented.  These
  // just suppress the compiler warning around an unused variabl
  //(void)  cur_path ;
  //(void)  old_path;
  int rv =0;

 //printf("args[1] == NULL: %i", !args[1]);
 //printf("%s\n", args[1]);

  if(args[1] != NULL  &&  strcmp(args[1], "-") ==0){
        if( chdir(old_path) != 0){
		return -errno;
	}else{
	chdir(old_path);
        strcpy(old_path,cur_path);
        init_cwd();
        //strcpy(cur_path, getcwd(buf, sizeof(buf)));
//	printf("%s\n", cur_path);
	//printf("%s\n", getcwd(buf, sizeof(buf)))
//	printf("wrong loop\n");
//	return rv;
	}	
  }
 else if (args[1] != NULL){
	//printf("%s\n", args[1]);
	strcpy(old_path,cur_path);
	//printf("%s\n", old_path);
	//printf("%s\n", cur_path);
	chdir(args[1]);
	//printf("%s\n", cur_path);
	init_cwd();
//	printf("%s\n", cur_path);
//	strcpy(cur_path, getcwd(buf, sizeof(buf)));
//	printf("right loop\n");
	return rv;
	
 }
  else{
	if( chdir(cur_path) != 0){
		return -errno;
	}else{
	     strcpy(old_path, cur_path);
	     chdir(cur_path);
	     init_cwd();
//	     printf("%s\n", cur_path);
//	     return rv;
  	}
  }


  return rv;

}

/* Handle an exit command. */
int handle_exit(char *args[MAX_ARGS], int stdin, int stdout) {
  exit(0);
  return 0; // Does not actually return
}


void handle_goheels(void){
 FILE  *asciiFile;
 int counter;
// read file now
 asciiFile = fopen("goheels.txt", "r");
 while( (counter = getc(asciiFile)) != EOF){
	 putchar(counter);
 }
 fclose(asciiFile);
}



static struct builtin builtins[] = {{"cd", handle_cd},
				    {"exit", handle_exit},
				    {'\0', NULL}};

/* This function checks if the command (args[0]) is a built-in.
 * If so, call the appropriate handler, and return 1.
 * If not, return 0.
 *
 * stdin and stdout are the file handles for standard in and standard out,
 * respectively. These may or may not be used by individual builtin commands.
 *
 * Places the return value of the command in *retval.
 *
 * stdin and stdout should not be closed by this command.
 *
 * In the case of "exit", this function will not return.
 */
int handle_builtin(char *args[MAX_ARGS], int stdin, int stdout, int *retval) {
  int rv = 0;
  // Lab 0: Your Code Here
  // Comment this line once implemented.  This just suppressesthe unused variable warning from the compiler.
  
  if(strcmp(*args,"cd") ==0 ){
	*retval = handle_cd(args, stdin, stdout);
	rv= 1;
  }else if(strcmp(*args, "exit" )== 0){
 	*retval =handle_exit(args, stdin, stdout);
  	rv =1;
  }else if(strcmp(*args, "goheels") ==0){
	  handle_goheels();
	  rv =1;
  }
  (void) builtins;
  return rv;
 
}

/* This function initially prints a default prompt of:
 * thsh>
 *
 * In Lab 1, Exercise 3, you will add the current working
 * directory to the prompt.  As in, if you are in "/home/foo"
 * the prompt should be:
 * [/home/foo] thsh>
 *
 * Returns the number of bytes written
 */
int print_prompt(void) {
  int ret = 0;
  // Print the prompt
  // file descriptor 1 -> writing to stdout
  // print the whole prompt string (write number of
  // bytes/chars equal to the length of prompt)
  //
  char *prompt = "] thsh> ";
  // Lab 1: Your code here
  char dir[MAX_INPUT];
  dir[0] = '[';
  dir[1] = '\0';
  strcat(dir, cur_path);
  prompt = strcat(dir, prompt);

  ret = write(1, prompt, strlen(prompt));
  return ret;
}
