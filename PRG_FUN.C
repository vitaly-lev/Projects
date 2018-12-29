/*
	<C> <Lion>

	prg_main.c

	Programm for Programmer on 8048

	Varios Function

	911010	v1.0

	Functions:
			uint 	atox(char *str);
			long	latox(char *str);

!!! whay bust[]... need initializing zerro?

*/

#include "prg_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include <process.h>
#include <spawno.h>
#include <memory.h>
#include <malloc.h>
#include <ctype.h>
#include <io.h>

/*
			Functions prototype
*/
uint 	atox(char *str);
long	latox(char *str);
void	dos_shell(void);
void	open_window(int, int, int, int, int, int, char *, char *);
void	address_screen_on(void);
void	address_screen_off(void);
void	device_screen_on(void);
void	device_screen_off(void);
int	CommonFieldCheck(FData2 *fd2);
int	hi(int);
int	in(int);
int	select_device(int, char **, char **);
void	open_window_dev(int, int, int, int, int, int, char *, char *);
int	open_scroll(int, int, int, int, char *, char *, int, char *[]);
int	open_scroll_close(int, int, int, int, char *, char *, int, char *[]);
void	*malloc_e(uint mem);
void	set_def(void);
void	first_set(void);
int	copy_d_prg_dev(char *, char *);
int	search_dev(byte, byte);
void	load_prg_small(struct device far *p);
void	write_chip_screen(void);
void	write_chip_no(byte mc, byte dc);
int	read_device_file(void);
void	read_par_file(void);
void	message_screen(int, char *, ...);
void 	sel_state(int *, uint *);
uint	GetEsc(void);
int	my_yes_no(int, char *, ...);
int	wprintf(int color, int _ur, int _lc, char *format, ...);
void	path_name(char *full_name, char *path, char *name);


/*
			External functions prototipe
*/
extern	int	ident(int);
extern	void receive_block(void);
extern	void	send_block(int);
extern	int	receive_b(int);
extern	void	j_exit(void);
extern	void	set_port(void);
extern	char far *fsp;
extern	char	flag_h;

/*
			Global variables
*/

char	prg_buffer[W_BUF_SIZE(prgm_ur, prgm_lc, prgm_lr, prgm_rc)];
char	flag_prg=0;

char	err_buffer[W_BUF_SIZE(errm_ur, errm_lc, errm_lr, errm_rc)];
char	flag_err=0;

FData2	fdr2;
char 	*new_device = "New Device";
char 	*new_type = "New Type";
struct 	device small_prg_dev;
int 	d_firma;
int 	d_type;
char	devst[20]="\0";
char	deved[20]="\0";
char	bufst[20]="\0";
char	bufed[20]="\0";
int	set_one;
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
int 	size_n;
int 	vendor_n;
int 	device_n;
ulong 	pulse_w_n;
ulong 	pulse_c_n;
ulong 	pulse_o_n;
int   	alg_n;
char	*buffer_common;
char	flag_common=-2;
char	file_write_name[80];
char	file_read_name[80];
char	ext_hexed_name[80];
char	full_fdn[90];
char	full_fpn[90];
char	*envptr;
char	flag_h=-2;


uint	csum;		/* Buffer Control Sum*/
long	stadrbuf;		/* start adress buffer data for programing	*/
long	endadrbuf;	/* end adress buffer data for programing	*/
long	stadrdev;		/* start adress device for programing		*/
long	endadrdev;	/* end adress device for programing		*/
uint	select_b;		/* Flags:	all  bytes select_b = 0*/
					/*		odd	bytes select_b = 1*/
					/*		even bytes select_b = 2*/

char *vcc_c[] =	/* replace in file prg_dev.c !!!*/
{
	"5.0",
	"6.0",
	"6.5",
	NULL,
};

char *vpp_c[] =	/* replace in file prg_dev.c !!!*/
{
	"12.5",
	"13.0",
	"21.0",
	"23.0",
	"25.0",
	NULL,
};

char *size_[] =	/* replace in file prg_dev.c !!!*/
{
	"2716   ",
	"2732   ",
	"2764   ",
	"27128  ",
	"27256  ",
	"27512  ",
	"8048   ",
	"8051   ",
	"8755   ",
	"27c64  ",
	"27c128 ",
	"27c256 ",
	"27c512 ",
	NULL
};

char *alg_c[] =
{
	"Normal",
	"Inteligent",
	"QuickPulse",
	NULL,
};

