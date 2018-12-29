/*
	<C> <Lion>

	prg_ibm.c

	Programm for Programmer on 8048

	Functions for work via RS232

	920225	v1.0 Beta6

	Functions:
			void	read_o(char *, byte *, char)
			int	send_one_command(byte, uint, uint, uint)
			void	prg_set_chip(char *)
			void	prg_set_power(char *)
			void	dump_o(char *, byte *)
			void	prg_chip(char *)
			void 	prg_run_test(void)
			void	reset_prg(void)
			void 	set_tx_buffer_command(byte, uint, uint, uint )
			void	set_tx_buffer_bytes(int, byte, byte)
			void	prg_mess_err(uint, byte);
			void	end_prg(void);
*/

#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#include <conio.h>
#include <scl1.h>
#include <scl1clor.h>
#include <scl1keys.h>
#include "prg_def.h"
/*
			Functions prototype
*/
void	read_chip(char);
void	read_chip_o(void);
void	read_chip_p(void);
int 	send_one_command(c_m_str *, uint, uint, uint);
void	prg_set_chip(char *);
void	prg_set_power(char *);
void	dump_o(char *, byte *);
void	prg_chip(void);
void	prg_run_test(void);
void	reset_prg(void);
void	verify_chip(char);
uint	blank_check(int *);
uint	write_poss(uint *);
int 	ident(int);
int 	set_prg_baud(void);
void	set_chip(void);		
int 	prg_mess_err(long *, byte);
void	save_prg_w(void);
void	end_verify(uint, uint);
void	end_read_chip(char, uint);
void	end_prg(uint err, uint key);
void	end_write_poss(uint, uint);
void	end_blank_check(uint, uint);


/*
			Global variables
*/

/*c_m_str C_PRG_RESET 		=	{ 0x01, RESET_PROGRAMMER};*/
c_m_str C_READ_PROGRAM 	=	{ 0x02, READ_PRG_CHIP};
c_m_str C_READ_ORIGINAL	=	{ 0x03, READ_ORIG_CHIP};
c_m_str C_PRG_CHIP 		=	{ 0x04, SET_PRG_CHIP};
					/*
						Set Programming CHIP:
							0 - 2716
							1 - 2732
							2 - 2764
							3 - 27128
							4 - 27256
							5 - 27512
							6 - 8048
							7 - 8051
							8 - 8755
							9 - 27c64
							10- 27c128
							11- 27c256
							12- 27c512
					*/
c_m_str C_PRG_POWER		=	{ 0x05, SET_PRG_POWER};
					/*
						Set Programming POWER:
							0 - 12.5v
							1 - 13v
							2 - 21v
							3 - 23v
							4 - 25v
					*/
c_m_str C_PRG_START		=	{ 0x06, START_PRG};
/*c_m_str C_PRG_TEST		=	{ 0x07, TEST_VPP};		/* see prg_test.c */
c_m_str C_VERIFY_SUM 		=	{ 0x08, VERIFY_CHIP};
c_m_str C_VERIFY_BYTE 		=	{ 0x09, VERIFY_CHIP};
c_m_str	C_BLANK_CHECK		=	{ 0x0A, BLANK_CHECK_CHIP};
c_m_str C_PRG_IDENT		=	{ 0x0B, READ_INTEL_IDENT};
c_m_str	C_PRG_BAUD		=	{ 0x0C, SET_PRG_BAUD};
c_m_str	C_WRITE_POSS		=	{ 0x0D, WRITE_POSSIBILITY};

/**/

byte speed_int[] =
	{
		3,	/* 38400 */
		6,	/* 19200 */
	  	12,	/* 9600  */
		96	/* 1200  */
	};

/**/

