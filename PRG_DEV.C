/*
	<C> <Lion>

	prg_dev.c

	Programm for Programmer on 8048

	Chip select

	920220	v1.0 Beta4

	Functions:
*/

#include "prg_def.h"
#include <scl1.h>
#include <scl1keys.h>
#include <scl1clor.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <conio.h>

#define V12 0
#define V13 1
#define V21 2
#define V23 3
#define V25 4
#define UNKNOWN -1

char *vpp_ch[]= {"12", "13", "21", "23", "25"};

char *size_c[] =
{
	" 2716 ",
	" 2732 ",
	" 2764 ",
	" 27128 ",
	" 27256 ",
	" 27512 ",
	" 8048 ",
	" 8051 ",
	" 8755 ",
	" 27c64 ",
	" 27c128 ",
	" 27c256 ",
	" 27c512 ",
	NULL,
};

char *sel_q[] =
{
	"  Save    ",
	"  Change  ",
	"  Abort   ",
	NULL,
};


char *sel_mn[] =
{
	"  Next  Device  ",
	"      Exit      ",
	NULL,
};

/*
			Functions prototype
*/
int	DeviceFieldCheck(FData2 *fd2);
int	all_window(char *, int, int, char *[]);
void	write_prg_dev(int, int);
int	cmpgle( unsigned *, unsigned *);
void	(far *fsend_one_command)();
void	(far *fset_port)();
void	(far *frestore_prg_w) ();

/*
			External Funcions Prototipe
*/
extern 	void	open_window(int, int, int, int, int, int, char *, char *);
extern 	void	open_window_dev(int, int, int, int, int, int, char *, char *);
extern 	int	open_scroll(int, int, int, int, char *, char *, int, char *[]);
extern 	int	open_scroll_close(int, int, int, int, char *, char *, int, char *[]);
extern 	uint	atox(char *str);
extern 	int	hi(int);
extern 	int	select_device(int, char **, char **);
extern 	int	copy_d_prg_dev(char *, char *);
extern	void	read_var(char *, char *);
extern 	void	*malloc_e(uint mem);
extern	void	message_screen(int, char *, ...);

/*
			External variable
*/
extern	char *new_device;
extern	char *new_type;
extern	struct device small_prg_dev;
extern	long size_x[];
extern	char	flag_common;
extern	char *vcc_c[4];
extern	char *vpp_c[6];
extern	char *size_[14];
extern	char *alg_c[4];
extern	byte	far	*bptr;
extern	int	cf;
extern	struct device far *prg_dev;
extern	char *namefile;
extern	char far *fsp;




/*
			Local variables
*/
uint	csum;   		/* Buffer Control Sum					*/
long	stadrbuf;		/* start adress buffer data for programing	*/
long	endadrbuf;	/* end adress buffer data for programing	*/
long	stadrdev;		/* start adress device for programing		*/
long	endadrdev;	/* end adress device for programing		*/
par	prgpar;		/* Varios parameters programmer 			*/
uint	select_b;		/* Select's byte flag 					*/
int	fill_c;
char 	d_firma_[16];
char 	d_type_[16];
char 	d_size_[20];
char 	d_vpp_[10];
char 	d_vcc_[10];
char 	d_vendor_[3];
char 	d_device_[3];
char 	d_algorithm_[20];
char 	d_ipulse_[10];
char 	d_cpulse_[10];
char 	d_opulse_[10];
int   	vpp_n;
int   	vcc_n;
int   	size_n;
int 	vendor_n;
int 	device_n;
ulong 	pulse_w_n;
ulong 	pulse_c_n;
ulong 	pulse_o_n;
int   	alg_n;
/*
need to set in two files (prg_dev, prg_set)
*/

char	buffer_common[W_BUF_SIZE(0, 0, num_prg+3, 79)];

int	one;
char	tag[20];
int	cs1=BLACK_WHITE;
int	cs2=WHITE_BLACK;

char	selss_buffer[W_BUF_SIZE(selss_ur, selss_lc, selss_lr, selss_rc)];
char	flag_ss=-2;

char	selq_buffer[W_BUF_SIZE(selq_ur, selq_lc, selq_lr, selq_rc)];
char	flag_q=-2;

char	selmn_buffer[W_BUF_SIZE(selmn_ur, selmn_lc, selmn_lr, selmn_rc)];
char	flag_mn=-2;

