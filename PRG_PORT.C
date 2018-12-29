/*
	<C> <Lion>

	prg_port.c

	Programm for Programmer on 8048

	Subroutines for I/O port

	910814	v1.0

	Functions:
			void	set_port(void);
			void	send_b(byte);
			int	receive_b(int);
			void	receive_block(void);
			void	send_block(int);
			void	j_exit(void);

!!! send_block() - need set timer and exit if timeout

*/

#include "prg_def.h"
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <conio.h>
#include <dos.h>

struct fos_inform {
       word      strsize;       /* size of the structure in bytes     */
       byte      majver;        /* FOSSIL spec driver conforms to     */
       byte      minver;        /* rev level of this specific driver  */
       char far *ident;         /* FAR pointer to ASCII ID string     */
       word      ibufr;         /* size of the input buffer (bytes)   */
       word      ifree;         /* number of bytes left in buffer     */
       word      obufr;         /* size of the output buffer (bytes)  */
       word      ofree;         /* number of bytes left in the buffer */
       byte      swidth;        /* width of screen on this adapter    */
       byte      sheight;       /* height of screen    "      "       */
       byte      speed;         /* ACTUAL speed, computer to modem    */
};


/*
	Local Defines
*/
#define I_MASK 0x21		/* interrupt register mask address */
#define RX_BLOCK 0
#define BIT0_ON 	(0x01)

/*
	Send block state names
*/
#define TX_ONE	0
#define TX_BLOCK	1
/*
	Interrupt-related definitions
*/
#define INT_8259A_PORT	0x20
#define EOI		0x20
#define RCV_DATA	4
#define THR_EMPTY	2
#define MODSTAT_CHNG	0

/*
	fossil defines
*/
#define FOS_SIGNATURE   0x1954
#define FOS_SETSPEED    0x00
#define FOS_PUTC        0x01
#define FOS_GETSTATUS   0x03
#define FOS_INIT        0x04
#define FOS_DEINIT      0x05
#define FOS_SETDTR      0x06
#define FOS_DUMP        0x09
#define FOS_PURGE       0x0a
#define FOS_SETFLOW     0x0f

/*
	Local functions
*/
void 	set_port(void);
void 	send_b(byte);
void	receive_block(void);
void	send_block(int);
void	j_exit(void);
void	set_ibm_baud(byte);
int 	send_one_command(c_m_str *, uint, uint, uint);
void	set_tx_buffer_command(byte, uint, uint, uint );
void	set_tx_buffer_bytes(long *, byte, byte);
void	set_c_sum(void);
void	restore_prg_w(void);
void	mes_err(char *);
void	set_mes_on(void);
void	set_mes_off(void);
void 	mes_ibm(char *);
void	mes_prg(char *);
void	mes_prog(char *);
void	mes_prog_w(long *, uint);
int	prg_mess_err(long *, byte);
void 	restore_all_w(char *);
void	save_all_w(void);
void	restore_stat_w(uint);
void	save_stat_w(void);
void 	_interrupt _far ascode(void);
void	snd_block(void);
void	rcv_block(void);
int	fos_init(void);
int 	fos_fillinbuf(void);
void	rx_block_progress(void);
int	fos_init(void);
int	get_fos_info(void);
void	com_purge(void);



/*
	Global variable
*/
c_m_str C_PRG_RESET 		=	{ 0x01, RESET_PROGRAMMER}; /* see prg_ibm.c */
byte	linestat;
byte	sum;
byte	tx_buffer[100];
byte	rx_buffer[1024+1];
char	flag_all=0;
char	all_buffer[W_BUF_SIZE(all_ur, all_lc, all_lr+1, all_rc+1)];
int	old_percent;
char	percent_line[] = "°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°";
int	flag_open=TRUE;
char	flag_prgs=0;
char	prgs_buffer[W_BUF_SIZE(prgs_ur, prgs_lc, prgs_lr+1, prgs_rc+1)];