/*
			External functions prototype
*/
extern	int 	read_binary(FILE *, byte *);
extern	void	send_b(byte);
extern	int 	receive_block(void);
extern	int 	read_par(char *, int, uint *, uint *, uint *);
extern	int 	set_end_addr(int);
extern	void	send_block(int);
extern	uint	receive_b(int);
extern	void	set_c_sum(void);
extern	void	set_port(void);
extern	void	cwsay(int w, int row, char *str);
extern	void	errors(char *str);
extern	void	*malloc_e(uint);
extern	void	write_chip_no(byte mc, byte dc);
extern	void	write_chip_screen(void);
extern	int 	search_dev(byte mc, byte dc);
extern	void	set_ibm_baud(byte);
extern	void	j_exit(void);
extern	void	address_screen_off(void);
extern	void	device_screen_off(void);
extern	int 	select_device(int, char **, char **);
extern	long	size_x[];
extern	void	address_screen_on(void);
extern	void	device_screen_on(void);
extern	void	mes_err(char *);
extern	void	set_mes_on(void);
extern	void	set_mes_off(void);
extern	void	mes_ibm(char *);
extern	void	mes_prg(char *);
extern	void	mes_prog(char *);
extern	void	mes_prog_w(long *, uint);
extern	void	save_prg_w(void);
extern	void	restore_all_w(char *);
extern	void	save_all_w(void);
extern	void	message_screen(int, char *, ...);
extern	void	sel_state(int *, uint *);
extern	int	old_percent;
extern	void	restore_prg_w(void);
extern	void	set_tx_buffer_bytes(long *, byte, byte);
extern	void restore_prg_w(void);
extern	uint GetEsc(void);
extern	void restore_stat_w(uint);
extern	void save_stat_w(void);


/*
			External variable
*/
extern	char	prg_buffer[];
extern	char	flag_prg;
extern	char	percent_line[];
extern	char	typebus;		/* 16 or 8 or lo or hi bus 		*/
extern	char	*seleoa[];
extern	byte	mb1[];
extern	byte	rx_buffer[];
extern	byte	tx_buffer[];
extern	byte far	*bptr;
extern	byte speed_int[];
extern	int	rx_count;
extern	int	timer_1;
extern	int 	chip;
extern	int	d1,d2;
extern	int	w_prg;
extern	int	w_end_prg;
extern	int	w_prom_err;
extern	int	sw;
extern	int	w_ibm;
extern	int	w_prom;
extern	int	w_prom_err;
extern	int	sum;
extern	int	devtype;
extern	int	devpower;
extern	int	select_b;		/* Byte selected: ALL ODD EVEN */
extern	int	set_baud;
extern	int	block_tx;
extern	int	flag_open;
extern	uint	curadrbuf;	/* current adress buffer data for programing	*/
extern	uint	curnumdev;	/* current device number for programing		*/
extern	long	stadrbuf;	/* start adress buffer data for programing	*/
extern	long	endadrbuf;	/* end adress buffer data for programing	*/
extern	long	stadrdev;	/* start adress device for programing		*/
extern	long	endadrdev;	/* end adress device for programing		*/
extern	MSData mmd;
extern	struct device small_prg_dev;
extern	struct device far *prg_dev;
extern	par	prgpar;
extern	char err_buffer[];
extern	char flag_err;


void prg_run_test(void)
{
/*
	if (send_one_command(C_RUN_MB1, C_MB1_TEST, 0, 0) == 0)
		message_screen(prgpar.c_error, "%s", PRG_RX_C_ERROR);
*/
}


int set_prg_baud(void)
{
	if (send_one_command(&C_PRG_BAUD, speed_int[prgpar.speed], 0, 0)) /* Set baud rate */
		{
			set_ibm_baud(speed_int[prgpar.speed]);
			return (TRUE);
		}
	else
		return (FALSE);
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

	if ((sel=select_device(SEL_AUTO, &name, &type)) != -1)
		{
		if (sel == 0)
			{
			sel = ident(ROM27512);
			}

		if (sel != 0)
			{
/*kill		copy_small_far(sel, &small_prg_dev);
*/			_fmemcpy(&small_prg_dev, prg_dev+sel, sizeof(struct device));

			stadrdev = 0;
			endadrdev = size_x[small_prg_dev.size];
			stadrbuf = 0;
			endadrbuf = endadrdev;
			}
		}
	address_screen_on();
	device_screen_on();

	free(type);
	free(name);
}