char	selst_buffer[W_BUF_SIZE(selst_ur, selst_lc, selst_lr, selst_rc)];
char	flag_st=-2;

int	devpower;
int	devtype;

char	full_fdn[90];

#define num_dev 11


LEData dev_firma =	 		/* p.2 */
	{
		0,				/* Promt's color */
		1,1,				/* Promt's position */
		D_FIRMA,
		0,				/* Field color */
		1, 36, 			/* Field position */
		15,				/* Length in columns */
		15,				/* Maximum number of char */
		CC_ANY,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_firma_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	


LEData dev_type =	 		/* p.2 */
	{
		0,				/* Promt's color */
		2,1,				/* Promt's position */
		D_TYPE,
		0,				/* Field color */
		2, 36, 			/* Field position */
		20,				/* Length in columns */
		20,				/* Maximum number of char */
		CC_ANY,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_type_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	


LEData dev_size =	 		/* p.2 */
	{
		0,				/* Promt's color */
		3, 1,			/* Promt's position */
		D_PROTOTYPE,
		0,				/* Field color */
		3, 36, 			/* Field position */
		20,				/* Length in columns */
		20,				/* Maximum number of char */
		CC_LETTER+CC_DIGIT,	/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_size_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		23,				/* Conditional paint color : int */
		17, 30,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_vpp =	 		/* p.2 */
	{
		0,				/* Promt's color */
		4, 1,			/* Promt's position */
		D_VPP,
		0,				/* Field color */
		4, 36, 			/* Field position */
		10,				/* Length in columns */
		10,				/* Maximum number of char */
		CC_DIGIT,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_vpp_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_vcc =	 		/* p.2 */
	{
		0,				/* Promt's color */
		5, 1,			/* Promt's position */
		D_VCC,
		0,				/* Field color */
		5, 36, 			/* Field position */
		10,				/* Length in columns */
		10,				/* Maximum number of char */
		CC_DIGIT,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_vcc_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_vendor	= 		/* p.2 */
	{
		0,				/* Promt's color */
		6, 1,			/* Promt's position */
		D_VENDOR,
		0,				/* Field color */
		6, 36, 			/* Field position */
		2,				/* Length in columns */
		2,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		d_vendor_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_device	= 		/* p.2 */
	{
		0,				/* Promt's color */
		7, 1,			/* Promt's position */
		D_DEVICE,
		0,				/* Field color */
		7, 36, 			/* Field position */
		2,				/* Length in columns */
		2,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		d_device_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	


LEData dev_algorithm =	 		/* p.2 */
	{
		0,				/* Promt's color */
		8, 1,			/* Promt's position */
		D_ALGORITHM,
		0,				/* Field color */
		8, 36, 			/* Field position */
		10,				/* Length in columns */
		10,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_ALGORITHM,		/* Array of mask char : char * */
		d_algorithm_,		/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	


LEData dev_ipulse	= 		/* p.2 */
	{
		0,				/* Promt's color */
		9, 1,			/* Promt's position */
		D_IPULSE,
		0,				/* Field color */
		9, 36, 			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		CC_DIGIT,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_ipulse_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_cpulse	= 		/* p.2 */
	{
		0,				/* Promt's color */
		10, 1,			/* Promt's position */
		D_CPULSE,
		0,				/* Field color */
		10, 36, 			/* Field position */
		3,				/* Length in columns */
		2,				/* Maximum number of char */
		CC_DIGIT,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_cpulse_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

LEData dev_opulse	= 		/* p.2 */
	{
		0,				/* Promt's color */
		11, 1,			/* Promt's position */
		D_OPULSE,
		0,				/* Field color */
		11, 36, 			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		CC_DIGIT,			/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		d_opulse_,			/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		1,				/* Insert mode cursor 0=none, 1=normal 2=big */
		2,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

FData1 d1[]=
	{				/* This is the FData1 structure: */
	
		LINE_EDITOR, &dev_firma, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_type,  	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_size, 		0, DeviceFieldCheck,
		LINE_EDITOR, &dev_vpp, 		0, DeviceFieldCheck,
		LINE_EDITOR, &dev_vcc, 		0, DeviceFieldCheck,
		LINE_EDITOR, &dev_vendor, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_device, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_algorithm, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_ipulse, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_cpulse, 	0, DeviceFieldCheck,
		LINE_EDITOR, &dev_opulse, 	0, DeviceFieldCheck,
		0
    };


void main(int argc, char **argv)
{
FData2	d2;
int	sel;
int	i;
int	j;
int	k,l;
char	*name;
char	*type;
struct device far *p;
int x;

	x=argc;

	name = malloc_e(20);
	type = malloc_e(20);

	read_var(argv[1], argv[2]);

	do
		{

		if (select_device(SEL_NEW, &name, &type) != -1)
			{
			Fields2(F_INIT,d1,&d2);
			copy_d_prg_dev(name, type);
			if (strcmp(name, new_device) != 0)
				strcpy(d_firma_, name);

			flag_common=0;
			open_window_dev(prgpar.c_all, 1, 0, 0, num_dev+2, 79, &flag_common, buffer_common);

   		Fields2(F_COLORS,d1,&d2,prgpar.c_all,prgpar.c_all);  /* Tell Fields2 to modify colors to new values */
			Fields2(F_DRAW,d1,&d2);				/* Draw fields */
			do
				{
				if (strcmp(name, new_device) == 0)
					d2.ActiveField = 0;
				else
					if (strcmp(type, new_type) == 0)
						d2.ActiveField = 1;
				else
					d2.ActiveField = 2;
/*
				SetCurPos(23,0); printf(" active field = %i ", d2.ActiveField);
*/
				one=0;
/*					d2.EventInfo = F_SET_POSITION;
*/
				DeviceFieldCheck(&d2);
				Fields2(F_ACTIVE,d1,&d2);				/* Activate, now our CheckF will be in control of program flow */

				sel=open_scroll_close(selq_ur, selq_lc, selq_lr, selq_rc, &flag_q, selq_buffer, sizeof(sel_q)/2-1, sel_q);

				}while(sel==2);	/*
								  	Save   = 1
								  	Change = 2
								  	Abort  = 3
								*/
			if (sel==1)
				{
				i=0;

				do
					{
				    		p = prg_dev+i;
						k=_fstrcmp(p->firma, d_firma_);
						l=_fstrcmp(p->type,  d_type_);
						j=_fstrcmp(p->firma, "");


/*					SetCurPos(20,0); printf("number=%i firma=%i type=%i end_of_file=%i", i, k, l, j);
					if(GetKey() == ESC)
						break;
*/
					i++;
					}while( (k|l) & j != 0);
				i--;

/*				SetCurPos(24,30); printf(" save in prg_devi[i]=%i ", i);
*/

				p= prg_dev+i;
				_fstrcpy(p->firma, d_firma_);
				_fstrcpy(p->type,  d_type_);
				p->size		= size_n;
				p->vpp		= vpp_n;
				p->vcc		= vcc_n;
				p->vendor 	= vendor_n;
				p->device 	= device_n;
				p->pulse_w	= (uint)pulse_w_n;
				p->pulse_c	= (uint)pulse_c_n;
				p->pulse_o	= (uint)pulse_o_n;
				p->algorithm	= alg_n;

				write_prg_dev(i, j);
				}
			Window(prgpar.c_all, 0, 0, num_dev+2, 79, &flag_common, buffer_common);
			}

		sel=open_scroll_close(selmn_ur, selmn_lc, selmn_lr, selmn_rc, &flag_mn, selmn_buffer, sizeof(sel_mn)/2-1, sel_mn);
	/*		
			SetCurPos(24,0); printf(" %i ", sel);
	*/
		}while(sel != 2 && sel != -1);	/* exit = 2 ESC == -1*/

	free(type);
	free(name);
}

int cmp(int *elem1, int *elem2)
{
	return(strcmp((char *)*elem1, (char *)*elem2));
}


int DeviceFieldCheck(FData2 *fd2)
{
int mess;
int tmpmess;
int sel;
int tmpi;

	mess=FieldCheck(fd2);	/* Let SCL1 FieldCheck function do most of the work */
	tmpmess=mess;

	do
		{

		switch (fd2->ActiveField)
		{
			case 0 : break;		/* Firma */
				
			case 1 : 				/* Device's type */
					if (tmpmess == LE_POSITION_UP) fd2->ActiveField = 2;
					break;
			case 2 : 				/* Device size (bytes) */
					sel = all_window(d_size_, fd2->ActiveField, sizeof(size_c)/2-1, size_c);
					if (sel != -1)
						{
						size_n = sel;
							strcpy(d_size_, size_[--size_n]);
						if (one==0)
							one++;
						if (tmpmess == F_POSITION_UP) fd2->ActiveField = 3;
						}
					Fields2(F_DRAW, d1, fd2);
					fd2->ActiveField = 3;
					mess = F_SET_POSITION;
					break;

			case 3 : 				/* Vpp voltage is (mV) */
					sel = all_window(d_vpp_, fd2->ActiveField, sizeof(vpp_c)/2-1, vpp_c);
					if (sel != -1)
						{
						vpp_n = sel;
						strcpy(d_vpp_, vpp_c[--vpp_n]);
						if (one==1)
							one++;
						if (tmpmess == F_POSITION_UP) fd2->ActiveField = 4;
						}
					Fields2(F_DRAW, d1, fd2);
					fd2->ActiveField = 4;
					mess = F_SET_POSITION;
					break;

			case 4 : 				/* Vcc during programming (mV) */
					sel = all_window(d_vcc_, fd2->ActiveField, sizeof(vcc_c)/2-1, vcc_c);
					if (sel != -1)
						{
						vcc_n = sel;
						strcpy(d_vcc_, vcc_c[--vcc_n]);
						if (one==2)
							one++;
						if (tmpmess == F_POSITION_UP) fd2->ActiveField = 5;
						mess = F_SET_POSITION;
						}
					Fields2(F_DRAW, d1, fd2);
					fd2->ActiveField = 5;
					mess = F_SET_POSITION;
					break;

			case 5 : 					/* Vendor ID is (HEX) */
					if (tmpmess == LE_POSITION_UP)
						{
						vendor_n = atox(d_vendor_);
						fd2->ActiveField = 6;
						}
					if (tmpmess == F_EXIT) fd2->ActiveField = 6;
					mess = F_SET_POSITION;
					break;

			case 6 : 					/* Device ID is (HEX) */
					if (tmpmess == LE_POSITION_UP)
						{
						device_n = atox(d_device_);
						fd2->ActiveField = 7;
						}
					if (tmpmess == F_EXIT) fd2->ActiveField = 7;
					mess = F_SET_POSITION;
					break;

			case 7 : 				/* Programming algorithm (N/I/Q) */
					CursorOff();
					tmpi = alg_n;
					sel = all_window(d_algorithm_, fd2->ActiveField, sizeof(alg_c)/2-1, alg_c);
					if (sel != -1)
						{
						alg_n = sel;
						strcpy(d_algorithm_, alg_c[--alg_n]);
						}
					switch (alg_n)
						{
						case 0: /* Normal */
								strcpy(d_cpulse_, NA);
								strcpy(d_opulse_, NA);
								break;

						case 1: /* Intelligent */
								if (tmpi != alg_n) /* old algorithm = new algorithm? */
									{
									strcpy(d_cpulse_, "0   "); pulse_c_n = 0;
									strcpy(d_opulse_, "0   "); pulse_o_n = 0;
									}
								break;

						case 2: /* Quik Pulse */
								if (tmpi != alg_n) /* old algorithm = new algorithm? */
									{
									strcpy(d_opulse_, NA); 	pulse_o_n = 0;
									strcpy(d_cpulse_, "0   ");pulse_c_n = 0;
									}
								break;
						}
					Fields2(F_DRAW, d1, fd2);
					fd2->ActiveField = 8;
 					mess = F_SET_POSITION;
					break;


			case 8 : 				/* Initial pulse width (us) */
					if (tmpmess == LE_POSITION_UP || tmpmess == F_EXIT)
						{
						pulse_w_n = atol(d_ipulse_);

						if ((pulse_w_n < 10) || (pulse_w_n > 50000))
							{
							message_screen(prgpar.c_error, "%s", RANGE_PULSE_ERROR);
							fd2->ActiveField = 8;
							mess = F_SET_POSITION;
							}
						else
							{
							switch (alg_n)
								{
								case 0: /* Normal */
										fd2->ActiveField = 9;
										mess = F_EXIT;
										break;

								case 1:
								case 2: /* Intelligent, Quick Pulse */
										fd2->ActiveField = 9;
										mess = F_SET_POSITION;
										break;
								}
							}
						}
					break;

			case 9 : 				/* Maximum pulse count */
					if (tmpmess == LE_POSITION_UP || tmpmess == F_EXIT)
						{
						pulse_c_n = atol(d_cpulse_);
						if ((pulse_c_n < 1) || (pulse_c_n > 100))
							{
							message_screen(prgpar.c_error, "%s", RANGE_PULSE_COUNT_ERROR);
							fd2->ActiveField = 9;
							mess = F_SET_POSITION;
							}
						else
							{
							switch (alg_n)
								{
								case 1: /* Intelligent */
										fd2->ActiveField = 10;
										mess = F_SET_POSITION;
										break;

								case 2: /* Quik Pulse */
										fd2->ActiveField = 10;
										mess = F_EXIT;
										break;
								}
							}
						}
					break;

			case 10 :				/* Overprogram pulse width (us) */
					if (tmpmess == LE_POSITION_UP || tmpmess == F_EXIT)
						{
						pulse_o_n = atol(d_opulse_);
						if ((pulse_o_n < 1) || (pulse_o_n > 50000))
							{
							message_screen(prgpar.c_error, "%s", RANGE_PULSE_OVER_ERROR);
							fd2->ActiveField = 10;
							mess = F_SET_POSITION;
							}
						else
						    	mess = F_EXIT;
						}
					break;
		}

		}while(
				 fd2->ActiveField==3 ||
				 fd2->ActiveField==2 ||
				 fd2->ActiveField==4 ||
				 fd2->ActiveField==7
			 );
	
	return(mess);
}

void write_prg_dev(int i, int j)
{
struct device prg_dev_small;
FILE *fp;
/*
	SetCurPos(24,20); printf("number of write%i  j=%i", sizeof(struct device)*i, j);
*/

	_fstrcpy(full_fdn, fsp);
	strcat(full_fdn, file_device_name);


	if((fp = fopen(full_fdn, "r+b")) == NULL)
		message_screen(prgpar.c_error, "%s", FILE_OPEN_ERROR);
	else
		{
		if (ftell(fp) == -1)
			message_screen(prgpar.c_error, "%s", FILE_SEEK_ERROR);
		else
			{
			if (fseek(fp, sizeof(struct device)*(long)i, SEEK_SET) != 0)
				message_screen(prgpar.c_error, "%s", FILE_SEEK_ERROR);
			else
				{
/*!!! kill		copy_small_far(i, &prg_dev_small);*/
		  
				_fmemcpy(&prg_dev_small, prg_dev+i, sizeof(struct device));



				if (fwrite(&prg_dev_small, sizeof(struct device), 1, fp) < 1)
						message_screen(prgpar.c_error, "%s", FILE_WRITE_ERROR);
				if (j==0)
					{
					i++;
					_fstrcpy((prg_dev+i)->firma, "");
					strcpy(prg_dev_small.firma, "");
					if (fwrite(&prg_dev_small, sizeof(struct device), 1, fp) < 1)
						message_screen(prgpar.c_error, "%s", FILE_WRITE_ERROR);
					}
				}
			}
		}
	fclose(fp);
}

int all_window(char *mess, int up, int ln, char *mass[])
{
int sel;
int i;

	Window(0, selss_ur+up, selst_lc, selss_ur+up, selst_rc, &flag_st, selst_buffer);
	for(i=selst_lc+strlen(mess)+1; i<selss_lc-1; i++)
		WriteScreen(prgpar.c_all, selss_ur+up, i, "-");
	WriteScreen(prgpar.c_all, selss_ur+up, i, ">");

	sel=open_scroll_close(selss_ur+up, selss_lc, selss_lr, selss_rc, &flag_ss, selss_buffer, ln, mass);
	Window(0, selss_ur+up, selst_lc, selss_ur+up, selst_rc, &flag_st, selst_buffer);

	return(sel);
}


/* Compares and returns greater than (1), less than (-1), or equal to (0).
 * This function is called by qsort and bsearch. When used with qsort the
 * order of entries is unimportant. When used with bsearch, elem1 is the
 * key to be found, and elem2 is the the current table entry.
 */
/*
int cmpgle( unsigned *elem1, unsigned *elem2 )
{
    if( *elem1 > *elem2 )
        return 1;
    else if( *elem1 < *elem2 )
        return -1;
    else
        return 0;
}
*/

