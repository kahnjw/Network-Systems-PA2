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
  int ack;
  //char filename[100];
  char data[512];
} frame;

typedef struct{

  int seqNum;
}ack;




int MoveForward(int* LB, int* RB, frame frameArray[], int arraySize) {
  int moveCount, i;
  
  if(*LB < *RB){
    
    for(i = *LB; i < *RB, frameArray[i].ack == 1; i++) {
      moveCount++;
      frameArray[i].ack = 0;
    }
    
  } else if (*LB > *RB) {
    
    for(i = *LB; i < arraySize, frameArray[i].ack == 1; i++) {
      moveCount++;
      frameArray[i].ack = 0;
    }
    
    for(i = 0; i < *RB, frameArray[i].ack == 1; i++) {
      moveCount++;
      frameArray[i].ack = 0;
    }

  }

  /* Set right bound and leftbount according to how far
   * everything moved 
   */
  *LB = ( *LB + moveCount ) % 8;
  *RB = ( *RB + moveCount ) % 8;
  
  return moveCount;
}

void setFrame(frame* f, int seqnum, int lframe, int dsize, int ack, char* data){

  if(sizeof(data) > 512){
    printf("setFrame data packet is too large.\n");
    exit(1);

  }

  (*f).seqNum = seqnum;
  (*f).lastFrame = lframe;
  (*f).dataSize = dsize;
  (*f).ack = ack;
  //strcpy((*f).filename,fname);
  strcpy((*f).data,data);

}

void SendNextFrames(int moveCount, frame frameArray[], int arraySize, int RB, FILE* fp) {
  int i, j, startSeq, finishSeq, isLastFrame;
  char data[512];

  startSeq = ( RB - moveCount ) % arraySize;
  finishSeq = ( startSeq + moveCount ) % arraySize;

  if(startSeq < finishSeq){
    
    for(i = startSeq; i < finishSeq; i++) {
      isLastFrame = readtoframe(data, fp);
      setFrame(&frameArray[i], i, isLastFrame, strlen(data), 0, data);
    }
    
  } else if (startSeq > finishSeq) {
    
    for (i = startSeq; i < arraySize; i++){
      isLastFrame = readtoframe(data, fp);
      setFrame(&frameArray[i], i, isLastFrame, strlen(data), 0, data);
    }

    for(i = 0; i < finishSeq; i++) {
      isLastFrame = readtoframe(data, fp);
      setFrame(&frameArray[i], i, isLastFrame, strlen(data), 0, data);
    }
    
  }
}

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

void setAck(ack* a, int seqnum){
  (*a).seqNum = seqnum;

}

void printFrame(frame f){

  printf("======================================\n");
  printf("Frame Sequence Number: %d\n",f.seqNum);
  printf("Last Frame?: %d\n",f.lastFrame);
  printf("Frame Size: %d\n",f.dataSize);
  //printf("File name: %s\n",f.filename);
  printf("Frame ack?: %d\n",f.ack);
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


void makedatapacket(char* creturn, frame f){
  
  //char* creturn = calloc(600,1);
  char sNum[4];
  char finish[4];
  char dSize[4];
  char ack[4];
  //char fname[100];
  char data[512];
  char delm[] = DELM;
    
  sprintf(sNum,"%d",f.seqNum);
  sprintf(finish,"%d",f.lastFrame);
  sprintf(dSize,"%d",f.dataSize);
  sprintf(ack,"%d",f.ack);
  //sprintf(fname,"%s",f.filename);
  sprintf(data,"%s",f.data);
  
  /* Concat together all fields and add delims */
  strcat(creturn,sNum);
  strcat(creturn,delm);
  strcat(creturn,finish);
  strcat(creturn,delm);                                                                  
  strcat(creturn,dSize);
  strcat(creturn,delm);
  strcat(creturn,ack);
  strcat(creturn,delm);
  strcat(creturn,data);
  //return creturn;
}

int readtoframe(char* c, FILE** fp){
  int result = 0;
  char temp[512];

  //fseek(*fp, 1, SEEK_CUR);

  printf("IN FILE AT: %ld\n",ftell(*fp));

  int readResult = fread(temp,1,512,*fp);
  
  strcpy(c,temp);
  
  printf("READ: \n%s\n\n\n",temp);
  
  
  if(readResult < 512){result = 1;}
  
  return result;

}


void makedatastruct(char* c, frame* sreturn){

  //frame* sreturn = calloc(600,1);
  char* p;
  char sNum[INTSIZE];
  char finish[INTSIZE];
  char dSize[INTSIZE];
  char ack[INTSIZE];
  //char fname[100];
  char data[512];
  
  p = strtok(c,DELM);
  strcpy(sNum,p);
  p = strtok(NULL,DELM);
  strcpy(finish,p);
  p = strtok(NULL,DELM);
  strcpy(dSize,p);
  p = strtok(NULL,DELM);
  strcpy(ack,p);
  p = strtok(NULL,DELM);
  strcpy(data,p);
  
  (*sreturn).seqNum = atoi(sNum);
  (*sreturn).lastFrame = atoi(finish);
  (*sreturn).dataSize = atoi(dSize);
  (*sreturn).ack = atoi(ack);
  //strcpy((*sreturn).filename,fname);
  strcpy((*sreturn).data,data);
  
  //return sreturn;
}
