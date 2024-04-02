#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {
  #ifdef DEBUG
    printf("Starting Fetch Stage\n");
    printf("PC value fdsfds: %lx\n", getPC());
  #endif
  // Get next instruction
  // byteType byte0 = *((byteType*) getPC());

  byteType byte0 = getByteFromMemory(getPC());
  byteType byte1 = 0;
  // printf("Almost there\n");  
  
  // printf("Got it!\n");
  printf("byte0: %x\n", byte0);
  
  byteType secondNibble = byte0 & 0x0f;
  byteType firstNibble = (byte0 >> 4) & 0xf;
  printf("firstNibble: %x\n", firstNibble);
  printf("secondNibble: %x\n", secondNibble);
  *icode = firstNibble;
  *ifun = secondNibble;
  
  switch(*icode) {
    case HALT: // Halt
      *valP = getPC() + 1;
      break;

    case NOP: // Nop
      *valP = getPC() + 1;
      break;

    case RRMOVQ: //rrmovq or cmovXX
      if(!*ifun) { // is rrmovq
        // printf("Got heare\n");
        // printf("attempt grab byte1\n");

        byteType byte1 = getByteFromMemory(getPC() + 1);
        printf("byte1: %x\n", byte1);
        *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
        *rB = byte1 & 0x0f; // Get the second nibble of byte1

        // add_byte1_to_rA_rB(*rA, *rB);
        // printf("Gotdsfa heare\n");

        

        *valP = getPC() + 2;

      } else { // is cmovXX

      }
      break;

    case IRMOVQ:
      
      // add_byte1_to_rA_rB(*rA, *rB);
      byte1 = getByteFromMemory(getPC() + 1);
      // *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
      *rB = byte1 & 0x0f; // Get the second nibble of byte1
      


      *valC = getWordFromMemory(getPC() + 2);
      *valP = getPC() + 10;
    
      break;

    case RMMOVQ:

      byte1 = getByteFromMemory(getPC() + 1);
      printf("byte1: %x\n", byte1);
      *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
      *rB = byte1 & 0x0f; // Get the second nibble of byte1

      *valC = getWordFromMemory(getPC() + 2);
      *valP = getPC() + 10;
    
      break;

    case MRMOVQ:

      byte1 = getByteFromMemory(getPC() + 1);
      printf("byte1: %x\n", byte1);
      *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
      *rB = byte1 & 0x0f; // Get the second nibble of byte1

      *valC = getWordFromMemory(getPC() + 2);
      *valP = getPC() + 10;

      break;

    case OPQ:

      byte1 = getByteFromMemory(getPC() + 1);
      printf("byte1: %x\n", byte1);
      *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
      *rB = byte1 & 0x0f; // Get the second nibble of byte1

      *valP = getPC() + 2;

      break;

    case JXX:

      *valC = getWordFromMemory(getPC() + 1);
      printf("starting jump, location: %lx\n", *valC);
      *valP = getPC() + 9;
    
      break;
    case CALL:
      *valC = getWordFromMemory(getPC() + 1);
      *valP = getPC() + 9;
      break;

    case RET:
    
      break;

    case PUSHQ:
    
      break;
    
    case POPQ:

      break;
            
  }

}

