/*
	<C> <Lion>

	prg_var.c

	Programm for Programmer on 8048

	Read Var from tmp file Function

	921201	v1.0

	Functions:	read_var();

*/

#include "prg_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <dos.h>
#include <memory.h>
#include "scl1.h"
#include "scl1clor.h"
#include "scl1keys.h"

par 	prgpar;
int 	proc_number;
int 	set_baud;
int 	fill_c=176;
byte 	far *bptr;
int 	cf = WHITE_BLACK;	/* Color fill char */
struct 	device far *prg_dev;
int 	port_flag;
int far *set_port_flag = &port_flag;
char 	namefile[80];
int	mes_prg_flag;
int	mes_ibm_flag;
int	mes_err_flag;
char far *frn;
char far *fwn;
char far *fsp;
char far *fed;

extern	uint	csum;		/* Buffer Control Sum*/
extern	long	stadrbuf;		/* start adress buffer data for programing	*/
extern	long	endadrbuf;	/* end adress buffer data for programing	*/
extern	long	stadrdev;		/* start adress device for programing		*/
extern	long	endadrdev;	/* end adress device for programing		*/
extern	uint	select_b;		/* Flags:	all  bytes select_b = 0*/
					/*		odd	bytes select_b = 1*/
					/*		even bytes select_b = 2*/
extern	int	fill_c;
extern	int set_one;
extern	int	modem_out_busy;

extern	int	port_add;
extern	byte	linestat;

void read_var(char *, char *);

void _far *ad;
uint _far *a;

void read_var(char *s, char *s1)
{
uint off, dseg;
uint *i;

	sscanf(s, "%4x", &off);
	sscanf(s1,"%4x", &dseg);

	FP_SEG(a) = dseg;
	FP_OFF(a) = off;

	FP_SEG(ad) = dseg;

	_fmemcpy(i, a+0, 2); FP_OFF(ad) = *i; _fmemcpy(&proc_number, 	ad, sizeof(proc_number));
	_fmemcpy(i, a+1, 2); FP_OFF(ad) = *i; _fmemcpy(&stadrbuf, 		ad, sizeof(stadrbuf));
	_fmemcpy(i, a+2, 2); FP_OFF(ad) = *i; _fmemcpy(&endadrbuf, 		ad, sizeof(endadrbuf));
	_fmemcpy(i, a+3, 2); FP_OFF(ad) = *i; _fmemcpy(&stadrdev, 		ad, sizeof(stadrdev));
	_fmemcpy(i, a+4, 2); FP_OFF(ad) = *i; _fmemcpy(&endadrdev, 		ad, sizeof(endadrdev));
	_fmemcpy(i, a+5, 2); FP_OFF(ad) = *i; _fmemcpy(&csum, 			ad, sizeof(csum));
	_fmemcpy(i, a+6, 2); FP_OFF(ad) = *i; _fmemcpy(&select_b, 		ad, sizeof(select_b));
	_fmemcpy(i, a+7, 2); FP_OFF(ad) = *i; _fmemcpy(&bptr, 			ad, sizeof(bptr));
	_fmemcpy(i, a+8, 2); FP_OFF(ad) = *i; _fmemcpy(&prgpar, 		ad, sizeof(prgpar));
	_fmemcpy(i, a+9, 2); FP_OFF(ad) = *i; _fmemcpy(&prg_dev, 		ad, sizeof(prg_dev));
	_fmemcpy(i, a+10, 2);FP_OFF(ad) = *i; _fmemcpy(&cf, 			ad, sizeof(cf));
	_fmemcpy(i, a+11, 2);FP_OFF(ad) = *i; _fmemcpy(&fill_c, 		ad, sizeof(fill_c));
	_fmemcpy(i, a+12, 2);FP_OFF(ad) = *i; _fmemcpy(&set_one, 		ad, sizeof(set_one));
	_fmemcpy(i, a+13, 2);FP_OFF(ad) = *i; _fmemcpy(&set_baud, 		ad, sizeof(set_baud));
	_fmemcpy(i, a+14, 2);FP_OFF(ad) = *i; _fmemcpy(&set_port_flag, 	ad, sizeof(set_port_flag));
	_fmemcpy(i, a+15, 2);FP_OFF(ad) = *i; _fmemcpy(&mes_prg_flag, 	ad, sizeof(mes_prg_flag));
	_fmemcpy(i, a+16, 2);FP_OFF(ad) = *i; _fmemcpy(&mes_ibm_flag, 	ad, sizeof(mes_ibm_flag));
	_fmemcpy(i, a+17, 2);FP_OFF(ad) = *i; _fmemcpy(&mes_err_flag, 	ad, sizeof(mes_err_flag));
	_fmemcpy(i, a+18, 2);FP_OFF(ad) = *i; _fmemcpy(&frn, 			ad, sizeof(frn));
	_fmemcpy(i, a+19, 2);FP_OFF(ad) = *i; _fmemcpy(&fwn, 			ad, sizeof(fwn));
	_fmemcpy(i, a+20, 2);FP_OFF(ad) = *i; _fmemcpy(&fsp, 			ad, sizeof(fsp));
	_fmemcpy(i, a+21, 2);FP_OFF(ad) = *i; _fmemcpy(&fed, 			ad, sizeof(fed));
}
