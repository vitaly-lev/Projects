/*
	<C> <Lion>

	prg_err.c

	Programm for Programmer on 8048

	Errors Subroutines

	920227	V1.0 Beta7

	Functions:

*/


#include "prg_def.h"
#include <scl1.h>

/*
			Functions prototype
*/

/*
			External functions prototipe
*/

/*
			Local constants
*/

struct ErrorMess em[] =
	{
		NAK_AT_RX_BLOCK,		"\nNAK at RX BLOCKS. Too many errors.\n",
		HEX_ERROR_FORMAT,		"Error in HEX format\n",
		HEX_ERROR_SUM,			"Checksum error in HEX file\n",
		PAR_FORMAT_ERROR,		"Format invalid\n",
		PAR_NUMBER_ERROR,		"Number parameters error\n",
		PAR_ADDRESS_ERROR,		"Address in parameter is to big\nValid address 0000 - 7FFF\n",
		PAR_BYTE_ERROR,		"Byte in parameter error\nValid range 00 - FF\n",
		PAR_DIGIT_ERROR,		"Invalid symbol in parameter string\n",
		PAR_RANGE_ERROR,		"Start address Biggest End address\n",
		PAR_FILE_ERROR,		"Parameter file ERROR.\n\nSet default parameter\n",
		FILE_OPEN_ERROR,		"\nCan't open file\n" ,
		FILE_ERROR, 			"Error processing file\n" ,
		FILE_LENGTH_ZERRO,		"Sorry Bambino, but Length of file = 0",
		FILE_LENGTH_BIG,		"File length is to big",
		FILE_SEEK_ERROR,		"File seek error",
		FILE_CLOSE_ERROR,		"File close error",
		FILE_WRITE_ERROR,		"Write file error",
		FILE_NUM_B_BIG_ERROR,	"Number bytes for read bigest buffer length",
		FILE_WRITE_PAR_ERROR,	"Write SETUP FILE error",
		FILE_OPEN_PAR_ERROR,	"Read SETUP FILE error",
		FILE_READ_ERROR,		"Read file error",
		FILE_DEV_ERROR,		"File with parameters\nprogramming PROM\nread error",
		FILE_DEVICE_ERROR,		"Warning!!!\nDevice's file has error.",
		SET_RANGE_ERROR,		"Nothing to do. Set rigth address range",
		COMMAND_ERROR,			"Invalid command\n",
		PRG_RX_C_ERROR,		"Programmer receive command failed\n",
		PRG_LINE_ERROR,		"Programmer - IBM line failed\n",
		MALLOC_ERROR,			"Memory allocate Error",
		RANGE_PULSE_ERROR,		"Initial Pulse Width Error.\nValue must bettwen 1-50000\n",
		RANGE_PULSE_COUNT_ERROR,	"Pulse Count Error.\nValue must bettwen 1-100\n",
		RANGE_PULSE_OVER_ERROR,	"Overprogram Pulse Width Error.\nValue must bettwen 1-50000\n",
	};
