/*
	<C> <Lion>

	prg_menu.c

	Programm for Programmer on 8048

	Windows Subroutines

	931020	v2.1

	Functions:
*/

#include "prg_def.h"
#include <stdio.h>
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <bios.h>
#include <process.h>
#include <spawno.h>

/*
			Functions prototype
*/
int	main_menu(void);
void	open_window(int color, int type, int ur, int lc, int lr, int rc, char *flag, char *buffer);
int	hi(int color);
int	in(int color);
void	help_alt(void);
void	run_ext_editor(char *);
long	read_binary(FILE *);
int	my_spawn(void);

/*
			External functions prototype
*/
extern	void		read_f(void);
extern	void		write_f(void);
extern	void		read_chip_o(void);
extern	void		read_chip_p(void);
extern	void		j_exit(void);
extern	void		prg_set_chip(char *);
extern	void		prg_set_power(char *);
extern	void		reset_prg(void);
extern	void 	prg_run_test(void);
extern	void		dump_o(char *, byte *);
extern	void		prg_chip(void);
extern	void		utdev(void);
extern	void		set_com(void);
extern	void 	hexasc(uint d, char *ss);
extern	int		set_range(void);
extern	void		prom(void);
extern	void		select_chip(void);
extern	void		test_vpp(void);
extern	void		test_vcc(void);
extern	void		verify_chip(char);
extern	void		clear(void);
extern	void		blank_check(uint *);
extern	void		write_poss(uint *);
extern	void		set_color(void);			
extern	void		set_prg(void);
extern	void		set_chip(void);
extern	void		device_screen_on(void);
extern	void		address_screen_on(void);
extern	void		device_screen_off(void);
extern	void		address_screen_off(void);
extern	void		dos_shell(void);
extern	void		test_pins(void);
extern	void		*malloc_e(uint);
extern	void		ident(int);
extern	void		sp(char *, int);
extern	void		set_port(void);
extern	void		message_screen(int, char *, ...);
/*
			Global variables
*/
FILE 	*hex_file;
char 	tmp[80];
uint 	d1,d2;
int 	chip;
int 	vpp;

/*
int 	f_19200	= 0;
int 	f_9600	= 1;
int 	f_4800	= 0;
int 	f_2400	= 0;
int 	f_1200	= 0;
int 	f_300	= 0;

int 	f_com1	= 0;
int 	f_com2	= 1;
*/

int	w_namdev;
int	w_prg;
int	w_range;
int	w_end_prg;
int	sw;		/*Main Window*/
int	w_ibm;
int	w_prom;
int	w_prom_err;
/*
			External variable
*/
extern	byte	tx_buffer[];
extern	byte	rx_buffer[];
extern	int	rx_count;
extern	int	timer_1;
extern	int	block_tx;
extern	long	stadrbuf;		/* start adress buffer data for programing	*/
extern	long	endadrbuf;	/* end adress buffer data for programing	*/
extern	long	stadrdev;		/* start adress device for programing		*/
extern	long	endadrdev;	/* end adress device for programing		*/
extern	char	typebus;		/* 16 or 8 or lo or hi bus 				*/
extern	uint	curadrbuf;	/* current adress buffer data for programing	*/
extern	uint	curnumdev;	/* current device number for programing		*/
extern	char	*seleoa[];
extern	par	prgpar;
extern	int	fill_c;
extern	char namefile[];
extern	uint csum;
extern	uint select_b;			/* Select's byte flag 					*/
extern	struct device far *prg_dev;
extern	char	*buffer_common;
extern	char	flag_common;
extern	int	cf;
extern	byte	far *bptr;
extern	int far *set_port_flag;
extern	int set_baud;
extern	char	flag_h;
extern	char	*envptr;			/* for debug */
extern	char *buffer_main;		/* for debug */
/*=================================================================*/
/*=================================================================*/
/*=================================================================*/


/*--------------------------*
 *	F U N C T I O N S   *
 *--------------------------*/

/*
	Main function for open menu windows
*/

MSBar mmb[]=

	{
		1,   6, 	ALTF, " File ",
		7,  11, 	ALTC, " CHIP ",
		13, 20, 	ALTB, " Buffer ",
		21, 29, 	ALTH, " HexEdit ",
		30, 36, 	ALTS, " Setup ",
		37, 43, 	ALTT, " Tests ",
	};

