/**** Project 60-656 ***/
/**** By Date: April 10th, 2019 ***/
/**** Team member info: ***/
/**** Yan Zhang #104194065 ***/
/**** Nuoer Gegen #104890439 ***/

/**** Client End ***/

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>


int main(int argc, char *argv[]){
	int spA=0;
	int scorePlayerB=0;
	int totalA=0;
	int totalB=0;
	char sentence[39];
	//char temp[1];
	// int tempA=0;
// 	int tempB=0;
	
	int server, portNumber, pid, n;
	struct sockaddr_in servAdd;     // server socket address
  
	if(argc != 3){
    	printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
    	exit(0);
  	}

	//install socket
  	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	fprintf(stderr, "Cannot create socket\n");
     	exit(1);
  	}

  	servAdd.sin_family = AF_INET;
  	sscanf(argv[2], "%d", &portNumber);
  	servAdd.sin_port = htons((uint16_t)portNumber);

	//ip conversion
  	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
  		fprintf(stderr, " inet_pton() has failed\n");
  		exit(2);
  	}

 	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
  		fprintf(stderr, "connect() has failed, exiting\n");
 		exit(3);
 	}

	while(1){		
		read(server, &sentence, sizeof(sentence));
		fprintf(stderr, "%s", sentence);	//the directive from server
	
		read(server, &spA, sizeof(spA));
		fprintf(stderr, "server's score in this turn: %d\n", spA);

		totalA += spA;
		fprintf(stderr, "server's TOTAL score: %d\n", totalA);
		
		if(spA == 0){
//            // kill(getppid(), SIGTERM);
			close(server);
            exit(0);
        }else{
        	fprintf(stderr, "Hit ENTER to play dice\n");
        	if(getchar() == '\n'){
        		scorePlayerB = rand()%6+1;	
        		fprintf(stderr, "\t***Score of player B: %d\n", scorePlayerB);
				totalB += scorePlayerB;
				fprintf(stderr, "\t***My TOTAL score: %d\n", totalB);
        	}
        	//tempB = htons(totalB);
        	write(server, &totalB, sizeof(totalB));
        }
		
	}
}
