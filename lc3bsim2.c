/*
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Full name of the first partner 
    Name 2: Full name of the second partner
    UTEID 1: UT EID of the first partner
    UTEID 2: UT EID of the second partner
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

#define Low8bits(x) ((x) & 0xFF)

/*sets the condition codes given a number */
void setNZP (int result){
  NEXT_LATCHES.N = 0;
  NEXT_LATCHES.Z = 0;
  NEXT_LATCHES.P = 0;
  /*Check to see if number is negative by masking most significant bit and shifting*/
  if(result > 0){ 
      NEXT_LATCHES.N = 1;
  }
  else if(result == 0){
    NEXT_LATCHES.Z = 1;
  }
  else {
    NEXT_LATCHES.P = 1;
  }
}

/*sign extends num given its length (numBits) */
int sEXT(int num, int numBits){
  int mask = num >> (numBits - 1);
  printf("This is the mask: %i \n", mask);
  if(mask == 1){
      num += (0xFFFFFFFF << numBits); 
    }
  return num;
}

void process_instruction(){
  #define MASK11TO9 0x0E00
  #define MASK5TO0  0x003F
  #define MASK8TO6  0x01C0
  /*  function: process_instruction
   *  
   *  /*  Process one instruction at a time  */
   /*1) Fetch the current instruction */
   int mach_code = (MEMORY[CURRENT_LATCHES.PC >> 1][1] << 8) + MEMORY[CURRENT_LATCHES.PC >> 1][0];

   printf("Current Machine code: 0x%.4x\n", mach_code);
   /*2)Decode */
   int opcode = mach_code >> 12;
   int immediate = 0;
   int offset = 0;
   int DR, mask, SR1, SR2, result, baseR;
   switch (opcode){
      case 0:
        printf("Opcode = BR");
        break;

       /*ADD*/ 
      case 1:
         /*test bit 5 */
        DR = (mach_code & 0x0E00) >> 9;
        SR1 = (mach_code & 0x01C0) >> 6;
        if ((mach_code & 0x20) >> 4){
           immediate = sEXT((mach_code & 0x001F), 5);
           result = CURRENT_LATCHES.REGS[SR1] + immediate; 
           setNZP(result);
           printf("Opcode = ADD, immediate.......DR: %i, SR1: %i, imm5: %i, result: %i \n", DR, SR1, immediate, result);
           NEXT_LATCHES.REGS[DR] = Low16bits(result);

           printf("Register %i \n", NEXT_LATCHES.REGS[DR]);
        }
        else{
          SR2 = mach_code & 0x0007;
          /*NEXT_LATCHES.REGS[DR] = Low16bits(CURRENT_LATCHES.REGS[SR1] + CURRENT_LATCHES.REGS[SR2]); */
          result = CURRENT_LATCHES.REGS[SR1] + CURRENT_LATCHES.REGS[SR2]; 
          setNZP (result);
          NEXT_LATCHES.REGS[DR] = Low16bits(result);
          printf("Opcode = ADD, register.......DR: %i, SR1: %i, SR2: %i, result: %i \n", DR, SR1, SR2, result);
        }

        
        break;
       /* Andrew is doing LDB, STB, LEA*/
      /*LDB*/
      case 2:
        SR1 = (mach_code & MASK11TO9) >> 9;
        baseR = (mach_code & MASK8TO6) >> 6;
        offset = (mach_code & MASK5TO0);
        result = sEXT(MEMORY[CURRENT_LATCHES.REGS[SR1] + offset][0], 8); 
        printf ("address: %i\n", CURRENT_LATCHES.REGS[SR1] + offset);
        NEXT_LATCHES.REGS[DR] = Low16bits(result);
        setNZP(result);
        printf("Opcode = LDB.......DR: %i, offset: %i, SR1: %i, result: %i \n", DR, offset, SR1, result);
        break;

      /*STB*/
      case 3:
        SR1 = (mach_code & MASK11TO9) >> 9;
        baseR = (mach_code & MASK8TO6) >> 6;
        offset = sEXT((mach_code & MASK5TO0),6);
        result = CURRENT_LATCHES.REGS[baseR] + offset;
        printf ("address: %i\n", result);
         /*store one byte into LSB or MSB depending on mem addr is odd or even*/ 
         /*Shift left 1 since mem is byte addressable */
        if (result & 0x1 == 1){
          MEMORY[result >> 1][1] = Low8bits(CURRENT_LATCHES.REGS[SR1]); 
        }
        else{
          MEMORY[result >> 1][0] = Low8bits(CURRENT_LATCHES.REGS[SR1]); 
        }
        printf("Opcode = STB.......SR: %i, offset: %i, BaseReg: %i, address: %i \n", SR1, offset, baseR, result);      
        break;

      case 4:
        break;

      case 5:
      /*AND Instruction*/
        DR = (mach_code & 0x0E00) >> 9;
        SR1 = (mach_code & 0x01C0) >> 6;
        if ((mach_code & 0x20) >> 4){
           immediate = sEXT((mach_code & 0x001F), 5);
           result = CURRENT_LATCHES.REGS[SR1] & immediate; 
           printf("Opcode = AND, immediate.......DR: %i, SR1: %i, imm5: %i, result: %i \n", DR, SR1, immediate, result);
           NEXT_LATCHES.REGS[DR] = Low16bits(result);
           printf("Register %i \n", NEXT_LATCHES.REGS[DR]);
        }
        else{
          SR2 = mach_code & 0x0007;
          /*NEXT_LATCHES.REGS[DR] = Low16bits(CURRENT_LATCHES.REGS[SR1] + CURRENT_LATCHES.REGS[SR2]); */
          result = CURRENT_LATCHES.REGS[SR1] & CURRENT_LATCHES.REGS[SR2]; 
          NEXT_LATCHES.REGS[DR] = Low16bits(result);
          printf("Opcode = AND, register.......DR: %i, SR1: %i, SR2: %i, result: %i \n", DR, SR1, SR2, result);
        }
        break;

      /*LDW*/ 
      case 6: 
      break;

      /*STW*/
      case 7: 
      SR1 = (mach_code & MASK11TO9) >> 9;
      baseR = (mach_code & MASK8TO6) >> 6;
      offset = sEXT((mach_code & MASK5TO0),6) << 1;
      result = CURRENT_LATCHES.REGS[baseR] + offset;
      printf ("address: %i\n", result);
       /*store word into memory one byte at a time. Shift left 1 since mem is byte addressable */
      MEMORY[result >> 1][0] = Low8bits(CURRENT_LATCHES.REGS[SR1]); 
      MEMORY[result >> 1][1] = Low8bits(CURRENT_LATCHES.REGS[SR1] >> 16); 
      printf("Opcode = STW.......SR: %i, offset: %i, BaseReg: %i, address: %i \n", SR1, offset, baseR, result);
      break;

      /*XOR*/
      case 9: 
      DR = (mach_code & 0x0E00) >> 9;
      SR1 = (mach_code & 0x01C0) >> 6;
        if  ((mach_code & 0x20) >> 4){
           immediate = sEXT((mach_code & 0x001F), 5);
           printf("this is the immediate: %i\n", immediate);
           result = CURRENT_LATCHES.REGS[SR1] ^ immediate; 
           printf("Opcode = XOR, immediate.......DR: %i, SR1: %i, imm5: %i, result: %i \n", DR, SR1, immediate, result);
           NEXT_LATCHES.REGS[DR] = Low16bits(result);
           printf("Register %i \n", NEXT_LATCHES.REGS[DR]);
        }
        else{
          SR2 = mach_code & 0x0007;
          /*NEXT_LATCHES.REGS[DR] = Low16bits(CURRENT_LATCHES.REGS[SR1] + CURRENT_LATCHES.REGS[SR2]); */
          result = CURRENT_LATCHES.REGS[SR1] ^ CURRENT_LATCHES.REGS[SR2]; 
          NEXT_LATCHES.REGS[DR] = Low16bits(result);
          printf("Opcode = XOR, register.......DR: %i, SR1: %i, SR2: %i, result: %i \n", DR, SR1, SR2, result);
        }  
      break;

      /*JMP*/
      case 12: 
      break;

      /*SHF*/
      case 13: 
        mask = (mach_code & 0x0030);
        immediate = (mach_code & 0x000F);
        DR = (mach_code & 0x0E00) >> 9;
        SR1 = (mach_code & 0x01C0) >> 6;
        result = sEXT(CURRENT_LATCHES.REGS[SR1], 16);
        /*LSHF*/
        if(mask == 0x00){
            NEXT_LATCHES.REGS[DR] = Low16bits(result << immediate);
            printf("Opcode = LSHF, immediate.......DR: %i, SR1: %i, imm5: %im mask: %i\n", DR, SR1, immediate, mask>>4);
        }
        /*RSHFL, 0s are shifted in the vacated positions*/
        else if(mask == 0x10){
          printf("Opcode = RSHFL, immediate.......DR: %i, SR1: %i, imm5: %im mask: %i\n", DR, SR1, immediate, mask>>4);
          while(immediate > 0){
            result = (result >> 1) & 0x00007FFF;
            immediate--;
          }
          NEXT_LATCHES.REGS[DR] = Low16bits(result);
        }
        /*RSHFA, sign is shifted*/
        else{
          NEXT_LATCHES.REGS[DR] = Low16bits(result >> immediate);
          printf("Opcode = RSHFA, immediate.......DR: %i, SR1: %i, imm5: %i, mask: %i\n", DR, SR1, immediate, mask>>4);
        }

      break;

      /*LEA*/
      case 14: 
        DR = (mach_code & MASK11TO9) >> 9;
        result = (sEXT((mach_code & 0x01FF), 16) << 1) + CURRENT_LATCHES.PC + 2;
        NEXT_LATCHES.REGS[DR] = result;
        printf("Opcode = LEA.......DR: %i, address: 0x%.4x\n", DR, result);
        setNZP(DR);
      break;

      /*TRAP*/
      case 15: 
      break;

      default:
        printf("Opcode = TRAP");
        break;
   }
   
   NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2; 
 }