MSOptions mmof[]=

	{
		2,	4,	" Read file  ",	'R',
		3,	4,	" Write file ",	'W',
		4,	4,	" DOS Shell  ",	'D',
		5,	4,	" Exit       ",	'E',
	};

MSOptions mmor[]=

	{
		2,	10,	" Select CHIP              ",	'S',
		3,	10,	" Programming CHIP         ",	'P',
		4,	10,	" Auto Setup PRG Chip      ",	'A',
		5, 	10,	" Blank check CHIP         ",	'B',
		6,	10,	" Write possibility        ",	'W',
		7,	10,	" Default Programmer setup ",	'D',
		8,	10,	" CHIP'S Data Base set     ",	'C',
	};

MSOptions mmob[]=

	{
		2,	16, " ORIGINAL -> buffer read      ", 'O',
		3,	16, " PROGRAMMING -> buffer read   ", 'P',
		4,	16, " Compare PROGRAMMING & buffer ", 'C',
		5,	16, " Fill Buffer                  ", 'F',
		6,	16, " Address range set            ", 'A',
	};

MSOptions mmoe[]=

	{	2,	24,	" HEX Editor (internal) ",	'H',
		3,	24,	" HEX Editor (external) ",	'E',
	};

MSOptions mmos[]=

	{
		2,	30,	" Port COM setup ", 'P',
		3,	30,	" Color Setup    ",	'C',
	};

MSOptions mmot[]=

	{
		2,	37,	" Set varios VPP   ", 'P',
		3,	37,	" Set varios VCC   ", 'C',
		4,	37,	" Set A4/VPP       ", '4',
		5,	37,	" All pins test    ", 'A',
	};

char wb[200];

MSWindow mmw[]=

	{
		1,	3,	6,	17,	4,	wb,	mmof,
		1,	9,	9,	36,	7,	wb,	mmor,
		1,	15,	7,	47,	5,	wb,	mmob,
		1,	23,	4,	47,	2,	wb,	mmoe,
		1,	29,	4,	47,	2,	wb,	mmos,
		1,	36,	6,	56,	4,	wb,	mmot,
	};

MSData mmd=
		{
			/* bar-menu colors */
		  BLACK_WHITE, WHITE_BLACK+HIGHLIGHT,

			/* pull-down menu colors */
		  WHITE_BLACK, BLACK_WHITE, WHITE_BLACK+HIGHLIGHT,

		  mmb, mmw, 6, 0, 0, 0
		};


char *alt_screen[]=
	{
 		"Ctrl-A Set Address range",
 		"Ctrl-B Blank Check Device",
 		"Ctrl-C Set Device",
 		"Ctrl-E HEX Editor (internal)",
 		" Alt-E HEX Editor (external)",
 		"Ctrl-F Fill Buffer",
		"Ctrl-I Inteligent Identifier read",
		"Ctrl-H This Help screen",
 		"Ctrl-L Read Original Device",
 		"Ctrl-V Verify Programming Device & Buffer",
		"Ctrl-R Read File from Disk",
 		"Ctrl-W Write File to Disk",
 		"Ctrl-X Exit from Program",
		"Ctrl-Z DOS Shell",
		"",
 		"Press any key",
		00
	};

char	*buffer_alt;
char	flag_alt=-2;


