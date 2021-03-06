/*
	<C> <Lion>

	prg_test.c

	Programm for Programmer on 8048

	Tests Subroutines

	911203	v1.0

	Functions:
*/

#include "prg_def.h"
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*
			Functions prototype
*/
void test_vpp(void);
void test_vcc(void);
void test_pins(void);
void test_a4vpp(void);

/*
			External functions prototype
*/
extern	void	open_window(int, int, int, int, int, int, char *, char *);
extern	void	*malloc_e(uint mem);
extern	void	set_mes_on(void);
extern	void	set_mes_off(void);
extern	int  send_one_command(c_m_str *, uint, uint, uint);
extern	void set_port(void);
extern	void restore_prg_w(void);
extern	void	message_screen(int, char *, ...);

/*
			Global variables
*/
c_m_str C_PRG_TEST	=	{ 0x07, "Test Vpp..."};	/* see prg_ibm.c */

#define TEST_VPP_	 0x0000
#define TEST_VCC_	 0x0001
#define TEST_PINS_ 0x0002
#define TEST_A4_ 	 0x0003

char	buffer_vpp[W_BUF_SIZE(vpp_ur, vpp_lc, vpp_lr, vpp_rc)];
char    flag_vpp=-2;

char	*buffer_pins;
char    flag_pins=-2;

/*
			External variable
*/
extern	par prgpar;


int seg_m;

char tmp_str[30];


void test_vpp(void)
{
int num=9;
uint vpp=0;
int tmp;

/*
	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);
	GSSBox(WHITE_WHITE, 5, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, 1, 0, 1);
	Shadow(BLUE_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc);
*/

	restore_prg_w();

	do
	{
		if ((send_one_command(&C_PRG_TEST, TEST_VPP_, vpp, 0)) == 0)
			message_screen(prgpar.c_error, "%s", PRG_RX_C_ERROR);

		sprintf(tmp_str, "Vpp = %u", vpp);
		WriteScreen(WHITE_BLACK, 15, 10, tmp_str);

		switch(getch())
		{
		case '+'	: if (vpp < 15) vpp++;	break;
		case '-'	: if (vpp > 0)  vpp--;	break;
		case 0x1b	: tmp = 'E';			break;
		}
	}while(tmp!='E');

	
/*	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);*/
}


void test_vcc(void)
{
int num=9;
uint vcc=8;
int tmp;

/*
	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);
	GSSBox(WHITE_WHITE, 5, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, 1, 0, 1);
	Shadow(BLUE_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc);
*/
	restore_prg_w();

	do
	{
		 if ((send_one_command(&C_PRG_TEST, TEST_VCC_, vcc, 0)) == 0)
			message_screen(prgpar.c_error, "%s", PRG_RX_C_ERROR);

		sprintf(tmp_str, "Vcc = %u", vcc);
		WriteScreen(WHITE_BLACK, 15, 10, tmp_str);

		switch(getch())
		{
		case '+' : if (vcc < 14) vcc +=2; break;
		case '-'	: if (vcc > 8)  vcc -=2; break;
		case 0x1b: tmp = 'E';	       break;
		}
	}while(tmp!='E');
	
/*	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);*/
}

void test_a4vpp(void)
{
int num=9;
uint vcc=8;
int tmp;

/*
	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);
	GSSBox(WHITE_WHITE, 5, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, 1, 0, 1);
	Shadow(BLUE_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc);
*/
	restore_prg_w();

	do
	{

		 if ((send_one_command(&C_PRG_TEST, TEST_A4_, vcc, 0)) == 0)
			message_screen(prgpar.c_error, "%s", PRG_RX_C_ERROR);

		switch(getch())
		{
		case '+' : if (vcc < 14) vcc +=2; break;
		case '-'	: if (vcc > 8)  vcc -=2; break;
		case 0x1b: tmp = 'E';	       break;
		}
	}while(tmp!='E');

	
/*	Window(BLACK_BLACK, vpp_ur, vpp_lc, vpp_ur+num*2, vpp_rc, &flag_vpp, buffer_vpp);*/
}


char *pins_m = "     ������Ŀ          ������Ŀ          ������Ŀ\n"
			  "     �21  20�          �15  14�          �15  14�\n"
			  "     �22  19�          �16  13�          �16  13�\n"
			  "     �23  18�          �17  12�          �17  12�\n"
			  "     �24  17�          �18  11�          �18  11�\n"
			  "     �25  16�          �19  10�          �19  10�\n"
			  "     �26  15�          �20   9�          �20   9�\n"
			  "     �27  14�          �21   8�          �21   8�\n"
			  "     �28  13�          �22   7�          �22   7�\n"
			  "     �29  12�          �23   6�          �23   6�\n"
			  "     �30  11�          �24   5�          �24   5�\n"
			  "     �31  10�          �25   4�          �25   4�\n"
			  "     �32   9�          �26   3�          �26   3�\n"
			  "     �33   8�          �27   2�          �27   2�\n"
			  "     �34   7�          �28   1�          �28   1�\n"
			  "     �35   6�          ��������          ��������\n"
			  "     �36   5�\n"
			  "     �37   4�\n"
			  "     �38   3�\n"
			  "     �39   2�\n"
			  "     �40   1�\n"
			  "     ��������\n";

