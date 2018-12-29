/*
	<C> <Lion>

	prg_ibm.c

	Programm for Programmer on 8048

	Functions for work via RS232

	910614	v1.0

	Functions:
			void clear(void)

*/
#include <stdio.h>
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include "prg_def.h"
/*
			Functions prototype
*/
void	clear(void);
int	ClFieldCheck(FData2 *cl2);   /* our CheckF prototype */

/*
			Global variables
*/
byte 	cbyte=0xff;
char	clearb[3];
char	clearst[6];
char	cleared[6];
char	buffer_cl[W_BUF_SIZE(cl_ur, cl_lc, cl_lr, cl_rc)];
char	flag_cl=0;

	LEData clb ={23, cl_ur+2, cl_lc+2, "Byte : ", 112, cl_ur+2, cl_lc+10, 2,2,
			CC_ANY,0,0,clearb,0,0,0,2,1,0,0,0,0,0,0,0};
	LEData clst={23, cl_ur+4, cl_lc+2, "Start address : ", 112, cl_ur+4, cl_lc+19, 4,4,
			CC_ANY,0,0,clearst,0,0,0,2,1,0,0,0,0,0,0,0};
	LEData cled={23, cl_ur+6, cl_lc+2, "End   address : ", 112, cl_ur+6, cl_lc+19, 4,4,
			CC_ANY,0,0,cleared,0,0,0,2,1,0,0,0,0,0,0,0};
	
	MBData mb1={23, 112, cl_ur+8, cl_lc+2, cl_ur+8, cl_lc+4, cl_ur+8, cl_lc+2, "Fill", 0,0,0,0};
	MBData mb2={23, 112, cl_ur+8, cl_lc+10, cl_ur+8, cl_lc+16, cl_ur+8, cl_lc+10, "Cancel", 0,0,0,0};

	
	/* This is the FData1 structure: */
	FData1 cl1[]={
				LINE_EDITOR, &clb,  0, ClFieldCheck,
				LINE_EDITOR, &clst, 0, ClFieldCheck,
				LINE_EDITOR, &cled, 0, ClFieldCheck,
				MOUSE_BUTTON,&mb1,  0, ClFieldCheck,
				MOUSE_BUTTON,&mb2,  0, ClFieldCheck,
				0
			   };

/*
			External functions prototype
*/
extern	uint		atox(char *);
extern	long		latox(char *);
extern	void		open_window(int, int, int, int, int, int, char *, char *);

/*
			External variable
*/
extern	long	stadrbuf;		/* start adress buffer data for programing	*/
extern	long	endadrbuf;	/* end adress buffer data for programing	*/
extern	byte far	*bptr;
extern	par	prgpar;


void clear(void)
{
FData2 cl2;
long clstaddr;
long cledaddr;
long i;

	sprintf(clearb,  "%02X", cbyte);
	sprintf(clearst, "%04X", stadrbuf);
	sprintf(cleared, "%04X", endadrbuf);

	open_window(prgpar.c_all, 1, cl_ur, cl_lc, cl_lr, cl_rc, &flag_cl, buffer_cl);

	Fields2(F_INIT,cl1,&cl2); 				/* Call Fields2 with a F_INIT message */
	Fields2(F_DRAW,cl1,&cl2); 				/* Draw fields */
	Fields2(F_ACTIVE,cl1,&cl2);				/* Activate, now our CheckF will be in control of program flow */

	/* Check the FData2 structure to see if the user presses ESCAPE 
	   or selects the CANCEL mouse button for exiting */

	if(cl2.ActiveField==3)
	{
		clstaddr	= latox(&clearst[0]);
		cledaddr	= latox(&cleared[0]);
		cbyte	= (byte)atox(&clearb[0]);

		if (cledaddr > endadrbuf)
			cledaddr = endadrbuf;

		for (i=clstaddr; i<=cledaddr; i++)
			*(bptr+i) = cbyte;
		
	}		  
		
	Window(BLACK_BLACK, cl_ur, cl_lc, cl_lr, cl_rc, &flag_cl, buffer_cl);

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
			break;

			case 1: /* second field */
			break;

			case 2: /* third field */
			break;

			case 3: /* fourth field */
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