int ident(int rom_n)
{
byte mc;
byte dc;
int attempt=0;
int sel=0;

	set_mes_off();

	if (send_one_command(&C_PRG_CHIP, rom_n, 0, 0)) /* Set 27512 chip */
		{
		if (send_one_command(&C_PRG_IDENT, 0, 1, 1))
			{
			while (rx_count != 3 && timer_1 != 0)
				{
				if (GetEsc() == ESC)
			   		break;
				}
			if (timer_1 == 0)
				{
				mes_prg(TIME_OUT_M);
				}
			else
				{
				if (!(char)sum)
					{
					mc = *rx_buffer;
					dc = *(rx_buffer+1);

					if ((mc == 0xff || dc == 0xff) && --rom_n > ROM27512)
						ident(rom_n);
					
					if (mc == 0xff || dc == 0xff)
						{
						restore_all_w(LREADING);
						message_screen(prgpar.c_error, "\n\n%s", CHIP_HAVNT_IDENT);
						save_all_w();
						sel = 0;
						}
					else
						{
						if ((sel = search_dev(mc, dc)) != 0)
							{
							write_chip_screen();
							}
						else
							{
							write_chip_no(mc, dc);
							}
						}
					}
				else
					{
						;/* csum error */
					}

				}
			}
		}
	set_mes_on();
	return sel;
}

