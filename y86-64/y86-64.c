#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {

  switch(*icode) {
    case 0x0:
      valP = (wordType*) getPC() + 1;
      break;

    case 0x1:
      valP = (wordType*) getPC() + 1;
      break;

    case 0x2:
      
      break;

    case 0x3:
    
      break;

    case 0x4:
    
      break;

    case 0x5:
    
      break;

    case 0x6:
    
      break;

    case 0x7:
    
      break;
    case 0x8:
    
      break;

    case 0x9:
    
      break;

    case 0xa:
    
      break;
            
  }

}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
 
}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
  
}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {
 
}

void writebackStage(int icode, int rA, int rB, wordType valE, wordType valM) {
 
}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {
  
  switch(icode) {
    case 0x0:
      setPC(valP);
      setStatus(STAT_HLT);
      break;

    case 0x1:
      setPC(valP);
      break;

    case 0x2:
    
      break;

    case 0x3:
    
      break;

    case 0x4:
    
      break;

    case 0x5:
    
      break;

    case 0x6:
    
      break;

    case 0x7:
    
      break;
    case 0x8:
    
      break;

    case 0x9:
    
      break;
      
    case 0xa:
    
      break;
  }

}

void stepMachine(int stepMode) {
  /* FETCH STAGE */
  int icode = 0, ifun = 0;
  int rA = 0, rB = 0;
  wordType valC = 0;
  wordType valP = 0;
 
  /* DECODE STAGE */
  wordType valA = 0;
  wordType valB = 0;

  /* EXECUTE STAGE */
  wordType valE = 0;
  bool Cnd = 0;

  /* MEMORY STAGE */
  wordType valM = 0;

  fetchStage(&icode, &ifun, &rA, &rB, &valC, &valP);
  applyStageStepMode(stepMode, "Fetch", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  decodeStage(icode, rA, rB, &valA, &valB);
  applyStageStepMode(stepMode, "Decode", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  executeStage(icode, ifun, valA, valB, valC, &valE, &Cnd);
  applyStageStepMode(stepMode, "Execute", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  memoryStage(icode, valA, valP, valE, &valM);
  applyStageStepMode(stepMode, "Memory", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  writebackStage(icode, rA, rB, valE, valM);
  applyStageStepMode(stepMode, "Writeback", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  pcUpdateStage(icode, valC, valP, Cnd, valM);
  applyStageStepMode(stepMode, "PC update", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  incrementCycleCounter();
}

/** 
 * main
 * */
int main(int argc, char **argv) {
  int stepMode = 0;
  FILE *input = parseCommandLine(argc, argv, &stepMode);

  #ifdef DEBUG
    printf("Starting Program\n");
  #endif
  printf("Starting Program\n");

  initializeMemory(MAX_MEM_SIZE);
  initializeRegisters();
  loadMemory(input);

  applyStepMode(stepMode);
  while (getStatus() != STAT_HLT) {
    stepMachine(stepMode);
    applyStepMode(stepMode);
#ifdef DEBUG
    printMachineState();
    printf("\n");
#endif
  }
  printMachineState();
  return 0;
}