uint	port_addr[] = { 0x03f8, 0x02f8, 0x03e8, 0x2e8 };
byte	hard_int[]  = { 0x0c,   0x0b,   0x0c,   0x0b  };
byte	iorq_int[]  = { 0xef,   0xf7,   0xef,   0xf7  };
uint	port_add;
byte	linestat;
void 	(_interrupt _far *old_vector)( void );
int	drvr_active=0;
int	comn;
byte	baudrate;
int	fossil;
union	REGS regs;
struct	SREGS sregs;
struct 	fos_inform fos_info;
int	modem_out_busy;	/* RS-232 hardware status flag
				          0=transmitter not active
				          1=transmitter active */
int	block_tx;
int	tx_state;
int	tx_count;
int	tx_number;
int	st_busy;
int	rx_state;
int	rx_count;
int	int_id_reg;			/* value in int id reg */
int	fos_inread;
int	fos_inwrite;

/*
	External Functions
*/
extern	void	timerins(void);
extern	void	timerrem(void);
extern	void	drvr_ins(void);
extern	void	trigger_tx(void);
extern	void	drvr_rem(void);
extern	void	timerrem(void);
extern	void	far load_main_seg(void);
extern	void	far load_old_seg(void);
void 	sel_state(int *, uint *);
extern	void	message_screen(int, char *, ...);
extern	int	wprintf(int color, int _ur, int _lc, char *format, ...);

/*
	External variable
*/
extern	int	timer_1;
extern	int	timer_2;
extern	int	f_com1;
extern	int	f_com2;
extern	par	prgpar;
extern	int	set_baud;
extern	byte far	*bptr;
extern	long stadrdev;
extern	long endadrdev;
extern	long stadrbuf;
extern	long endadrbuf;
extern	int	mes_prg_flag;
extern	int	mes_ibm_flag;
extern	int	mes_err_flag;
extern	char	flag_prg;
extern	byte	tx_buffer[];
extern	byte	rx_buffer[];
extern	char	prg_buffer[];
extern	int	iir_add;
extern	byte	baudrate;
extern	char err_buffer[];
extern	char flag_err;

/*--------------------------*
 *	F U N C T I O N S   *
 *--------------------------*/

void set_ibm_baud(byte baud)
{
	baudrate = baud;
	outp(port_addr[comn]+3, 0x80);
	outp(port_addr[comn],   baudrate);
	outp(port_addr[comn]+1, 0x00);
	outp(port_addr[comn]+3, 0x07);
}

void set_port(void)
{

	timer_1 = 0;
	timer_2 = 0;
	timerins();
	baudrate = S_9600;
	modem_out_busy = 0;
	
	
	fossil = TRUE;

	if (!fos_init())
		{
   	fossil = FALSE;
   	return(drvr_ins());
		}
}

void send_b(byte tx_byte)
{
	if (fossil)
		{
		regs.h.ah = FOS_PUTC;
		regs.h.al = tx_byte;
		regs.x.dx = prgpar.port;
		int86(0x14, &regs, &regs);
/*!!!	screen_char_out(tx_byte); */
		}
	else
		{
		tx_buffer[0] = tx_byte;
		tx_number = 1;
		timer_2 = 2;
		while(modem_out_busy)
			{
			if (timer_2 == 0)
				break;

			}
		trigger_tx();
		}
}

void send_block(int num)
{
	if (fossil)
		{
		regs.x.ax = 0x1900;
		regs.x.dx = prgpar.port;
		regs.x.cx = num;
		segread(&sregs);
		sregs.es  = sregs.ds;
		regs.x.di = (uint)&tx_buffer[0];
		int86x(0x14,&regs,&regs,&sregs);
/*		channel_s[number].block_tx = TRUE;*/
		tx_number = num;
/*!!!	for (i=0; i<bnum; i++)
			screen_char_out(channel_s[number].temp_tx[i]);*/

		}
	else
		{
		tx_number = num;
		timer_2 = 2;
		while(modem_out_busy)
			{
			if (timer_2 == 0)
				break;

			}
		trigger_tx();
		}
}