void prg_chip(void)
{
int i,k;
int err;
byte csum;
int count_c=8;
int count_i=2;
int rb;
byte number_b;
uint num_err=0;
long prg_addr;
uint key;
char tmp[80];
long isum=0;
int line=prgs_ur+1;
long np, nl, nb;
int iflag;
char is[prgs_rc-prgs_lc+3];
long new_addr;
int nc;



	for (i=0; i<sizeof(is)-1; i++)
		is[i] = IPULSE_SYM;
	is[i]='\0';

	restore_prg_w();
	set_port();
	key = 0;
	err = 0;

	if (send_one_command(&C_PRG_CHIP, small_prg_dev.size,
							    (small_prg_dev.vpp<<8)+(small_prg_dev.vcc),
								small_prg_dev.algorithm) != 0)
		{
		if (prgpar.prg_blank == 'y')
			{
			flag_open = FALSE;
			if (((err = blank_check(&key)) > 0) && (key != ESC))
				{
				if (prgpar.prg_write_p == 'y')
					err = write_poss(&key);
				else
					end_blank_check(err, key);
				}
			flag_open = TRUE;
			}

		if (err == 0)
			{
			if (key != ESC)
				{
				if (send_one_command(&C_PRG_POWER, small_prg_dev.pulse_w/100,
											small_prg_dev.pulse_c,
											small_prg_dev.pulse_o/100) != 0)
					{
					if (send_one_command(&C_PRG_START, 0, 0, 0) != 0)
						{

						while ((rx_count < 2) && (timer_1 != 0))
							{
							;
							}

						rb=(timer_1 == 0)? -1 : *rx_buffer;

						if (rb == ACK)
							{
							restore_all_w(LPROGRAMMING);
							restore_stat_w(small_prg_dev.algorithm);
  							prg_addr = new_addr = stadrdev;
							old_percent = num_err = 0;

							while ((prg_addr <= endadrdev) && key!=ESC)
								{
								count_c = 8;
								do
									{
									receive_block();
									number_b=((endadrdev-prg_addr) < 4)? (byte)(endadrdev-prg_addr+1) : (byte)4;
									set_tx_buffer_bytes(&prg_addr, number_b, 0);
									send_block(BLOCK);
									iflag=TRUE;
								
									while (!block_tx)
										{
										if (iflag && small_prg_dev.algorithm!=0)
											{
											np=(prg_addr-stadrdev)*100 / (endadrdev-stadrdev);
											nl=prgs_ur+1+((prgs_lr-1) - (prgs_ur+1))*np /100;

											if (nl != line)
												{
												nb = (isum*100) / ((prg_addr-new_addr)*small_prg_dev.pulse_c);
												nc = (int)(nb * (prgs_rc-prgs_lc-3))/100;

												if (nc == 0)
													WriteScreen(prgpar.c_all, (int)nl, prgs_rc-nc-2, OK_M);
												else
													WriteScreen(prgpar.c_all, (int)nl, prgs_rc-nc-2, is+strlen(is)-nc);
											
/*
SetCurPos(3,55); printf("line=%i %i %li isum=%li", nl, nc, nb, isum);
SetCurPos(24,0); printf(" isum=%i  prga=%li  newa=%li pels=%i", isum, prg_addr, new_addr, small_prg_dev.pulse_c);
*/
												iflag=FALSE;
												line=(int)nl;
												isum=0;
												new_addr = prg_addr;
												}
											}
										}

									while ((rx_count < 2) && (timer_1 != 0))
										{
										if ((key = GetEsc()) == ESC)
											break;
										}

									rb=(timer_1 == 0)? -1 : *rx_buffer;
									err = FALSE;

									switch (rb)
										{
										case ACK :
											isum += *(rx_buffer+1);
											prg_addr += 4;
											iflag=TRUE;
											mes_prog_w(&prg_addr, num_err);
											break;
		
										case NAK	:
											sprintf(tmp,"NAK:%lX", prg_addr);
											mes_prg(tmp);
											count_c--;
											break;
				
										case ERROR_P:
											isum += small_prg_dev.pulse_c * number_b;
											err = TRUE;
											while((rx_count < 6) && (timer_1 != 0))
												{
												;
												}

											if (timer_1 != 0)
												{
												csum = 0;
												for (k=0; k<6; k++)
			 									csum += *(rx_buffer+k);
												if (csum != 0)
													{
													count_i -= 1;
													err = TRUE;
													}
												else
													num_err += prg_mess_err(&prg_addr, number_b);

												mes_prog_w(&prg_addr, num_err);
												prg_addr += 4;
												}
											break;

										case -1 :
											mes_ibm(TIME_OUT_BY_RX_BYTE);
											if (send_one_command(&C_PRG_START, 0, 0, 0) == 0)
												{
												message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
												count_c = 0;
												}
											break;
				
										default :
											sprintf(tmp, "%s:%02X", RX_UNKNOWN_BYTE, rb);
											mes_ibm(tmp);
											if (send_one_command(&C_PRG_START, 0, 0, 0) == 0)
												{
												message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
												count_c = 0;
												}
											break;
										}

									}
								while(rb!=ACK && count_c!=0 && err!=TRUE && key!=ESC);

								if (count_c==0 && key!=ESC)
									{
	    								message_screen(prgpar.c_error, "\n\n%s", PRG_LINE_ERROR);
									break;
									}
								}
							}

							if (key!=ESC && count_c!=0)
							{
								do
									{
									receive_block();
									set_tx_buffer_bytes(0, 4, 1);
									send_block(BLOCK);
									while (!block_tx)
										{
										;
										}
									while ((rx_count == 0) && (timer_1 != 0))
										{
										;
										}
									if((rb=*rx_buffer) != ACK)
										count_i--;

									}
								while (rb!=ACK && count_i!=0);
							}

						if (num_err == 0 && key!=ESC)
					    		{
							flag_open=FALSE;
							WaitTime(50);
							verify_chip('s');
							flag_open=TRUE;
							}
						else
					    		{
							end_prg(num_err, key);
							save_all_w();
							}
				
						save_stat_w();
						}
					else
						{
						;
						}
					}
				}
			}
		}
	save_prg_w();
}

