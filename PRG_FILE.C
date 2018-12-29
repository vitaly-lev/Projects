/*
	<C> <Lion>

	prg.c

	Programm for Programmer on 8048

	Subroutines for work with files

	920226	V1.0 Beta6

	Functions:
			int	read_hex(FILE *, byte *);
			long	read_binary(FILE *, byte *);
			int 	read_f(char *, byte *);
			int 	write_f(char *, byte *);
			int 	conv_hex(char *,byte *);
*/


#include <stdio.h>
#include <string.h>
#include <io.h>
#include <ctype.h>
#include "prg_def.h"
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include <stdlib.h>

/*
			Functions prototype
*/
int		read_hex(FILE *);
long 		read_binary(FILE *);
void 		read_f(void);
void 		write_f(void);
int 		conv_hex(char *);
void 		write_disk(FILE *, long, long, int);

/*
			Local variable
*/
char 	flag_f=-2;
char	f_buffer[W_BUF_SIZE(f_ur, f_lc, f_lr, f_rc)];

char 	flag_fw=-2;
char	fw_buffer[W_BUF_SIZE(fw_ur, fw_lc, fw_lr, fw_rc)];

char	fwbuff_st_addr[15];
char	fwbuff_end_addr[15];
byte 	*a;		/* for "write_disk" subroutine */
char 	file_read_name[80];

/*
			External variable
*/
extern	byte	far *bptr;
extern	int 	chip;
extern	long endadrdev;
extern	par	prgpar;	/* Varios parameters programmer 	*/
extern	long	stadrbuf;
extern	long	endadrbuf;
extern	uint	select_b;
extern	byte	*rx_buffer;
extern	char far *frn;
extern	char far *fwn;
/*
			External functions prototype
*/
extern	uint atox(char *str);
extern	void	open_window(int, int, int, int, int, int, char *, char *);
extern	int	ClFieldCheck(FData2 *);
extern	long	latox(char *);
extern	void	open_window_dev(int, int, int, int, int, int, char *, char *);
extern	int	in(int);
extern	int	hi(int);
extern	int	my_yes_no(int, char *, ...);
extern	void	message_screen(int, char *, ...);
extern	void	*malloc_e(uint mem);
/*
*/

char	file_write_name[80];

