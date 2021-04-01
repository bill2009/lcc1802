// 1802 Simulator for Camel Forth
// Copyright (c) 2009 Harold Rabbie

// This is an instruction simulator for the RCA 1802 8-bit microprocessor
// Intended to run CamelForth applications
// Invocation:	1802sim [-f] [-i] [-e] PROGNAME
// -f Enable Forth-level tracing
// -i Enable instruction-level tracing
// -e Echo text read by ACCEPT
// PROGNAME.OBJ	- Intel hex file to load into simulated memory
// PROGNAME.LST	- Assembly listing file containing symbol definitions
// The usual shell I/O re-direction allows you to read Forth programs
// from a text file, and also to save generated output
// e.g. 1802sim CF1802 < test.4th > output.log

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef enum {FALSE, TRUE} boolean;

boolean ftrace = FALSE;		// Forth-level tracing
boolean itrace = FALSE;		// Instruction-level tracing
boolean echo = FALSE;		// Echo ACCEPT'ed lines to console

//************** RCA 1802 registers ****************

typedef union {
    struct {
	byte	lo;
	byte	hi;
    } b;
    word	w;
} reg_t;	// Intel order 16-bit register

reg_t R [16];		// Pointer registers
#define RN R[N].w	// As 16-bits

byte P, X, opcode;	// Current PC, X-reg, instruction

reg_t DFU;		// D and DF flag

#define D  DFU.b.lo	// 8-bit D register
#define DF DFU.b.hi	// LS bit is DF flag
#define DW DFU.w	// As 9 bits

byte sim_mem [65536];	// Simulated 64kB memory

//************** Assembler symbol table ****************

typedef struct {
    char name[ 9 ];
    word addr;
} symtab_t;		// Assembler symbol table

#define MAX_SYMS 1000

symtab_t symtab [MAX_SYMS];

int num_symbols;

//************** CamelForth register assignments *********

#define RSP R[3].w
#define PSP R[2].w
#define IP  R[1].w
#define NEXT R[4].w
#define EXEC R[0].w

word param_stack, return_stack, abort_addr, end_dict;
word breakpoint = 0;

void print_ftrace (void);
int print_val (word addr);

#define makew(lo,hi) (((hi)<<8) | (lo))	/* make 16-bit word from two bytes */

#define mread(addr) (sim_mem [(addr)])	/* Read from simulated memory */

void mwrite (word addr, byte data) {	// Write to simulated memory		
	if (addr < end_dict) {	//Check for write-protect violation
		printf ("Attempt to write to pre-defined dictionary area\n");
		print_ftrace ();
		exit (1);
	}
	sim_mem [addr] = data;
}

//************** Opcode implementations *********

void unimplemented (void) {
	printf ("Unimplemented opcode at ");
	print_val (R[P].w - 1);
	printf ("\n");
	exit (1);
}

void LDN (byte N) {
	if (N == 0) unimplemented ();		// IDL instruction
	D = mread (RN);
}

void INC (byte N) {
	RN++;
}

void DEC (byte N) {
	RN--;
}

void LDA (byte N) {
	D = mread (RN++);
}

void STR (byte N) {
	mwrite (RN, D);
}

void GLO (byte N) {
	D = R[N].b.lo;
}

void GHI (byte N) {
	D = R[N].b.hi;
}

void PLO (byte N) {
	R[N].b.lo = D;
}

void PHI (byte N) {
	R[N].b.hi= D;
}

void SEP (byte N) {
	P = N;
	if (N != 0) return;
// trace SEP R0 (return from next)
	if (ftrace) print_ftrace ();
	
	if (EXEC == breakpoint) 
		print_ftrace();

	if (RSP > return_stack) {
		printf ("Return stack underflow\n");
		print_ftrace ();
		EXEC = abort_addr;
		RSP = return_stack;
	}
	if (PSP > param_stack) {
		printf ("Param stack underflow\n");
		print_ftrace ();
		EXEC = abort_addr;
		PSP = param_stack;
	}
}