/*
   chip's number:        2            1            0

	column number:   5        4   3	   2	   1		  0
*/

#define crow 1
#define	ccol 50

void test_pins(void)
{
uint row=crow+14;
uint key;
uint chip=0;
uint pnum=1;
uint cnum=0;
uint orow=row;
uint ocol=ccol-cnum;

	set_mes_off();

	if((buffer_pins=malloc_e(W_BUF_SIZE(pins_ur, pins_lc, pins_lr, pins_rc))) != NULL)
		{
		open_window(prgpar.c_all, 1, pins_ur, pins_lc, pins_lr, pins_rc, &flag_pins, buffer_pins);
		WriteScreen(prgpar.c_all, pins_ur+1, pins_lc+1, pins_m);
		WriteChar(prgpar.c_all, row, ccol-cnum, 1, '<');
		
		do
			{
			
			if (!(send_one_command(&C_PRG_TEST, TEST_PINS_, chip, pnum)))
				message_screen(prgpar.c_error, "%s", PRG_RX_C_ERROR);

			key = GetKey();
			switch (chip)
				{
				case 0:
					{
					switch (key)
						{
						case UP:
							if (pnum < 13)
								{
								pnum++;
								row--;
								}

							else
								if (pnum > 15)
								{
								pnum--;
								row--;
								}
							else
								Beep();
							break;

						case DOWN:
							if (pnum > 14 && pnum < 27)
								{
								pnum++;
								row++;
								}
							else
								if (pnum < 15 && pnum > 1)
								{
								pnum--;
								row++;
								}
							else
								Beep();
							break;

						case LEFT:
							cnum++;
							pnum = 29-pnum;
							if (cnum == 2)
								chip++;
							break;

						case RIGHT:
							if (cnum == 1)
								{
								pnum = 29-pnum;
								cnum--;
								}
							else
								Beep();
							break;
							
						}
					}
					break;

				case 1:
					{
					switch (key)
						{
						case UP:
							if (pnum <13)
								{
								pnum++;
								row--;
								}
							else
								if (pnum >15)
								{
								pnum--;
								row--;
								}
							else
								Beep();
							break;

						case DOWN:
							if (pnum > 14 && pnum < 27)
								{
								pnum++;
								row++;
								}
							else
								if (pnum < 15 && pnum > 1)
								{
								pnum--;
								row++;
								}
							else
								Beep();
							break;

						case LEFT:
							cnum++;
							if (cnum == 4)
								{
								pnum = 1;
								chip = 2;
								row  = 20+crow;
								}
							else
								pnum = 29-pnum;
							break;

						case RIGHT:
							cnum--;
							if (cnum == 1)
								chip = 0;
							pnum = 29-pnum;
							break;
						}
					}
					break;

				case 2:
					{
					switch (key)
						{
						case UP:
							if (pnum < 19)
								{
								pnum++;
								row--;
								}
							else
								if (pnum >21)
								{
								pnum--;
								row--;
								}
							else
								Beep();
							break;

						case DOWN:
							if (pnum > 20 && pnum < 39)
								{
								pnum++;
								row++;
								}
							else
								if (pnum < 21 && pnum > 1)
								{
								pnum--;
								row++;
								}
							else
								Beep();
							break;

						case LEFT:
							if (cnum == 5)
								Beep();
							else
								{
								cnum++;
								pnum = 41-pnum;
								}
							break;

						case RIGHT:
							cnum--;
							if (cnum == 3)
								{
								chip = 1;
								pnum = 28;
								row  = crow+14;
								}
							else
								pnum = 41-pnum;
							break;
						}
					}
					break;

				}/* swith chip */


			WriteChar(prgpar.c_all, orow, ocol, 1, ' ');

			if (((chip == 0 || chip == 1) && pnum == 28) || pnum == 40)
				{
				pnum--;
				row--;
				}
			if (((chip == 0 || chip == 1) && pnum == 14) || (chip == 2 && pnum == 20))
				{
				pnum--;
				row++;
				}
			
			if (cnum % 2 == 0)
				WriteChar(prgpar.c_all, orow=row, ocol=ccol-cnum*9, 1, '<');
			else
				WriteChar(prgpar.c_all, orow=row, ocol=ccol-cnum*9, 1, '>');


			}
		while (key != ESC);

		Window(prgpar.c_all, pins_ur, pins_lc, pins_lr, pins_rc, &flag_pins, buffer_pins);
		free(buffer_pins);
		}
	set_mes_on();
}
	