/*
int receive_b(int sec, int number)
{

	rx_count = 0;
	timer_1 = sec;
	
	while ((timer_1 != 0) && (rx_count == 0))
		{
		if (fossil)
			{
           	if ((fos_inread < fos_inwrite) || fos_fillinbuf(number))
				{
				rx_count++;
!!!			screen_char_in(channel_s[number].temp_rx[channel_s[number].rx_count]);
				}
			}
		}

	if (rx_count == 0)
		return(-1);
	else
	{
		com_purge(number);
		return(*rx_buffer);
	}
*/
	
	
/*	
	rx_count = 0;
	timer_1 = sec;
	while ((timer_1 != 0) && (rx_count == 0))
	{
		;
	}
	if (rx_count == 0)
		return(-1);
	else
	{
		receive_block();
		return(*rx_buffer);
	}

}
*/


void receive_block(void)
{
	rx_state = RX_BLOCK;
	rx_count = 0;
	timer_1 = 2;		
	sum = 0;
}


void j_exit(void)
{
	drvr_rem();
	timerrem();
}


int send_one_command(c_m_str *command, uint p1, uint p2, uint p3)
{
int count_r = N_RESET;
int count_c;
uint rb;
int key=0;

	do
		{
		count_c=N_TX_COMMAND;
		do
			{
/*
			if (set_baud == FALSE)
				{
				receive_block();
				set_tx_buffer_command(C_PRG_BAUD.c, prgpar.speed, 0, 0);
				mes_ibm(C_PRG_BAUD.m);
				send_block(BLOCK);
				while(!block_tx)
					;
*/
/*					{
					if (timer_2 == 0)
						break;
					}
*/
/*
				switch (rb=receive_b(1))
					{
					case ACK : 	mes_prg(OK_M);
								set_ibm_baud(speed_int[prgpar.speed]);
								set_baud=TRUE;
								break;
		
					case NAK : 	mes_prg(NOT_ACK_M); break;
					case 0xffff : 	mes_prg(NOT_ANSWER_M); break;
					default  : 	mes_prg(QUESTIONS_M); break;
					}
				}
*/
	 
			receive_block();
			set_tx_buffer_command(command->c, p1, p2, p3);
			mes_ibm(command->m);
			send_block(BLOCK);
			
/*			SetCurPos(24,0); printf("block WILL send..."); GetKey();*/
			
/*			while(!block_tx)
				{
				if (timer_2 == 0)
					break;
				;
				}
*/
			
/*			SetCurPos(24,0); printf("block sended");  GetKey();*/

			while ((timer_1 != 0) && (rx_count == 0))
			{
				;
			}

			if (rx_count == 0)
				rb=0xffff;
			else
				rb=rx_buffer[0];

			switch (rb)
				{
				case ACK : 	mes_prg(OK_M); break;
				case NAK : 	mes_prg(NOT_ACK_M); break;
				case 0xffff : 	mes_prg(NOT_ANSWER_M); break;
				default  : 	mes_prg(QUESTIONS_M); break;
				}
			count_c--;

			key = KeyReady();

			}while (rb!=ACK && count_c!=0 && key!=ESC);

		if (rb!=ACK && key!=ESC)
			{
			j_exit();
			set_port();
			set_baud = FALSE;

			set_tx_buffer_command(C_PRG_RESET.c, 0,0,0);
			mes_ibm(C_PRG_RESET.m);
			send_block(BLOCK);
			
/*			while(!block_tx)
				{
				if (timer_2 == 0)
					break;
				;
				}
*/
			if (rx_count == 0)
				rb=0xffff;
			else
				rb=rx_buffer[0];

			switch (rb)
				{
				case ACK : 	mes_prg(OK_M); break;
				case NAK : 	mes_prg(NOT_ACK_M); break;
				case 0xffff : 	mes_prg(NOT_ANSWER_M); break;
				default  : 	mes_prg(QUESTIONS_M); break;
				}
			count_r--;
		
			key = KeyReady();

			}
		}while (rb!=ACK && count_r!=0 && key!=ESC);

	receive_block();

	if (rb == ACK)
		return(rb);
	else
		if (key == 0)
			message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
		return(0);
}