void read_chip_o(void)
{
	read_chip('o');
}

void read_chip_p(void)
{
	read_chip('p');
}

void read_chip(char chip)
{
int k;
c_m_str command;
long addr;
int err;
char tmp_str[20];
int block_length;
long ltemp;
uint key=0;


	restore_prg_w();
	set_port();

	if (send_one_command(&C_PRG_CHIP, small_prg_dev.size, 0, 0) != 0)
		{
		if (chip == 'o')
			command = C_READ_ORIGINAL;
		else
			command = C_READ_PROGRAM;

		restore_all_w(LREADING);
		old_percent = 0;
		addr = stadrdev;
		err = 0;

		do
			{
			if (endadrdev-addr > RX_BLOCK_LENGTH)
				block_length = RX_BLOCK_LENGTH;
			else
				block_length = (uint)(endadrdev-addr+1l);


			if (send_one_command(&command, (uint)addr, (uint)endadrdev, block_length) == 0)
				{
				message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
				err = 1;
				}
			else
				{
				do
					{
					while (
						  (rx_count != block_length+1) &&
						  (timer_1 !=0) 			 &&
						  ((key=GetEsc()) != ESC)
						 )
						{
						ltemp = addr+rx_count;
						mes_prog_w(&ltemp, 0);
						}
					
					if (key==ESC)
						break;

					if (timer_1 == 0)
						{
						mes_prg(TIME_OUT_M);
						sprintf(tmp_str, "\n%u", rx_count);
						mes_prg(tmp_str);

						receive_block();
						send_b(NAK);
						err = 1;
						}
					else
						{
						if ((byte)sum != 0)
							{
							mes_prg(SUM_ERR_M);
							receive_block();
							send_b(NAK);
							err = 1;
							}
						else
							{
							for (k=0; k<block_length; k++)
								{
								*(bptr+addr) = rx_buffer[k];
								addr++;
								}
							mes_prog_w(&addr, 0);
							receive_block();
							send_b(ACK);
							err = 0;
							}
						}
					}
				while ((err == 0) && (addr < endadrdev) && key!=ESC);
				}
			} while (addr<endadrdev && key!=ESC);
	
		end_read_chip(chip, key);
		save_all_w();
		}
	save_prg_w();
}

