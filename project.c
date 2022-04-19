#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    /*
    Switch case for each in ALU Control
    Some will use bit shifts
    Check sign
    Update *ALUresult
    000
    001
    101
    011
    100
    101
    110
    111
    */

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    /*
    PC input is address from Memory
    check word alignment (Start of mem address must be divisible by 4)
    Assign address to instruction
    
    Return 1 for halt conditions (ProjectGuideline 2.4)
    Else, return 0
    */
    
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    /*
    Split 32 bits into 8 bit segments (Use hex assignments then bitmask)
    use temp holders for assignments then assign to registers 

    op,	// instruction [31-26]
	r1,	// instruction [25-21]
	r2,	// instruction [20-16]
	r3,	// instruction [15-11]
	funct,	// instruction [5-0]
	offset,	// instruction [15-0]
	jsec;	// instruction [25-0]

    */

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    /*
    Switch case (op)
    r - op, rs, rt, rd, shamt, funct
    i - op, rs, rt, immediate
    j - op, address

    Assign to struct controls:
    	char RegDst;        0 OR 1 = path of multiplexer, 2 = ignore
	    char Jump;          0 OR 1 = path of multiplexer, 2 = ignore
	    char Branch;        0 OR 1 = path of multiplexer, 2 = ignore
	    char MemRead;       0 = disable, 1 = enable, 2 = ignore
	    char MemtoReg;      0 OR 1 = path of multiplexer, 2 = ignore
	    char ALUOp;         See first function
	    char MemWrite;      0 = disable, 1 = enable, 2 = ignore
	    char ALUSrc;        0 OR 1 = path of multiplexer, 2 = ignore
	    char RegWrite;      0 = disable, 1 = enable, 2 = ignore

    Return 1 for halt
    Else return 0
    */


}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    /*
    Read Reg->data1
         Reg->data2
    */
   *data1 = Reg[r1];
   *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    /*
    Pretty much turning a 16 bits into 32 bits
    Check if 16th bit(offset) is 1 or 0
    Insert 16 1's or 16 0's depending on check.
    Assign to extended_value
    */

   if((offset >> 15) == 1) {
       *extended_value = offset | 0xffff0000;
   } else {
       *extended_value = offset & 0x0000ffff;
   }

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    /*
    Call ALU
    switch case (ALUOp)
    */
   switch (ALUOp) {
       case 0x0: // add / don't care
       case 0x1: // sub
       case 0x2: // set <
       case 0x3: // set < unsigned
       case 0x4: // and
       case 0x5: // or
       case 0x6: // shift extended_value L 16 bits
            break;
       case 0x7: // R-type
            switch (funct) {
                case 0x20: // add
                    ALUOp = 0x0;
                    break;
                case 0x2a: // slt S
                    ALUOp = 0x2;
                    break;
                case 0x2b: // slt U
                    ALUOp = 0x3;
                    break;
                case 0x24: // and
                    ALUOp = 0x4;
                    break;
                case 0x25: // or
                    ALUOp = 0x5;
                    break;
                default:
                    return 1;
            }
        break;
        default:
            return 1;
   }
   ALU(data1, data2, ALUOp, ALUresult, Zero);
   return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