void set_tx_buffer_command(byte command, uint saddr, uint eaddr, uint number)
{

	*(tx_buffer+0) = PASSWORD;
	*(tx_buffer+1) = command;
	*(tx_buffer+2) = (byte)(saddr & 0x00ff);
	*(tx_buffer+3) = (byte)((saddr>>8) & 0x00ff);
	*(tx_buffer+4) = (byte)(eaddr & 0x00ff);
	*(tx_buffer+5) = (byte)((eaddr>>8) & 0x00ff);
	*(tx_buffer+6) = (byte)(number & 0x00ff);
	*(tx_buffer+7) = (byte)((number>>8) & 0x00ff);
	set_c_sum();

/*	SetCurPos(24,0); printf(" %4x %4x %4x", saddr, eaddr, number); GetKey();
*/
}

void set_tx_buffer_bytes(long *addr, byte number, byte flag)
{
uint i;
int sel;
uint mul;

	*(tx_buffer+0) = (byte)(*addr & 0x00ff);
	*(tx_buffer+1) = (byte)((*addr>>8) & 0x00ff);
	*(tx_buffer+2) = number;

	sel_state(&sel, &mul);

	for (i=0; i<number; i++)
		{
/*		SetCurPos(24,0);
		printf("%4d  %4d  %4X     ", sel, mul, bptr + stadrbuf + staddrdev + sel + ((*addr - stadrdev + i) * mul));
		key=GetKey();
*/
		*(tx_buffer+i+3) = *(bptr + stadrbuf + stadrdev + sel + ((*addr - stadrdev + i) * mul));
		}
/*                         |  |		 |          |    |______________________________|   |
                           |  |		 |		  |                      |                |__ number of byte
                           |  |		 |		  |                      |____________________ calculate ordering number byte for writing
                           |  |		 |		  |___________________________________________ device start address (set by user in ADDRESS RANGE Revime)
                           |  |		 |______________________________________________________ buffer start address (set by user in ADDRESS RANGE Revim
                           |  |_______________________________________________________________ buffer base address
                           |__________________________________________________________________ real value of byte
*/
	for (i=number; i<5; i++)
		*(tx_buffer+i+3) = flag;

	set_c_sum();

}


void set_c_sum(void)
{
int i, tmp;

	*(tx_buffer+8) = 0;

	for(i=0; i<8; i++)
        *(tx_buffer+8) += *(tx_buffer+i);

	tmp = *(tx_buffer+8);
	tmp = -tmp;
	*(tx_buffer+8) = (char)tmp;
}

void restore_all_w(char *tmp)
{
int i;
	if (flag_all == 0 || flag_all == -2)
		Window(prgpar.c_all, all_ur, all_lc, all_lr, all_rc, &flag_all, all_buffer);
	else
		Cls(prgpar.c_all, all_ur, all_lc, all_lr, all_rc);

	Box(prgpar.c_all, 4, all_ur, all_lc, all_lr, all_rc);

	WriteScreen(prgpar.c_all, all_ur,   all_lc+(all_rc-all_lc-strlen(tmp))/2+1, tmp);
	WriteScreen(prgpar.c_all, all_ur+1, all_lc+6,   "Bytes");
	WriteScreen(prgpar.c_all, all_ur+1, all_rc-6-6, "Errors");
	WriteScreen(prgpar.c_all, all_ur+2, all_lc+5+5, "0");
	WriteScreen(prgpar.c_all, all_ur+2, all_rc-6-1, "0");
	for (i=all_lc+2; i<=all_rc-2; i++)
		WriteScreen(prgpar.c_all, all_ur+4, i, "°");
	WriteScreen(prgpar.c_all, all_ur+5, all_lc+((all_lc-all_rc)/2), "0%");
}


