#ifndef __COPROCESSORS_H__
#define __COPROCESSORS_H__

/*#define COPROC_1 0x40000000
#define COPROC_2 0x40000030
#define COPROC_3 0x40000060
#define COPROC_4 0x40000090*/

#define COPROC_1_RST 0x40000000
#define COPROC_1_RW 0x40000004
#define COPROC_2_RST 0x40000030
#define COPROC_2_RW 0x40000034
#define COPROC_3_RST 0x40000060
#define COPROC_3_RW 0x40000064
#define COPROC_4_RST 0x40000090
#define COPROC_4_RW 0x40000094

#define CTRL_SL_RST 0x400000C0
#define CTRL_SL_RW 0x400000C4



//int buffer[64];
/*
--   0x40000000  COPROCESSOR 1 (reset)
--   0x40000010  COPROCESSOR 1 (input/output)

--   0x40000030  COPROCESSOR 2 (reset)
--   0x40000040  COPROCESSOR 2 (input/output)

--   0x40000060  COPROCESSOR 3 (reset)
--   0x40000070  COPROCESSOR 3 (input/output)

--   0x40000090  COPROCESSOR 4 (reset)
--   0x400000A0  COPROCESSOR 4 (input/output)
*/

inline void coproc_reset(volatile unsigned int* _adr)
{
        //volatile unsigned int* adr = (volatile unsigned int*)_adr;
	(*_adr) = 0;
}

inline void coproc_write(volatile unsigned int* _adr, int value)
{
//        volatile unsigned int* adr = (volatile unsigned int*)_adr;
	*(_adr) = value;
}

inline int coproc_read(volatile unsigned int* _adr)
{
//        volatile unsigned int* adr = (volatile unsigned int*)_adr;
	return *(_adr);
}

/*void test_coproc(unsigned int* _adr, int value)
{
	volatile unsigned int* adr = (volatile unsigned int*)_adr;
 	my_printfh("Testing coprocessor : ", adr, value);
	coproc_reset( adr );
	coproc_write( adr+1, value );
	int res = coproc_read( adr+1 );
	my_printf(" + result  = ", res);
}*/

#endif
