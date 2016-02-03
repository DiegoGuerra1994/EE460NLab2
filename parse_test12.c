	/*
	Name 1: Andrew Wong
	Name 2: Diego Guerra
	UTEID 1: aw27772
	UTEID 2: dag3222
	*/


	#include <stdio.h>	/* standard input/output library */
	#include <stdlib.h>	/* Standard C Library */
	#include <string.h>	/* String operations library */
	#include <ctype.h>	/* Library for useful character operations */
	#include <limits.h>	/* Library for definitions of common variable type characteristics */

	/*char* lPtr;*/

	#define MAX_LINE_LENGTH 	255
	#define symbolTableLength 	500
	#define OFFSET9				9
	#define OFFSET11			11
	#define MASK_OFFS9 			0x000001FF
	#define MASK_OFFS11 		0x000007FF
	#define MASK_NBIT			0x0800
	#define MASK_ZBIT			0x0400
	#define MASK_PBIT			0x0200

	enum{
	   DONE, OK, EMPTY_LINE
	};

	enum opcode{
		BR, ADD, LDB, STB, JSR, AND, LDW, STW, RTI, XOR, NU, NU2, JMP, SHF, LEA, TRAP
	};

	typedef struct table{
		int addr;
		char name[20];
	} sym_table;

	sym_table symbol_table[symbolTableLength]; /*make symbol table array*/

	int label; /*true if parser found label*/

	int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4)
	{	
	   label = 0;
	   char * lRet ,*lPtr;
	   int i;
	   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
	   for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
	   
           /* convert entire line to lowercase */
	   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	   /* ignore the comments */
	   lPtr = pLine; 

	   while( *lPtr != ';' && *lPtr != '\0' && *lPtr != '\n' )
		lPtr++;

	    *lPtr = '\0';
	   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
		return( EMPTY_LINE );

	   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ){ /* found a label */
		*pLabel = lPtr;
		label = 1;
		/*printf("label found: %s\n", lPtr); */
		if( !( lPtr = strtok( NULL, "\t\n ," )))  return( OK );
	   } /*null pointer may be specified, in which case the function continues scanning 
	     where a previous successful call to the function ended.*/
	   
           *pOpcode = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ))) return(OK ); 
	   
           *pArg1 = lPtr;
	   
           if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg2 = lPtr;
	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg3 = lPtr;

	   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

	   *pArg4 = lPtr;

	   return( OK );
	}
	 /*returns -1 if invalid opcode*/
	int isOpcode(char* inStr){
		if (strcmp(inStr,"add") == 0){
			return 0;
		}
		else if (strcmp(inStr,"and") == 0){
			return 0;
		}
		else if (strcmp(inStr,"br") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brn") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brp") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brnp") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brz") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brnz") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brzp") == 0){
			return 0;
		}
		else if (strcmp(inStr,"brnzp") == 0){
			return 0;
		}
		else if (strcmp(inStr,"halt") == 0){
			return 0;
		}
		else if (strcmp(inStr,"jmp") == 0){
			return 0;
		}
		else if (strcmp(inStr,"jsr") == 0){
			return 0;
		}
		else if (strcmp(inStr,"jsrr") == 0){
			return 0;
		}
		else if (strcmp(inStr,"ldb") == 0){
			return 0;
		}
		else if (strcmp(inStr,"ldw") == 0){
			return 0;
		}
		else if (strcmp(inStr,"lea") == 0){
			return 0;
		}
		else if (strcmp(inStr,"nop") == 0){
			return 0;
		}
		else if (strcmp(inStr,"not") == 0){
			return 0;
		}
		else if (strcmp(inStr,"ret") == 0){
			return 0;
		}
		else if (strcmp(inStr,"lshf") == 0){
			return 0;
		}
		else if (strcmp(inStr,"rshfl") == 0){
			return 0;
		}
		else if (strcmp(inStr,"rshfa") == 0){
			return 0;
		}
		else if (strcmp(inStr,"rti") == 0){
			return 0;
		}
		else if (strcmp(inStr,"stb") == 0){
			return 0;
		}
		else if (strcmp(inStr,"stw") == 0){
			return 0;
		}
		else if (strcmp(inStr,"trap") == 0){
			return 0;
		}
		else if (strcmp(inStr,"xor") == 0){
			return 0;
		}
		else{
			return -1;
		}

	}

