// *** Hardwarespecific defines *** adapted for lcc1802
//#define cbi(reg, bitmask) *reg &= ~bitmask
//#define sbi(reg, bitmask) *reg |= bitmask
//#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
//#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

//#define cport(port, data) port &= data
//#define sport(port, data) port |= data

//#define swap(type, i, j) {type t = i; i = j; j = t;}

#define regtype volatile uint8_t
#define regsize uint8_t
