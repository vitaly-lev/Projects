/*
	<C> <Lion>

	prg_main.c

	Programm for Programmer on 8048

	Main Function

	920228	V2.0 Beta0

	Functions:	void main(void)

*/

#include <scl1.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <dos.h>
#include <scl1clor.h>
#include "prg_def.h"

/*
			External functions prototype
*/
extern	int	main_menu(void);
extern	void	read_par_file(void);
extern	void	j_exit(void);
extern	void	set_port(void);
extern	void first_set(void);
extern	int	read_device_file(void);
extern	void	*malloc_e(uint);
extern	void set_mes_on(void);
extern	void set_mes_off(void);
extern	void	message_screen(int, char *, ...);
/*
			Local variable
*/
int	cur_row;
int	cur_col;
char	flag_main=0;
char	*buffer_main;
char	sys_path[80];
char	sys_name[80];
char 	far *fsp; /* pointer on full system path */

/*
			External variable prototipe
*/
extern	struct	ErrorMess	em[];
extern	byte far	*bptr;
extern	struct device far *prg_dev;
extern	int	set_one;
extern	par	prgpar;
extern	int	set_baud;
extern	int	fill_c;
extern	int	cf;
extern	int far *set_port_flag;
extern	void	path_name(char *full_name, char *path, char *name);
extern	char	*envptr;
extern	int fossil;

void main(int argc, char **argv)
{
int i;
	
	i=argc;

	printf("\nPROM Programmer V2.1 by Vitaly Lev. 10-16-1993");
	printf("\nCopyright 1993 by <Lion>");
	printf("\n2:462/10 - FIDO NET");
	printf("\nvl@ccrd.lviv.ua - Internet\n");

	path_name(*(argv), sys_path, sys_name);
	fsp = sys_path;

	cur_col = GetCurCol();
	cur_row = GetCurLine();
	CursorOff();
	TrapInt23();
	InitUserError(em);
	SetErrorBoxColor(RED_WHITE);
	prgpar.c_error = RED_WHITE;

	if ((buffer_main = malloc_e(W_BUF_SIZE(0,0,24,79))) != NULL)
		{

		if ((bptr=halloc(65536*2,sizeof(byte))) == NULL)
			message_screen(prgpar.c_error, "%s", MALLOC_ERROR);
		else
		{
			if ((prg_dev=halloc(1000,sizeof(struct device))) == NULL)
				message_screen(prgpar.c_error, "%s", MALLOC_ERROR);
			else
				{

				Window(BLACK_BLACK,0,0,24,79,&flag_main, buffer_main);
					FillBlock(cf,1,0,24,79,fill_c);

				if (read_device_file() == 1);
					{
					read_par_file();
					SetErrorBoxColor(prgpar.c_error);
					set_mes_on();
					set_one = 0;
					set_baud = FALSE;
					set_port();
					if (!fossil)
						{
						*set_port_flag = FALSE;
					
						if ((envptr = getenv ("COMSPEC")) == NULL)
							{
							envptr = malloc_e(20);
							strcpy(envptr, "command.com");
							}
					
						main_menu();
						}
					Window(BLACK_BLACK,0,0,24,79,&flag_main, buffer_main);
					j_exit();
					}
				}
			_ffree(prg_dev);
			SetCurPos(cur_row, cur_col);
			}
		_ffree(bptr);
		free(buffer_main);
		}
	CursorOn();
	if (fossil)
		{
		printf("\nError: Fossil driver detected");
		printf("\nPlease Reload Fossil from memory\n");
		}
}