/*offsBIts is the number of bits allocated for the offset
  pointer is the current address of the parser
  symbol is the name of the label*/

int returnOffset(char* symbol, int pointer, int offsBits){
	int x;
	for(x=0; x < symbolTableLength; x++){
		/*printf("name: %s	",symbol_table[x].name);
		printf("addr: %i\n",symbol_table[x].addr); */
		if(strcmp(symbol_table[x].name, symbol) == 0){
				int offset =  symbol_table[x].addr - (pointer + 1);
				/*printf ("2 to power of 11 = %i   ",  1 << 11); */
				 /*check if offset is within max offset value*/
				offsBits -= 1;
				int offsetAmount = (1 << offsBits); /*This will be 256 if 9 bits, 1024 if 11 bits*/
				if (offset >= offsetAmount){ 
					exit (4);
				}
				else if(offset < (offsetAmount*-1)){
					exit(4);
				}
				else{
					return offset;
				}
		}
	}
	/*undefined label - If symbol doesn't exist. throw error*/
	exit(1);
	return -1;

}

int toNum( char * pStr ){
   char * t_ptr;
   char * orig_pStr;
   int t_length,k;
   int lNum, lNeg = 0;
   long int lNumLong;

   orig_pStr = pStr;
   if( *pStr == '#' )			/* decimal */
   { 
     pStr++;
     if( *pStr == '-' )			/* dec is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isdigit(*t_ptr))
       {
	 printf("Error: invalid decimal operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNum = atoi(pStr);
     if (lNeg)
       lNum = -lNum;
 
     return lNum;
   }
   else if( *pStr == 'x' )	/* hex */
   {
     pStr++;
     if( *pStr == '-' )			/* hex is negative */
     {
       lNeg = 1;
       pStr++;
     }
     t_ptr = pStr;
     t_length = strlen(t_ptr);
     for(k=0;k < t_length;k++)
     {
       if (!isxdigit(*t_ptr))
       {
	 printf("Error: invalid hex operand, %s\n",orig_pStr);
	 exit(4);
       }
       t_ptr++;
     }
     lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
     lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
     if( lNeg )
       lNum = -lNum;
     return lNum;
   }
   else
   {
	printf( "Error: invalid operand, %s\n", orig_pStr);
	exit(4); 
	/*This has been changed from error code 3 to error code 4, see clarification 12 */
   }
}

/*
numRegs is number of arguments that are registers 
numArgs is number of arguments after opcode */

void errorcheck4(char* arg1, char* arg2, char* arg3, int numRegs, int numArgs){
	int numbArgs = 0;
	int x=0;
	/*count number of operands, exit if incorrect */
	if (arg1[0] != '\0'){
		numbArgs++;
	}
	if (arg2[0] != '\0'){
		numbArgs++;
	}
	if (arg3[0] != '\0'){
		numbArgs++;
	}
	/*printf("number of arguments: %i\n", numbArgs); */
	if (numbArgs != numArgs){
		exit(4);
	}

	/*check if registers are are r0 to r7*/
	/*also check if given the correct number of registers*/	
	if(!(strcmp(arg1, "r0") == 0 || strcmp(arg1, "r1") == 0  || strcmp(arg1, "r2") == 0  || strcmp(arg1, "r3") == 0  
		|| strcmp(arg1, "r4") == 0  || strcmp(arg1, "r5") == 0   || strcmp(arg1, "r6") == 0  || strcmp(arg1, "r7") == 0) && x <  numRegs){
		exit(4);
	}
	x++;
	if(!(strcmp(arg2, "r0") == 0 || strcmp(arg2, "r1") == 0  || strcmp(arg2, "r2") == 0  || strcmp(arg2, "r3") == 0  
		|| strcmp(arg2, "r4") == 0  || strcmp(arg2, "r5") == 0   || strcmp(arg2, "r6") == 0  || strcmp(arg2, "r7") == 0) && x <  numRegs){
		exit(4);
	}
	x++;
	if(!(strcmp(arg3, "r0") == 0 || strcmp(arg3, "r1") == 0  || strcmp(arg3, "r2") == 0  || strcmp(arg3, "r3") == 0 
	   || strcmp(arg3, "r4") == 0  || strcmp(arg3, "r5") == 0   || strcmp(arg3, "r6") == 0  || strcmp(arg3, "r7") == 0) && x < numRegs){
		exit(4);
	}
}
	/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */
int main (int argc, char* argv[]){

	   char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
	   int lRet;
	   int k = 0;
	   int ctr = 0;
	   int addrCtr; 		/*used in 2nd pass of program*/
	   int mach_code = 0;
	   int arg1_num = 0;
	   int orig = 0; 		/*start addr of program*/
	   int offs = 0; 		/*offset from current addr to label*/
	   int endDefined = 0;  /*true if input file has .END */
	   int numArgs = 0;

	   FILE * pOutfile;
	   pOutfile = fopen(argv[2], "w" );

	   FILE * lInfile;
	   lInfile = fopen(argv[1], "r");	/* open the input file */

        /*=================================1st pass: generate symbol table=================================*/
	   /*rewind(lInfile);*/
	   do{	
	 		lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
			/*printf("1st pass");
			printf("	lRet: %i\n", lRet); */

	 		  /*return error if multiple .orig*/
			if (strcmp(lOpcode, ".orig") == 0 && ctr != 0){
				exit(4);
			}
			
			 /*find start address of program*/
			if (strcmp(lOpcode, ".orig") == 0){
        	    orig = toNum(lArg1);
				/*Error condition to see if the starting address is odd*/
        	    if(orig%2!=0){
        	        exit(3);
        	    }
        	}

        	else if (strcmp(lOpcode, ".end") == 0){
        	         	endDefined = 1;
        	}

			/*printf("start addr: %i\n",orig); */
			/*check to see if the label is a valid label before putting it into the symbol table*/
			if (label){
				int i = 0;
				while (lLabel[i] != 0){
					/*Checks to see if it is no alphanumberic, not IN, not OUT, not GETC, not PUTS, doesnt start with x and doesnt start with a number*/
					if (!isalnum(lLabel[i]) || strcmp(lLabel, "in") == 0 || strcmp(lLabel, "out") == 0 || strcmp(lLabel, "getc") == 0 || strcmp(lLabel, "puts") == 0 || lLabel[0] == 'x'|| isdigit(lLabel[0]) != 0){
						exit(4);
					} 
					else if(strlen(lLabel) > 20){
						exit(4);
					}
					/*Label cannot be the same name as a register*/
					else if(strcmp(lLabel, "r0") == 0 || strcmp(lLabel, "r1") == 0 || strcmp(lLabel, "r2") == 0 || strcmp(lLabel, "r3") == 0 || strcmp(lLabel, "r4") == 0 || strcmp(lLabel, "r5") == 0 || strcmp(lLabel, "r6") == 0 || strcmp(lLabel, "r7") == 0){
						exit(4);
					}
					i++;
				}

				int x =0;
				/*If symbol already exists in the symbol table, throw error*/
				do{
					if(strcmp(lLabel, symbol_table[x].name) == 0){
						exit(4);
					}
					x++;
				}while(x<k);

				symbol_table[k].addr = orig + ctr;
				strcpy(symbol_table[k].name, lLabel);
				/*printf("label (T/F): %i 1st pass address: %i	label: %s\n", label, symbol_table[k].addr, symbol_table[k].name); */
				label = 0;
				k++; 
			}
			if (lRet != EMPTY_LINE){
				ctr++;
			}
	    } while( lRet != DONE );

	   	/*printf ("=========> End defined: %i", endDefined);*/
	      /*If 1st pass did not find .END, then throw an error*/
	   	if(endDefined == 0){
	   		error(4);
	   	}

		 /*print out symbol table*/
		/*int x;
		for(x=0; x < 50; x++){
                printf("name: %s	addr: 0x%.4X\n",symbol_table[x].name,symbol_table[x].addr);                         
        } */

	   /*===========================2nd pass: generate machine code================================*/
       addrCtr = orig;
	   rewind(lInfile);
	   do
	   {	
	   	numArgs = 0;
	   	mach_code = 0;
	   	lRet = readAndParse( lInfile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
		printf("label:%s 	opcode:%s 	arg1:%s 	arg2:%s 	arg3:%s\n", lLabel, lOpcode, lArg1, lArg2, lArg3); 

		 /*check for 4 or more arguments */
		if (lArg4[0] != '\0'){
			exit(4);
		}
		/*generate machine code (hex) given the parsed opcodes*/
		if( lRet != DONE && lRet != EMPTY_LINE ){

				 /* return error if .orig is not the first instruction*/
				if (addrCtr == orig){
					if (strcmp(lOpcode, ".orig") == 0){
						/*check if .orig is too large*/
						if (toNum(lArg1) < (1 << 16)){
							fprintf( pOutfile, "0x%.4X\n", orig);
						}
						else{
							exit(4);
						}			
					}
					else{
						exit(4);
					}				
				}

			
				else if (lOpcode[0] == 'b' && lOpcode[1] == 'r'){
					/*BRnzp can only have labels*/
					errorcheck4(lArg1, lArg2, lArg3, 0, 1);
					if(lArg2[0] == 'x' || lArg2[0] == '#'){
						exit(4);
					}
					/*set n, z, p bits based on br suffix*/
					if(strcmp(lOpcode, "br") == 0 || strcmp(lOpcode, "brnzp") == 0){
						/*no n, z, p bits to set */
						mach_code |= (MASK_NBIT + MASK_ZBIT + MASK_PBIT);
					}
					else if(strcmp(lOpcode, "brn") == 0){
						mach_code |= MASK_NBIT;
					}
					else if(strcmp(lOpcode, "brz") == 0){
						mach_code |= MASK_ZBIT;
					}
					else if(strcmp(lOpcode, "brp") == 0){
						mach_code |= MASK_PBIT;
					}
					else if(strcmp(lOpcode, "brnz") == 0){
						mach_code |= (MASK_NBIT + MASK_ZBIT);
					}
					else if(strcmp(lOpcode, "brnp") == 0){
						mach_code |= (MASK_NBIT + MASK_PBIT);
					}
					else { 
						mach_code |= (MASK_ZBIT + MASK_PBIT); /*BRzp*/
					}
			
					mach_code += (returnOffset(lArg1, addrCtr, OFFSET9) & MASK_OFFS9); 
					/*printf("addrCtr: 0x%.4X 	mach Code: 0x%.4X\n",addrCtr, mach_code); */
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}

				else if (strcmp(lOpcode, "add") == 0){
					mach_code = (ADD << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					mach_code &= 0xFFC0; /*clearing the last 6 bits*/
					/*see if the number being added is a constant number*/
					if( (lArg3[0] == 'x') || (lArg3[0] == '#')){
						errorcheck4(lArg1, lArg2, lArg3, 2, 3);
						/*Error condition to see if number being added is in range*/
						int imm5 = toNum(lArg3);
						if(imm5 > 15 || imm5 < -16){
							exit(3);
						}
						mach_code |= 0x20; /*Changing the 5th bit to 1 since we are adding a constant*/
						mach_code |= (imm5 & 0x0000001F);
					}
					/*Argument 3 is a register*/
					else{
						errorcheck4(lArg1, lArg2, lArg3, 3, 3);
						mach_code += (lArg3[1] - 0x30);
					}
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}


				else if (strcmp(lOpcode, "ldb") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					 /*extract reg number from arguments, then shift to correct place*/
					mach_code = (LDB << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6); 

					 /* put constant in bits[5:0]*/
					mach_code &= 0xFFC0;

					int boffset6 = toNum(lArg3);
					if(boffset6 < -32 || boffset6 > 31){
						exit(3);
					}

					mach_code |= (boffset6 & 0x0000002F);

					fprintf( pOutfile, "0x%.4X\n", mach_code);
			
				}				
				else if (strcmp(lOpcode, "stb") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					/*extract reg number from arguments, then shift to correct place*/
                    mach_code = (STB << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);

                    /* put constant in bits[5:0]*/
                    mach_code &= 0xFFC0;

                    int boffset6 = toNum(lArg3);
					if(boffset6 < -32 || boffset6 > 31){
						exit(3);
					}

					mach_code |= (boffset6 & 0x0000002F);
                    
					fprintf( pOutfile, "0x%.4X\n", mach_code);

				}

				else if (strcmp(lOpcode, "jsr") == 0) {
					errorcheck4(lArg1, lArg2, lArg3, 0, 1);
					/*JSR can only have labels*/
					if(lArg2[0] == 'x' || lArg2[0] == '#'){
						exit(4);
					}

					mach_code = (JSR << 12) + (1<<11) + (returnOffset(lArg1, addrCtr, OFFSET11) & MASK_OFFS11);
					fprintf( pOutfile, "0x%.4X\n", mach_code);		 
				}

				else if (strcmp(lOpcode, "jsrr") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 1, 1);
					mach_code = (JSR << 12) + ((lArg1[1] - 0x30) << 6);
					fprintf( pOutfile, "0x%.4X\n", mach_code);		
				}


				else if (strcmp(lOpcode, "and") == 0){
					mach_code = (AND << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					mach_code &= 0xFFC0; /*clearing the last 6 bits*/
					/*see if the number being added is a constant number*/
					if( (lArg3[0] == 'x') || (lArg3[0] == '#')){
						errorcheck4(lArg1, lArg2, lArg3, 2, 3);
						/*Error condition to see if number being anded is in range*/
						int imm5 = toNum(lArg3);
						if(imm5 > 15 || imm5 < -16){
							exit(3);
						}
						mach_code |= 0x20; /*Changing the 5th bit to 1 since we are adding a constant*/
						mach_code |= (imm5 & 0x0000001F);
					}
					/*Argument 3 is a register*/
					else{
						errorcheck4(lArg1, lArg2, lArg3, 3, 3);
						mach_code += (lArg3[1] - 0x30);
					}
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}

				else if (strcmp(lOpcode, "ldw") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);


					mach_code = (LDW << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					
					int offset6 = toNum(lArg3);
					if(offset6 < -32 || offset6 > 31){
						exit(3);
					}

					mach_code |= (offset6 & 0x0000002F);
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "stw") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					int offset6 = toNum(lArg3);
					if(offset6 < -32 || offset6 > 31){
						exit(3);
					}
					mach_code = (STW << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					mach_code &= 0xFFC0;
					mach_code |= (offset6 & 0x0000002F);
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "rti") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 0);
					mach_code = (RTI << 12);
					mach_code &= 0xF000;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "xor") == 0){
					mach_code = (XOR << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					mach_code &= 0xFFC0; /*clearing the last 6 bits*/
					/*see if the number being added is a constant number*/
					if( (lArg3[0] == 'x') || (lArg3[0] == '#')){
						errorcheck4(lArg1, lArg2, lArg3, 2, 3);
						int imm5 = toNum(lArg3);
						if(imm5 > 15 || imm5 < -16){
							exit(3);
						}

						mach_code |= 0x20; /*Changing the 5th bit to 1 since we are adding a constant*/
						mach_code |= (imm5 & 0x0000001F);
					}
					/*Argument 3 is a register*/
					else{
						errorcheck4(lArg1, lArg2, lArg3, 3, 3);
						mach_code += (lArg3[1]- 0x30);
					}
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "jmp") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 1, 1);
					mach_code = (JMP << 12) + ((lArg1[1] - 0x30) << 6);
					mach_code &= 0xF1C0;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "ret") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 0);
					mach_code = (JMP << 12) + 0x01C0;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "lshf") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					int amount4 = toNum(lArg3);
					/*Error checking to see the amount being shifted is within range 0 to 15*/
					if(amount4 < 0 || amount4 > 15){
						exit(3);
					}
					mach_code = (SHF << 12) + ((lArg1[1] - 0x30) << 9)+((lArg2[1] - 0x30) << 6)+ amount4;
					mach_code &= 0xFFCF; /*Make bits 5 and 4 00*/
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "rshfl") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					/*Error checking to see the amount being shifted is within range 0 to 15*/
					int amount4 = toNum(lArg3);
					if(amount4 < 0 || amount4 > 15){
						exit(3);
					}
					mach_code = (SHF << 12) + ((lArg1[1] - 0x30) << 9)+((lArg2[1] - 0x30) << 6)+ amount4;
					mach_code &= 0xFFDF;/*clear bit 5*/
					mach_code |= 0x0010;/*set bit 4*/
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "rshfa") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 3);
					int amount4 = toNum(lArg3);
					if(amount4 < 0 || amount4 > 15){
						exit(3);
					}
					/*Error checking to see the amount being shifted is within range 0 to 15*/
					mach_code = (SHF << 12) + ((lArg1[1] - 0x30) << 9)+((lArg2[1] - 0x30) << 6)+ amount4;
					mach_code |= 0x0030;/*Make bits 5 and 4 11*/
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "lea") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 1, 2);
					/*LEA can only have labels*/
					if(lArg2[0] == 'x' || lArg2[0] == '#'){
						exit(4);
					}
					mach_code = (LEA << 12)+ ((lArg1[1] - 0x30) << 9) + (returnOffset(lArg2, addrCtr, OFFSET9) & MASK_OFFS9);
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "trap") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 1);
					int trapVect8 = toNum(lArg1);
					/*Error checking to see the trap vector is within the range 0 to 255*/
					if(trapVect8 < 0 || trapVect8 > 255){
						exit(3);
					}
					mach_code = (TRAP << 12) + trapVect8;
					mach_code &= 0xF0FF;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "halt") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 0);
					mach_code = (TRAP << 12) + 0x25;
					mach_code &= 0xF0FF;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "nop") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 0);
					mach_code = 0x0000;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, "not") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 2, 2);
					mach_code = (XOR << 12) + ((lArg1[1] - 0x30)<<9) + ((lArg2[1] - 0x30)<<6);
					mach_code |= 0x003F;
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if(strcmp(lOpcode, ".fill") == 0){
					errorcheck4(lArg1, lArg2, lArg3, 0, 1);

				/*Not sure if this is the error check for missing operand*/
					mach_code = toNum(lArg1);
					fprintf( pOutfile, "0x%.4X\n", mach_code);
				}
				else if (strcmp(lOpcode, ".end") == 0){
					break;

				}
				else{
					exit(2);
				}

				/*fprintf( pOutfile, "0x%.4X\n", mach_code);*/
				/*printf("MACH_CODE!:  %i\n", mach_code); */
				addrCtr++;

		}
	   } while( lRet != DONE );
	   fclose(pOutfile);

	}


