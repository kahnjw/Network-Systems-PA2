#include <stdio.h>
#include <unistd.h> /* close() */
#include <strings.h> /* memset() */
#include <stdlib.h>

#define INTSIZE 4


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
  int i, j;


  bzero(&sNum,sizeof(sNum));

  /*
  for(i = 0, j; i < INTSIZE; i++, j++){
    creturn[j] = (*char)f.seqNum[i];
  }

  // sNum[0] = c[0];

  for(i = 0, j; i < INTSIZE; i++, j++) {
    creturn[j] = f.[j];
  }
  
  for(i = 0, j; i < 512; i++, j++) {
    data[j] = f.lastFrame[i];
  }
  */
  /*sprintf(seqptr,"%d",f.seqNum);
  
  sprintf(lFrameptr,"%d",f.lastFrame);
  
  //strcpy(lFrameptr,f.lastFrame);

  sprintf(dataptr,"%s",f.data);*/
  
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
  strcat(creturn,finish);                                                                  
  strcat(creturn,dSize);                                                                  
  strcat(creturn,data);
  return creturn;
}


frame* makedatastruct(char* c){

  frame* sreturn = calloc(600,1);
  char sNum[INTSIZE];
  char finish[INTSIZE];
  char dSize[INTSIZE];
  char data[512];
  int i;
  int j = 0;
  
  for(i = 0, j; i < INTSIZE; i++, j++){
    sNum[i] = c[j];
  }

  // sNum[0] = c[0];

  for(i = 0, j; i < INTSIZE; i++, j++) {
    finish[i] = c[j];
  }
  
  for(i = 0, j; i < 512; i++, j++) {
    data[i] = c[j];
  }

  
  // strncpy(sNum,c,4);
  // strncpy(finish,c,4);
  // strncpy(dSize,c,4);

  
  printf("sNum: %d\n",atoi(sNum));

  (*sreturn).seqNum = (int)sNum;

  return sreturn;
}
