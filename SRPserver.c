/* Sample UDP server program, server.c,  */
/* Note: This is a very simple example just to get you started.
   For example, we show the server calls recvfrom() to receive data.
   The following are not shown below, but will have to be added to
   your server program for the assignment:
 * you will have to add sendto_() to send ACK's
 * you will also have to initialize the library using init_net_lib()
 * both sendto_() and init_net_lib() need to be declared as external
 * you should probably call select() from within the loop
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <stdlib.h>
#include <time.h>
#include "sendto_.h"

#include "types.h"

#define LOCAL_SERVER_PORT 50007
#define RWS 4
#define MAXSEQNUM 6


int main(int argc, char *argv[]) {

	struct sockaddr_in cliAddr, servAddr;
	unsigned int cliLen;
	int sock, rc;
	char recvmsg[100];

	/* You should have some command-line processing that follows the format
	   ./server_SRP <error_rate> <random_seed> <output_file> <receive_log>
	   */

	if(argc<5) {
		printf("usage : %s <error rate> <random seed> <send_file> <send_log> \n", argv[0]);
		exit(1);
	}
	printf("error rate : %f\n",atof(argv[1]));

	typedef struct{

	  int seqNum;
	  int lastFrame; //Last frame flag                                                       \
                                                                                           \
                                                                                            
	  char data[512];
	} frame;


	/* Note: recvyou must initialize the network library first before calling
	   sendto_().  The arguments are the <errorrate> and <random seed> */
	init_net_lib(atof(argv[1]), atoi(argv[2]));

	/* socket creation */
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	  printf("Unable to connect socket!\n");
	  exit(1); 
	}

	/* bind local server port.  Note the server port must be a
	   "well-known" whose value is known by the client */
	bzero(&servAddr,sizeof(servAddr));                    //zero the struct
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind (sock, (struct sockaddr * ) &servAddr, sizeof(servAddr));
	if(rc<0) {
	  printf("%s: cannot bind port number %d \n",argv[0], LOCAL_SERVER_PORT);
	  exit(1); 
	}


	cliLen = sizeof(cliAddr);

	printf("%s: waiting for data on port UDP %u\n",argv[0],LOCAL_SERVER_PORT);
	bzero(&recvmsg,sizeof(recvmsg)); 


	if((recvfrom(sock, &recvmsg, sizeof (recvmsg), 0,
		     (struct sockaddr *) &cliAddr, (socklen_t*) &cliLen))<0){
	  perror("recvfrom()");
	  exit(1);
	}
	printf("Received Message: %s\n", recvmsg);

	ack ack1;
	ack1.seqNum = 0;
	ack ackresponse[1];

	ackresponse[0] = ack1;
	sendto_(sock,ackresponse, sizeof(ackresponse),0, (struct sockaddr *) &cliAddr,
		sizeof(cliAddr));

	
}

