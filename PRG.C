/*
	<C> <Lion>

	prg.c

	Programm for Programmer on 8048

	910614	v1.0

*/


#include <stdio.h>
#include <io.h>
#include <string.h>
#include <process.h>
#include <ctype.h>
#include <conio.h>
#include "prg_def.h"
/*
			Functions prototype
*/
uint 		atox(char *str);
void 		clear(char *, byte *);
void 		dump_b(char *, byte *);
void 		quit( int error );
void 		phelp(void);
char 		errors_y(char *);
int 		read_par(char *, int, uint *, uint *, uint *);
void 		test(void);
int		set_end_addr(int);
/*
			External functions prototype
*/
extern	void		set_port(int);
extern	int 		readf(char *, byte *);
extern	int 		write_f(char *, byte *);
extern	void		j_exit(void);
extern	void		read_chip(char *, byte *, char);
extern	void		prg_set_chip(char *);
extern	void		prg_set_power(char *);
extern	void		reset_prg(void);
extern	void 		prg_run_test(void);
extern	void		dump_o(char *, byte *);
extern	void		prg_chip(char *);
/*
			Global variables
*/
FILE *hex_file;
byte mass[MASS_SIZE];
byte mb1[PRG_MB1_LENGTH]={0x00};
char tmp[80];
uint d1,d2;
int chip;
int vpp;
/*
			External variable
*/
extern	int	chan_no = 01;
extern	byte	tx_buffer[];
extern	byte	rx_buffer[];
extern  int	rx_count;
extern	int	timer_1;
extern	int	block_tx;
/*
			Start main programm here
*/

void main(void)
{
int ch;
	set_port(1);

	d1=0;
	clear("c",mass);
	printf("<C> <Lion>\nROM Programmer v1.0\n");
	while(1)
	{
	 	printf(">");
	 	gets(tmp);
	 	switch (tolower(tmp[0]))
	 	{
	  		case 'c' : clear(tmp,mass); break;
	  		case 'd' : switch (tolower(tmp[1]))
	  		{
	   			case 'b' : dump_b(tmp, mass); continue;
	   			case 'o' : dump_o(tmp, rx_buffer); continue;
		  		default  : quit(COMMAND_ERROR); continue;
	  		}

	  		case 'f' : clear(tmp,mass); break;
	  		case 'h' : phelp(); break;
			case 'p' : switch (tolower(*(tmp+1)))
			{
				case 'c' : prg_set_chip(tmp); continue;
				case 'p' : prg_set_power(tmp); continue;
				case 'r' : reset_prg(); continue;
				case 's' : prg_chip(tmp); continue;
				case 't' : prg_run_test(); continue;
		  		default  : quit(COMMAND_ERROR); continue;
			}
	  		case 'r' : switch (tolower(tmp[1]))
			{
				case 'f' : readf(tmp, mass); continue;
				case 'o' : read_chip(tmp, mass, 'o'); continue;
				case 'p' : read_chip(tmp, mass, 'p'); continue;
		  		default  : quit(COMMAND_ERROR); continue;
			}
	  		case 'q' : j_exit(); break;
	  		case 'w' : switch (tolower(tmp[1]))
			{
				case 'f' : write_f(tmp,mass); continue;
		  		default  : quit(COMMAND_ERROR); continue;
			}
	  		case CR  : break;
	  		case LF  : break;

	  		default  : quit(COMMAND_ERROR); break;
	 	}

	}
}

void phelp(void)
{
	printf("\nc[addr1] [addr2] [byte]                  - clear buffer");
	printf("\ndb[addr1] [addr2]                        - dump  buffer");
	printf("\ndo[addr1] [addr2]                        - dump  original chip");
	printf("\nf[addr1] [addr2] [byte]                  - fill  buffer with byte");
	printf("\nh                                        - help");
	printf("\nr [d:][path]name.ext                     - read file in buffer");
	printf("\nq                                        - quit in system");
	printf("\nw[addr1] [addr2] [d:][path]name.ext      - write buffer in file");
	printf("\n");
}






void dump_b(char *str, byte *mass)
{
uint p1,p2,p3;
register int i,j,k;
register byte l;

	i=read_par(str,2,&p1,&p2,&p3);

	if (i == -1)
		{
			quit(PAR_FORMAT_ERROR);
			_cexit;
		}
	else
		if (i == 0) d2=d1+NUMBER_DUMP_STR*16;
		else
			if (i == 1) { d1=p1; d2=p1+NUMBER_DUMP_STR*16; }
			else
				if (i == 2) { d1=p1; d2=p2; }
				else
					{
						quit(PAR_NUMBER_ERROR);
						_cexit;
					}


	i=d1/16; k=d2/16;
	for (i; i<k; i++)
		{
			printf("%04hX : ", d1);
			for (j=0; j<16; j++)
				{
					l=mass[d1+j];
					printf("%02X ", l);
				}
			printf("  ");
			for (j=0; j<16; j++)
				{
					l=mass[d1+j];
					if(l < 0x20)
						printf(".");
					else
						printf("%c", l);
				}
			d1+=j;
			printf("\n");
		}
}

