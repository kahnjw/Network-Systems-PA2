#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>   /* memset() */
#include <sys/time.h> /* select() */
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define INTSIZE 4
#define DELM "/"

typedef struct{

  int seqNum;
  int lastFrame; //Last frame flag
  int dataSize;
  char filename[100];
  char data[512];
} frame;

typedef struct{

  int seqNum;
}ack;

int ballinselect(int sock, fd_set* readFDS, int tsec, int tusec){
  
  
  int iSockRet, iSelRet;
  struct timeval timeVal;

  timeVal.tv_sec = tsec;
  timeVal.tv_usec = tusec;
  
  FD_ZERO(readFDS);
  FD_SET(sock, readFDS);

  iSelRet = select(FD_SETSIZE, readFDS, NULL, NULL, &timeVal);

  return iSelRet;

}

void setFrame(frame* f, int seqnum, int lframe, int dsize, char* fname,char* data){

  if(sizeof(data) > 512){
    printf("setFrame data packet is too large.\n");
    exit(1);

  }

  (*f).seqNum = seqnum;
  (*f).lastFrame = lframe;
  (*f).dataSize = dsize;
  strcpy((*f).filename,fname);
  strcpy((*f).data,data);

}

void setAck(ack* a, int seqnum){
  (*a).seqNum = seqnum;

}

void printFrame(frame f){

  printf("======================================\n");
  printf("Frame Sequence Number: %d\n",f.seqNum);
  printf("Last Frame?: %d\n",f.lastFrame);
  printf("Frame Size: %d\n",f.dataSize);
  printf("File name: %s\n",f.filename);
  printf("Data: %s\n",f.data);
  printf("======================================\n");
}


char* makeackmsg(ack a){

  char *acknowledge = calloc(16,1);
  sprintf(acknowledge,"%d",a.seqNum);
  return acknowledge;
}

ack* makeackstruct(char a[]){
  ack* ackreturn = calloc(16,1);

  (*ackreturn).seqNum = atoi(a);

  return ackreturn;
}

ack* makeackfromframe(frame f){
  ack* ackreturn = calloc(16,1);

  (*ackreturn).seqNum = f.seqNum;

  return ackreturn;
}


char *makedatapacket(frame f){
  
  char* creturn = calloc(600,1);
  char sNum[4];
  char finish[4];
  char dSize[4];
  char fname[100];
  char data[512];
  char delm[] = DELM;
    
  sprintf(sNum,"%d",f.seqNum);
  sprintf(finish,"%d",f.lastFrame);
  sprintf(dSize,"%d",f.dataSize);
  sprintf(fname,"%s",f.filename);
  sprintf(data,"%s",f.data);
  
  /* Concat together all fields and add delims */
  strcat(creturn,sNum);
  strcat(creturn,delm);
  strcat(creturn,finish);
  strcat(creturn,delm);                                                                  
  strcat(creturn,dSize);
  strcat(creturn,delm);
  strcat(creturn,fname);
  strcat(creturn,delm);
  strcat(creturn,data);
  return creturn;
}


frame* makedatastruct(char* c){

  frame* sreturn = calloc(600,1);
  char* p;
  char sNum[INTSIZE];
  char finish[INTSIZE];
  char dSize[INTSIZE];
  char fname[100];
  char data[512];
    
  p = strtok(c,DELM);
  strcpy(sNum,p);
  p = strtok(NULL,DELM);
  strcpy(finish,p);
  p = strtok(NULL,DELM);
  strcpy(dSize,p);
  p = strtok(NULL,DELM);
  strcpy(fname,p);
  p = strtok(NULL,DELM);
  strcpy(data,p);

  (*sreturn).seqNum = atoi(sNum);
  (*sreturn).lastFrame = atoi(finish);
  (*sreturn).dataSize = atoi(dSize);
  strcpy((*sreturn).filename,fname);
  strcpy((*sreturn).data,data);

  return sreturn;
}