void SEX (byte N) {
	X = N;
}

#define FETCH mread(R[P].w++)

#define SBR R[P].b.lo = ofs

void BRANCH (byte N) {		// 0x3N
	byte ofs = FETCH;

	switch (N) {
	case 0:	          SBR; break;	// BR
	case 2:	 if (!D)  SBR; break;	// BZ
	case 3:	 if (DF)  SBR; break;	// BDF
	case 8:	 	       break;	// SKP
	case 10: if (D)   SBR; break;	// BNZ
	case 11: if (!DF) SBR; break;	// BNF
	default: unimplemented ();	// 31 BQ, 34-37 Bn, 39 BNQ, 3C-3F BNn
	}
}

#define LBR { PC = R[P].w; R[P].b.hi = mread (PC++); R[P].b.lo = mread (PC); }
#define LSKP R[P].w += 2

void LBRANCH (byte N) {		// 0xCN
	word PC;

	switch (N) {
	case 0:	LBR; 				break;	// LBR
	case 2:	if (!D)	  LBR	else LSKP; 	break;	// LBZ
	case 3: if (DF)   LBR	else LSKP;	break;	// LBDF
	case 4: 				break;	// NOP
	case 6: if (D)    LSKP; 		break;	// LSNZ
	case 7: if (!DF)  LSKP; 		break;	// LNF
	case 8: LSKP; 				break;	// LSKP
	case 10: if (D)   LBR	else LSKP;	break;	// LBNZ
	case 11: if (!DF) LBR	else LSKP; 	break;	// LBNF
	case 14: if (!D)  LSKP; 		break;	// LSZ
	case 15: if (DF)  LSKP; 		break;	// LSDF
	default: unimplemented ();	// C1 LBQ, C5 LSNQ, C9 LBNQ, CC LSIE, CD LSQ
	}
}

void CODE60 (byte N) {			// 0x6N
	word length, addr, i;
	byte c, * string;

	switch (N) {
	case 0:	R[X].w++; break;	// IRX
	case 1: 			// OUT 1 - EMIT character
		c = mread (PSP);	// lo byte
		putchar (c & 0x7F);
		PSP += 2;		// pop word
		break;
	case 2:				// OUT 2 - set tracing flags
		c = mread (PSP);	// lo byte
		ftrace = (c & 1);	// Bit 0 - Forth tracing
		itrace = (c & 2);	// Bit 1 - Instruction tracing
		echo   = (c & 4);	// Bit 2 - Echo ACCEPTed lines
		PSP += 2;
		break;
	case 9:				// IN 1 - ACCEPT line
		length = makew (mread (PSP), mread (PSP + 1));
		PSP += 2;
		addr =  makew (mread (PSP), mread (PSP + 1));
		fflush (stdout);

		string = &sim_mem [addr];
		
		if (!fgets (string, length, stdin)) 
			exit (0);

		length = strlen (string);
		for (i = 0; i < length; i++)	// replace control chars with space
			if (string [i] < ' ') string [i] = ' ';

		mwrite (PSP, length & 0xFF);		// return length of string
		mwrite (PSP + 1, length >> 8);

		if (echo) printf ("%s\n", string);
		
		break;
	default: unimplemented();	// 62-67 OUT, 6A-6F INP, 68 undefined
	}
}

#define MOPR (mread (R[X].w))		/* Memory operand */