void verify_chip(char rev)
{
int k;
c_m_str command;
long addr;
int err;
char tmp_str[15];
int block_length;
uint csum;
uint rcsum;
byte error_screen;
int r_byte;
uint num_err;
uint old_num_err=0;
uint key=0;
byte tmpb;

	old_percent = 0;
	error_screen = 0;
	num_err = 0;

	restore_prg_w();
	set_port();

	if (send_one_command(&C_PRG_CHIP, small_prg_dev.size, 0, 0) != 0)
		{
		restore_all_w(LVERIFY);

		if (rev == 's')
			command = C_VERIFY_SUM;
		else
			command = C_VERIFY_BYTE;


		addr = stadrdev;
		err = 0;

		do	
			{
			if (endadrdev-addr > RX_BLOCK_LENGTH)
				block_length = RX_BLOCK_LENGTH;
			else
				block_length = (uint)(endadrdev-addr+1l);


			if (send_one_command(&command, (uint)addr, (uint)endadrdev, block_length) == 0)
				{
				message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
				err = 1;
				}
			else
				{
				do
					{
					if (rev == 's')
						{
						if (error_screen == 0 )
							{
							while ((rx_count != 3) && (timer_1 !=0))
								{
								;
								}
							}
						else
							{
							r_byte = 0;
							old_num_err = num_err;

							while ((rx_count != block_length+1) && (timer_1 !=0))
								{
								if (rx_count > r_byte)
									{
									if ((tmpb=*(bptr+addr+r_byte)) != rx_buffer[r_byte])
										{
										sprintf(tmp_str, "%04X  %02X  %02X", (uint)addr+r_byte,
															   	  	  tmpb,
																 	  rx_buffer[r_byte]);
										mes_err(tmp_str);
										num_err++;
										}
									r_byte++;
									}
								}
							}
						
						if (timer_1 == 0)
							{
							mes_prg(TIME_OUT_M);
							sprintf(tmp_str, "\n%u", rx_count);
							mes_prg(tmp_str);
							num_err = old_num_err;

							receive_block();
							send_b(NAK);
							err = 1;
							}
						else
							{
							if ((byte)sum != 0)
								{
								mes_prg(SUM_ERR_M);
								receive_block();
								send_b(NAK);
								num_err = old_num_err;
								err = 1;
								}
							else
								{					  	
													/* No previos block error*/
								if (error_screen == 0)
									{
									for (csum=0, k=0; k<block_length; k++)
										{
										csum += *(bptr+addr);
										addr++;
										}
									rcsum = rx_buffer[1];
									rcsum = (rcsum << 8) + rx_buffer[0];
									if (csum != rcsum)
										{
										receive_block();
										send_b(NO_CSUM);
										err = 0;
										addr -= block_length;
										error_screen = 1;
										}
									else
										{
										receive_block();
										send_b(ACK);
										err = 0;
										}
									}
								else /* Previos block had errors. Clear flag errors on screen.*/
									{
									addr +=block_length;
									error_screen = 0;
									receive_block();
									send_b(ACK);
									err = 0;
									}
								
								mes_prog_w(&addr, num_err);

								} /* csum */

							} /* time_out */

						} /* if rev == 's'*/

					} while ((err == 0) && (addr < endadrdev) && (key=GetEsc())!=ESC);
				}

			if (key == ESC)
				break;

			} while (addr < endadrdev);

		end_verify(num_err, key);
		save_all_w();
		}

	save_prg_w();
}


uint blank_check(int *key)
{
long addr;
uint err;
char tmp_str[15];
int  block_length;
uint num_err=0;
uint utemp;

	restore_prg_w();
	set_port();

	if (send_one_command(&C_PRG_CHIP, small_prg_dev.size, 0, 0) != 0)
		{
		*key = 0;
		addr = stadrdev;
		err = 0;
		num_err = 0;
		old_percent = 0;

		do
			{
			if (endadrdev-addr > RX_BLOCK_LENGTH)
				block_length = RX_BLOCK_LENGTH;
			else
				block_length = (uint)(endadrdev-addr+1l);


			if (send_one_command(&C_BLANK_CHECK, (uint)addr, (uint)endadrdev, block_length) == 0)
				{
				message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
				err = 1;
				}
			else
				{
				restore_all_w(LBLANKCHECK);
				
				do
					{
					while (
							(rx_count != 3) &&
							(timer_1 !=0)   &&
							((*key = GetEsc()) != ESC))
				   		{
							;
				   		}
					
					if (*key == ESC)
						break;

					if (timer_1 == 0)
						{
						mes_prg(TIME_OUT_M);
						sprintf(tmp_str, "\n%u", rx_count);
						mes_prg(tmp_str);
						receive_block();
						send_b(NAK);
						err = 1;
						}
					else
						{
						if ((byte)sum != 0)
							{
							mes_prg(SUM_ERR_M);
							receive_block();
							send_b(NAK);
							err = 1;
							}
						else
							{
							utemp = (rx_buffer[1] << 8) + rx_buffer[0];
							num_err += utemp;
							receive_block();
							send_b(ACK);
							addr += block_length;
							err = 0;
							mes_prog_w(&addr, num_err);
							if (num_err > 0)
								addr = endadrdev;
							} /* Csum */

						} /* time_out */

					} while ((err == 0) && (addr < endadrdev) && *key != ESC);

				if (*key == ESC)
					break;
				}
			}
		while (addr < endadrdev);

		if (flag_open==TRUE || err!=0)
			{
			end_blank_check(num_err, *key);
			save_all_w();
			}
		}

	save_prg_w();
	return(num_err);
}

