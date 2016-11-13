/* x86s running Linux */

#include <string.h>

static char rcsid[] = "$Id: linux.c,v 1.5 1998/09/16 20:41:09 drh Exp $";

#ifndef LCCDIR
#define LCCDIR "/usr/local/bin"
#endif

char *suffixes[] = { ".c", ".i", ".s", ".o", ".out", 0 };
char inputs[256] = "";
char *include[] = {"-I" LCCDIR "include", "-I/usr/include", 0 };
char *cpp[] = { LCCDIR "cpp", "-D__STRICT_ANSI__", "$1", "$2", "$3", 0 };
char *com[] = { LCCDIR "rcc", "-target=xr18DH", "$1", "$2", "$3", 0 };
char *as[] = { LCCDIR "asl", "-cpu 1802", "-i " LCCDIR "include", "-L", "-quiet", "", "-o", "$3", "$1", "$2", 0 };
char *ld[] = { LCCDIR "p2hex", "", "", "", "", "$2", "$3", "","",0 }; //wjr dec 12
char *peep[] = { LCCDIR "copt", LCCDIR "include/lcc1802.opt", "-I", "$2", "-O", "$3", 0 }; //scp oct 10

extern char *concat(char *, char *);

int option(char *arg) {
	if (strncmp(arg, "-lccdir=", 8) == 0) {
		include[0] = concat("-I", concat(&arg[8], "/include"));
		cpp[0] = concat(&arg[8], "/cpp");
		com[0] = concat(&arg[8], "/rcc");
		as[0] = concat(&arg[8], "/asl");
		ld[0]  = concat(&arg[8], "/p2hex");
		peep[0]  = concat(&arg[8], "/copt");
	} else if (strcmp(arg, "-p") == 0 || strcmp(arg, "-pg") == 0) {
		//ld[7] = "/usr/lib/gcrt1.o";
		//ld[18] = "-lgmon";
		;
	} else if (strcmp(arg, "-b") == 0) 
		;
	else if (strcmp(arg, "-g") == 0)
		;
	else if (strncmp(arg, "-ld=", 4) == 0)
		//ld[0] = &arg[4];
		;
	else if (strcmp(arg, "-static") == 0) {
		//ld[3] = "-static";
		//ld[4] = "";
		;
	} else
		return 0;
	return 1;
}