void set_mes_on(void)
{
	mes_prg_flag = TRUE;
	mes_ibm_flag = TRUE;
	mes_err_flag = TRUE;
}

void set_mes_off(void)
{
	mes_prg_flag = FALSE;
	mes_ibm_flag = FALSE;
	mes_err_flag = FALSE;
}

void mes_err(char *ms)
{
	if (mes_err_flag)
		{
		ScrollUp(prgpar.c_all, err_ur, err_lc, err_lr, err_rc, 1);
		WriteScreen(WHITE_BLACK, err_lr, err_lc, ms);
		}
}


void restore_prg_w(void)
{

	if (flag_prg == 0 || flag_prg == -2)  /* Window was open */
		{
		Window(prgpar.c_all, prgm_ur, prgm_lc, prgm_lr, prgm_rc, &flag_prg, prg_buffer);
		Box(prgpar.c_all, 1, prgm_ur, prgm_lc, prgm_lr, prgm_rc);
		WriteScreen(prgpar.c_all, prgm_ur, ibm_lc, PRGW_IBM);
		WriteScreen(prgpar.c_all, prgm_ur, prg_lc, PRGW_PRG);
		
		Window(prgpar.c_all, errm_ur, errm_lc, errm_lr, errm_rc, &flag_err, err_buffer);
		Box(prgpar.c_all, 1, errm_ur, errm_lc, errm_lr, errm_rc);
		WriteScreen(prgpar.c_all, errm_ur, err_lc, PRGW_ERRORS);
		WriteScreen(prgpar.c_all, errm_lr, err_lc+7,  "W");
		WriteScreen(prgpar.c_all, errm_lr, err_lc+11, "R");
		
		WriteScreen(prgpar.c_all, 0, 0, PRESS_ESC_FOR_INT);
		DrawBoxLine(prgpar.c_all, 1, prgm_ur, ibm_rc+1, prgm_lr, ibm_rc+1);
		}
}

void mes_prog_w(long *daddr, uint num_err)
{
int new_percent;
int i;

	wprintf(prgpar.c_all, all_ur+2, all_lc+6,   "%5lu", (*(daddr))-stadrdev);
	wprintf(prgpar.c_all, all_ur+2, all_rc-5-6, "%5u", num_err);

	if (*daddr >= endadrdev)
		{
		old_percent=new_percent=100;
		WriteScreen(prgpar.c_all, all_ur+5, all_lc+((all_rc-all_lc)/2)-2,  "100%");
		for (i=all_lc+2; i<=all_rc-2; i++)
			WriteScreen(prgpar.c_all, all_ur+4, i, "Û");

		}
	else
		{
		new_percent = (uint)(((*daddr-stadrdev)*100) / (endadrdev-stadrdev));
		if (new_percent > old_percent)
			{
			old_percent = new_percent;
			wprintf(prgpar.c_all, all_ur+5, all_lc+((all_rc-all_lc)/2)-1, "%3u\x25\x25", new_percent);
			for (i=all_lc+2; i<=((all_rc-all_lc-4)*new_percent)/100+all_lc+1; i++)
				WriteScreen(prgpar.c_all, all_ur+4, i, "Û");
			}
		}
}

void save_all_w(void)
{
	Window(prgpar.c_all, all_ur, all_lc, all_lr, all_rc, &flag_all, all_buffer);
}

void mes_prg(char *ms)
{
	if (mes_prg_flag)
		{
		ScrollUp(prgpar.c_all, prg_ur, prg_lc, prg_lr, prg_rc, 1);
		WriteScreen(WHITE_BLACK, prg_lr, prg_lc+1, ms);
		}
}

void mes_ibm(char *ms)
{
	if (mes_ibm_flag)
		{
		ScrollUp(prgpar.c_all, ibm_ur, ibm_lc, ibm_lr, ibm_rc, 1);
		WriteScreen(WHITE_BLACK, ibm_lr, ibm_lc+1, ms);
		}
}


