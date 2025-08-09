/*
Darya Pylypenko
DP24S
07/22/2025
The program in this file is the individual work of Darya Pylypenko
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

int text[105];
int regFile[32];
int dataMem[32];
int pc=0,cycle=1,stall=0,branch=0,mispred=0;
typedef struct
{
	int op;
	int rs, rt,rd;
	int imm, brt,funct,shamt;
}Inst;
Inst inst[105];
typedef struct
{
	Inst in;
	int ins;
	int pc4;
}IFID;

typedef struct 
{
	Inst in;
	int ins;
	int pc4;
	int rd1,rd2;
}IDEX;

typedef struct
{
	Inst in;
	int ins;
	int alu, write;
	int reg;
}EXMEM;

typedef struct
{
	Inst in;
	int ins;
	int mem,alu;
	int reg;
}MEMWB;

typedef struct 
{
	int pc;
	IFID ifid;
	IDEX idex;
	EXMEM exmem;
	MEMWB memwb;
}state;

const int SNT=0, WNT=1, WT=2, ST=3;

typedef struct 
{
	int pc, target, state;
}BP;

BP bp[102];

int getOp(int ins);
int getRs(int ins);
int getRt(int ins);
int getRd(int ins);
int getImm(int ins);
int getBrTar(int ins, int pc);
int getFunct(int ins);
int getShamt(int ins);
void getReg(int newIns,char rs[]);
void printInst(Inst in,int ins);
void getInst(int num, Inst *in, int pc);
void printState(state st);
int read1(Inst in);
int read2(Inst in);
void init(state *st);
void initBranchPredictor();
int predictBranch(int instAddr);
void updateBranchPredictor(int instAddr, int actual, int predicted);

int detectHazard(state *st) {

	if (st->idex.in.op == 35) { 
		int loadReg = st->idex.in.rt;
		if (st->ifid.in.rs == loadReg || st->ifid.in.rt == loadReg) {
			return 1; 
		}
	}
	return 0; 
}

int needForwarding(state *st, int reg) {

	if (reg == 0) return 0;
	

	if (st->exmem.in.op == 0) { 
		if (st->exmem.in.funct == 32 || st->exmem.in.funct == 34) { 
			if (st->exmem.reg == reg && st->exmem.reg != 0) {
				return 1; 
			}
		}
	} else if (st->exmem.in.op == 12 || st->exmem.in.op == 13) { 
		if (st->exmem.reg == reg && st->exmem.reg != 0) {
			return 1;
		}
	}
	
	if (st->memwb.in.op == 0) { 
		if (st->memwb.in.funct == 32 || st->memwb.in.funct == 34) { 
			if (st->memwb.reg == reg && st->memwb.reg != 0) {
				return 2; 
			}
		}
	} else if (st->memwb.in.op == 12 || st->memwb.in.op == 13 || st->memwb.in.op == 35) { 
		if (st->memwb.reg == reg && st->memwb.reg != 0) {
			return 2; 
		}
	}
	
	return 0;
}

int main()
{
	state curState, newState;
	int textSize = 0, dataSize = 0;
	int instruction;
	char line[256];
	
	init(&curState);
	initBranchPredictor();
	
	while (fgets(line, sizeof(line), stdin)) {
		if (strlen(line) <= 1) {
			break;
		}
		instruction = atoi(line);
		text[textSize++] = instruction;
		
		if (instruction == 1) { 
			break;
		}
	}
	
	while (fgets(line, sizeof(line), stdin)) {
		if (strlen(line) > 1) {
			dataMem[dataSize++] = atoi(line);
		}
	}
	
	pc = 0;
	cycle = 1;
	printState(curState);

	while (curState.memwb.in.op != 1) { 

		if (curState.memwb.in.op == 35) { 
			regFile[curState.memwb.reg] = curState.memwb.mem;
		} else if (curState.memwb.in.op == 0) { 
			if (curState.memwb.in.funct == 32 || curState.memwb.in.funct == 34) { 
				regFile[curState.memwb.reg] = curState.memwb.alu;
			}
		} else if (curState.memwb.in.op == 32 || curState.memwb.in.op == 34) { 
			regFile[curState.memwb.reg] = curState.memwb.alu;
		} else if (curState.memwb.in.op == 12 || curState.memwb.in.op == 13) { 
			regFile[curState.memwb.reg] = curState.memwb.alu;
		}
		
		newState.memwb.in = curState.exmem.in;
		newState.memwb.ins = curState.exmem.ins;
		newState.memwb.alu = curState.exmem.alu;
		newState.memwb.reg = curState.exmem.reg;
		if (curState.exmem.in.op == 35) { 
			if (curState.exmem.alu >= 0 && curState.exmem.alu < 1000) {
				newState.memwb.mem = dataMem[curState.exmem.alu / 4];
			} else {
				newState.memwb.mem = 0;
			}
		} else if (curState.exmem.in.op == 43) { 
			if (curState.exmem.alu >= 0 && curState.exmem.alu < 1000) {
				dataMem[curState.exmem.alu / 4] = curState.exmem.write;
			}
		}
		
		newState.exmem.in = curState.idex.in;
		newState.exmem.ins = curState.idex.ins;
		newState.exmem.alu = 0;
		newState.exmem.write = curState.idex.rd2;
		newState.exmem.reg = 0;
		
		switch (curState.idex.in.op) {
			case 0: 
				if (curState.idex.in.funct == 32) { 
					newState.exmem.alu = curState.idex.rd1 + curState.idex.rd2;
					newState.exmem.reg = curState.idex.in.rd;
				} else if (curState.idex.in.funct == 34) { 
					newState.exmem.alu = curState.idex.rd1 - curState.idex.rd2;
					newState.exmem.reg = curState.idex.in.rd;
				} else if (curState.idex.in.funct == 0) { 
					newState.exmem.alu = curState.idex.rd2 << curState.idex.in.shamt;
					newState.exmem.reg = curState.idex.in.rd;
				}
				break;
			case 32: 
				newState.exmem.alu = curState.idex.rd1 + curState.idex.rd2;
				newState.exmem.reg = curState.idex.in.rd;
				break;
			case 34: 
				newState.exmem.alu = curState.idex.rd1 - curState.idex.rd2;
				newState.exmem.reg = curState.idex.in.rd;
				break;
			case 12: 
				newState.exmem.alu = curState.idex.rd1 & curState.idex.in.imm;
				newState.exmem.reg = curState.idex.in.rt;
				break;
			case 13: 
				newState.exmem.alu = curState.idex.rd1 | curState.idex.in.imm;
				newState.exmem.reg = curState.idex.in.rt;
				break;
			case 35:
			case 43:
				newState.exmem.alu = curState.idex.rd1 + curState.idex.in.imm;
				newState.exmem.reg = curState.idex.in.rt;
				break;
			case 5: 
				newState.exmem.alu = (curState.idex.rd1 != curState.idex.rd2);
				branch++;

				int actual = (curState.idex.rd1 != curState.idex.rd2);
				int predicted = predictBranch((curState.idex.pc4 - 4) / 4);
				if (actual != predicted) {
					mispred++;

					newState.ifid.ins = 0;
					newState.ifid.pc4 = 0;
					newState.ifid.in.op = 0;
					if (actual) {
						pc = curState.idex.in.brt - 4;
					} else {
						pc = curState.idex.pc4 - 4; 
					}
				}
				updateBranchPredictor((curState.idex.pc4 - 4) / 4, actual, predicted);
				break;
		}
		
		if (detectHazard(&curState)) {

			stall++;
			newState.idex.in.op = 0; 
			newState.idex.ins = 0;
			newState.idex.pc4 = curState.ifid.pc4;
			newState.idex.rd1 = 0;
			newState.idex.rd2 = 0;
			newState.ifid = curState.ifid;
		} else {
			newState.idex.ins = curState.ifid.ins;
			newState.idex.pc4 = curState.ifid.pc4;
			getInst(curState.ifid.ins, &newState.idex.in, curState.ifid.pc4 - 4);
			newState.idex.rd1 = read1(newState.idex.in);
			newState.idex.rd2 = read2(newState.idex.in);
			
			if (curState.ifid.in.op == 5) { 
				int predicted = predictBranch((curState.ifid.pc4 - 4) / 4);
				if (predicted) {
					newState.ifid.ins = 0;
					newState.ifid.pc4 = 0;
					newState.ifid.in.op = 0;
					pc = curState.idex.in.brt - 4; 
				}
			}
		}

		if (pc < 105 * 4 && pc >= 0) {
			int textIndex = pc / 4;
			if (textIndex < 1000) {
				newState.ifid.ins = text[textIndex];

				getInst(text[textIndex], &newState.ifid.in, pc);
			} else {
				newState.ifid.ins = 0;
				newState.ifid.in.op = 0;
			}
			newState.ifid.pc4 = pc + 4;
		} else {
			newState.ifid.ins = 0;
			newState.ifid.in.op = 0;
			newState.ifid.pc4 = pc + 4;
		}
		
		pc += 4;
		cycle++;
		printState(newState);
		curState = newState;
	}
	
	printf("********************\n");
	printf("Total number of cycles executed: %d\n", cycle);
	printf("Total number of stalls: %d\n", stall);
	printf("Total number of branches: %d\n", branch);
	printf("Total number of mispredicted branches: %d\n", mispred);
	return 0;
}

int getOp(int ins)
{
	unsigned int newIns = ins >> 26;
	if(newIns ==0) // for R types
	{
		newIns = ins & 0x3F;
	}
	if(newIns == -29)
		newIns = 35;
	if(newIns == -21)
		 newIns = 43;
	return newIns;
}

int getRs(int ins)
{
	int newIns = ins & 0x03FFFFFF;
	newIns = newIns >>21;
	return newIns;
}

int getRt(int ins)
{
	int newIns = ins & 0x001FFFFF;
	newIns = newIns >>16;
	return newIns;
}

int getRd(int ins)
{
	int newIns = ins & 0x0000FFFF;
	newIns = newIns >>11;
	return newIns;
}

int getImm(int ins)
{
	int newIns = ins & 0x0000FFFF;
	short imm = (short)newIns;
	//printf("%d\t",imm);
	return imm;
}
int getBrTar(int ins, int pc)
{
	int target = ins &0x0000FFFF;
	short offset = (short) target;
	offset = offset * 4;
	return offset + pc;
}
int getFunct(int ins)
{
	int newIns = ins &0x0000003F;
	return newIns;
}
int getShamt(int ins)
{
	int newIns = ins&0x000007F0;
	newIns = newIns >> 6;
	return newIns;
}

void getReg(int newIns,char rs[] )
{
	if(newIns == 0)
		strcpy(rs,"0");
	else if(newIns ==1)
		strcpy(rs,"1");
	else if(newIns >1 && newIns <=3)
	{
		rs[0]='v';
		rs[1] = newIns-2 + 48;
		rs[2]='\0';
	}
	else if(newIns <=7)
	{
		rs[0]='a';
		rs[1] = newIns-4 + 48;
		rs[2]='\0';
	}
	else if(newIns <=15)
	{
		rs[0]='t';
		rs[1] = newIns-8 + 48;
		rs[2]='\0';
	}
	else if(newIns <=23)
	{
		rs[0]='s';
		rs[1] = newIns-16 + 48;
		rs[2]='\0';
	}
	else if(newIns <=25)
	{
		rs[0]='t';
		rs[1] = newIns-16 + 48;
		rs[2]='\0';
	}
	else if(newIns <=27)
	{
		rs[0]='k';
		rs[1] = newIns-26 + 48;
		rs[2]='\0';
	}	
	else if(newIns == 28)
		strcpy(rs,"gp");
	else if(newIns == 29)
		strcpy(rs,"sp");
	else if(newIns == 30)
		strcpy(rs,"fp");
	else if(newIns == 31)
		strcpy(rs,"ra");
}
void printInst(Inst in,int ins)
{
	char out[100] ="";
	char rs[5],rt[5],rd[5];
	int func;
	getReg(in.rs,rs);
	getReg(in.rt,rt);
	getReg(in.rd,rd);
	switch(in.op)
	{
		case 35: sprintf(out,"lw $%s,%d($%s)",rt,in.imm,rs);
				 break;
		case 43: sprintf(out,"sw $%s,%d($%s)",rt,in.imm,rs);
				break;
		case 12:sprintf(out,"andi $%s,$%s,%d",rt,rs,in.imm);
				break;
		case 13: sprintf(out,"ori $%s,$%s,%d",rt,rs,in.imm);
				break;
		case 5: sprintf(out,"bne $%s,$%s,%d",rs,rt,in.imm);
				break;
		case 1: sprintf(out,"halt");
				break;
		case 32: sprintf(out,"add $%s,$%s,$%s",rd,rs,rt);
				break;
		case 34: sprintf(out,"sub $%s,$%s,$%s",rd,rs,rt);
				break;
		case 0: if(ins==0)
					sprintf(out,"NOOP");
				else sprintf(out,"sll, $%s,$%s,%d",rd,rt,in.shamt);
				break;
	}
	printf("%s\n",out);
}
void getInst(int num, Inst *in, int pc)
{
	in->op = getOp(num);
	in->rs = getRs(num);
	in->rt = getRt(num);
	in->rd = getRd(num);
	in->imm = getImm(num);
	in->brt = getBrTar(num,pc);
	in->funct = getFunct(num);
	in->shamt = getShamt(num);
}
void printState(state st)
{
	int i;
	char rs[5],rt[5],rd[5];
	printf("********************\n");
	printf("State at the beginning of cycle %d\n",cycle);
	printf("\tPC = %d\n",pc);
	printf("\tData Memory:\n");
	for(i=0; i<16; i++)
	{
		printf("\t\tdataMem[%d] = %d\t\tdataMem[%d] = %d\n",i,dataMem[i],i+16,dataMem[i+16]);
	}
	printf("\tRegisters:\n");
	for(i=0; i<16; i++)
	{
		printf("\t\tregFile[%d] = %d\t\tregFile[%d] = %d\n",i,regFile[i],i+16,regFile[i+16]);
	}
	printf("\tIF/ID:\n");
	printf("\t\tInstruction: ");
	printInst(st.ifid.in,st.ifid.ins);
	printf("\t\tPCPlus4: %d\n",st.ifid.pc4);
	printf("\tID/EX:\n");
	printf("\t\tInstruction: ");
	printInst(st.idex.in,st.idex.ins);
	printf("\t\tPCPlus4: %d\n",st.idex.pc4);
	printf("\t\tbranchTarget: %d\n",st.idex.in.brt);
	printf("\t\treadData1: %d\n\t\treadData2: %d\n",st.idex.rd1,st.idex.rd2);
	printf("\t\timmed: %d\n",st.idex.in.imm);
	getReg(st.idex.in.rs,rs);
	getReg(st.idex.in.rt,rt);
	getReg(st.idex.in.rd,rd);
	printf("\t\trs: %s\n\t\trd: %s\n\t\trd: %s\n",rs,rt,rd);
	printf("\tEX/MEM\n");
	printf("\t\tInstruction: ");
	printInst(st.exmem.in,st.exmem.ins);
	getReg(st.exmem.reg,rs);
	printf("\t\taluResult: %d\n\t\twriteDataReg: %d\n\t\twriteReg: %s\n",st.exmem.alu,st.exmem.write,rs);
	printf("\tMEM/WB\n");
	printf("\t\tInstruction: ");
	printInst(st.memwb.in,st.memwb.ins);
	getReg(st.memwb.reg,rs);
	printf("\t\twriteDataMem: %d\n\t\twriteDataALU: %d\n\t\twriteReg: %s\n",st.memwb.mem,st.memwb.alu,rs);
}	
int read1(Inst in)
{
	return regFile[in.rs];
}
int read2(Inst in)
{
	if(in.op == 0) { 
		if(in.funct == 32 || in.funct == 34)
			return regFile[in.rt];
		else if(in.funct == 0)
			return regFile[in.rt];
		else
			return 0;
	}
	else if(in.op == 32 || in.op == 34)
		return regFile[in.rt];
	else if(in.op == 5)
		return regFile[in.rt];
	else if(in.op == 35 || in.op == 43)
		return in.imm;
	else
		return in.imm;
}

void init(state *st)
{
	int i;
	
	st->ifid.ins = 0;
	st->ifid.pc4 = 0;
	st->idex.ins = 0;
	st->idex.pc4 = 0;
	st->idex.rd1 = 0;
	st->idex.rd2 = 0;
	st->exmem.ins = 0;
	st->exmem.alu = 0;
	st->exmem.write = 0;
	st->exmem.reg = 0;
	st->memwb.ins = 0;
	st->memwb.mem = 0;
	st->memwb.alu = 0;
	st->memwb.reg = 0;
	
	st->ifid.in.op = 0;
	st->idex.in.op = 0;
	st->exmem.in.op = 0;
	st->memwb.in.op = 0;
	

	pc = 0;
	cycle = 1;
	stall = 0;
	branch = 0;
	mispred = 0;
}

int branchPredictor[100] = {0};


void initBranchPredictor() {
	for (int i = 0; i < 100; i++) {
		branchPredictor[i] = 1;
	}
}

int predictBranch(int instAddr) {
	return branchPredictor[instAddr] >= 2; 
}

void updateBranchPredictor(int instAddr, int actual, int predicted) {
	if (predicted && !actual) {
		if (branchPredictor[instAddr] > 0) branchPredictor[instAddr]--;
	} else if (!predicted && actual) {
		if (branchPredictor[instAddr] < 3) branchPredictor[instAddr]++;
	}
}
