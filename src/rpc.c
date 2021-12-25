#include "rpc.h"

//Short Branch Function Macros
#define DEF_BRCOND(NAME, TEST) \
	void NAME(RCAState *S) { \
		if(TEST) BR(S); \
		else SKP(S); \
	}
#define DEF_BRCOND_PAIR(NORMAL_NAME, NEG_NAME, TEST) \
	DEF_BRCOND(NORMAL_NAME, TEST) \
	DEF_BRCOND(NEG_NAME, !(TEST))

//Long Branch Function Macros
#define DEF_LBRCOND(NAME, TEST) \
	void NAME(RCAState *S) { \
		if(TEST) BR(S); \
		else LSKP(S); \
	}
#define DEF_LBRCOND_PAIR(NORMAL_NAME, NEG_NAME, TEST) \
	DEF_LBRCOND(NORMAL_NAME, TEST) \
	DEF_LBRCOND(NEG_NAME, !(TEST))

//Long Skip Function Macros
#define DEF_LSKPCOND(NAME, TEST) \
	void NAME(RCAState *S) { \
		if(TEST) LSKP(S); \
	}
#define DEF_LSKPCOND_PAIR(NORMAL_NAME, NEG_NAME, TEST) \
	DEF_LSKPCOND(NORMAL_NAME, TEST) \
	DEF_LSKPCOND(NEG_NAME, !(TEST))

//Maybe add a DEF_NCODE or something to handle the N code...

void (*Table[])(RCAState *) = {
	IDL, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN, LDN,
	INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC, INC,
	DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC, DEC,
	BR,  BQ,  BZ,  BDF, B1,  B2,  B3,  B4,  SKP, BNQ, BNZ, BNF, BN1, BN2, BN3, BN4,
	LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA, LDA,
	STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR, STR,
	IRX, OUT, OUT, OUT, OUT, OUT, OUT, OUT, IDK, INP, INP, INP, INP, INP, INP, INP,
	RET, DIS, LDXA, STXD, ADC, SDB, RSHR, SMB, SAV, MARK, REQ, SEQ, ADCI, SDBI, RSHL, SMBI,
	GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO, GLO,	
	GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI, GHI,
	PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO, PLO,
	PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI, PHI,
	LBR, LBQ, LBZ, LBDF, NOP, LSNQ, LSNZ, LSNF, LSKP, LBNQ, LBNZ, LBNF, LSIE, LSQ, LSZ, LSDF,
	SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP, SEP,
	SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX, SEX,
	LDX, OR,  AND, XOR, ADD, SD,  SHR, SM,  LDI, ORI, ANI, XRI, ADI, SDI, SHL, SMI
};	

char OpcodeNames[][5] = {
	"IDL", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN", "LDN",
	"INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC", "INC",
	"DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC", "DEC",
	"BR",  "BQ",  "BZ",  "BDF", "B1",  "B2",  "B3",  "B4",  "SKP", "BNQ", "BNZ", "BNF", "BN1", "BN2", "BN3", "BN4",
	"LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA", "LDA",
	"STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR", "STR",
	"IRX", "OUT", "OUT", "OUT", "OUT", "OUT", "OUT", "OUT", "IDK", "INP", "INP", "INP", "INP", "INP", "INP", "INP",
	"RET", "DIS", "LDXA", "STXD", "ADC", "SDB", "RSHR", "SMB", "SAV", "MARK", "REQ", "SEQ", "ADCI", "SDBI", "RSHL", "SMBI",
	"GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO", "GLO",	
	"GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI", "GHI",
	"PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO", "PLO",
	"PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI", "PHI",
	"LBR", "LBQ", "LBZ", "LBDF", "NOP", "LSNQ", "LSNZ", "LSNF", "LSKP", "LBNQ", "LBNZ", "LBNF", "LSIE", "LSQ", "LSZ", "LSDF",
	"SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP", "SEP",
	"SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX", "SEX",
	"LDX", "OR",  "AND", "XOR", "ADD", "SD",  "SHR", "SM",  "LDI", "ORI", "ANI", "XRI", "ADI", "SDI", "SHL", "SMI"
};

byte Lower4(byte Val) {
	return Val & 0b1111; 
}

byte Higher4(byte Val) {
	return Val >> 4;
}

byte Lower8(word Val) {
	return Val & 0b11111111;
}

byte Higher8(word Val) {
	return Val >> 8;
}

void LDN(RCAState *S) {
	byte N = Lower4(S->IN);
	S->D = S->M[S->R[N]];
}

void INC(RCAState *S) {
	byte N = Lower4(S->IN);
	S->R[N]++;
}

void DEC(RCAState *S) {
	byte N = Lower4(S->IN);
	S->R[N]--;
}

void BR(RCAState *S) {
   	S->R[S->P] = (S->R[S->P] & 0xFF00) + S->M[S->R[S->P]];
}

