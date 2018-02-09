/* Tri Dang
 * CS351
 * Project 1 UNIX Shell
 * 2/7/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LINE 80 //max length of command
#define MAX_HISTORY 35
int main(void) {

int size = MAX_LINE/2+1;
char args[size]; //array to store command line arguments
int should_run = 1; 	//flag to determine when to exit program
char *parray[sizeof(args)];
char *token;
char amp[2] = "&";
char *keywords[10] = {"exit","history", "!", "!!"};

//variables for history feature
int historyc = 0;
char *history[MAX_HISTORY];

while (should_run) {
	printf("osh>");
	fflush(stdout);
	pid_t pid;
	//read user input and puts entire command into an array
	if (fgets(args, size, stdin) != NULL) {
		int i = 1;
		int containsamp = 0;
		//printf("fgets: %s\n", args);
		//puts current command into history buffer		
		history[historyc++] = strdup(args);
		//printf("historyc = %d\n", historyc);
		//printf("history1 %s\n", history[0]);
		//printf("history2 %s\n", history[1]);
		//printf("history3 %s\n", history[2]);
		
		//check if ampersand exists		
		if (strstr(args, amp) != NULL) {
			containsamp = 1;
		}
		
		//get first token from array of commands
		token = strtok(args, " ");
		
		//exit out of shell program command
		if (strstr(token, keywords[0]) != NULL ) {
			printf("Exiting shell program!\n");				
			return 0;
		}
		//enable history
		else if (strstr(token, keywords[1]) != NULL ) {
			for (int j = historyc; j > 0; j--) {		
				printf("%d.%s", j, history[j-1]);
			}
		}
		else {
			//push into pointer array
			parray[0] = token;
		
			//get rest of token and push into pointer array
			while (token != NULL) {
				token = strtok(NULL, " ");
				parray[i] = token;
				i++;
			}
		
			//fork a child proccess using fork()	
			pid = fork();
	
			if (pid < 0) {		//error occured
				fprintf(stderr, "Command Failed");
				return 1;
			}
	
			else if (pid == 0) {	//child process will invoke execvp
				//printf("command1 %s\n", parray[0]);
				//printf("command2 %s\n", parray[1]);
				//printf("command3 %s\n", parray[2]);
				//printf("amps = %d\n", containsamp);	
				//if input is !!
				if (strstr(parray[0], keywords[3]) != NULL) {
					int i;
					printf("Double exclamation\n");
					if (strstr(history[historyc-2], keywords[3]) != NULL){
						printf("No commands in history\n");
					}
					//get token from history buffer
					else {
						printf("Has command in history");
						token = strtok(history[historyc-2], " ");
						parray[0] = token;
						while (token != NULL) {
							token = strtok(NULL, " ");
							parray[i] = token;
							i++;
						}
						execvp(parray[0], parray);
					}
				}
				else {	//regular command execution			
					execvp(parray[0], parray);
					//printf("Reached child\n");
				}	
			}	

			else {		//parent process
				if (containsamp == 0) {	//if no &, parent wait for child to complete
					//printf("Waiting for Child\n");
					wait(NULL);
					//printf("Child Complete\n");
					
				}
				else {
					printf("Did not wait for Child\n");
				}
			}
			//printf("Parent Complete\n");				
		}
		//return 0;
	}
	
}
//if command included &, parent will invoke wait()
return 0;
}

//implement strdup to allow copying of string in buffer
char *strdup(const char *str) {
	size_t len = strlen(str) + 1;
	char *dup = malloc(len);
	if (dup != 0)
		memmove(dup, str, len);
	return(dup);
}
