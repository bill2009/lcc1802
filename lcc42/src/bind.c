//wjr Nov 2012: includes m1, cosmac, x1802
//wjr Dec 21 2012:includes m1.xr16, xr18
//wjr mar 3, 2013 adding xr18no
//april 10 - looks like xr18.fl was added march 20th.
//april 23 replace xr18 with xr182b
//Oct 2 replace XR18ng with xr18DH
//Feb 22 2017 replacing xr182b with xr18NW - 1806 adaptation
//20-04-28 replacing xr18no with xr18CX
//21-3-20 moving to new laptop. source directory in c:\lcc42\lcc42
#include "c.h"
#undef yy
#define yy \
xx(alpha/osf,    alphaIR) \
xx(mips/irix,    mipsebIR) \
xx(m1,    		 m1IR) \
xx(xr16,    	 xr16IR) \
xx(xr18NW,    	 xr18NWIR) \
xx(xr18DH,    	 xr18DHIR) \
xx(xr18CX,    	 xr18CXIR) \
xx(xr188B,    	 xr188BIR) \
xx(sparc/sun,    sparcIR) \
xx(sparc/solaris,solarisIR) \
xx(x86/win32,    x86IR) \
xx(x86/linux,    x86linuxIR) \
xx(symbolic/osf, symbolic64IR) \
xx(symbolic/irix,symbolicIR) \
xx(symbolic,     symbolicIR) \
xx(bytecode,     bytecodeIR) \
xx(null,         nullIR)

#undef xx
#define xx(a,b) extern Interface b;
yy

Binding bindings[] = {
#undef xx
#define xx(a,b) #a, &b,
yy
	NULL, NULL
};
#undef yy
#undef xx
