/*
	<C> <Lion>

	prg_var.c

	Programm for Programmer on 8048

	Read Var from tmp file Function

	921201	v1.0

	Functions:	read_var();
				sp();

*/

#include "prg_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include "scl1.h"
#include "scl1clor.h"
#include "scl1keys.h"
#include <process.h>

/* #include <spawno.h>*/


extern	void	*malloc_e(uint mem);
extern	void j_exit(void);
extern	void set_port(void);
extern	void	read_par_file(void);
extern 	void	address_screen_on(void);
extern 	void	address_screen_off(void);
extern 	void	device_screen_on(void);
extern 	void	device_screen_off(void);
void	message_screen(int, char *, ...);

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
char	full_name[90];
void	*ad[50];

extern	uint	csum;		/* Buffer Control Sum*/
extern	long	stadrbuf;		/* start adress buffer data for programing	*/
extern	long	endadrbuf;	/* end adress buffer data for programing	*/
extern	long	stadrdev;		/* start adress device for programing		*/
extern	long	endadrdev;	/* end adress device for programing		*/
extern	uint	select_b;		/* Flags:	all  bytes select_b = 0*/
					/*		odd	bytes select_b = 1*/
					/*		even bytes select_b = 2*/
extern	int	fill_c;
extern	int	set_one;
extern	int	modem_out_busy;

extern	int	port_add;
extern	byte	linestat;

extern	char file_read_name[];
extern	char file_write_name[];
extern	char ext_hexed_name[];

extern	char far *fsp;
extern	char	flag_h;

extern	char *buffer_main;		/* for debug */

void sp(char *name, int number)
{
char *buffer_h;
char far *frn;
char far *fwn;
char far *fed;
char *tstr;
char *tstr1;
char *s[4];	/* for transmitt far pointer via command string */
struct SREGS cpuregs;

	j_exit();

	frn = file_read_name;
	fwn = file_write_name;
	fed = ext_hexed_name;

	tstr  =malloc_e(20);
	tstr1 =malloc_e(20);
	segread(&cpuregs);
	
	if ((buffer_h = malloc_e(W_BUF_SIZE(h_ur, h_lc, h_lr, h_rc))) != NULL)
		{
		address_screen_off();
		device_screen_off();

		Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);


		s[0] = full_name;
		sprintf(tstr, "%4x\0", ad);
	 	sprintf(tstr1,"%4x\0", cpuregs.ds);
		s[1] = tstr;
		s[2] = tstr1;
		s[3] = NULL;

		ad[0]  = &number;
		ad[1]  = &stadrbuf;
		ad[2]  = &endadrbuf;
		ad[3]  = &stadrdev;
		ad[4]  = &endadrdev;
		ad[5]  = &csum;
		ad[6]  = &select_b;
		ad[7]  = &bptr;
		ad[8]  = &prgpar;
 		ad[9]  = &prg_dev;
 		ad[10] = &cf;
		ad[11] = &fill_c;
		ad[12] = &set_one;
		ad[13] = &set_baud;
		ad[14] = &set_port_flag;
		ad[15] = &mes_prg_flag;
		ad[16] = &mes_ibm_flag;
		ad[17] = &mes_err_flag;
		ad[18] = &frn;
		ad[19] = &fwn;
		ad[20] = &fsp;
		ad[21] = &fed;


		_fstrcpy(full_name, fsp);
		strcat(full_name, name);
		strcat(full_name, ".ovl");
		
		if (spawnv(P_WAIT, full_name, s) == -1)
			message_screen(prgpar.c_error, "%s", COULD_NOT_SPAWN_PROCESS);
		
		Window(BLACK_BLACK, h_ur, h_lc, h_lr, h_rc, &flag_h, buffer_h);
		free(buffer_h);

		address_screen_on();
		device_screen_on();
		}

	free(tstr1);
	free(tstr);
	
	set_port();

}

