/*
	<C> <Lion>

	prg_def.h

	Programm for Programmer on 8048

	Defines & i.t.c

	910614	v1.0

*/

#define byte unsigned char
#define uint unsigned int
#define ulong unsigned long
#define word unsigned int

#define TRUE 1
#define FALSE 0

#define normal 0		/* Normal algorithm */
#define intel 1			/* Intelligent algorithm */
#define quick 2			/* QuickPulse algorithm */

#define S_9600	12
#define S_19200	6
#define S_38400	3

#define ROM27512 5
#define ROM27256 4
#define ROM27128 3
#define ROM2764  2
#define ROM2732  1
#define ROM2716  0

#define CC_HEX "1234567890ABCDEFabcdef"
#define CC_ALGORITHM "NIQniq"
#define PRG_HEXEDITOR 	"prghex"
#define PRG_DEVICE 	"prgdev"
#define PRG_SET		"prgset"
#define TMP_FILE_NAME	"##prgtmp.tmp"
#define PROC_TEST_VPP	1
#define PROC_TEST_VCC	2
#define PROC_TEST_PINS	3
#define PROC_SET_COM	4
#define PROC_SET_COLOR	5
#define PROC_PRG_SET	6
#define PROC_TEST_A4VPP	7
#define PROC_READ_FILE	8
#define PROC_WRITE_FILE	9
/**/
#define SEL_AUTO	0
#define SEL_NEW		1
#define SEL_ROM		2

/*#define DEBUG 1*/

/*
			Structures Define
*/
typedef struct {
			int i;
			int row, col;
			char name[15];
			int par, ccol;
			int l,r,u,d;
		} win_set_str;

typedef struct
		{
			char name[15];
			int row,crow,ccol;
		} win;

typedef struct fstr
		{
			long attempts;
			long send;
			long block;
			long errors;
		};

typedef struct
		{
			byte c;
			char m[30];
		} c_m_str;

struct SELECT_D   	     	/* Structure of Window Options Array element 0	*/
	{               		/* [0].pid=# of items, [0].pick= window size    	*/
	char	*t;		/* Title to Print. Max Length is Window Size    	*/
	int	prgdev;		/*Number device for programmer			*/
	int	prgpow;		/*Number power for programmer			*/
	int	num;     	/* Number items 0-3 bit type size 4-8 algoritm  	*/
	};

struct SELECT_F       		/* Structure of Window Options Array element 0  */
	{               		/* [0].pid=# of items, [0].pick= window size    */
	char *f;	     		/* Title to Print. Max Length is Window Size    */
	struct SELECT_D *p; 	/* Pointer to structure for this entry */
	};

struct SELECT_P
	{
		char *s;
		int  prgpow;
	};

struct	SIZE_D
	{
		int dev;
		long size;
	};

struct device
	{
		char	firma[15];
		char	type[15];
		uint	size;
		uint	vpp;
		uint	vcc;
		uint	vendor;
		uint	device;
		uint	pulse_w;
		uint	pulse_c;
		uint	pulse_o;
		uint	algorithm;
		uint	temp1;
		uint	temp2;
		uint	temp3;
		uint	temp4;
		uint	temp5;
		uint	temp6;
		uint	temp7;
		uint	temp8;
		uint	temp9;
	};

typedef struct
		{
			int port;
			int speed;
			int c_menu;
			int c_all;
			int c_normal;
			int c_error;
			int c_hex_border;
			int c_hex_hex;
			int c_hex_asc;
			char prg_blank;
			char prg_write_p;
			char prg_firma[15];
			char prg_device[15];
			char file_read[80];
			char file_write[80];
			byte fill_byte;
			char ext_hexed[80];
		}par;
/*
*/
#define MAXSTRING 	256
#define MASS_SIZE	65536*2
#define NUMBER_DUMP_STR 16
#define RX_BLOCK_LENGTH	1024		/* length receiv's block */
#define N_TX_COMMAND 	3		/* attempts send command number */
#define N_RESET		2		/* number attempts send "reset" */
#define file_par_name	"prg.set"	/* programmer sets file name */
#define file_device_name	"prg.dev" 	/* programmer devices file name */
#define num_prg 6
/**/
#define OK			0+256
#define HEX_OK			0+256
#define HEX_ERROR_FORMAT 	1+256
#define HEX_END          	2+256
#define HEX_ERROR_SUM		3+256
/*
			Constants
*/
#define C_2716  0
#define C_2732  1
#define C_2764  2
#define C_27128 3
#define C_27256 4
#define C_27512 5
#define C_8048  6
#define C_8051  7
/**/
#define C_12V   0
#define C_21V   1
#define C_23V   2
#define C_25V   3
/**/
#define CR 			0x0D
#define NAK 			0x15
#define ERROR_P			0x60
#define NP			0x55
#define NC			0x56
#define ACK			06
#define NO_CSUM			0x57
#define PASSWORD		'L'
#define BLOCK			0x09		/*Number bytes per TX block*/
/*
*/
/*
	C_RUN_MB1		07	Run subroutine in MB1*/
/*
	Memory Bank 1 subroutine
*/
#define C_MB1_TEST		01	/*Test subroutine in MB18*/
/*
	*******************************
*/
#include "prg_win.h"
#include "prg_mess.h"