//Alt. NBR
void SKP(RCAState *S) {
	S->R[S->P] += 1;
}

DEF_BRCOND_PAIR(BQ, BNQ, S->Q) 
DEF_BRCOND_PAIR(BZ, BNZ, S->D == 0) 
DEF_BRCOND_PAIR(BDF,BNF, S->DF)
DEF_BRCOND_PAIR(B1, BN1, S->EF[0])
DEF_BRCOND_PAIR(B2, BN2, S->EF[1])
DEF_BRCOND_PAIR(B3, BN3, S->EF[2])
DEF_BRCOND_PAIR(B4, BN4, S->EF[3])

// Load Advance (I wonder what the usecase is for this one...)
void LDA(RCAState *S) {
	LDN(S);
	byte N = Lower4(S->IN);
	S->R[N] += 1;
}

// Store Via N
void STR(RCAState *S) {
	byte N = Lower4(S->IN);
	S->M[S->R[N]] = S->D;
}

void IRX(RCAState *S) {
	S->R[S->X] += 1;
}

void OUT(RCAState *S) {
	byte N = Lower4(S->IN);
	S->BUS[N - 1] = S->M[S->R[S->X]];
	S->R[S->X] += 1;
}

void INP(RCAState *S) {
	byte N = Lower4(S->IN);
	S->M[S->R[S->X]] = S->BUS[(N & 7) - 1];
	S->D = S->BUS[(N & 7)-1];
}

void RET(RCAState *S) {
	byte Val = S->M[S->R[S->X]];
	S->X = Val >> 4;
	S->P = Val & 7;
	S->R[S->X] += 1;
	S->IE = 1;
}

//Disable
//TODO: Figure out what the use of this even is, maybe fix it up...
void DIS(RCAState *S) {
	RET(S);
	S->IE = 0;
}

//Load via X and Advance
void LDXA(RCAState *S) {
	S->D = S->M[S->R[S->X]];
	S->R[S->X] += 1;
}

//Store via X and Decrement
void STXD(RCAState *S) {
	S->M[S->R[S->X]] = S->D;
	S->R[S->X] -= 1;
}

void Util_ADC8(RCAState *S, byte B) {	
	word Res = 0;
	Res += S->D;
	Res += B;
	Res += S->DF;

	S->DF = (Res >> 8) != 0;
	S->D = Res & 0xFF;
}

//XXX: Not sure if correct
void Util_SDB8(RCAState *S, byte B, bool negD) {
	word Res = 0x0100;
	Res += S->D * (negD? -1 : 1);
	Res += B;
	Res -= !S->DF;

	S->DF = (Res >> 8) != 0;
	S->D = Res & 0xFF;
}

void ADC(RCAState *S) {
	Util_ADC8(S, S->M[S->R[S->X]]);
}

void SDB(RCAState *S) {
	Util_SDB8(S, S->M[S->R[S->X]], true);
}

void RSHR(RCAState *S) {
	SHR(S);
	S->D |= S->DF << 7;
}

//Subtract Memory with Borrow
void SMB(RCAState *S) {
	Util_SDB8(S, -(S->M[S->R[S->X]]), false);
}

//Save
void SAV(RCAState *S) {
	S->M[S->R[S->X]] = S->T;
}

//Push X,P to stack
void MARK(RCAState *S) {
	S->T = (S->X << 4) + S->P ;
	S->M[S->R[2]] = S->T;
	S->X = S->P;
	S->R[2] -= 1;
}

//Reset Q
void REQ(RCAState *S) {
	S->Q = false;
}

//Set Q
void SEQ(RCAState *S) {
	S->Q = true;
}

//Add with carry, Immediate
void ADCI(RCAState *S) {
	Util_ADC8(S, S->M[S->R[S->P]]);
}

//Subtract with borrow, Immediate
void SDBI(RCAState *S) {
	Util_SDB8(S, S->M[S->R[S->P]], true);
}

//Ring Shift Left with Carry
void RSHL(RCAState *S) {
	SHL(S);
	S->D |= S->DF;
}

//Subtract memory with borrow, Immediate
void SMBI(RCAState *S) {
	Util_SDB8(S, -(S->M[S->R[S->P]]), false);
	S->R[S->P] += 1;
}

//Get low reg N
void GLO(RCAState *S) {
	byte N = Lower4(S->IN);
	S->D = S->divR[N][0];
}

//Get high reg N
void GHI(RCAState *S) {
	byte N = Lower4(S->IN);
	S->D = S->divR[N][1];
}

//Put low reg N
void PLO(RCAState *S) {
	byte N = Lower4(S->IN);
	S->divR[N][0] = S->D;
}

//Put high reg N
void PHI(RCAState *S) {
	byte N = Lower4(S->IN);
	S->divR[N][1] = S->D;
}