void ALU_7N (byte N) {			// 0x7N
	byte DFS;

	switch (N) {
	case 0: exit (0);			break;	// RET - unimplemented
	case 2: D = mread (R[X].w++); 		break;	// LDXA
	case 3: mwrite (R[X].w--, D); 		break;	// STXD
	case 4: DW = MOPR + D + DF; 		break;	// ADC
	case 5: DW = MOPR - D - !DF; DF = !DF; 	break;	// SDB
	case 6: DFS = D & 1; DW >>= 1; DF = DFS;	break;	// RSHR
	case 7: DW = D - MOPR - !DF; DF = !DF; 	break;	// SMB
	case 12: DW = FETCH + D + DF; 		break;	// ADCI
	case 13: DW = FETCH - D - !DF; DF = !DF;	break;	// SDBI
	case 14: DW = (DW << 1) | DF; DF &= 1;	break;	// RSHL
	case 15: DW = D - FETCH - !DF; DF = !DF;	break;	// SMBI
	default: unimplemented ();	
			// 71 DIS, 78 SAV, 79 MARK, 7A REQ, 7B SEQ
	}
}
	
void ALU_FN (byte N) {
	switch (N) {			// 0xFN
	case 0:	D = MOPR; 			break;	// LDX
	case 1:	D |= MOPR; 			break;	// OR
	case 2:	D &=  MOPR; 			break;	// AND
	case 3:	D ^= MOPR; 			break;	// XOR
	case 4:	DW = D + MOPR; 			break;	// ADD
	case 5:	DW = MOPR - D; DF = !DF; 	break;	// SD
	case 6:	DF = DW & 1; D >>=1; 		break;	// SHR
	case 7:	DW = D - MOPR; DF = !DF; 	break;	// SM
	case 8:	D = FETCH; 			break;	// LDI
	case 9:	D |= FETCH; 			break;	// ORI
	case 10: 	D &= FETCH; 		break;	// ANI
	case 11: 	D ^= FETCH; 		break;	// XRI
	case 12: 	DW = D + FETCH; 	break;	// ADI
	case 13: 	DW = FETCH - D; DF = !DF; 	break;	// SDI
	case 14: 	DW <<= 1; DF &= 1; 		break;	// SHL
	case 15: 	DW = D - FETCH; DF = !DF; 	break;	// SMI
	}
}

//	Dispatch table for bits [7:4] of opcode

typedef void (*opcode_t)(byte);

const opcode_t MSB_decode [16] =  {
	&LDN,		// 0N
	&INC,		// 1N
	&DEC,		// 2N
	&BRANCH,	// 3N
	&LDA,		// 4N
	&STR,		// 5N
	&CODE60,	// 6N
	&ALU_7N,	// 7N
	&GLO,		// 8N
	&GHI,		// 9N
	&PLO,		// AN
	&PHI,		// BN
	&LBRANCH,	// CN
	&SEP,		// DN
	&SEX,		// EN
	&ALU_FN		// FN
};

word read_hex_file (const char *file_name);
void read_symbols (const char *file_name);

//************** Disassembler for instruction-level tracing

typedef struct {
	char * MSB_disasm;
	char * LSB_disasm [16];
} disasm_t;

const disasm_t disassemble [16] = {
	{ "LDN %d" },
	{ "INC %d" },
	{ "DEC %d" },
	{ NULL, { "BR", "BQ", "BZ", "BDF", "B1", "B2", "B3", "B4", 
		  "SKP", "BNQ", "BNZ", "BNF", "BN1", "BN2", "BN3", "BN4" }},
	{ "LDA %d" },
	{ "STR %d" },
	{ "I/O %d" },
	{ NULL, { "RET", "DIS", "LDXA", "STXD", "ADC", "SDB", "SHRC", "SMB", 
		  "SAV", "MARK", "REQ", "SEQ", "ADCI", "SDBI", "SHLC", "SMI" }},
	{ "GLO %d"},
	{ "GHI %d" },
	{ "PLO %d" },
	{ "PHI %d" },
	{ NULL, {"LBR", "LBQ", "LBZ", "LBDF", "NOP", "LSNQ", "LSNZ", "LSNF",
		"LBNF", "LSKP", "LBNQ", "LBNZ", "LSIE", "LSQ", "LSZ", "LSDF" }},
	{ "SEP %d" },
	{ "SEX %d" },
	{ NULL, { "LDX", "OR", "AND", "XOR", "ADD", "SD", "SHR", "SM", 
		  "LDI", "ORI", "ANI", "XRI", "ADI", "SDI", "SHL", "SMI" } }
};