int main_menu(void)
{
int k=0;
int key;
int i=0;

	mmd.BarNColor = prgpar.c_all;
	mmd.BarRColor = hi(prgpar.c_all);
	mmd.MenuNColor= prgpar.c_all;
	mmd.MenuRColor= in(prgpar.c_all);
	mmd.MenuHColor= hi(prgpar.c_all);

	MenuSystem(MS_SHADOW_OFF,	(MSData *)0);	/* Draw shadow 	*/
	MenuSystem(MS_ALT_ON,		(MSData *)0);	/* ALT sensitive 	*/
	MenuSystem(MS_DRAW,			       &mmd);	/* Draw			*/

	device_screen_on();
	address_screen_on();
	
	do
	{
		if ((k = KeyReady()) != 0)
		{
			k = MenuSystem(MS_KEY, &mmd, k);


			if (KeyReady())
				k = GetKey();

			switch (k)
			{
				case CTRLA : set_range();   				break;
				case	CTRLB : blank_check(&key);			break;
				case CTRLC : set_chip();					break;
				case CTRLE : sp(PRG_HEXEDITOR,0);			break;
				case  ALTE : run_ext_editor(prgpar.ext_hexed);break;
				case CTRLF : clear();					break;
				case CTRLI : ident(ROM27512);				break;
				case CTRLH : help_alt();					break;
				case  KEYH : help_alt();					break;
				case    F1 : help_alt();					break;
				case CTRLL : read_chip_o();				break;
				case CTRLV : verify_chip('s'); 			break;
				case CTRLR : sp(PRG_SET, PROC_READ_FILE);	break;
				case CTRLW : sp(PRG_SET, PROC_WRITE_FILE);	break;
				case CTRLX : 							break;
		 		case CTRLZ : dos_shell();   				break;
			}
		}
		else
		{
			k = MenuSystem(MS_CHECK, &mmd);
		 
			if (k == MS_SELECT)
				{
					switch(mmd.Menu)
					{

					case 1: /* File */

						switch(mmd.Option)
						{
						case 1 : sp(PRG_SET, PROC_READ_FILE);	break;
						case 2 : sp(PRG_SET, PROC_WRITE_FILE);	break;
						case 3 : dos_shell(); break;
						case 4 : k=ALTX;	break;
						}
						break;

					case 2: /* ROM */
		
						switch(mmd.Option)
						{
						case 1 : set_chip();			break;
						case 2 : prg_chip();			break;
						case 3 : ident(ROM27512);		break;
						case 4 : blank_check(&key);		break;
						case 5 : write_poss(&key);		break;
						case 6 : sp(PRG_SET, PROC_PRG_SET);break;
						case 7 : sp(PRG_DEVICE,0);		break;
						}
						break;

					case 3: /* Buffer */

						switch(mmd.Option)
						{
						case 1 : read_chip_o();	  break;
						case 2 : read_chip_p();	  break;
						case 3 : verify_chip('s'); break;
						case 4 : clear(); 		  break;
						case 5 : set_range(); 	  break;
						}
						break;

					case 4: /* Edit */

						switch(mmd.Option)
						{
						case 1 : sp(PRG_HEXEDITOR,0); 			break;
						case 2 : run_ext_editor(prgpar.ext_hexed); 	break;
						}
						break;

					case 5: /* Setup */

						switch(mmd.Option)
						{
						case 1 : 	sp(PRG_SET, PROC_SET_COM);
						    		if (*set_port_flag == TRUE)
									{
									j_exit();
									set_port();
									set_baud = FALSE;
									*set_port_flag = FALSE;
									}
								break;

						case 2 : sp(PRG_SET, PROC_SET_COLOR); break;
						}
						break;

					case 6: /* Tests */

						switch(mmd.Option)
						{
						case 1 : sp(PRG_SET, PROC_TEST_VPP);  break;
						case 2 : sp(PRG_SET, PROC_TEST_VCC);  break;
						case 3 : sp(PRG_SET, PROC_TEST_A4VPP);break;
						case 4 : sp(PRG_SET, PROC_TEST_PINS); break;
						}
						break;

					} /* switch mmd.Option */
				}
		}
	}while (k != ALTX);

	device_screen_off();
	address_screen_off();

	return(0);
}

void help_alt(void)
{
int i=0;

	if ((buffer_alt = malloc_e(W_BUF_SIZE(alt_ur, alt_lc, alt_lr, alt_rc))) != NULL)
		{
		open_window(prgpar.c_all, 1, alt_ur, alt_lc, alt_lr, alt_rc, &flag_alt, buffer_alt);

		while (alt_screen[i] != NULL)
			WriteScreen(prgpar.c_all, alt_ur+i, alt_lc+2, alt_screen[i++]);

		GetKey();
		Window(prgpar.c_all, alt_ur, alt_lc, alt_lr, alt_rc, &flag_alt, buffer_alt);
		free(buffer_alt);
		}
}