long size_x[] =
{
	0x000007ff,
	0x00000fff,
	0x00001fff,
	0x00003fff,
	0x00007fff,
	0x0000ffff,
	0x000007ff,
	0x000007ff,
	0x000007ff,
	0x00001fff,
	0x00003fff,
	0x00007fff,
	0x0000ffff,
	0x000007ff,
};

/*
			External variable
*/
extern	int		mes_prg_flag;
extern	int		mes_ibm_flag;
extern	int		mes_err_flag;
extern	long		stadrdev;
extern	long		endadrdev;
extern	long		stadrbuf;
extern	long		endadrbuf;
extern	int		cf;
extern	int		fill_c;
extern	par 		prgpar;
struct	device	small_prg_dev;
extern	char 	*vpp_c[];
extern	struct device far *prg_dev;
extern	uint 	select_b;
extern	byte far	*bptr;
extern	MSData 	mmd;

char	buffer_device[W_BUF_SIZE(device_ur, device_lc, device_lr, device_rc)];
char	flag_device=-2;

char	buffer_address[W_BUF_SIZE(address_ur, address_lc, address_lr, address_rc)];
char	flag_address=-2;
char	selfi_buffer[W_BUF_SIZE(selfi_ur, selfi_lc, selfi_lr, selfi_rc)];
char	flag_fi=-2;
char	seld_buffer[W_BUF_SIZE(seld_ur, seld_lc, seld_lr, seld_rc)];
char	flag_d=-2;
			    
