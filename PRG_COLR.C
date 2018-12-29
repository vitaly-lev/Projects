/*
	<C> <Lion>

	prg_colr.c

	Programm for Programmer on 8048

	Color setup Subroutines

	920220	v1.0 Beta4

	Functions:
*/

#include "prg_def.h"
#include <stdio.h>
#include <string.h>
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
/*
			Functions prototype
*/
void	set_color(void);
int	change_color(int *);
void	number_color(int colr);
void	change_menu_color(int *, char *);

/*
			Global variable
*/
char	buffer_c[W_BUF_SIZE(c_ur, c_lc, c_lr, c_rc)];
char	flag_c=0;

char	buffer_s[W_BUF_SIZE(s_ur, s_lc, s_lr, s_rc)];
char	flag_s=0;

/*
			External functions prototype
*/
extern	void save_par(void);
extern	void	open_window(int, int, int, int, int, int, char *, char *);
extern	int	wprintf(int color, int _ur, int _lc, char *format, ...);

/*
			External variable
*/
extern	int	cf;		/* Main screen color 			*/
extern	int	fill_c;	/* Main screen fill charter 		*/
extern	par	prgpar;	/* Varios parameters programmer 	*/

void set_color(void)
{

	Window(BLACK_BLACK,     c_ur, c_lc, c_lr, c_rc, &flag_c, buffer_c);
	FillBlock(cf,		    c_ur, c_lc, c_lr, c_rc, fill_c);
	FillBlock(cf,		    c_ur+1, c_lc+2, c_lr-2, c_rc, ' ');
	
	Window(BLACK_BLACK,		s_ur, s_lc, s_lr, s_rc, &flag_s, buffer_s);
	WriteScreen(WHITE_BLACK,	s_ur,   s_lc+1, "Press UP & DOWN    for change foregraund color.");
	WriteScreen(WHITE_BLACK, s_ur+1, s_lc+1, "Press RIGHT & LEFT for change backgraund color.");
	WriteScreen(WHITE_BLACK, s_ur+2, s_lc+1, "Press ESC          for interrupt changes.");
	WriteScreen(WHITE_BLACK, s_ur+3, s_lc+1, "Press ENTER        for story changes.");

	change_menu_color(&prgpar.c_menu,   	"[Main Menu]");
	change_menu_color(&prgpar.c_all,	 	"[All Window Message]");
	change_menu_color(&prgpar.c_normal, 	"[Normal Message]");
	change_menu_color(&prgpar.c_error,		"[Error  Message]");
	change_menu_color(&prgpar.c_hex_border, "[HEX ED, Border&Work Inf.]");
	change_menu_color(&prgpar.c_hex_hex,	"[HEX ED, Hex values]");
	change_menu_color(&prgpar.c_hex_asc,	"[HEX ED, Asc values]");

	Window(BLACK_BLACK,	    s_ur, s_lc, s_lr, s_rc, &flag_s, buffer_s);
	Window(BLACK_BLACK,     c_ur, c_lc, c_lr, c_rc, &flag_c, buffer_c);

	SetErrorBoxColor(prgpar.c_error);
	save_par();
}

void change_menu_color(int *menu_color, char *type_menu)
{
int key;
int temp_c;

	temp_c = *menu_color;

	do
	{
		GSSBox(temp_c,	1,			c_ur, c_lc, c_lr-3, c_rc-1, 0, 0, 0);
		WriteScreen(temp_c,			c_ur+2, c_lc+4, "Text  example");
		WriteScreen(WHITE_BLACK,		c_ur, c_lc+(c_rc-c_lc-strlen(type_menu))/2, type_menu);

		key = change_color(&temp_c);

	}while (key!=ESC && key!=ENTER);

	if (key == ENTER)
		*menu_color = temp_c;
}

int change_color(int *cl)
{
int key;
int colr;

	colr = *cl;
	number_color(colr);

	key = GetKey();

	switch (key)
		{
		case UP		: 	if (((colr & 0x000f) + 1) <= 0x000f) 
							colr++;
						else  
				   			colr &= 0x00f0;
				   		break;

		case DOWN		: 	if (((colr & 0x000f) - 1) >=  0) 
							colr--;
						else
							colr |= 0x000f;
						break;

		case RIGHT	:	if (((colr & 0x00f0) + 0x0010) <= 0x0070) 
							colr +=0x0010;
						else
							colr &= 0x000f;
						break;

		case LEFT		:	if (((colr & 0x00f0) - 0x0010) >=  0) 
							colr -=0x0010;
						else
							colr |= 0x0070;
						break;
		}

	number_color(colr);
	*cl = colr;
	return(key);
}

void number_color(int colr)
{
	wprintf(WHITE_BLACK, c_lr, c_lc+1, "Foreg=%02d   Back=%02d", colr & 0x000f, colr>>4 & 0x000f);
}