uint write_poss(uint *key)
{
int	k;
long	addr;
int	err;
char	tmp_str[20];
int	block_length;
uint	num_err=0;
long	ltemp;
int	sel;
uint	mul;
byte	btemp;

	restore_prg_w();
	set_port();

	if (send_one_command(&C_PRG_CHIP, small_prg_dev.size, 0, 0) != 0)
		{
		*key = 0;
		addr = 0; /*!!!*/
		err = 0;
		old_percent = 0;
		num_err = 0;

		do
			{
			if (endadrdev-addr > RX_BLOCK_LENGTH)
				block_length = RX_BLOCK_LENGTH;
			else
				block_length = (uint)(endadrdev-addr+1l);


			if (send_one_command(&C_READ_PROGRAM, (uint)addr, (uint)endadrdev, block_length) == 0)
				{
				message_screen(prgpar.c_error, "\n\n%s", PRG_RX_C_ERROR);
				err = 1;
				}
			else
				{
				restore_all_w(LWRITEPOSSIBILITY);
				do
					{
					while ((rx_count != block_length+1) && (timer_1 !=0))
						{
						ltemp = addr+rx_count;
						mes_prog_w(&ltemp, num_err);
						if ((*key = GetEsc()) == ESC)
					   		break;
						}

					if (timer_1 == 0)
						{
						mes_prg(TIME_OUT_M);
						sprintf(tmp_str, "\n%u", rx_count);
						mes_prg(tmp_str);
						receive_block();
						send_b(NAK);
						err = 1;
						}
					else
						{
						if ((byte)sum != 0)
							{
							mes_prg(SUM_ERR_M);
							receive_block();
							send_b(NAK);
							err = 1;
							}

						else
							{
							sel_state(&sel ,&mul);

							for (k=0; k<block_length; k++)
								{
								btemp = *(bptr + stadrbuf + stadrdev + sel + ((addr - stadrdev) * mul));
								if (	(btemp & rx_buffer[k]) != btemp)
									num_err++;
								addr++;
								}
							mes_prog_w(&addr, num_err);
							receive_block();
							send_b(ACK);
							err = 0;
							if (num_err > 0)
								addr = endadrdev;
							}
						}
					}
				while (err==0 && addr<endadrdev && *key!=ESC);

				if (*key == ESC)
					break;
				}
			}
		while (addr < endadrdev);
		
		if (flag_open==TRUE || err!=0)
			{
			end_write_poss(num_err, *key);
			save_all_w();
			}
		}

	save_prg_w();

	return(num_err);
}

void end_prg(uint err, uint key)
{
	if (key == ESC)
		{
		if (err == 0)
			message_screen(prgpar.c_error, 	CANCEL_0_ERRORS,
										PRG_CANCEL,
							    			ERRORS, DURING_PRG
					    );
		else
			message_screen(prgpar.c_error,	CANCEL_1_ERRORS,
										PRG_CANCEL,
							    			err,ERRORS, DURING_PRG
					    );
		}
	else
		{
		if (err == 0)
			message_screen(prgpar.c_normal,	COMPLETE_0_ERRORS,
										PRG_COMPLETE,
								    		ALL_BYTE_PRG
					    );
		else
			message_screen(prgpar.c_error,	COMPLETE_1_ERRORS,
										PRG_COMPLETE,
								    		err, ERRORS, DURING_PRG
					    );
		}
}

void end_verify(uint err, uint key)
{
	if (key == ESC)
		{
		if (err == 0)
			message_screen(prgpar.c_error,	CANCEL_0_ERRORS,
					    					VERIFY_CANCEL,
							    			ERRORS, DURING_PROCESS
					    );
		else
			message_screen(prgpar.c_error,	CANCEL_1_ERRORS,
										VERIFY_CANCEL,
							    			err,ERRORS, DURING_PROCESS
					    );
		}
	else
		{
		if (err == 0)
			message_screen(prgpar.c_normal,	COMPLETE_0_ERRORS,
					    					VERIFY_COMPLETE,
										ALL_BYTE_COMPARE
				    	);
		else
			message_screen(prgpar.c_error,	COMPLETE_1_ERRORS,
					    					VERIFY_COMPLETE,
								      	err, BYTES_NOT_COMPARE,
										FREE
				    	);

		}
	
}