void restore_stat_w(uint alg)
{
	Window(prgpar.c_all, prgs_ur, prgs_lc, prgs_lr, prgs_rc, &flag_prgs, prgs_buffer);
	Box(prgpar.c_all, 1, prgs_ur, prgs_lc, prgs_lr, prgs_rc);
	WriteScreen(prgpar.c_all, prgs_ur, prgs_lc+1, STATISTICS);
	if (alg == 0) /* Normal */
		{
		WriteScreen(prgpar.c_normal, prgs_ur+10, prgs_lc+1, INFORMATION);
		WriteScreen(prgpar.c_normal, prgs_ur+11, prgs_lc+1, NOT_AV);
		WriteScreen(prgpar.c_normal, prgs_ur+12, prgs_lc+1, IN_NORMAL_ALG);
		}
	else
		{
		DrawBoxLine(prgpar.c_all, 1, prgs_ur+1, prgs_rc-2, prgs_lr-1, prgs_rc-2);
		}
}

void save_stat_w(void)
{
	Window(prgpar.c_all, prgs_ur, prgs_lc, prgs_lr, prgs_rc, &flag_prgs, prgs_buffer);
}

void drvr_ins(void)
{
int mask;
int tmp;
uint _far *fp;

	if (drvr_active)
		return;

	comn = prgpar.port;
	port_add = port_addr[comn];
	
	FP_SEG(fp) = 0;
	FP_OFF(fp) = 0x400+comn*2;
	*fp = port_add;

	mask=inp(I_MASK);
	outp(I_MASK, 0xff);
	old_vector = _dos_getvect(hard_int[comn]);
	_dos_setvect(hard_int[comn], ascode);
 	
	set_ibm_baud(baudrate);

	linestat = (byte)inp(port_add+6);
	linestat = (byte)inp(port_add+6);

	outp(port_add+1, 0x03);	/* enable rcvr and xmtr interrupts */
	inp(port_add+2);  		/* clear pending interrupts */
	inp(port_add);			/* clear rcvr interrupt by reading rxb */

	outp(port_add+4, 0x09);	/* reset programmer*/

	for (tmp=0; tmp<20000; tmp++)
		;

	outp(port_add+4, 0x08); 	/* enable dtr, rts and interrupt (out 2) */
	outp(I_MASK, mask & iorq_int[comn]);

	switch (comn)
		{
		case 0: outp(port_addr[2], 0xff); break;
		case 1: outp(port_addr[3], 0xff); break;
		case 2: outp(port_addr[0], 0xff); break;
		case 3: outp(port_addr[1], 0xff); break;
		}

	if ((inp(port_add+2) & 0xf8) != 0)
		message_screen(prgpar.c_error, "%s (COM%i)", COM_PORT_NOT_FOUND, comn+1);
	else
		drvr_active = 1;
}

void drvr_rem(void)
{
int mask;
int i;

	if (!drvr_active)
		return;

	send_b(0xff);
	for (i=0; i<1000; i++)
		;

	mask=inp(I_MASK);
	outp(I_MASK, 0xff);
	_dos_setvect(hard_int[comn], old_vector);
	outp(I_MASK, mask);

	drvr_active = 0;
}

/*
	ascode - MNP RS-232 interrupt service routine

     This routine is called by 'async' (in module 'async.asm')
     to service an async I/O interrupt.
*/

void _interrupt _far ascode(void)
{

/* Handle interrupts for as long as there are any. 
*/

for (;;)
	{

/* Read the Interrupt Identification Register.  If bit 0 is on, 
** there is no interrupt.  In this case cancel the interrupt by writing ** the "end-of-interrupt" byte to the Oper Ctl Word 2 and return. 
*/
	
	int_id_reg = inp(port_add+2);

	if ((int_id_reg & BIT0_ON) == 0x01)
		{
			outp(INT_8259A_PORT, EOI);
			return;
		}

/* Since bit 0 is off, there is an interrupt.  Take action based
** on the type of interrupt. 
*/

	switch (int_id_reg)
		{

/* Receive buffer full interrupt
*/
		case RCV_DATA:				
			rcv_block();
			break;

/* Transmit buffer empty interrupt
*/
		case THR_EMPTY:
			snd_block();
			break;

/* Modem status change interrupt
*/
		case MODSTAT_CHNG:
			break;

		}
	}
}

