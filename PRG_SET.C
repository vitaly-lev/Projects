/*
	<C> <Lion>

	prg_menu.c

	Programm for Programmer on 8048

	Windows Subroutines

	911010	v1.0

	Functions:
			void set(void);
			void screen_par(void);
			int screen_out(int);
			void save_par(int []);
			void read_par_file(void);
			void set_def(void);
*/

#include "scl1.h"
#include "scl1clor.h"
#include "scl1keys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <dos.h>
#include "prg_def.h"

/*
			Functions prototype
*/
void	set_com(void);
void	screen_par(void);
void	save_par(void);
int	SetFieldCheck(FData2 *fd2);
void	set_prg(void);
void	set_file(void);
void	device_screen_on(void);
void	address_screen_on(void);
int	set_range(void);
		 		
/*
			External Functions prototype
*/
extern void	open_window(int, int, int, int, int, int, char *, char *);
extern void	open_window_dev(int, int, int, int, int, int, char *, char *);
extern void	address_screen_on(void);
extern void	address_screen_off(void);
extern void	device_screen_on(void);
extern void	device_screen_off(void);
extern void	j_exit(void);
extern void	set_port(void);
extern int	CommonFieldCheck(FData2 *fd2);
extern int	select_device(int, char **, char **);
extern void	*malloc_e(uint);
extern uint	atox(char *);
extern void	read_var(char *, char *);
extern void	set_chip(void);
extern int	copy_d_prg_dev(char *, char *);
extern void	test_vpp(void);
extern void	test_vcc(void);
extern void	test_pins(void);
extern void	set_color(void);
extern void	receive_block(void);
extern void	test_a4vpp(void);
extern void	message_screen(int, char *, ...);
extern void	read_f(void);
extern void	write_f(void);

/*
			External variable
*/
extern	char	*buffer_common;
extern	char	flag_common;
extern	char	file_write_name[];
extern	char	file_read_name[];
extern	char ext_hexed_name[];
extern	byte far	*bptr;
extern	char *new_device;
extern	char *new_type;
extern	long	stadrdev;
extern	long	endadrdev;
extern	long	stadrbuf;
extern	long	endadrbuf;
extern	long	latox(char *);
extern	struct device far *prg_dev;
extern	struct device small_prg_dev;
extern	long size_x[];
extern	int set_baud;
extern	int proc_number;
extern	int far *set_port_flag;
extern	char	*namefile;
extern	char far *fsp;
extern	uint csum;
extern	void _far *ad;
extern	uint _far *a;

/*
			Local variable
*/

par	prgpar;
char	*buffer_set;
char	flag_set=-2;
int	port;
int	speed;
int	set_one;
char	full_fpn[90];


FData2 fwprg2;


SData1 sp1[]=
	{
		set_ur+1, set_lc+1, "COM1",
		set_ur+2, set_lc+1, "COM2",
		set_ur+3, set_lc+1, "COM3",
		set_ur+4, set_lc+1, "COM4",

		0
	};

SData1 ss1[]=
	{
		set_ur+1, set_lc+10, "38400",
		set_ur+2, set_lc+10, "19200",
		set_ur+3, set_lc+10, " 9600",
		set_ur+4, set_lc+10, " 1200",
		0
	};

SData2 sp2={0,0,0,"",0,0,0,0};
SData2 ss2={0,0,0,"",0,0,0,0};


FData1 fds1[]=
	{
		SELECT,sp1,&sp2,CommonFieldCheck,
		SELECT,ss1,&ss2,CommonFieldCheck,
		0
	};

char yes_mask[] = "YyNn";
char blank[4];

