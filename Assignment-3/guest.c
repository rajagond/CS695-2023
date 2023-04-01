#include <stddef.h>
#include <stdint.h>

#define PRINT8BIT_PORT 0xE9
#define PRINT32BIT_PORT 0xE8
#define STR_PORT 0xE7
#define NUM_EXITS_PORT 0xEA
#define GVA_TO_HVA_PORT 0xEB
#define GET_TIMESTAMP_PORT 0xEC

/* ----------------------- start: in/out instruction --------------------------------*/
static inline void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static inline void out32(uint16_t port, uint32_t value) {
	asm("out %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static inline uint32_t in32(uint16_t port) {
	uint32_t ret;
	asm("in %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );
	return ret;
}

/* ----------------------- end: in/out instruction --------------------------------*/

/* ----------------------- start: hypercalls functions --------------------------------*/

void HC_print8bit(uint8_t val)
{
	outb(PRINT8BIT_PORT, val);
}

void HC_print32bit(uint32_t val)
{
	out32(PRINT32BIT_PORT, val);
}

void HC_printStr(const char *str)
{
	out32(STR_PORT, (uint32_t)((uintptr_t)str));
}

uint32_t HC_numExits()
{
	return in32(NUM_EXITS_PORT);
}

uint32_t HC_gvaToHva(uint32_t gva)
{
	out32(GVA_TO_HVA_PORT, gva);
	return in32(GVA_TO_HVA_PORT);
}

uint32_t HC_getTimestamp()
{
	return in32(GET_TIMESTAMP_PORT);
}

/* ----------------------- end: hypercalls functions --------------------------------*/

/* ----------------------- start: main function --------------------------------*/
void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	const char *p;

	for (p = "Hello, world!\n"; *p; ++p)
		HC_print8bit(*p);


	/*----------Don't modify this section. We will use grading script---------*/
	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(2048);
	HC_print32bit(4294967295);

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();

	char *str = "CS 695 Assignment 3\n";
	HC_printStr(str);

	num_exits_b = HC_numExits();

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);

	uint32_t hva;
	hva = HC_gvaToHva(1024);
	HC_print32bit(hva);
	hva = HC_gvaToHva(4294967295);
	HC_print32bit(hva);

	uint32_t tsc;
	tsc = HC_getTimestamp();
	HC_print32bit(tsc);
	/*------------------------------------------------------------------------*/

	*(long *) 0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}

/* ----------------------- end: main function --------------------------------*/