//************** Print instruction-level trace

void print_itrace (void) {
	int len;

	byte opcode;
	const disasm_t * disasm;
	const char * str;

	len = print_val (R[P].w);

	printf ("            " + len);

	opcode = mread (R[P].w);
	disasm = &disassemble [(opcode >> 4) & 0xF];
	if (disasm->MSB_disasm) {
		str = disasm->MSB_disasm;
		printf (str, opcode & 0xF);
		len = strlen (str);
		if ((opcode & 0xF) < 10) len--;
	}
	else {
		str = disasm->LSB_disasm [opcode & 0xF];
		len = strlen (str);
		printf (str);
	}
	printf ("        " + len);
	printf ("DF:%d D:%02X [X]:%02X\n", DF, D, MOPR);

}

//************** Find assembler symbol in symbol table

word find_symbol (const char * symbol_name) {
	int i;
	symtab_t * this_symbol;

	for (i = 0, this_symbol = symtab; i < num_symbols; i++, this_symbol++ )
		if (!strcmp (this_symbol->name, symbol_name))
			return this_symbol->addr;
	printf ("Unable to find %s in symbol table\n", symbol_name);
	exit (1);
}

//************** Print Forth-level trace

void print_ftrace (void) {
	word addr;
	int len;

	len = print_val (IP - 2);	// Forth IP
	printf (":");
	printf ("            " + len);
	len = print_val (EXEC);		// Forth CFA
	printf ("            " + len);

	printf ("RSTK:");		// Forth Return Stack
	for (addr = return_stack - 2; addr >= RSP; addr -= 2) {
		printf (" ");
		print_val (makew (mread (addr + 1), mread (addr) ));
	}
	printf ("\t PSTK:");		// Forth Param stack
	for (addr = param_stack - 2; addr >= PSP; addr -= 2) 
		printf (" %02X%02X", mread (addr + 1), mread (addr));
	printf ("\n");
}

//************** Read input files, interpret opcodes

int main (int argc, char *argv[]) {
	char * filename = NULL;
	char *arg;
	int argnum;

	for (argnum = 1; argnum < argc; argnum++) {
	    arg = argv [argnum];
	    if (arg [0] == '-')
		switch (arg [1]) {
		case 'f': ftrace = TRUE; break;
		case 'i': itrace = TRUE; break;
		case 'e': echo = TRUE; break;
		}
	    else filename = arg;
	}

	if (!filename) {
		printf ("Must provide target app file name\n");
		exit (1);
	}
	
	P = X = 0;
	R[0].w = read_hex_file (filename);
	read_symbols (filename);

	return_stack = find_symbol ("RETURNST");
	param_stack  = find_symbol ("PARAMSTA");
	abort_addr   = find_symbol ("ABORT");
	end_dict     = find_symbol ("ENDDICT");

	printf ("Starting address: %4.4X\n", R[0].w);
	for (;;) {
		if (itrace) print_itrace ();
		opcode = FETCH;
		MSB_decode [(opcode >> 4) & 0xF] (opcode & 0xF);
	}
	return 0;
}

//************** Read Intel Hex .OBJ file

#define BUFSIZE 128
char buf [ BUFSIZE ];

void bad_record (const char *message) {
	printf ("Bad record : %s : %s\n", message, buf);
	exit (1);
}

