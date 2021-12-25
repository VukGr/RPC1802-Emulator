#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned short word;

typedef struct {
	byte D; //Size 8b
	byte P; //Size 4b (separated for speed)
	byte X; //Size 4b (separated for speed)
	union {
		word R[16]; //Size 16x2x8b (Separated for ease of use)
		byte divR[16][2];
	};
	//Flags, might wanna combine these...
	bool DF; //Carry (Why D??)
	bool Q; //???
	bool IE; //Interrupt Enable
	bool EF[4]; //IO pins
	byte T; //Size 4b
	byte IN;
	byte BUS[7];
	byte *M;
	size_t MSize;
} RCAState;

typedef enum {
	DBG_CONTINUE,
	DBG_UNKNOWN,
	DBG_BADARGS,
	DBG_STAY,
} Debug_RetCode;

void IDL(RCAState *S);
void LDN(RCAState *S);
void INC(RCAState *S);
void DEC(RCAState *S);
void BR(RCAState *S);
void SKP(RCAState *S);
void BQ(RCAState *S);
void BNQ(RCAState *S);
void BZ(RCAState *S);
void BNZ(RCAState *S);
void BDF(RCAState *S);
void BNF(RCAState *S);
void B1(RCAState *S);
void BN1(RCAState *S);
void B2(RCAState *S);
void BN2(RCAState *S);
void B3(RCAState *S);
void BN3(RCAState *S);
void B4(RCAState *S);
void BN4(RCAState *S);
void LDA(RCAState *S);
void STR(RCAState *S);
void IRX(RCAState *S);
void OUT(RCAState *S);
void INP(RCAState *S);
void RET(RCAState *S);
void DIS(RCAState *S);
void LDXA(RCAState *S);
void STXD(RCAState *S);
void ADC(RCAState *S);
void SDB(RCAState *S);
void RSHR(RCAState *S);
void SMB(RCAState *S);
void SAV(RCAState *S);
void MARK(RCAState *S);
void REQ(RCAState *S);
void SEQ(RCAState *S);
void ADCI(RCAState *S);
void SDBI(RCAState *S);
void RSHL(RCAState *S);
void SMBI(RCAState *S);
void GLO(RCAState *S);
void GHI(RCAState *S);
void PLO(RCAState *S);
void PHI(RCAState *S);
void LBR(RCAState *S);
void LSKP(RCAState *S);
void LBQ(RCAState *S);
void LBNQ(RCAState *S);
void LBZ(RCAState *S);
void LBNZ(RCAState *S);
void LBDF(RCAState *S);
void LBNF(RCAState *S);
void LSQ(RCAState *S);
void LSNQ(RCAState *S);
void LSZ(RCAState *S);
void LSNZ(RCAState *S);
void LSDF(RCAState *S);
void LSNF(RCAState *S);
void LSIE(RCAState *S);
void SEP(RCAState *S);
void SEX(RCAState *S);
void LDX(RCAState *S);
void OR(RCAState *S);
void AND(RCAState *S);
void XOR(RCAState *S);
void ADD(RCAState *S);
void SD(RCAState *S);
void SHR(RCAState *S);
void SHL(RCAState *S);
void SM(RCAState *S);
void LDI(RCAState *S);
void ORI(RCAState *S);
void ANI(RCAState *S);
void XRI(RCAState *S);
void ADI(RCAState *S);
void SDI(RCAState *S);
void SMI(RCAState *S);
void NOP(RCAState *S);
void IDK(RCAState *S);
void State_Dump(RCAState *S);