void end_blank_check(uint err, uint key)
{
	if (key == ESC)
		{
		if (err == 0)
			message_screen(prgpar.c_error,	CANCEL_0_ERRORS,
										BLANK_CHECK_CANCEL,
							    			ERRORS, DURING_PROCESS
					    );
		else
			message_screen(prgpar.c_error,	CANCEL_1_ERRORS,
										BLANK_CHECK_CANCEL,
							    			err,ERRORS, DURING_PROCESS
					    );
		}
	else
		{
		if (err == 0)
			message_screen(prgpar.c_normal,	COMPLETE_0_ERRORS,
					    					BLANK_CHECK_COMPLETE,
										ALL_BYTE_CLEAR
						);
		else
			message_screen(prgpar.c_error,	COMPLETE_1_ERRORS,
					    					BLANK_CHECK_COMPLETE,
								      	err, BYTES_NOT_CLEAR,
										FREE
						);
		}
}


void end_write_poss(uint err, uint key)
{
	if (key == ESC)
		{
		if (err == 0)
			message_screen(prgpar.c_normal,	CANCEL_0_ERRORS,
										WRITE_POSS_CANCEL,
							    			ERRORS, DURING_PROCESS
					    );
		else
			message_screen(prgpar.c_error,	CANCEL_1_ERRORS,
										WRITE_POSS_CANCEL,
							    			err,ERRORS, DURING_PROCESS
					    );
		}
	else
		{
		if (err == 0)
			message_screen(prgpar.c_normal,	COMPLETE_0_ERRORS,
										WRITE_POSS_COMPLETE,
								    		BUFFER_MAY_WRITE
					    );
		else
			message_screen(prgpar.c_error,	COMPLETE_1_ERRORS,
										WRITE_POSS_COMPLETE,
								    		err, ERRORS, DURING_PROCESS
					    );
		}
}

void end_read_chip(char chip, uint key)
{
char temp[20];

	if (chip == 'o')
		strcpy(temp, ORIGINAL);
	else
		strcpy(temp,PROGRAMMING);

	if (key == ESC)
		message_screen(prgpar.c_error, "%s\n\n(%s)",	READ_CANCEL, temp);
	else
		message_screen(prgpar.c_normal, "%s\n\n(%s)",READ_COMPLETE, temp);
}


int prg_mess_err(long *addr, byte number_b)
{
byte	num;
int	err;
char	tmp_str[20];
int	sel;
uint	mul;

	num = 0;
	err = 0;

	sel_state(&sel, &mul);

	while (num < number_b)
		{
		if (*(bptr + stadrbuf + stadrdev + sel + ((*addr - stadrdev + num) * mul)) != *(rx_buffer+1+num))
			{
			sprintf(tmp_str, "%04X  %02X  %02X",
										(uint)*addr+num,
										*(bptr + stadrbuf + stadrdev + sel + ((*addr - stadrdev + num) * mul)),
										*(rx_buffer+1+num));
			mes_err(tmp_str);
			err++;
			}
		num++;
		}
	return(err);
}

void save_prg_w(void)
{
	if (flag_open == TRUE)
		{
		Window(prgpar.c_all, prgm_ur, prgm_lc, prgm_lr, prgm_rc, &flag_prg, prg_buffer);
		Window(prgpar.c_all, errm_ur, errm_lc, errm_lr, errm_rc, &flag_err, err_buffer);
		MenuSystem(MS_DRAW, &mmd); 	/* Draw menu system	*/
		}
}