//Long Branch
void LBR(RCAState *S) {
	S->divR[S->P][1] = S->M[S->R[S->P]];
	S->divR[S->P][0] = S->M[S->R[S->P] + 1];
}

void LSKP(RCAState *S) {
	S->R[S->P] += 2;
}

DEF_LBRCOND_PAIR(LBQ, LBNQ, S->Q)
DEF_LBRCOND_PAIR(LBZ, LBNZ, S->D == 0)
DEF_LBRCOND_PAIR(LBDF, LBNF, S->DF)

DEF_LSKPCOND_PAIR(LSQ, LSNQ, S->Q)
DEF_LSKPCOND_PAIR(LSZ, LSNZ, S->D == 0)
DEF_LSKPCOND_PAIR(LSDF, LSNF, S->DF)

DEF_LSKPCOND(LSIE, S->IE)

void SEP(RCAState *S) {
	byte N = Lower4(S->IN);
	S->P = N;
}

void SEX(RCAState *S) {
	byte N = Lower4(S->IN);
	S->X = N;
}

void LDX(RCAState *S) {
	S->D = S->M[S->R[S->X]];
}

void OR(RCAState *S) {
	S->D |= S->M[S->R[S->X]];
}

void AND(RCAState *S) {
	S->D &= S->M[S->R[S->X]];
}

void XOR(RCAState *S) {
	S->D ^= S->M[S->R[S->X]];
}

void ADD(RCAState *S) {
	Util_ADC8(S, S->M[S->R[S->X]] - S->DF);
}

void SD(RCAState *S) {
	Util_SDB8(S, S->M[S->R[S->X]] + !(S->DF), true);
}

void SHR(RCAState *S) {
	S->DF = S->D & 1;
	S->D >>= 1;
}

void SHL(RCAState *S) {
	S->DF = S->D >> 7;
	S->D <<= 1;
}

void SM(RCAState *S) {
	Util_SDB8(S, -S->M[S->R[S->X]] + !(S->DF), false);
}

void LDI(RCAState *S) {
	S->D = S->M[S->R[S->P]];
	S->R[S->P] += 1;
}

void ORI(RCAState *S) {
	S->D |= S->M[S->R[S->P]];
	S->R[S->P] += 1;
}

void ANI(RCAState *S) {
	S->D &= S->M[S->R[S->P]];
	S->R[S->P] += 1;
}

void XRI(RCAState *S) {
	S->D ^= S->M[S->R[S->P]];
	S->R[S->P] += 1;
}

void ADI(RCAState *S) {
	Util_ADC8(S, S->M[S->R[S->P]] - S->DF);
	S->R[S->P] += 1;
}

void SDI(RCAState *S) {
	Util_SDB8(S, S->M[S->R[S->P]] + S->DF, true);
}

void SMI(RCAState *S) {
	Util_SDB8(S, -S->M[S->R[S->P]] + !(S->DF), false);
}

void NOP(RCAState *S) {
	return;
}

void IDK(RCAState *S) {
	printf("Unimplemented command: %d\n", S->IN);
}

void IDL(RCAState *S) {
	State_Dump(S);
	printf("Pause\n");
	getchar();
}

void State_Dump(RCAState *S) {
	printf("State:\n");
	printf("	D: %02X\n", S->D);
	printf("	P: %01X,	X: %01X\n", S->P, S->X);
	printf("	DF: %01X,	IE: %01X,	Q: %01X\n", S->DF, S->IE, S->Q);
	printf("	IN: %02X (%s)\n", S->IN, OpcodeNames[S->IN]);
	printf("	T: %02X\n", S->T);
	printf("	Regs:\n");
	for(int i = 0; i < 4; i++) {
		printf("	");
		for(int j = 0; j < 4; j++) {
			printf("	%04X ", S->R[(i*4)+j]);
		}
		printf("\n");
	}
	printf("	Mem (not full):\n");
	for(int i = 0; i < 20; i++) {
		printf("		%04X: ", i*16);
		for(int j = 0; j < 16; j++) {
			printf("%02hhX ", S->M[(i*16)+j]);
		}
		printf("\n");
	}
}

RCAState *State_New(int mem_size) {
	RCAState *S = malloc(sizeof(RCAState));
	S->D = 0;
	S->P = 0;
	S->X = 0;
	S->DF = false;
	S->Q = false;
	S->IE = true;
	S->T = 0;
	S->IN = 0;

	for(int i = 0; i < 4; i++) 
		S->EF[i] = false;
	for(int i = 0; i < 7; i++)
		S->BUS[i] = 0;
	for(int i = 0; i < 16; i++)
		S->R[i] = 0;

	S->M = malloc(sizeof(byte) * mem_size);
	for(int i = 0; i < mem_size; i++)
		S->M[i] = 0;
	S->MSize = mem_size;

	return S;
}

