#include <stdio.h>
#include <unistd.h> /* close() */
#include <strings.h> /* memset() */
#include <stdlib.h>

#define INTSIZE 4
#define DELM "/"

typedef struct{

  int seqNum;
  int lastFrame; //Last frame flag
  int dataSize;
  char data[512];
} frame;

typedef struct{

  int seqNum;
}ack;



char makeackmsg(ack a){
  static char acknowledge[8];
  sprintf(acknowledge,"%d",a.seqNum);
  return *acknowledge;
}

ack makeackstruct(char a[]){
  static ack ackreturn;

  ackreturn.seqNum = atoi(a);

  return ackreturn;
}

char *makedatapacket(frame f){
  
  char* creturn = calloc(600,1);
  char sNum[4];
  char finish[4];
  char dSize[4];
  char data[512];
  char delm[] = DELM;
  int i, j;


  bzero(&sNum,sizeof(sNum));
  
  sprintf(sNum,"%d",f.seqNum);
  sprintf(finish,"%d",f.lastFrame);
  sprintf(dSize,"%d",f.dataSize);
  sprintf(data,"%s",f.data);
  
  /*
  strcat(sNum,finish);
  strcat(sNum,dSize);
  strcat(sNum,data);
  */

  strcat(creturn,sNum);
  strcat(creturn,delm);
  strcat(creturn,finish);
  strcat(creturn,delm);                                                                  
  strcat(creturn,dSize);
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
  char data[512];
  int i;
  int j = 0;
  
  p = strtok(c,DELM);
  strcpy(sNum,p);
  p = strtok(NULL,DELM);
  strcpy(finish,p);
  p = strtok(NULL,DELM);
  strcpy(dSize,p);
  p = strtok(NULL,DELM);
  strcpy(data,p);

  (*sreturn).seqNum = atoi(sNum);
  (*sreturn).lastFrame = atoi(finish);
  (*sreturn).dataSize = atoi(dSize);
  strcpy((*sreturn).data,data);

  return sreturn;
}