void run_ext_editor(char *name)
{
FILE *tf;
int num;
char *buffer_h;
byte *tmpbuf;
int i;
int flag=TRUE;
char *full_name;
char *tmp;

	j_exit();

	if ((full_name=malloc_e(90)) != NULL &&
	    (tmpbuf=malloc_e(2048))   != NULL &&
	    (buffer_h=malloc_e(W_BUF_SIZE(h_ur, h_lc, h_lr, h_rc))) !=NULL)
		{
		if (strlen(name) == 0)
			message_screen(prgpar.c_error, "\n%s", EXT_EDITOR_NOT_SET);
		else
			{

			if ((tmp = getenv("TMP")) != NULL)
				{
				strcpy(full_name, tmp);
				strcat(full_name, TMP_FILE_NAME);
				}
			else
				{
				strcpy(full_name, TMP_FILE_NAME);
				strcpy(tmp, (".."));
				}

			if ((tf=fopen(full_name, "wb")) == NULL)
				message_screen(prgpar.c_error, "%s\n(%s)", ERROR_CREATE_TMP_FILE, full_name);
			else
				{
				num=(int)((endadrbuf+1)/2048);

				for (i=0; i<num; i++)
					{
					_fmemcpy(tmpbuf, bptr, 2048);
					if (fwrite(tmpbuf, 2048, 1, tf) != 1)
						{
						flag=FALSE;
				 		break;
						}
					}

				if (!flag || (fclose(tf) != 0))
					message_screen(prgpar.c_error, "%s\n(%s)", ERROR_WRITE_TMP_FILE, full_name);
				else
					{

					Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);
					
					
					if (spawnlo(tmp, name, full_name, full_name, NULL) == -1)
						message_screen(prgpar.c_error, "%s\n(%s)", COULD_NOT_LOAD_EXT_EDITOR, name);
					else
						{
						if ((tf=fopen(full_name, "rb")) == NULL)
							message_screen(prgpar.c_error, "%s\n(%s)", ERROR_LOAD_TMP_FILE, full_name);
						else
							read_binary(tf);
						}
					
					Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);
					}
				}
			}
		}

	free(buffer_h);
	free(tmpbuf);
	free(full_name);

	remove(full_name);
	set_port();
	CursorOff();
}

long read_binary(FILE *binary_file)
{
long l;
int num=0;
int numr;
long lenb;
int i;
int num2;
int j;
byte *lp;
int parity=0, start=0;


	if ((lp = malloc_e(1024)) == NULL)
		return(0);

	switch (select_b)
		{
		case 0: parity = 1;
			   start  = 0;
			   break;

		case 1: parity = 2;
			   start  = 0;
			   break;

		case 2: parity = 2;
			   start  = 1;
			   break;
		}

	l = filelength(fileno(binary_file));
	if (l == 0l)
		return(message_screen(prgpar.c_error, "%s", FILE_LENGTH_ZERRO));
	else
	{
		lenb = endadrbuf-stadrbuf+1;
		if (lenb < 0)
			return(message_screen(prgpar.c_error, "%s", SET_RANGE_ERROR));
		else
		{
			if (l < lenb)
				numr = (int)(l/1024);
			else
				numr = (int)(lenb/1024);

			for (i=0; i<numr; i++)
			{
				num = fread(lp, 1, 1024, binary_file);
				
				if (num < 1024)
					return(message_screen(prgpar.c_error, "%s", FILE_READ_ERROR));

				for (j=0; j<1024; j++)
					*(bptr + stadrbuf + (i*1024*parity) + (j*parity) + start) = *(lp+j);
			}
		 	
			if ((l % 1024) != 0)
			{
				if (lenb < 1024)
				{
			 		if (l < lenb)
						num2 = (int)l;
					else
						num2 = (int)lenb;
				}
				else
				{
					if (l <= lenb)
						num2 = (int)(l % 1024);
					else
						num2 = (int)(lenb-(numr*1024));
			   	}
				
				num = fread(lp, 1, num2, binary_file);

				for (j=0; j<num2; j++)
					*(bptr + stadrbuf + (numr*1024*parity) + (j*parity) + start) = *(lp+j);

				if (num < num2)
					return(message_screen(prgpar.c_error, "%s", FILE_READ_ERROR));
			}
		}

		fclose(binary_file);
		return(num);
	}
}