word read_hex_file (const char *fnm) {	// returns starting address

    char *in_record_ptr;
    byte checksum;
    int count, out_address, type, temp, i;
    FILE * fd;

    char file_name [256];
    strcpy (file_name, fnm);
    strcat (file_name, ".OBJ");

    if( ! (fd = fopen( file_name, "r") ) ) {
        printf( "Can't open input file %s\n", file_name );
	exit (1);
    }

    for(;;) {
        if (!fgets( buf, BUFSIZE, fd)) break;
        in_record_ptr = buf;
        if( * in_record_ptr++ != ':' ) continue;

        if( sscanf( in_record_ptr, "%2x%4x%2x", &count, &out_address,
        	&type ) != 3 ) bad_record( "Invalid count, address, type" );

        if (type) {
		fclose (fd);
		return out_address;      // type 1 is EOF
	}

        in_record_ptr += 8;
    	checksum = (out_address & 0xFF) + (out_address >>8) + count;

    	for( i = 0; i <= count; i++ ) {
        	// read xsum field, too
        	if( sscanf( in_record_ptr, "%2x", &temp ) != 1 ) 
			bad_record( "Invalid hex data" );
		sim_mem [out_address++] = temp;
        	checksum += temp;
        	in_record_ptr += 2;
    	}	
    	if( checksum ) bad_record( "Checksum error" );
    }
    printf ("Premature end of file %s\n", file_name);
    exit (1);
    return 0;
}       // end read_hex_file

//************** Read symbol table from .LST file 
//************** produced by PseudoSAM cross-assembler

void read_symbols (const char *fnm) {	// returns starting address

    char *inbuf_ptr;
    int i, j;
    symtab_t * symtab_ptr;
    FILE *fd;

    char file_name [256];
    strcpy (file_name, fnm);
    strcat (file_name, ".LST");

    if( ! (fd = fopen( file_name, "r") ) ) {
        printf( "Can't open input file %s\n", file_name );
	exit (1);
    }

    symtab_ptr = symtab;
    num_symbols = 0;

    for(;;) {
        if (!fgets( buf, BUFSIZE, fd )) break;
        inbuf_ptr = buf;
	
        if( * inbuf_ptr < ' ') inbuf_ptr++;	// skip leading control char
        if( * inbuf_ptr < ' ') inbuf_ptr++;	// skip leading control char

	for (i=0; i < 6; i++, inbuf_ptr +=15) {
		if (!isalpha (*inbuf_ptr) )  break;
		if (num_symbols++ >= MAX_SYMS) bad_record ("Too many symbols" );
		for (j = 0; j < 8; j++ ) 		// strip trailing blanks
			if (inbuf_ptr[j] == ' ') {
				inbuf_ptr [j] = '\0';
				break;
			}

		strncpy (symtab_ptr->name, inbuf_ptr, 8);
		(symtab_ptr++)->addr = strtol(inbuf_ptr + 9, NULL, 16);
	}
    }
    fclose (fd);
}

// Look up address in symbol table and print symbolically
// Return length of string printed

int print_val (word addr) {
	int i;

	symtab_t * this_symbol;
	word this_addr, nearest_addr, offset;
	char * nearest_name;

	if (addr >= 0xFF00) {
		printf ("-%02X", (-addr)& 0xFF);
		return 3;
	}

	offset = 0xFFFF;

	for (i = 0, this_symbol = symtab; i < num_symbols; i++, this_symbol++ ) {
		this_addr = this_symbol->addr;
		if (this_addr > addr) continue;
		if (addr - this_addr < offset) {
			nearest_name = this_symbol->name;
			nearest_addr = this_addr;
			offset = addr - nearest_addr;
		}
	}
	if (addr < 0x30) {
		printf ("%04X", addr);
		return 4;
	}
	if (!offset ) {
		printf ("%s", nearest_name);
		return strlen (nearest_name);
	}
	if (addr < 0x80) {
		printf ("%04X", addr);
		return 4;
	}
	if (offset < 0x100) {
		printf ("%s+%02X", nearest_name, offset);
		return strlen (nearest_name) + 3;
	}
	printf ("%04X", addr);
	return 4;
}

	