void State_Destroy(RCAState *S) {
	free(S->M);
	free(S);
}	

void State_Exec(RCAState *S, byte opcode) {
	S->IN = opcode;
	(*Table[opcode])(S);	
}

char* File_ReadWhole(char *name, size_t *size) {
	FILE *f = fopen(name, "rb");
	
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	char *str = (char *)malloc((fsize+1)*sizeof(char));
	fread(str, 1, fsize, f);
	str[fsize] = '\0';
	
	fclose(f);

	if(size != NULL) *size = fsize;
	return str;
}

void State_LoadRom(RCAState *S, byte *rom, size_t rom_size) {
	for(int i = 0; i < rom_size && i < S->MSize; i++) {
		S->M[i] = rom[i];
	}
}

void State_Tick(RCAState *S) {
	S->IN = S->M[S->R[S->P]];
	S->R[S->P] += 1;
	(*Table[S->IN])(S);	
	if(S->BUS[0]) {
		putchar(S->BUS[0]);
		S->BUS[0] = 0;
	}
	if(S->BUS[1]) {
		//TODO: Implement this
	}
}

//TODO: Fix this, very messy
//Maybe make a debug object
bool step = false; // Maybe merge this into just step_counter
int step_counter = 0;
bool breakpoint = false;
int breakpoint_reg = 0;
int breakpoint_val = 0;
bool autodump = false;
char command[1024] = { 0 };
bool debug = false;

//TODO: Overflow checking.
void Util_GetLine(char *buf) {
	char first = getchar();

	if(first == '\n')
		return;
	
	for(char ch = first; ch != '\0' && ch != '\n'; buf++, ch = getchar()) {
		*buf = ch;
	}

	*buf = '\0';
}

//TODO: Write output for these.
Debug_RetCode Debug_ExecCmd(RCAState *S, char *cmd) {
	char op;
	int arg1, arg2;
	int argc = sscanf(cmd, "%c %X %X", &op, &arg1, &arg2);
	
	switch(op) {
		//Step
		case 's':
			if(argc != 2) 
				return DBG_BADARGS;
			step = true;
			step_counter = arg1;
			return DBG_CONTINUE;
		//Breakpoint
		case 'b':
			if(argc != 3)
				return DBG_BADARGS;
			breakpoint = true;
			breakpoint_reg = arg1;
			breakpoint_val = arg2;
			return DBG_STAY;
		//Remove breakpoint
		case 'r':
			breakpoint = false;
			return DBG_STAY;
		//Continue/Exit step mode
		case 'c':
			step = false;
			return DBG_CONTINUE;
		case 'a':
			autodump = !autodump;
			return DBG_STAY;
		//Dump state
		case 'd':
			State_Dump(S);
			//TODO: Very messy, make the return value an error code.
			return DBG_STAY;
		//Quit
		case 'q':
			exit(0);
		//Unknown
		default:
			return DBG_UNKNOWN;
	}
}

Debug_RetCode Debug_Parse(RCAState *S) {
	printf("> ");
	
	Util_GetLine(command);

	if(command[0] == '\n' || command[0] == '\0')
		return DBG_STAY;

	return Debug_ExecCmd(S, command);
}

void Debug_Cmdline(RCAState *S) {
	Debug_RetCode ret;
	while((ret = Debug_Parse(S)) != DBG_CONTINUE) {
		if(ret == DBG_UNKNOWN)
			printf("Unknown command.\n");
		else if(ret == DBG_BADARGS) 
			printf("Bad arguments.\n");
	}
}

void Debug_Tick(RCAState *S) {
	if(autodump)
		State_Dump(S);

	if(step) {
		if(step_counter == 0) {
			step = false;
			Debug_Cmdline(S);
		}
		else {
			step_counter--;
		}
	}

	if(breakpoint) {
		if(S->R[breakpoint_reg] == breakpoint_val)
			Debug_Cmdline(S);	
	}
}

//TODO: Rename Util_X8 to just Util_X, maybe find a better prefix than util.
//TODO: Separate actual util stuff into a different file.
//TODO: Maybe change the State_X to RCA_X or smth
//TODO: Maybe don't mention RCA if it's a company, switch to elf?
int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Correct usage: rpc <rom file>\n");
		return 0;
	}
	else if(argc == 3 && !strcmp(argv[2],"-d")) {
		debug = true;
	}

	byte in;
	RCAState *S = State_New(0xFFFF);
	
	size_t rom_size = 0;
	byte *rom = (byte *)File_ReadWhole(argv[1], &rom_size);
	State_LoadRom(S, rom, rom_size); 
	free(rom);

	if(debug) Debug_Cmdline(S);

	while(true) {
		if(debug) Debug_Tick(S);
		State_Tick(S);
		//scanf("%02hhX", &in);
		//getchar(); 
	}

	return 0;
}