LEData fwname = 			/* p.2 */
	{
		0,				/* Promt's color */
		9, 18,			/* Promt's position */
		"File name : ", 	/* String */
		WHITE_BLACK,		/* Field color */
		9, 30,			/* Filed position */
		30,				/* Length in columns */
		80,				/* Maximum number of char */
		CC_FILESPEC+CC_PATH,/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		0,				/* Array of mask char : char * */
		file_write_name, 	/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		2,				/* Insert mode cursor 0=none, 1=normal 2=big */
		1,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	


LEData fwbuff_st = 		/* p.2 */
	{
		0,				/* Promt's color (Seting in routine) */
		11, 18,			/* Promt's position */
		"Buffer START addr (hex) : ", 	/* String */
		0,				/* Field color (Setting in routine) */
		11, 45,			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		fwbuff_st_addr,	/* Destination buffer : char * */
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

LEData fwbuff_end = 		/* p.2 */
	{
		0,				/* Promt's color */
		12, 18,			/* Promt's position */
		"Buffer END   addr (hex) : ", 	/* String */
		0,				/* Field color */
		12, 45,			/* Field position */
		5,				/* Length in columns */
		5,				/* Maximum number of char */
		0,				/* Type of charter accepted p.10 */
		0,				/* 0 accept char in mask , 1 discard chars in mask */
		CC_HEX,			/* Array of mask char : char * */
		fwbuff_end_addr,	/* Destination buffer : char * */
		0,				/* Chars used in format string : char * */
		0,				/* Array of exit keys scan/acsii code : uint * */
		0,				/* 1 = Insert ON, 0 = typeover */
		2,				/* Insert mode cursor 0=none, 1=normal 2=big */
		1,				/* Typeover mode cursor */
		0,				/* Conditional paint color : int */
		0, 0,			/* Start and End position for conditional paint */
		0,				/* Return information : EventInfo */
		0,				/* Current possition into buffer (offset) uint */
		0,				/* Horizontal scroll counter 0=none */
		0,				/* =1 if line was edited */
	};	

SData1 fwsel1[]=			/* This is select's first structure p.176 */
	{       
   	14,38,"ALL",
   	14,45,"ODD",
		14,53,"EVEN",
		0
   };

int fwselect;

SData2 fwsel2=
	{
		0,				/* Promt's color */
		14, 18,			/* Promt's position */
		"Selected bytes   : ", 	/* String */
		0,				/* Exit keys : uint * */
		1,				/* Number of option avaliable : int */
		0,				/* Default selection */
		0,				/* Return information : EventInfo */
	};

MBData fwmb1=
	{
		0, 				/* Color used when button is not selected */
		0,	 			/* Color used for a selected button */
		16, 19,			/* Upper row & left  column for Mouse area position */
		16, 26,			/* Lower row & right column for Mouse area position */
		16, 20,			/* Possition for Mouse Button Text */
		"Write           Press ESC to Cancel",			/* String */
		0,				/* If a box shell be drawn around a Mouse Button */
		1,				/* Active flag */
		0,				/* Exit keys : uint * */
		0,				/* Return information : EventInfo */
	};

MBData fwmb2=
	{
		23, 				/* Color used when button is not selected */
		112, 			/* Color used for a selected button */
		16, 40,			/* Upper row & left  column for Mouse area position */
		16, 40,			/* Lower row & right column for Mouse area position */
		16, 40,			/* Possition for Mouse Button Text */
		"Press ESC to Cancel",			/* String */
		0,				/* If a box shell be drawn around a Mouse Button */
		0,				/* Active flag */
		0,				/* Exit keys : uint * */
		0,				/* Return information : EventInfo */
	};


FData1 fw1[]=  			/* This is the FData1 structure: */
	{
				LINE_EDITOR, 	&fwname, 		0, 		ClFieldCheck,
				LINE_EDITOR, 	&fwbuff_st, 	0, 		ClFieldCheck,
				LINE_EDITOR,	&fwbuff_end, 	0, 		ClFieldCheck,
				SELECT,		fwsel1, 		&fwsel2,	ClFieldCheck,
				MOUSE_BUTTON,	&fwmb1,		0,		ClFieldCheck,
				0
    };

/*--------------------------*
 *	F U N C T I O N S   *
 *--------------------------*/

void write_f(void)
{
int	select;
FData2	fw2;
FILE 	*fs;
long	fw_ed_addr;
long	fw_st_addr;

	_fstrcpy(file_write_name, fwn);

	open_window_dev(prgpar.c_all, 1, fw_ur, fw_lc, fw_lr, fw_rc, &flag_fw, fw_buffer);

	sprintf(fwbuff_st_addr,  "%05X", stadrbuf);
	sprintf(fwbuff_end_addr, "%05X", endadrbuf);

	Fields2(F_INIT,fw1,&fw2); 				/* Call Fields2 with a F_INIT message */
	Fields2(F_COLORS,fw1,&fw2,prgpar.c_all,prgpar.c_all); 				/* Call Fields2 with a F_INIT message */

	fwmb1.RColor		= prgpar.c_error;
	fwmb2.RColor		= prgpar.c_error;

	Fields2(F_DRAW,fw1,&fw2); 				/* Draw fields */
	Fields2(F_ACTIVE,fw1,&fw2);				/* Activate, now our CheckF will be in control of program flow */

	/* Check the FData2 structure to see if the user presses ESCAPE
	   or selects the CANCEL mouse button for exiting */

	if(fw2.ActiveField==4)
	{
		fw_st_addr= latox(fwbuff_st_addr);
		fw_ed_addr= latox(fwbuff_end_addr);
		select	= fwsel2.Position;

		if (fw_ed_addr < fw_st_addr)
			message_screen(prgpar.c_error, "%s", PAR_RANGE_ERROR);
		else
		{
			if((fs=fopen(file_write_name,"rb+")) != NULL)
			{
				if ((filelength(fileno(fs))))
				{
					if (my_yes_no(prgpar.c_error, "%s", FILE_EXIST) == TRUE)
						write_disk(fs, fw_st_addr, fw_ed_addr, select);
				}
				else
				{
					if (fseek(fs, 0l, SEEK_SET))
						message_screen(prgpar.c_error, "%s", FILE_SEEK_ERROR);
					else
						write_disk(fs, fw_st_addr, fw_ed_addr, select);
				}
			}
			else
			{
				if((fs=fopen(file_write_name,"wb")) != NULL)
					write_disk(fs, fw_st_addr, fw_ed_addr, select);
				else
					message_screen(prgpar.c_error, "%s", FILE_OPEN_ERROR);
			}
		}
	}
  	Window(prgpar.c_all, fw_ur, fw_lc, fw_lr, fw_rc, &flag_fw, fw_buffer);
}


void write_disk(FILE *fs, long st_addr, long ed_addr, int sel)
{
long i=0;
long j=0;
long k;

	switch (sel)
	{
		case 0 : 	i=st_addr; 
				j=1;
				break;

		case 1 :	if ((st_addr % 2) == 0)
					i = st_addr;
				else
					i = st_addr+1;
				j=2;
				break;

		case 2 :	if ((st_addr % 2) != 0)
					i = st_addr;
				else
					i = st_addr+1;
				j=2;
				break;
	}

		
	for (k=i; k<=ed_addr; k +=j)
	{
	  	*a = *(bptr+k);

		if (fwrite(a, 1, 1, fs) < 1)
		{
			message_screen(prgpar.c_error, "%s", FILE_WRITE_ERROR);
			break;
		}
	}

	if(fclose(fs))
		message_screen(prgpar.c_error, "%s", FILE_CLOSE_ERROR);
}


void read_f(void)
{
int i;
char ch;
FBData	fbd;
FILE	*rfile;

	_fstrcpy(file_read_name, frn);

	Window(prgpar.c_all, f_ur, f_lc, f_lr, f_rc, &flag_f, f_buffer);
	CursorOff();

	FileBox2(FB_INIT, &fbd);
	fbd.NColor = prgpar.c_all;
	fbd.RColor = in(prgpar.c_all);
	fbd.UpperRow = f_ur;
	fbd.LeftCol = f_lc;
	fbd.LowerRow = f_lr;
	fbd.RightCol = f_rc;
	fbd.Filename = file_read_name;
	FileBox2(FB_DRAW, &fbd);

	if ( FileBox2(FB_ACTIVE, &fbd) == FB_OK)
	{
		if ((rfile = fopen(file_read_name,"rb")) == NULL)
			message_screen(prgpar.c_error, "%s", FILE_OPEN_ERROR);
		else
			{
				_fstrcpy(frn, file_read_name);
				i=strlen(file_read_name);
				do
					ch = file_read_name[i--];
                		while (ch != '.');
				if ((strncmp(&file_read_name[i+2],"hex",3)) == 0)
					read_hex(rfile);
				else
					read_binary(rfile);
			}
	}

	Window(prgpar.c_all, f_ur, f_lc, f_lr, f_rc, &flag_f, f_buffer);
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

int read_hex(FILE *hex_file)
{
int state;
char tmp_str[MAXSTRING];
int x=1;

	while(x)
	{
		if(fgets(tmp_str,MAXSTRING-1,hex_file) == NULL)
			if(feof(hex_file))
				break;
			else
				message_screen(prgpar.c_error, "%s", FILE_ERROR);
		state=conv_hex(tmp_str);
	}
	fclose(hex_file);
	return(0);
}

int conv_hex(char *hex)
{
long	pointer;
int	i, number, store;
char	tmp[MAXSTRING];
long	sum;

	if (hex[0] == ':')
	{
		sum = 0;

		strncpy(tmp,hex+1,2); tmp[2] = '\0'; number  = atox(tmp); sum += number;
		strncpy(tmp,hex+3,4); tmp[4] = '\0'; pointer = latox(tmp);
		sum = (sum + pointer) + (pointer/256);
		strncpy(tmp,hex+7,2); tmp[2] = '\0'; store   = atox(tmp); sum += store;
		if (store == 0)
		{
			for (i=9; i<9+number*2; i+=2)
			{
				strncpy(tmp,hex+i,2); tmp[2] = '\0';
				*(bptr+pointer) = (byte)atox(tmp);
				sum += *(bptr+pointer++);
			}
			strncpy(tmp,hex+i,2); tmp[2] = '\0';
			sum += atox(tmp);
			if (sum % 256 == 0)
				return(HEX_OK);
			else
				return(HEX_ERROR_SUM);
		}
		else
			return(HEX_OK);
	}
	else
		return(HEX_ERROR_FORMAT);

}


int ClFieldCheck(FData2 *cl2)
{
int Mess;

	/* Let SCL1 FieldCheck function do most of the work */
	Mess=FieldCheck(cl2);

	/* Our only job is to watch for the F1 help key. It will be
	reported by Fields2 with an ILLEGAL_KEY message in the FData2
	structure element.  The message and the key value will be
	stored in the EventInfo element */

	if(cl2->Message == LE_ILLEGAL_KEY && cl2->EventInfo==F1)
	{

		/* F1 key pressed, save cursor status and turn it off */
		PushCursor();
		CursorOff();

		/* cl2->ActiveField tells us the field number that is active,
		we will use it to display context sensitive help */

		switch(cl2->ActiveField)
		{
			case 0: /* first field */
			MessageOn(BLACK_WHITE,"Use arrow keys to select if you\nwant to create backup files.");
			break;

			case 1: /* second field */
			MessageOn(BLACK_WHITE,"Type Backup files directory.");
			break;

			case 2: /* third field */
			MessageOn(BLACK_WHITE,"Press ENTER to validate\nyour selection.");
			break;

			case 3: /* fourth field */
			MessageOn(BLACK_WHITE,"Press ENTER or ESC to\ncancel your selection.");
			break;
		}

		Mess=F_POSITION_STAY;  /* return the position stay message so
                         	that the cursor does not move */
		WaitKeyMouse();        /* wait for a key */
		MessageOff();          /* restore screen */
		PopCursor();           /* restore cursor */
	}
	return(Mess);  /* Mess holds the message Fields2 will receive, a
       	         POSITION_STAY message or the message returned
                	by FieldCheck */
}


