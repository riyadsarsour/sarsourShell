/* COMP 530: Tar Heel SHell */

#include "thsh.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(int argc, char **argv, char **envp) {
  // flag that the program should end
  bool finished = 0;
  int input_fd = 0; // Default to stdin
  int ret = 0;


  // Lab 1:
  // Add support for parsing the -d option from the command line
  int debug =0;  
  // and handling the case where a script is passed as input to your shell

  // Lab 1: Your code here

  ret = init_cwd();
  if (ret) {
    printf("Error initializing the current working directory: %d\n", ret);
    return ret;
  }

  ret = init_path();
  if (ret) {
    printf("Error initializing the path table: %d\n", ret);
    return ret;
  }

  if(argv[1] != NULL && strcmp(argv[1], "-d")==0){
	  debug =1;
  }

  if(argv[1] != NULL && strcmp(argv[1], "-d") != 0  && open(argv[1], O_RDONLY) >= 0 ){ 
//		  printf("started open\n");
		  input_fd = open(argv[1], O_RDONLY);
//		  close(input_fd);
  }


  while (!finished) {

    int length;
    // Buffer to hold input
    char cmd[MAX_INPUT];
    // Get a pointer to cmd that type-checks with char *
    char *buf = &cmd[0];
    char *parsed_commands[MAX_PIPELINE][MAX_ARGS];
    char *infile = NULL;
    char *outfile = NULL;
    int pipeline_steps = 0;
  
   

    if (!input_fd) {
      ret = print_prompt();
      if (ret <= 0) {
	// if we printed 0 bytes, this call failed and the program
	// should end -- this will likely never occur.
	finished = true;
	break;
      }
    }

    // Reset memory from the last iteration
    for(int i = 0; i < MAX_PIPELINE; i++) {
      for(int j = 0; j < MAX_ARGS; j++) {
	parsed_commands[i][j] = NULL;
      }
    }

    // Read a line of input
    length = read_one_line(input_fd, buf, MAX_INPUT);
    if (length <= 0) {
      ret = length;
      break;
    }

    // Pass it to the parser
    pipeline_steps = parse_line(buf, length, parsed_commands, &infile, &outfile);
    if (pipeline_steps < 0) {
      printf("Parsing error.  Cannot execute command. %d\n", -pipeline_steps);
      continue;
    }

    // Just echo the command line for now
    // file descriptor 1 -> writing to stdout
    // print the whole cmd string (write number of
    // chars/bytes equal to the length of cmd, or MAX_INPUT,
    // whichever is less)
    //
    // Comment this line once you implement
    // command handling
    //dprintf(1, "%s\n", cmd);
    
    
    

    
    // In Lab 1, you will need to add code to actually run the commands,
    // add debug printing, and handle redirection and pipelines, as
    // explained in the handout.
    //
    // For now, ret will be set to zero; once you implement command handling,
    // ret should be set to the return from the command.
    ret = 0;

    // run command
   int i=0;
   while(parsed_commands[i][0] != NULL){
	   if(parsed_commands[i][0] != NULL  && debug && (handle_builtin(parsed_commands[i], 0, 1, &ret) ==  0)){
		   fprintf(stderr, "RUNNING: [%s]\n", parsed_commands[i][0]);
	   	   ret = run_command(parsed_commands[i], 0, 1, true);
		   if(ret ==256) ret =0;
		   fprintf(stderr,"ENDED: [%s] (ret=%i)\n", parsed_commands[i][0], ret);
		   i++;
		   continue;
	   }
	   if (debug ==0 && (handle_builtin(parsed_commands[i], 0, 1, &ret) == 0)) {    
		   ret = run_command(parsed_commands[i], 0, 1, true);
	   }
	   else{  

		   if(debug != 0 ){
//		   printf("hellomofo\n");
		   fprintf(stderr,"RUNNING: [%s]\n", parsed_commands[i][0]);
                   fprintf(stderr ,"ENDED: [%s] (ret=%i)\n", parsed_commands[i][0], ret);
	   	   }
	   }
	   
	   i++;
    }

   if( ret == 256){
	   ret =0;
   }



    // Do NOT change this if/printf - it is used by the autograder.
    if (ret) {
      printf("Failed to run command - error %d\n", ret);
    }

  }
  close(input_fd);
  return ret;
}
