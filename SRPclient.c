/* client.c */
/* This is a sample UDP client/sender.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <sys/time.h> /* select() */
#include <signal.h>
#include <unistd.h>
#include "sendto_.h"

#include "types.h"


#define REMOTE_SERVER_PORT 50007
#define SWS 4
#define MAXSEQNUM 6

struct sockaddr_in cliAddr, remoteServAddr, recAddr;

void creatPacket(int seqnum, char filepath[], frame f); 

int main(int argc, char *argv[]) {

  int sock;
  unsigned int recLen;
  int i = 0;
  frame framearray[8];
  char* sendFrame;

    //check command line args.
    if(argc<6) {
	printf("usage : %s <server> <error rate> <random seed> <send_file> <send_log> \n", argv[0]);
	exit(1);
    }

    printf("error rate : %f\n",atof(argv[2]));

    

    /* Note: you must initialize the network library first before calling
       sendto_().  The arguments are the <errorrate> and <random seed> */
    init_net_lib(atof(argv[2]), atoi(argv[3]));

    /* get server IP address (input must be IP address, not DNS name) */

    printf("%s: sending data to '%s' \n", argv[0], argv[1]);

    remoteServAddr.sin_family = AF_INET;
    remoteServAddr.sin_port = htons(REMOTE_SERVER_PORT);
    inet_pton(AF_INET, argv[1], &remoteServAddr.sin_addr);

    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    /* socket creation */
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      printf("Unable to connect socket!\n");
      exit(1);
    }

    char msg[] = "send this message bitch!!!";
    char recvmsg[100];
    printf("size of msg: %d\n",sizeof(msg));
    setFrame(&framearray[0],0,0,sizeof(msg),msg);
    
    printFrame(framearray[0]);
    
    sendFrame = makedatapacket(framearray[0]);

 
    if((sendto_(sock,sendFrame, strlen(sendFrame),0, (struct sockaddr *) &remoteServAddr,
		sizeof(remoteServAddr)))< 0 ){
      printf("Error Sending\n");
      perror("sendto()");
      exit(1);
    }

    free(sendFrame);
   
    /*
    if(recvfrom(sock, &recvmsg, sizeof (recvmsg), 0,(struct sockaddr *) &recAddr, &recLen) < 0){
      perror("recvfrom()");
      exit(1);
    }

    printf("Received Ack!\n");
    */

}