LEData fwprg_blank =	 		/* p.2 */
	{
		0,				/* Promt's color */
		1,7,				/* Promt's position */
		"Blank check device before programming : ", 	/* String */
		0,				/* Field color */
		1, 47,			/* Field position */
		1,				/* Length in columns */
		1,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		yes_mask,			/* Array of mask char : char * */
		blank,			/* Destination buffer : char * */
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

char write_p[4];

LEData fwprg_write =	 		/* p.2 */
	{
		0,				/* Promt's color */
		2,2,				/* Promt's position */
		"Test WRITE POSSIBILITY if device not clear : ", 	/* String */
		0,				/* Field color */
		2, 47,			/* Field position */
		1,				/* Length in columns */
		1,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		yes_mask,			/* Array of mask char : char * */
		write_p,			/* Destination buffer : char * */
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

LEData fwprg_firma =	 		/* p.2 */
	{
		0,				/* Promt's color */
		3,2,				/* Promt's position */
		"                      Default Firma select : ",
		0,				/* Field color */
		3, 47,			/* Field position */
		20,				/* Length in columns */
		20,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		small_prg_dev.firma,/* Destination buffer : char * */
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

LEData fwprg_device =	 		/* p.2 */
	{
		0,				/* Promt's color */
		4,2,				/* Promt's position */
		"                     Default Device select : ",
		0,				/* Field color */
		4, 47,			/* Field position */
		20,				/* Length in columns */
		20,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		small_prg_dev.type,	/* Destination buffer : char * */
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


char file_read[80];

LEData fwfile_read =	 		/* p.2 */
	{
		0,				/* Promt's color */
		5,2,				/* Promt's position */
		"            Directory & name for Read file : ",
		0,				/* Field color */
		5, 47,			/* Field position */
		30,				/* Length in columns */
		80,				/* Maximum number of char */
		CC_SEARCH,		/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		file_read,		/* Destination buffer : char * */
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

char file_write[80];

LEData fwfile_write =	 		/* p.2 */
	{
		0,				/* Promt's color */
		6,2,				/* Promt's position */
		"           Directory & name for Write file : ",
		0,				/* Field color */
		6, 47,			/* Field position */
		30,				/* Length in columns */
		80,				/* Maximum number of char */
		CC_SEARCH,		/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		file_write,		/* Destination buffer : char * */
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

char fill_byte[4];

LEData ffill_byte =	 		/* p.2 */
	{
		0,				/* Promt's color */
		7,2,				/* Promt's position */
		"                    Fill buffer byte (HEX) : ",
		0,				/* Field color */
		7, 47,			/* Field position */
		3,				/* Length in columns */
		3,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,				/* Array of mask char : char * */
		fill_byte,		/* Destination buffer : char * */
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


char editor_name[80];

LEData ext_editor =	 		/* p.2 */
	{
		0,				/* Promt's color */
		8,2,				/* Promt's position */
		"    Ext. Hex Editor (Full: path, name.ext) : ",
		0,				/* Field color */
		8, 47,			/* Field position */
		30,				/* Length in columns */
		80,				/* Maximum number of char */
		CC_SEARCH,	  	/* Array of mask char : char * */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		editor_name,		/* Destination buffer : char * */
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


FData1 fwprg1[]=  			/* This is the FData1 structure: */
	{
		LINE_EDITOR, 	&fwprg_blank, 		0, 		SetFieldCheck,
		LINE_EDITOR, 	&fwprg_write,	 	0, 		SetFieldCheck,
		LINE_EDITOR,	&fwprg_firma,		0,		SetFieldCheck,
		LINE_EDITOR,	&fwprg_device,		0,		SetFieldCheck,
		LINE_EDITOR, 	&fwfile_read, 		0, 		SetFieldCheck,
		LINE_EDITOR, 	&fwfile_write,	 	0, 		SetFieldCheck,
		LINE_EDITOR,	&ffill_byte,		0,		SetFieldCheck,
		LINE_EDITOR,	&ext_editor,		0,		SetFieldCheck,
		0,
	};




void main(int argc, char **argv)
{
int x;

	x = argc;
	
	read_var(argv[1], argv[2]);

	set_port();

	switch (proc_number)
		{
		case PROC_TEST_VPP:		test_vpp();	break;
		case PROC_TEST_VCC:		test_vcc(); 	break;
		case PROC_TEST_PINS:	test_pins(); 	break;
		case PROC_SET_COM:		set_com();	break;
		case PROC_SET_COLOR:	set_color();	break;
		case PROC_PRG_SET:		set_prg();	break;
		case PROC_TEST_A4VPP:	test_a4vpp(); 	break;
		case PROC_READ_FILE:	read_f();		break;
		case PROC_WRITE_FILE:	write_f();	break;
		}

	j_exit();
}


void set_chip(void)
{
int	sel;
char	*name;
char	*type;

	name = malloc_e(20);
	type = malloc_e(20);

	address_screen_off();
	device_screen_off();

	if ((sel=select_device(SEL_ROM, &name, &type)) != -1)
		{

/*		if (sel == 0)
			{
			sel = ident(ROM27512);
			}

		if (sel != 0)
			{
			}
*/
/*kill	copy_small_far(sel, &small_prg_dev);
*/		_fmemcpy(&small_prg_dev, prg_dev+sel, sizeof(struct device));

		stadrdev = 0;
		endadrdev = size_x[small_prg_dev.size];
		stadrbuf = 0;
		endadrbuf = endadrdev;
		}

	address_screen_on();
	device_screen_on();

	free(type);
	free(name);
}


void set_com(void)
{
FData2 fd2;
uint *i;

	Fields2(F_INIT,fds1,&fd2);

	if ((buffer_set = malloc_e(W_BUF_SIZE(set_ur, set_lc, set_lr, set_rc))) != NULL)
		{
		open_window(prgpar.c_all, 1, set_ur, set_lc, set_lr, set_rc, &flag_set, buffer_set);
	
		sp2.Position = prgpar.port;
		ss2.Position = prgpar.speed;

   	Fields2(F_COLORS,fds1,&fd2,prgpar.c_all,prgpar.c_all);
		Fields2(F_DRAW,fds1,&fd2); 
		Fields2(F_ACTIVE,fds1,&fd2);

		if(fd2.ActiveField==4 || fd2.EventInfo==ESC)
			{
			prgpar.port = sp2.Position;
			prgpar.speed = ss2.Position;
			*set_port_flag = TRUE;
			}

		Window(prgpar.c_menu, set_ur, set_lc, set_lr, set_rc, &flag_set, buffer_set);
		free(buffer_set);
		save_par();
		_fmemcpy(i, a+8, 2); FP_OFF(ad) = *i; _fmemcpy(ad, &prgpar, sizeof(prgpar));
		}
}

int SetFieldCheck(FData2 *fd2)
{
int mess;
/*LEData *pl;*/

	/* Let SCL1 FieldCheck function do most of the work */
	mess=FieldCheck(fd2);

	if (fd2->ActiveField == 1 && mess == LE_POSITION_UP)
	{
		set_chip();
		strcpy(prgpar.prg_firma, small_prg_dev.firma);
		strcpy(prgpar.prg_device,small_prg_dev.type);

		Fields2(F_DRAW,fwprg1,&fwprg2);				/* Draw fields */
		fd2->ActiveField = 4;
		mess = F_SET_POSITION;
	}
	else
	if (fd2->ActiveField == 4 && mess == LE_POSITION_DOWN)
	{
		set_chip();
		strcpy(prgpar.prg_firma, small_prg_dev.firma);
		strcpy(prgpar.prg_device,small_prg_dev.type);

		Fields2(F_DRAW,fwprg1,&fwprg2);				/* Draw fields */
		fd2->ActiveField = 1;
		mess = F_SET_POSITION;
	}

/*
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


	else
		if (fd2->Message == LE_BUFFER_END)
		{
			pl = fwprg1[fd2->ActiveField].Structure1;
			ch = pl->Buffer[0];

			if (ch == 'Y' || ch == 'y')
			{
				printf("yes");
				pl->Buffer = "yes";
				mess = F_POSITION_UP;
			}
			else
			{
				printf("no ");
				pl->Buffer = "no ";
				mess = F_POSITION_UP;
			}
		}
*/
	return(mess);

}


void save_par(void)
{
FILE *fp;
par *parptr=&prgpar;


	_fstrcpy(full_fpn, fsp);
	strcat(full_fpn, file_par_name);

	if((fp = fopen(full_fpn,"wb")) != NULL)
	{
		if ((fwrite(parptr,sizeof(prgpar),1,fp)) != 0)
			fclose(fp);
		else
			message_screen(prgpar.c_error, "%s", FILE_WRITE_PAR_ERROR);
	}
	else
		message_screen(prgpar.c_error, "%s", FILE_OPEN_PAR_ERROR);
}




void set_prg(void)
{
	Fields2(F_INIT,fwprg1,&fwprg2);				/* Call Fields2 with a F_INIT message */

	fwprg_blank.Buffer[0] = prgpar.prg_blank;
	fwprg_write.Buffer[0] = prgpar.prg_write_p;

	_fmemcpy(&small_prg_dev, prg_dev+copy_d_prg_dev(prgpar.prg_firma, prgpar.prg_device), sizeof(struct device));

	strcpy(fwfile_read.Buffer, prgpar.file_read);
	strcpy(fwfile_write.Buffer, prgpar.file_write);
	strcpy(ext_editor.Buffer, prgpar.ext_hexed);
	itoa(prgpar.fill_byte, fill_byte, 16);

	if ((buffer_common = malloc_e(W_BUF_SIZE(0, 0, sizeof(fwprg1)/sizeof(FData1), 79))) != NULL)
		{
		open_window_dev(prgpar.c_all, 1, 0, 0, sizeof(fwprg1)/sizeof(FData1), 79, &flag_common, buffer_common);

   	Fields2(F_COLORS,fwprg1,&fwprg2,prgpar.c_all,prgpar.c_all);  /* Tell Fields2 to modify colors to new values */
		Fields2(F_DRAW,fwprg1,&fwprg2);				/* Draw fields */
		Fields2(F_ACTIVE,fwprg1,&fwprg2);				/* Activate, now our CheckF will be in control of program flow */

		/* Check the FData2 structure to see if the user presses ESCAPE
		or selects the CANCEL mouse button for exiting */

		if(fwprg2.EventInfo==ESC)
			{
			prgpar.prg_blank   = (char)tolower((int)fwprg_blank.Buffer[0]);
			prgpar.prg_write_p = (char)tolower((int)fwprg_write.Buffer[0]);
			strcpy(prgpar.prg_firma,  small_prg_dev.firma);
			strcpy(prgpar.prg_device, small_prg_dev.type);
			
			strcpy(prgpar.file_read,  fwfile_read.Buffer);
			strcpy(file_read_name,    fwfile_read.Buffer);
			
			strcpy(prgpar.file_write, fwfile_write.Buffer);
			strcpy(file_write_name,   fwfile_write.Buffer);

			strcpy(prgpar.ext_hexed,	 ext_editor.Buffer);
			strcpy(ext_hexed_name, 	 ext_editor.Buffer);
			
			prgpar.fill_byte = (byte)atox(fill_byte);
			}

		Window(prgpar.c_all,  0, 0, sizeof(fwprg1)/sizeof(FData1), 79, &flag_common, buffer_common);
		free(buffer_common);
		save_par();
		}
}
					




