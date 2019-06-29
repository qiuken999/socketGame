/**** Project 60-656 ***/
/**** By Date: April 10th, 2019 ***/
/**** Team member info: ***/
/**** Yan Zhang #104194065 ***/
/**** Nuoer Gegen #104890439 ***/

/**** Service End ***/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>


void serviceClient(int);
int client;

int main(int argc, char *argv[]){  
  int sd, portNumber, status;
  struct sockaddr_in servAdd;      // client socket address
  
 if(argc != 2){
    printf("Call model: %s <Port Number>\n", argv[0]);
    exit(0);
  }
  
  //***Install socket
  
  //int socket(int domain, int type, int protocol);
  //When a realiable byte-stream connection is requested across the internet.
  //arg1 - AF_INET for internet, when AF_UNIX for same machine IPC
  //arg2 - type of communication: SOCK_STREAM [reliable byte stream connection as TCP]
  //arg3 - low level protocol. set to 0 in system calls, means correct/default protocol
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot create socket\n");
    exit(1);
  }
  sd = socket(AF_INET, SOCK_STREAM, 0);
  servAdd.sin_family = AF_INET;		//For TCP/IP, have to set as AF_INET to point [address family
  servAdd.sin_addr.s_addr = htonl(INADDR_ANY);		//bytes conversion - host to network (long)
  sscanf(argv[1], "%d", &portNumber);
  servAdd.sin_port = htons((uint16_t)portNumber);		//host to network (short) because post number is shorter expression
  
  //***bind
  //arg2 - cast the ptr to protocol-specific skt add structure to
  //	be a generic socket add structure
  //int bind(int, struct sockaddr *, socklen t);
  bind(sd, (struct sockaddr *) &servAdd, sizeof(servAdd));
  
  //int listen(int sockfd, int backlog);
  //arg2 - define max length to which queue of pending connections for
  //	sockfd may grow. If full -> ECONNREFUSED or retransmission (later)
  listen(sd, 5);

  while(1){
    printf("Waiting for a client to chat with\n");
    //int accept(int s, struct sockaddr *addr, socklen t *addrlen);
	//Returns a file descriptor for a new socket 
    client = accept(sd, NULL, NULL);
    printf("Got a client, start chatting\n");
    
    if(!fork())		//child
      serviceClient(client);		//***serviceClient

    //close(client);
    //pid_t waitpid(pid_t pid,int *status,int options);
	//arg1 - specific child
	//arg3 - constant, WNOHANG(if no child exit, still immediate back rather than wait)
    waitpid(0, &status, WNOHANG);
  }
}

//***child process
//play with client
//be player & referee [at the same time]
//1st gets in an infinite loop
void serviceClient(int sd){
	//(Server)player A & (Client)player B
	int scorePlayerA=0; 
	int scorePlayerB=0;
	int totalA=0;
	int totalB=0;
	
	char sentence[39];
	char outcomeAwin[32];
	char outcomeBwin[31];
	//infinite loop
	//check who reached 100
	//write(STDOUT_FILENO, "Hello", 6);
	while(1){
		//dup2 redirects so that screen become the client
		dup2(client, STDOUT_FILENO);
		if (totalA >= 100){
			strcpy(outcomeAwin, "\nGame over: you lost the game\n");
			write(STDOUT_FILENO, &outcomeAwin, sizeof(outcomeAwin));	//to client
			
			fprintf(stderr, "\nGame over: you won the game\n");
			close(client);
			exit(0);
			//return;		//out the serviceClient()
		}
		else if (totalB >= 100) {
			strcpy(outcomeBwin, "\nGame over: you won the game\n");
			write(STDOUT_FILENO, &outcomeBwin, sizeof(outcomeBwin));	//to client
			
			fprintf(stderr, "\nGame over: you lost the game\n");
			close(client);
			exit(0);
			//return;		//out the serviceClient()
		}
		else{
			strcpy(sentence, "\nGame on: you can now play your dice\n");
			write(STDOUT_FILENO, &sentence, sizeof(sentence));	//client
			
			fprintf(stderr, "\nGame on: playing dice\n");	//server
			scorePlayerA = rand()%6+1;
			fprintf(stderr, "Score of player A: %d\n", scorePlayerA);	//server
			write(STDOUT_FILENO, &scorePlayerA, sizeof(scorePlayerA));		//client
			
			totalA += scorePlayerA;
			fprintf(stderr, "Total score of Player A: %d\n", totalA);
			
			//read from the client for player B
			read(client, &totalB, sizeof(totalB));

		}
	}
}
