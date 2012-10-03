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

    char msg[] = "send this";
    char recvmsg[100];
    ack recvAck;
    char * ackTest;
    ack* ackptrTest;

    recvAck.seqNum = 1;

    ackTest = makeackmsg(recvAck);

    printf("Ack Message: %s\n", ackTest);
    
    ackptrTest = makeackstruct(ackTest);

    printf("Make Ack test: %d\n",(*ackptrTest).seqNum);

    frame frame1;
    frame1.seqNum = 35;    
    frame1.lastFrame = 1;
    frame1.dataSize = sizeof(msg);
    strcpy(frame1.data, "Hello this is a test");

    char* test;
    frame* ftest;

    test = makedatapacket(frame1);
   
    printf("Packet test: %s\n",test);

    ftest = makedatastruct(test);
    

    printf("Frame test sequence num: %d\n",(*ftest).seqNum);
    printf("Last Frame: %d\n",(*ftest).lastFrame);
    printf("Data Size: %d\n", (*ftest).dataSize);
    printf("Data Sent: %s\n", (*ftest).data);

    free(test);
    free(ftest);
    
    


    /*for(i = 0; i < sizeof(test); i++){
      printf("%c",test[i]);
      
      }*/

   
    /*
    if((sendto_(sock,msg, strlen(msg),0, (struct sockaddr *) &remoteServAddr,
		sizeof(remoteServAddr)))< 0 ){
      printf("Error Sending\n");
      perror("sendto()");
      exit(1);
      }*/


    /*printf("Sent message.\n");

    int n = recvfrom(sock, &recvmsg, sizeof (recvmsg), 0,
		     (struct sockaddr *) &recAddr, &recLen);
    printf("Received Ack!\n");
    recvAck.seqNum = ((ack)recvmsg[0]).seqNum;
    printf("Received Frame %d",recvAck.seqNum);*/

}