void add_byte1_to_rA_rB(int *rA, int *rB) {
  printf("attempt grab byte1");
  byteType byte1 = getByteFromMemory(getPC() + 1);
  *rA = (byte1 >> 4) & 0xf;  //Get the first nibble of byte1
  *rB = byte1 & 0x0f; // Get the second nibble of byte1
  printf("sucsess grab byte1");
}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
  #ifdef DEBUG
    printf("Starting Decode Stage\n");
    // printf("rcx: %lx\n", getRegister(0));
  #endif

  switch(icode) {
    case RRMOVQ:
      // printf("decode rA: %lx, rB: %lx\n", rA, rB);
      // printf("rA: %lx, rb: %lx\n", getRegister(rA), getRegister(rB));
      *valA = getRegister(rA);
      break;
    
    case RMMOVQ:
      *valA = getRegister(rA);
      *valB = getRegister(rB);
      break;
    
    case MRMOVQ:
      *valB = getRegister(rB);
      break;

    case OPQ:
      *valA = getRegister(rA);
      *valB = getRegister(rB);
      break;
    
    case CALL:
      *valB = getRegister(RSP);
      break;


  }

 
}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
  #ifdef DEBUG
    printf("Starting Exicute Stage\n");
    // printf("rcx: %lx\n", getRegister(0));
  #endif
  
  switch(icode) {
    case RRMOVQ:
      // printf("ValA in  exicute Stage: %lx\n", valA);
      *valE = 0 + valA;
      break;
    case IRMOVQ:
      *valE = 0 + valC;
      break;

    case RMMOVQ:
      *valE = valB + valC;
      break;
    
    case MRMOVQ:
      *valE = valB + valC;
      break;
    
    case OPQ:
    printf("going into opq with ifun: %x\n", ifun);
      switch(ifun) {
        case 0:
          *valE = valB + valA;
          break;
        case 1:
          *valE = valB - valA;
          break;
        case 2:
          *valE = valB & valA;
          break;
        case 3:
          *valE = valB ^ valA;
          break;
      }

      bool zf = 0;
      bool sf = 0;
      bool of = 0;

      if(*valE == 0) { zf = 1; }
      if(*valE < 0) { sf = 1; }
      // printf("vale: %lx, sf: %lx\n", *valE, sf);
      if(((valA ^ valB) & (valA ^ *valE)) >> 31) { of = 1;}

      setFlags(sf, zf, of);
      break;

    case JXX:
      printf("ifun: %x, Cond(ifun): %x\n", ifun, Cond(ifun));
      *Cnd = Cond(ifun);
      printf("Cnd %x\n", *Cnd);
      break;

    case CALL:
      *valE = valB - 8;
      break;

  }

}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {
  #ifdef DEBUG
    printf("Starting Memory Stage\n");
  #endif

  switch (icode) {
    case RMMOVQ:
      setWordInMemory(valE, valA);
      break;
    case MRMOVQ:
      *valM = getWordFromMemory(valE);
      break;
    case CALL:
      setWordInMemory(valE, valP);
      break;
  }
 
}

void writebackStage(int icode, int rA, int rB, wordType valE, wordType valM) {
  #ifdef DEBUG
    printf("Starting Writeback Stage\n");
  #endif


  switch(icode) {
    case RRMOVQ:
      printf("Wrote something in  Writeback Stage: %lx\n", valE);
      setRegister(rB, valE);
      break;
    case IRMOVQ:
      printf("Wrote something in  Writeback Stage: %lx\n", valE);
      setRegister(rB, valE);
      break;
    case MRMOVQ:
      setRegister(rA, valM);
      break;
    case OPQ:
      setRegister(rB, valE);
      break;
    case CALL:
      setRegister(RSP, valE);
      break;
  }
}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {
  #ifdef DEBUG
    printf("Starting PC Update Stage\n");
  #endif
  
  switch(icode) {
    case HALT: // Halt
      // setPC(valP);
      setStatus(STAT_HLT);
      break;

    case JXX:
      printf("Evaluating Jumping, Cnd: %x\n", Cnd);
      if(Cnd) { 
        printf("actualy jumping, about to set pc to: %lx\n", valC);
        setPC(valC); 
        return;
      }
      break;
    case CALL:
      setPC(valC);
      return;
      break;

    case 0x9:
    
      break;
      
    case 0xa:
    
      break;
  }
  
  setPC(valP);

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

  // Testing
  // setRegister(0, 0xace);
  printf("getRegester0: %lx\n", getRegister(0));

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
  #ifdef DEBUG
    printf("Loaded Memory\n");
  #endif

  applyStepMode(stepMode);
  while (getStatus() != STAT_HLT) {
    stepMachine(stepMode);
    applyStepMode(stepMode);
    // printf("get dsafffffffffffffffffffffffffffffffffffffffffffffffffffffcycle count: %d\n", getCycleCount());
    // if(getCycleCount() > 100) {setStatus(STAT_HLT);}
#ifdef DEBUG
    printMachineState();
    printf("\n");
#endif
  }
  printMachineState();
  return 0;
}