/*
	trigger_tx - initiate send framer
*/

void trigger_tx(void)
{

	if (!modem_out_busy)
		{
		block_tx = FALSE;
		tx_count = 0;
		modem_out_busy = 1;
		tx_state = TX_ONE;
		outp(port_add, tx_buffer[0]);
		if (tx_number == 0x01)
			st_busy = FALSE;
		else
			{
			st_busy = TRUE;
			tx_count++;
			}

		}
}

/*
	Receive block
*/

void rcv_block()
{

byte rcv_char;

rcv_char = (char)inp(port_add);

/* Take action based on current state of receive finite state machine.
** 'rf_state' is the state variable.
*/
	switch (rx_state)
	{
		case RX_BLOCK:
		{
			rx_buffer[rx_count] = rcv_char;
			rx_count++;
			sum += rcv_char;
			timer_1 = 3;
			break;
		}
	}
}

/*
	snd_block  - handle a transmit holding register empty
		        interrupt.  This subroutine sends a byte-mode frame.
*/

void snd_block()
{
byte snd_char;

/* Take action based on the current state of the framer state machine. 
*/

switch (tx_state)
	{

    	case TX_ONE:
		if (st_busy)
		{
			snd_char = tx_buffer[tx_count];
			outp(port_add, snd_char);
			tx_count++;
			tx_state++;
			if (tx_number == tx_count)
			   st_busy = FALSE;
			break;
		}
		else
		{
			modem_out_busy = 0;
			block_tx = TRUE;
		}
		break;

    	case TX_BLOCK :

		snd_char = tx_buffer[tx_count];
		outp(port_add, snd_char);
		tx_count++;
		if (tx_count == tx_number)
		{
			st_busy = FALSE;
			tx_state = TX_ONE;
		}
		break;

	}
}

int fos_init(void)
{

	regs.h.ah = FOS_INIT;
	regs.h.al = 0;
	regs.x.dx = prgpar.port;
	int86(0x14, &regs, &regs);
   return ((regs.x.ax != FOS_SIGNATURE || regs.h.bh < 5 || regs.h.bl < 0x1b) ? 0 : 1);
}

int get_fos_info(void)
{
	regs.x.ax = 0x1b00;
	regs.x.cx = sizeof (struct fos_inform);
	regs.x.dx = prgpar.port;
	segread(&sregs);
	sregs.es  = sregs.ds;
	regs.x.di = (uint)&fos_info;
	int86x(0x14,&regs,&regs,&sregs);

	return (regs.x.ax == sizeof (struct fos_inform));
}

void com_purge(void)
{
	rx_state = RX_BLOCK;
	rx_count = 0;
/*	n_rx = 0;*/
	timer_1 = 10;
	sum = 0;

	if (fossil)
		{
		regs.h.ah = FOS_PURGE;
		int86(0x14, &regs, &regs);
		fos_inread = 0;
		fos_inwrite = 0;
		}
}

int fos_fillinbuf(void)
{
	regs.x.ax = 0x1800;
	regs.x.dx = prgpar.port;
	regs.x.cx = RX_BLOCK_LENGTH;
	segread(&sregs);
	sregs.es  = sregs.ds;
	regs.x.di = (uint)&rx_buffer[rx_count];
	int86x(0x14,&regs,&regs,&sregs);
	fos_inwrite = regs.x.ax;
	fos_inread = 0;
	rx_count += regs.x.ax;

/*	for (i=0; i<regs.x.ax; i++)
!!!		screen_char_in(channel_s[number].temp_rx[i]);*/

	return (fos_inwrite);
}

void rx_block_progress(void)
{
  	fos_fillinbuf();
}

