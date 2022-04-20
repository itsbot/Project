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
	
    switch((int)ALUControl){
        //Z = A+B
        case 0:
            *ALUresult = A + B;
            break;
        //Z = A-B
        case 1:
            *ALUresult = A - B;
            break;
        //If A<B, Z = 1, otherwise Z = 0
        case 2:
            if((signed) A < (signed) B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        //If A<B, Z = 1, otherwise Z = 0 UNSIGNED
        case 3:
            if(A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
        //Z = A AND B
        case 4:
            *ALUresult = A & B;
            break;
        //Z = A OR B
        case 5:
            *ALUresult = A | B;
            break;
        //Shift left B by 16 bits
        case 6:
            *ALUresult = B << 16;   
            break;
        //Z = NOT A
        case 7:
            *ALUresult = ~A;
            break;
    }
    
    //Check to see if the result is zero
    if(*ALUresult == 0)
        *Zero = 1;
    else
        *Zero = 0;

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{    
	//PC input is address from Memory (PC >> 2)
	unsigned i = PC >> 2;

	//  check word alignment (Start of mem address must be divisible by 4)
	if(PC % 4 != 0)
		return 1;

	// Get instruction from Memory then return 0;  Assign address to instruction
	*instruction = Mem[i];
	return 0;  
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
   // Split 32 bits into 8 bit segments (Use hex assignments then bitmask)
   // use temp holders for assignments then assign to registers 
	*op = instruction >> 26;
	*r1 = (instruction & (0b11111 << 20)) >> 21;
	*r2 = (instruction & (0b11111 << 15)) >> 16;
	*r3 = (instruction & (0b11111 << 10)) >> 11;
	*funct = instruction & (0b111111);
	*offset = instruction & (0b1111111111111111);
	*jsec = instruction & (0b11111111111111111111111111);
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

   // based on opcode changes values in control
   switch(op)
   {
       // r-type (add, sub, and, or, set less than, set less than unsigned)
       case 0:
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls-> RegWrite = 1;
            break;

        // jump
        case 2:
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls-> RegWrite = 0;
            break;
        // beq
        case 4:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls-> RegWrite = 0;
            break;
        // addi
        case 8:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls-> RegWrite = 1;
            break;
        // slti
        case 10:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0; 
            controls->ALUSrc = 1;
            controls-> RegWrite = 1;
            break;
        // slti unsigned
        case 11:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0; 
            controls->ALUSrc = 1;
            controls-> RegWrite = 1;
            break;
        // lui
        case 15:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls-> RegWrite = 1;
            break;
        // lw
        case 35:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls-> RegWrite = 1;
            break;

        // sw
        case 43:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls-> RegWrite =0; 
            break;

        default:
            return 1;
   }
    return 0;	
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
    Check Halt
    Call ALU
    switch case (ALUOp)
    Break down case by case, for r-type check funct then reassign ALUOp per case
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
    /*
    Check for halt conditions

    */
    if (MemRead == 1) {
        if (ALUresult % 4 == 0) {
            *memdata = Mem[ALUresult >> 2];
        }
        else
            return 1;
    }
    if (MemWrite == 1) {
        if (ALUresult % 4 == 0) {
            Mem[ALUresult >> 2] = data2;
        }
        else    
            return 1;
    }
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    /*
    Check if Writing, then check register dest
    write memdata or ALUresult to registers
    */
    if (RegWrite == 1) {
        if (MemtoReg == 1) {
            if (RegDst == 0 ) {
                Reg[r2] = memdata;
            }
            else 
                Reg[r3] = memdata;
        }
        if (MemtoReg == 0) {
            if (RegDst == 0) {
                Reg[r2] = ALUresult;
            }
            else
                Reg[r3] = ALUresult;
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{   
    /*
    Update PC counter for Seq, Branch, or Jump
    (PC >> 2) 
    */
    // Sequential Instruction
    *PC += 4;
    // Branch
    if (Zero == 1 && Branch == 1) {
        *PC += extended_value << 2;
    }
    // Jump
    if (Jump == 1) {
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    } 
}