LEData	rdevst=
	{
		0,				/* Promt's color */
		address_ur+1, address_lc+2,		/* Promt's position */
		"Addr device : ", 				/* String */
		0,							/* Field color */
		address_ur+1, address_lc+2+14,	/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		devst,			/* Destination buffer : char * */
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


LEData	rdeved=
	{
		0,				/* Promt's color */
		address_ur+1, address_lc+2+19,			/* Promt's position */
		"-",					  				/* String */
		0,									/* Field color */
		address_ur+1, address_lc+2+20,			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		deved,			/* Destination buffer : char * */
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


LEData	rbufst=
	{
		0,				/* Promt's color */
		address_ur+2, address_lc+2,				/* Promt's position */
		"Addr buffer :",	 			/* String */
		0,					 				/* Field color */
		address_ur+2, address_lc+2+14,			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		bufst,			/* Destination buffer : char * */
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


LEData	rbufed=
	{
		0,				/* Promt's color */
		address_ur+2, address_lc+2+19,		/* Promt's position */
		"-",					 			/* String */
		0,					 			/* Field color */
		address_ur+2, address_lc+2+20,		/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		bufed,			/* Destination buffer : char * */
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


SData1 sb1[]=			/* This is select's first structure */
	{
        address_ur+5, address_lc+2,  "ALL",
        address_ur+5, address_lc+11, "ODD",
		address_ur+5, address_lc+20, "EVEN",
		0
	};

SData2 sb2={0, address_ur+3, address_lc+2, "Select byte ",0,0,0,0};

FData1 fdr1[]=
	{				/* This is the FData1 structure: */
	
		LINE_EDITOR,&rdevst,0,CommonFieldCheck,
		LINE_EDITOR,&rdeved,0,CommonFieldCheck,
		LINE_EDITOR,&rbufst,0,CommonFieldCheck,
		LINE_EDITOR,&rbufed,0,CommonFieldCheck,
		SELECT,sb1,&sb2,CommonFieldCheck,
		0
    };



int select_device(int first, char **name, char **type)
{
int	i=0;
int	j=0;
int	k;
int	l;
char	device_name[50][20];
char	*items_n[50];
char	device_type[50][20];
char	*items_d[50];
int	sel_dev;
int	sel_type;

	if (first == SEL_AUTO)
		{
		strcpy(&device_name[0][0], AUTO);
		*items_n = &device_name[0][0];
		j=1;
		i=1;
		}
	else
	if (first == SEL_ROM)
		{
		i=1;
		}
	do
		{
		_fstrcpy(&device_name[j][0], (prg_dev+i)->firma);
		k=0;
		do
			{
		    	if (strcmp(&device_name[j][0], &device_name[k][0]) != 0)
				k++;
			else
				break;
			}
		while(k<j);

		if (k==j || j==0)
			{
			*(items_n+j) = &device_name[j][0];
			j++;
			}
		i++;

		}
	while (_fstrcmp((prg_dev+i)->firma, "") != 0);

	*(items_n+j) = NULL;

	sel_dev = open_scroll(selfi_ur, selfi_lc, selfi_lr, selfi_rc, &flag_fi, selfi_buffer, j, items_n);

	if (sel_dev != -1)
		{
		sel_dev--;

		if (sel_dev == 0 && first == SEL_AUTO)
			{
			Window(0, selfi_ur, selfi_lc, selfi_lr, selfi_rc, &flag_fi, selfi_buffer);
			return 0;
			}
		else
		if (sel_dev == 0 && first == SEL_NEW)
			{
			strcpy(*name, new_device);
			Window(0, selfi_ur, selfi_lc, selfi_lr, selfi_rc, &flag_fi, selfi_buffer);
			return 0;
			}
		else
			{
			strcpy(*name, &device_name[sel_dev][0]);

			if (first == SEL_NEW)
				{
				*(items_d) = new_type;
				j=i=1;
				}
			else
				j=i=0;

			while (_fstrcmp(prg_dev[j].firma, "") != 0)
				{
				if (_fstrcmp((prg_dev+j)->firma, *name) == 0)
					{
					_fstrcpy(&device_type[i][0], (prg_dev+j)->type);
					*(items_d+i) = &device_type[i][0];
					i++;
					}
				j++;
				}

			*(items_d+i) = NULL;
	/*			qsort((void *)items_d, i, sizeof(int), cmpgle);*/

 			sel_type = open_scroll_close(seld_ur, seld_lc, seld_lr, seld_rc, &flag_d, seld_buffer, i, items_d);
			if (sel_type != -1)
				{
				if (--sel_type == 0 && first == SEL_NEW)
					strcpy(*type, new_type);
				else
					strcpy(*type, device_type[sel_type]);
				}
			else
				i=-1;
			}
			
		}
	else
		i=-1;

	Window(0, selfi_ur, selfi_lc, selfi_lr, selfi_rc, &flag_fi, selfi_buffer);

	if (i!=-1)
		{
		i=0;
		do
			{
			k=_fstrcmp((prg_dev+i)->firma, *name);
			l=_fstrcmp((prg_dev+i)->type,  *type);
			j=_fstrcmp((prg_dev+i)->firma, "");

			i++;
			}while( (k|l) & j != 0);
		i--;
		}
	return(i);
}



void set_def(void)
{
	set_one			= 1;
	prgpar.port		= 0;				/* COM1						*/
	prgpar.speed		= 2;				/* Speed = 9600				*/
	prgpar.c_menu 		= 0x35;			/* Color Main Manu				*/
	prgpar.c_all		= 0x03;			/* Color All Windows			*/
	prgpar.c_normal	= 0x03;			/* Color Normal Message			*/
	prgpar.c_error		= 0x6b;			/* Color Error Message			*/
	prgpar.c_hex_border	= 0x0a;			/* Color HEX EDITOR Border		*/
	prgpar.c_hex_hex	= 0x0e;			/* Color HEX EDITOR Hex values	*/
	prgpar.c_hex_asc	= 0x0e;			/* Color HEX EDITOR Asc values	*/
	strcpy(prgpar.prg_firma, "????????");	/* Device's Firma				*/
	strcpy(prgpar.prg_device,"????????");	/* Default device = 2764			*/
	prgpar.prg_blank	= 'n';
	prgpar.prg_write_p	= 'n';
	prgpar.fill_byte	= 0xff;
	strcpy(prgpar.ext_hexed, '\0');
}

void first_set(void)
{
int sel;
long i;

	strcpy(file_write_name, prgpar.file_write);
	strcpy(file_read_name, prgpar.file_read);
	strcpy(ext_hexed_name, prgpar.ext_hexed);

	_fmemcpy(&small_prg_dev, prg_dev+(sel=copy_d_prg_dev(prgpar.prg_firma, prgpar.prg_device)), sizeof(struct device));
	
	if (sel==0)
		{
		strcpy(small_prg_dev.firma, "????????");
		strcpy(small_prg_dev.type,  "????????");
		}

	stadrdev = 0;
	endadrdev = size_x[small_prg_dev.size];
	stadrbuf = 0;
	endadrbuf = endadrdev;
	select_b = 0; /* ALL */


	for (i=0; i<=endadrdev; i++)
		*(bptr+i) = prgpar.fill_byte;

}

int copy_d_prg_dev(char *name, char *type) /* must be write in two files:prg_dev, prg_set */
{
int	sel;
int	k,l,j;
struct device far *p;

	sel=0;
	do
		{
			p = prg_dev+sel;
			k=_fstrcmp(p->firma, name);
			l=_fstrcmp(p->type,  type);
			j=_fstrcmp(p->firma, "");
			sel++;
		}while( (k|l) & j != 0);
	sel--;

	if (j==0)
		sel=0;

	load_prg_small(prg_dev+sel);
	
	return(sel);
}


int search_dev(byte mc, byte dc)
{
int	sel;
int	k,l,j;
struct device far *p;

	sel=0;
	do
		{
			p = prg_dev+sel;
			k = p->vendor;
			l = p->device;
			j=_fstrcmp(p->firma, "");
			sel++;
		}while((!((k == (int)mc) && (l == (int)dc))) && j != 0);
	sel--;

	if (j==0)
		sel=0;

	if (sel != 0)
		load_prg_small(prg_dev+sel);
	return sel;
}

void load_prg_small(struct device far *p)
{
	_fstrcpy(d_firma_, p->firma);
	_fstrcpy(d_type_,  p->type);

	strcpy(d_size_, size_[size_n = p->size]);
	strcpy(d_vpp_, vpp_c[vpp_n = p->vpp]);
	strcpy(d_vcc_, vcc_c[vcc_n = p->vcc]);
	itoa(vendor_n  = p->vendor, d_vendor_, 16);
	itoa(device_n  = p->device, d_device_, 16);
	ltoa(pulse_w_n = p->pulse_w, d_ipulse_, 10);
	ltoa(pulse_c_n = p->pulse_c, d_cpulse_, 10);
	ltoa(pulse_o_n = p->pulse_o, d_opulse_, 10);
	strcpy(d_algorithm_, alg_c[alg_n = p->algorithm]);
	
	switch (alg_n) /* algoritm number */ 
		{
		case 0: /* Normal */
				strcpy(d_cpulse_, NA);
				strcpy(d_opulse_, NA);
				break;

		case 1: /* Intelligent */
				break;

		case 2: /* Quik Pulse */
				strcpy(d_opulse_, NA);
				break;
		}
}

#define chip_ur 2
#define chip_lc 15
#define chip_lr chip_ur+15
#define chip_rc chip_lc+50
char chip_flag=-2;
char *chip_buffer;

void write_chip_screen(void)
{
char *temp;

	chip_buffer = malloc_e(W_BUF_SIZE(chip_ur, chip_lc, chip_lr, chip_rc));
	temp = malloc_e(256);

	open_window(prgpar.c_all, 1, chip_ur, chip_lc, chip_lr, chip_rc, &chip_flag, chip_buffer);
	
	wprintf(prgpar.c_all, chip_ur+1,  chip_lc+1, "%s%s", D_FIRMA, d_firma_);
	wprintf(prgpar.c_all, chip_ur+2,  chip_lc+1, "%s%s", D_TYPE, d_type_);

	wprintf(prgpar.c_all, chip_ur+4,  chip_lc+1, "%s%s", D_VPP, d_vpp_);
	wprintf(prgpar.c_all, chip_ur+5,  chip_lc+1, "%s%s", D_VCC, d_vcc_);
	wprintf(prgpar.c_all, chip_ur+6,  chip_lc+1, "%s%s", D_VENDOR, d_vendor_);
	wprintf(prgpar.c_all, chip_ur+7,  chip_lc+1, "%s%s", D_DEVICE, d_device_);
	wprintf(prgpar.c_all, chip_ur+8,  chip_lc+1, "%s%s", D_ALGORITHM, d_algorithm_);
	wprintf(prgpar.c_all, chip_ur+9,  chip_lc+1, "%s%s", D_IPULSE, d_ipulse_);
	wprintf(prgpar.c_all, chip_ur+10, chip_lc+1, "%s%s", D_CPULSE, d_cpulse_);
	wprintf(prgpar.c_all, chip_ur+11, chip_lc+1, "%s%s", D_OPULSE, d_opulse_);
	wprintf(prgpar.c_all, chip_ur+13, chip_lc+1, "%s", PRESS_ESC);

	GetKey();
	Window(prgpar.c_all, chip_ur, chip_lc, chip_lr, chip_rc, &chip_flag, chip_buffer);

	free(temp);
	free(chip_buffer);
}

void write_chip_no(byte mc, byte dc)
{
char *temp;

	chip_buffer = malloc_e(W_BUF_SIZE(chip_ur, chip_lc, chip_lr, chip_rc));
	temp = malloc_e(256);

	open_window(prgpar.c_all, 1, chip_ur, chip_lc, chip_lr, chip_rc, &chip_flag, chip_buffer);
	
	wprintf(prgpar.c_all, chip_ur+6,  chip_lc+1, "%s%02X", D_VENDOR, mc);
	wprintf(prgpar.c_all, chip_ur+7,  chip_lc+1, "%s%02X", D_DEVICE, dc);
	wprintf(prgpar.c_all, chip_ur+9,  chip_lc+1, "%s", NO_DEVICE);
	wprintf(prgpar.c_all, chip_ur+13, chip_lc+1, "%s", PRESS_ESC);

	GetKey();
	Window(prgpar.c_all, chip_ur, chip_lc, chip_lr, chip_rc, &chip_flag, chip_buffer);

	free(temp);
	free(chip_buffer);
}

int read_device_file(void)
{

FILE *fp;
int i=0;
int ret=0;
int c;
struct device prg_dev_small;

	_fstrcpy(full_fdn, fsp);
	strcat(full_fdn, file_device_name);

	fp = fopen(full_fdn, "rb");

	if ((fp) == NULL || filelength(fileno(fp)) < sizeof(struct device))
		{
		if (YesNo(prgpar.c_error, prgpar.c_all, 1, FILE_NOT_EXIST) == 1)
			{
			fclose(fp);
			_fstrcpy(prg_dev->firma, new_device);
			_fstrcpy(prg_dev->type, new_type);
			prg_dev->size		= 0;
			prg_dev->vpp		= 0;
			prg_dev->vcc		= 0;
			prg_dev->vendor	= 0xff;
			prg_dev->device	= 0xff;
			prg_dev->pulse_w	= 1000;
			prg_dev->pulse_c	= 15;
			prg_dev->pulse_o	= 3000;
			prg_dev->algorithm	= 1;

			_fstrcpy((prg_dev+1)->firma, "");
			ret=1;
			i=1;
			if ((fp = fopen(full_fdn, "wb")) == NULL)
				message_screen(prgpar.c_error, "%s", FILE_OPEN_ERROR);
			else
/*kill			copy_small_far(0, &prg_dev_small);
*/				_fmemcpy(&prg_dev_small, prg_dev, sizeof(struct device));

				if (fwrite(&prg_dev_small, sizeof(struct device), 1, fp) == 0)
					message_screen(prgpar.c_error, "%s", FILE_ERROR);
				else
					{
/*kill				copy_small_far(1, &prg_dev_small);
*/					_fmemcpy(&prg_dev_small, prg_dev+1, sizeof(struct device));

					if (fwrite(&prg_dev_small, sizeof(struct device), 1, fp) == 0)
						message_screen(prgpar.c_error, "%s", FILE_ERROR);
					else
						if (fclose(fp) != 0)
							message_screen(prgpar.c_error, "%s", FILE_ERROR);
					}
			}
		}
		else
		{
		i=0;
		do
			{
			c = fread(&prg_dev_small,sizeof(struct device),1,fp);
/* kill		copy_far_small(i, &prg_dev_small);
*/			_fmemcpy(prg_dev+i, &prg_dev_small, sizeof(struct device));

			i++;
			}
		while (c == 1);

		if (i == 1 && c == 0)
			message_screen(prgpar.c_error, "%s", FILE_DEVICE_ERROR);
		fclose(fp);
		}
	return(ret);
}

void read_par_file(void)
{
FILE *fp;
int c;
par *parptr=&prgpar;

	_fstrcpy(full_fpn, fsp);
	strcat(full_fpn, file_par_name);

	if (set_one == 0)
		{
		if((fp = fopen(full_fpn,"rb")) == NULL)
			{
			set_def();
			}
		else
			{
			c = fread(parptr,sizeof(prgpar),1,fp);
			if (c != 1)
				{
					message_screen(prgpar.c_error, "%s", PAR_FILE_ERROR);
					set_def();
					fclose(fp);
				}
			else
				{
				fclose(fp);
				first_set();
				set_one = 1;
				}
			}
		}
	select_b = 0;

}

uint atox(char *str)
{
uint i, id;

	id=0;
	for (i=0; i<strlen(str); i++)
	{
		switch (toupper(str[i]))
		{
			case 'A' : {id = id*16 + 10; break;}
			case 'B' : {id = id*16 + 11; break;}
			case 'C' : {id = id*16 + 12; break;}
			case 'D' : {id = id*16 + 13; break;}
			case 'E' : {id = id*16 + 14; break;}
			case 'F' : {id = id*16 + 15; break;}
			default  :  id = id*16 + (int)(str[i]-'0');
		}
	}
	return(id);
}

long latox(char *str)
{
uint i;
long id;

	id=0;
	for (i=0; i<strlen(str); i++)
	{
		switch (toupper(str[i]))
		{
			case 'A' : {id = id*16 + 10; break;}
			case 'B' : {id = id*16 + 11; break;}
			case 'C' : {id = id*16 + 12; break;}
			case 'D' : {id = id*16 + 13; break;}
			case 'E' : {id = id*16 + 14; break;}
			case 'F' : {id = id*16 + 15; break;}
			default  :  id = id*16 + (int)(str[i]-'0');
		}
	}
	return(id);
}


void dos_shell(void)
{
char *buffer_h;

	j_exit();

	if ((buffer_h = malloc_e(W_BUF_SIZE(h_ur, h_lc, h_lr, h_rc))) != NULL)
		{
		Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);

		CursorOn();
		Cls(WHITE_BLACK, 0, 0, 24, 79);
		SetCurPos(24,0);
		printf("%s", TYPE_EXIT_TO_RETURN);

		if( spawnlo("e:", envptr, envptr, NULL) == -1 )
			message_screen(prgpar.c_error, "%s", COULD_NOT_SPAWN_PROCESS);
		
		Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);
	
		free(buffer_h);

		CursorOff();
		}
	else
		{
		message_screen(prgpar.c_error, "%s", NOT_ENOUGH_MEMORY_FOR_SPAWN);
		}
	
	set_port();

}

void open_window(int color, int type, int ur, int lc, int lr, int rc, char *flag, char *buffer)
{
	Window(BLACK_BLACK,		ur, lc, lr, rc, flag, buffer);
/*	FillBlock(cf,			ur, lc, lr, rc, fill_c);
	FillBlock(cf,			ur+1, rc+1, lr, rc, ' ');
*/
	GSSBox(color,	type,	ur, lc, lr-1, rc-1, 0, 0, 0);
}

int hi(int color)
{
	return(color+8);
}

int in(int color)
{
	return( (color>>4 & 0x000f) | (color << 4 & 0x00f0));
}

void device_screen_on(void)
{
	open_window(prgpar.c_all, 1, device_ur, device_lc, device_lr, device_rc, &flag_device, buffer_device);

	wprintf(prgpar.c_all, device_ur+1, device_lc+1, "Company : %s", small_prg_dev.firma);
	wprintf(prgpar.c_all, device_ur+2, device_lc+1, " Device : %s", small_prg_dev.type);
	wprintf(prgpar.c_all, device_ur+3, device_lc+1, "    Vpp : %sV", vpp_c[small_prg_dev.vpp]);
}

void device_screen_off(void)
{
	Window(prgpar.c_all, device_ur, device_lc, device_lr, device_rc, &flag_device, buffer_device);
}

int set_range(void)
{
	Fields2(F_ACTIVE,fdr1,&fdr2);          		/* Activate, now our CheckF will be in control of program flow */

	/* Check the FData2 structure to see if the user presses ESCAPE
	or selects the CANCEL mouse button for exiting */

	if(fdr2.EventInfo==ESC)
	{
		stadrbuf	= latox(&bufst[0]);
		endadrbuf	= latox(&bufed[0]);
		stadrdev	= latox(&devst[0]);
		endadrdev	= latox(&deved[0]);
		select_b 	= sb2.Position;
		return(0);
	}		  
}

void address_screen_on(void)
{
	open_window(prgpar.c_all, 1, address_ur, address_lc, address_lr, address_rc, &flag_address, buffer_address);

	sprintf(bufst,"%5.5lX\0", stadrbuf);
	sprintf(bufed,"%5.5lX\0", endadrbuf);
	sprintf(devst,"%5.5lX\0", stadrdev);
	sprintf(deved,"%5.5lX\0", endadrdev);

   Fields2(F_INIT,fdr1,&fdr2);           					/* Call Fields2 with a F_INIT message */
   Fields2(F_COLORS,fdr1,&fdr2,prgpar.c_all,prgpar.c_all);  	/* Tell Fields2 to modify colors to new values */
   Fields2(F_DRAW,fdr1,&fdr2);			     			/* Draw fields */
}

void address_screen_off(void)
{
	Window(prgpar.c_all, address_ur, address_lc, address_lr, address_rc, &flag_address, buffer_address);
}

void open_window_dev(int color, int type, int ur, int lc, int lr, int rc, char *flag, char *buffer)
{
	Window(color,		ur, lc, lr, rc, flag, buffer);
	GSSBox(color,	type,	ur, lc, lr, rc, 0, 0, 0);
}

int open_scroll(int ur, int lc, int lr, int rc, char *flag, char *buffer, int lines, char *b[])
{
int lines_s;

	Window(prgpar.c_menu,    ur, lc, lr, rc, flag, buffer);
	if (lines <= (lines_s=lr-ur-ur))
		lines_s = lines;
	if (lines_s <=2)
		lines_s = lines;

	return(ScrollList(prgpar.c_menu, hi(prgpar.c_menu), ur+1, lc+1, lines_s, b));
}

int open_scroll_close(int ur, int lc, int lr, int rc, char *flag, char *buffer, int lines, char *b[])
{
int sel;
	sel = open_scroll(ur, lc, lr, rc, flag, buffer, lines, b);
	Window(0, ur, lc, lr, rc, flag, buffer);
	return(sel);
}

void *malloc_e(uint mem)
{
void *p;


	if ((p=malloc(mem)) == NULL)
		message_screen(prgpar.c_error, "%s", MALLOC_ERROR);
	else
		return p;
}



int CommonFieldCheck(FData2 *fd2)
{
int mess;
/*LEData *pl;*/

	/* Let SCL1 FieldCheck function do most of the work */
	mess=FieldCheck(fd2);


	if (fd2->Message == LE_ILLEGAL_KEY)
	{
		if (fd2->EventInfo == UP)
			mess = F_POSITION_DOWN;
		else
		if (fd2->EventInfo == DOWN)
			mess = F_POSITION_UP;
		else
		{
			Beep();
			mess = F_POSITION_STAY;
		}
	}

	return(mess);

}

void sel_state(int *sel, uint *mul)
{
	switch (select_b)
		{
	case 0 : 	*sel = 0;	
			*mul = 1;
			break;

	case 1 :  if ((stadrbuf % 2) == 0)
				*sel = 0;
			else
				*sel = 1;
			*mul = 2;
			break;

	case 2 :	if ((stadrbuf % 2) != 0)
				*sel = 0;
			else
				*sel = 1;
			*mul = 2;
			break;
		}

}

/*
	Full-screen window version of printf that takes row, column, textcolor,
	and background color as its first arguments, followed by normal printf
	format strings (except that \t is not handled). You can specify -1 for
	any of the first arguments to use the current value. The function returns
	the number of characters printed, or a negative number for errors.
*/
int wprintf(int color, int _ur, int _lc, char *format, ...)
{
short ret;
va_list marker;
char *buffer;

	/* It's probably safe to use a buffer length of 512 bytes or five times
	 * the length of the format string.
	 */

	if( (buffer = malloc_e( 256 )) == NULL )
		return -1;

	/* Write text to a string and output the string. */
	va_start( marker, format );
	ret = vsprintf( buffer, format, marker );
	va_end( marker );

	WriteScreen(color, _ur, _lc, buffer);

	free( buffer );
	return ret;
}

void message_screen(int color, char *format, ...)
{
char *temp;
va_list marker;
int ret;
char *sym;
char *sym_old;
int line;
int col;
int flag=TRUE;
int num;
char flag_all=-2;
byte *all_buffer;

	
	if ((temp = malloc_e(256)) != NULL && (all_buffer = malloc_e(W_BUF_SIZE(all_ur, all_lc, all_lr, all_rc))) != NULL)
		{
		va_start(marker, format);
		ret = vsprintf(temp, format, marker);
		va_end(marker);
		
		if (ret)
			{
			Window(BLACK_BLACK, all_ur, all_lc, all_lr, all_rc, &flag_all, all_buffer);
			Cls(color,  	all_ur, all_lc, all_lr, all_rc);
			Box(color, 0, 	all_ur, all_lc, all_lr, all_rc);
			WriteScreen(color, all_lr, all_rc-sizeof(PRESS_ESC_KEY)-1, PRESS_ESC_KEY);
			
			line = all_ur+1;
			sym_old = temp;

			do
				{
				if ((sym = strchr(sym_old, '\n')) != NULL)
					{
					num = sym - sym_old;
					col = all_lc + (((all_rc - all_lc) - num)/2);
					}
				else
					{
					num = temp+strlen(temp) - sym_old;
					col = all_lc + (((all_rc - all_lc) - num)/2);
					flag = FALSE;
					}

/*				SetCurPos(24,0); printf("flag=%1x line=%i col=%i num=%i sym_old=%x end_p=%x %s",
									flag, line, col, sym-sym_old, sym_old, temp+strlen(temp), sym_old);

*/				if (num != 0)
					WriteScreenLen(color, line, col, num, sym_old);
				
				line++;
				sym_old = sym+1;
				}
			while (flag);

			GetKey();
			Window(BLACK_BLACK, all_ur, all_lc, all_lr, all_rc, &flag_all, all_buffer);
			}

  		free(all_buffer);
		free(temp);
		}
}

uint GetEsc(void)
{
	if (KeyReady())
		return(GetKey());
	else
		return(0);
}

int my_yes_no(int color, char *format, ...)
{
char *temp;
va_list marker;
int ret;
char *sym;
char *sym_old;
int line;
int col;
int flag=TRUE;
int num;
int c_yes;
int c_no;
int y_n=FALSE;
int flag_exit=TRUE;
int c_temp;
char flag_ac=-2;
char *ac_buffer;


	if ( (temp = malloc_e(256)) != NULL &&
		(ac_buffer = malloc_e(W_BUF_SIZE(ac_ur, ac_lc, ac_lr, ac_rc))) != NULL
	   )
		{
		va_start(marker, format);
		ret = vsprintf(temp, format, marker);
		va_end(marker);
		
		if (ret)
			{
			Window(BLACK_BLACK, ac_ur, ac_lc, ac_lr, ac_rc, &flag_ac, ac_buffer);
			Cls(color,  	ac_ur, ac_lc, ac_lr, ac_rc);
			Box(color, 0, 	ac_ur, ac_lc, ac_lr, ac_rc);
			WriteScreen(color, ac_lr, ac_rc-sizeof(PRESS_ESC_KEY_FOR_CANCEL)-1, PRESS_ESC_KEY_FOR_CANCEL);
			line = ac_ur+1;
			sym_old = temp;

			do
				{
				if ((sym = strchr(sym_old, '\n')) != NULL)
					{
					num = sym - sym_old;
					col = ac_lc + (((ac_rc - ac_lc) - num)/2);
					}
				else
					{
					num = temp+strlen(temp) - sym_old;
					col = ac_lc + (((ac_rc - ac_lc) - num)/2);
					flag = FALSE;
					}

/*				SetCurPos(24,0); printf("flag=%1x line=%i col=%i num=%i sym_old=%x end_p=%x %s",
									flag, line, col, sym-sym_old, sym_old, temp+strlen(temp), sym_old);
*/
				if (num != 0)
					WriteScreenLen(color, line, col, num, sym_old);
				
				line++;
				sym_old = sym+1;
				}
			while (flag);

			
			do
				{
				if (y_n==FALSE)
					{
					c_yes = color;
					c_no  = (color>>4 & 0x0f) | (color<<4 & 0xf0);
					}
				else
					{
					c_yes = (color>>4 & 0x0f) | (color<<4 & 0xf0);
					c_no  = color;
					}

				WriteScreen(c_yes, ac_lr-1, ac_lc+10, YES_MES);
				WriteScreen(c_no,  ac_lr-1, ac_rc-sizeof(NO_MES)-10, NO_MES);
				
				switch (GetKey())
					{
					case ESC:
						y_n = FALSE;
						flag_exit = TRUE;
						break;

					case RIGHT:
					case LEFT:
						c_temp = c_yes;
						c_no   = c_yes;
						c_yes  = c_no;
						y_n    = ~y_n;
						flag_exit = FALSE;
						break;
					
					case KEYY:
					case KEYy:
						y_n = TRUE;
						flag_exit = TRUE;
						break;

					case KEYN:
					case KEYn:
						y_n = FALSE;
						flag_exit = TRUE;
						break;

					case ENTER:
						flag_exit = TRUE;
						break;

					default:
						TSound(1000, 8);
						break;
					}
				}
			while (flag_exit != TRUE);

			Window(BLACK_BLACK, ac_ur, ac_lc, ac_lr, ac_rc, &flag_ac, ac_buffer);
			}
  
		free(ac_buffer);
		free(temp);
		}
	return(y_n);
}


/*
	divide full string (path+name of file) to path & name

	input: string with path & file name in variable *full_name

	output:	file name in variable *name
		   	path in variable *path
*/
void path_name(char *full_name, char *path, char *name)
{
char *t_name;
int len;

	if (
		((t_name=strrchr(full_name, (int)'\\')) != NULL) ||
		((t_name=strrchr(full_name, (int)':')) != NULL)
	   )
		{
		len=(strlen(full_name)-strlen(t_name));
		strncpy(path, full_name, len);
		strcpy(path+len, "\\\0");
		strcpy(name, full_name+strlen(path));
		}
	else
		{
		strcpy(path, '\0');
		strcpy(name, full_name);
		}
}