void clear(char *str, byte *mass)
{
uint i,p1,p2,p3=0;
byte p3b;


	switch (read_par(str,1,&p1,&p2,&p3))
		{
		 case -1 :	quit(PAR_FORMAT_ERROR); break;
		 case  0 :
		 						for (i=0; i<MASS_SIZE; i++)
									mass[i] = 0xff;
		 						break;
		 case  1 :
								quit(PAR_NUMBER_ERROR);
								break;
		 case  2 :
								if ((p1 < MASS_SIZE) && (p2 < MASS_SIZE))
									{
		 								for (p1; p1<=p2; p1++)
											mass[p1] = 0xff;
										break;
									}
								else
									{
										quit(PAR_ADDRESS_ERROR);
										break;
									}
		 case  3 :
								if ((p1 < MASS_SIZE) && (p2 < MASS_SIZE))
									{
										if (p3 > 0xff)
											{
												quit(PAR_BYTE_ERROR);
												break;
											}
										else
											{
												p3b=(byte)p3;
		 										for (p1; p1<=p2; p1++)
													mass[p1] = p3b;
												break;
											}
									}
								else
									{
										quit(PAR_ADDRESS_ERROR);
										break;
									}
		}
}

int read_par(char *str, int start, uint *p1, uint *p2, uint *p3)
{
int i,j,k;
char tmp[80];
int npar=0;
uint p[3];

	i=start;

	do
		{
			if(isxdigit(str[i]))
				{}
			else
				if((str[i] == ',') || (str[i] == ' ') || (str[i] == '\0'))
					{
						if (str[i] == '\0')
							break;
					}
				else
					{
						quit(PAR_DIGIT_ERROR);
						npar=-1;
						break;
					}
		}while(str[++i] != '\0');

	i=start;
	if ((npar != -1) && (str[i] != '\0'))
		{
			if ((str[i] == ' ') || (str[i] == ',')) i++;

			npar=0;

			for (k=1; k<=3; k++)
				{
					j=0;
					while((str[i] != ' ') && (str[i] != ',') && (str[i] != '\0'))
						{
							tmp[j++] = str[i++];
						}
					if (j>0)
						{
							tmp[j]='\0';
							p[npar]=atox(tmp);
							npar++;
							if (str[i] != '\0')
								i++;
						}
				}
			*p1=p[0]; *p2=p[1]; *p3=p[2];
		}
	return(npar);
}

char errors_y(char *str)
{
	int ch;

		printf("%s? (Y/N):", str);
		ch=getche();
		printf("\n");
		return((char)ch);
}


/* Handles errors */
void quit( int error )
{
	switch( error )
	{

		case NAK_AT_RX_BLOCK :
			printf("\nNAK at RX BLOCKS. Too many errors.\n");
			break;

		case HEX_ERROR_FORMAT :
			printf("Error in HEX format\n");
			break;

		case HEX_ERROR_SUM		:
			printf("Check summ error in HEX file\n");
			break;

		case PAR_FORMAT_ERROR	:
			printf("Format invalid\n");
			break;

		case PAR_NUMBER_ERROR	:
			printf("Number parameters error\n");
			break;

		case PAR_ADDRESS_ERROR :
			printf("Address in parameter is to big\nValid address 0000 - 7FFF\n");
			break;

		case PAR_BYTE_ERROR :
			printf("Byte in parameter error\nValid range 00 - FF\n");
			break;

		case PAR_DIGIT_ERROR :
			printf("Invalid symbol in parameter string\n");
			break;

		case FILE_OPEN_ERROR:
			printf( "Can't open input file\n" );
			break;

		case FILE_ERROR:
			printf( "Error processing file\n" );
			break;

		case FILE_LENGTH_ZERRO:
			printf("Sorry Bambino, but Length of file = 0");
			break;

		case FILE_LENGTH_BIG:
			printf("File length is to big");
			break;

		case FILE_SEEK_ERROR:
			printf("File seek error");
			break;

		case FILE_CLOSE_ERROR:
			printf("File close error");
			break;

		case FILE_WRITE_ERROR:
			printf("Write file error");
			break;

		case COMMAND_ERROR:
			printf("Invalid command\n");
			break;

		case PRG_RX_C_ERROR:
			printf("Prgogrammer receive command failed\n");
			break;

		case PRG_LINE_ERROR:
			printf("Prgogrammer - IBM line failed\n");
			break;
	}
}


