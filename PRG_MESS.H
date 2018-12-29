#define OK_M 		"OK"
#define NOT_ACK_M 	"NAK"
#define QUESTIONS_M 	"???"
#define NOT_ANSWER_M	"NOT ANSWER"
#define SUM_ERR_M	"CSUMM ERR"
#define TIME_OUT_M	"TIME OUT"
#define NA		"N/A"


#define VERIFY_COMPLETE		"Verify chip complete"
#define WRITE_POSS_COMPLETE	"Write Possibility complete"
#define PRESS_ANY_KEY		"Press ESC key"
#define PRG_COMPLETE		"Programming complete"
#define READ_COMPLETE		"Read Chip Complate"
#define READ_CANCEL		"Read Chip Cancel"
#define VERIFY_CANCEL		"Verify Chip Cancel"
#define BLANK_CHECK_CANCEL	"Blank Check Chip Cancel"
#define CHIP_HAVNT_IDENT		"Device hav't\nIntelligent Identefier"
#define BLANK_CHECK_COMPLETE	"Blank Check Chip complete"
#define ALL_BYTE_CLEAR		"All bytes Clear"
#define ALL_BYTE_COMPARE		"All bytes Compared OK"
#define ALL_BYTE_PRG		"All bytes written OK"
#define BYTES_NOT_COMPARE 	"bytes NOT Compare"
#define BYTES_NOT_CLEAR		"bytes NOT Clear"
#define WRITE_POSS_COMPLETE	"Write Possibility complete"
#define BUFFER_MAY_WRITE		"Buffer may will write in chip"
#define ERRORS			"Errors"
#define PRESS_ESC_FOR_INT	"Press ESC for process interrupt                                                 "
#define PRESS_ESC_KEY		"Press ESC key"
#define PRGW_IBM		"IBM"
#define PRGW_PRG		"PROGRAMMER"
#define PRGW_ERRORS		"ERRORS"
#define ORIGINAL		"Original socket"
#define PROGRAMMING		"Programming socket"
#define PRG_CANCEL		"Programming cancel"
#define WRITE_POSS_CANCEL 	"Write Possibility cancel"
#define DURING_PRG		"During Programming"
#define DURING_PROCESS		"During Process"
#define LREADING	   	" ===== READING ===== " /* don't change length!!! */
#define LPROGRAMMING		" === PROGRAMMING === " /* don't change length!!! */
#define LVERIFY			" ===== VERIFY ===== "  /* don't change length!!! */
#define LBLANKCHECK		" ===BLANK  CHECK=== "  /* don't change length!!! */
#define LWRITEPOSSIBILITY 	" =WRITE POSSIBILITY= " /* don't change length!!! */
#define CANCEL_0_ERRORS		"\n%s\n\n0 %s %s"
#define CANCEL_1_ERRORS		"\n%s\n\n%i %s %s"
#define COMPLETE_0_ERRORS	"%s\n\n%s"
#define COMPLETE_1_ERRORS	"%s\n\n%i %s %s"
#define STATISTICS		"STATISTICS"
#define FREE			"\0"
#define IPULSE_SYM		'.'
#define INFORMATION		"      Information       " /* don't change length!!! */
#define NOT_AV			"     NOT Avaliable      " /* don't change length!!! */
#define IN_NORMAL_ALG		"In NORMAL  PRG Algorithm" /* don't change length!!! */
#define TIME_OUT_BY_RX_BYTE	"T/Out by RX byte"	/* don't increese length!!! */
#define RX_UNKNOWN_BYTE		"RX Unkn. b"		/* don't increese length!!! */
#define RESET_PROGRAMMER		"Reset Programmer"	/* don't increese length!!! */
#define READ_PRG_CHIP		"Read PRG Chip"		/* don't increese length!!! */
#define READ_ORIG_CHIP		"Read ORIGINAL Chip"	/* don't increese length!!! */
#define SET_PRG_CHIP		"Set PRG Chip"		/* don't increese length!!! */
#define SET_PRG_POWER		"Set PRG Power"		/* don't increese length!!! */
#define START_PRG		"Start Programming"	/* don't increese length!!! */
#define TEST_VPP		"Test Vpp"		/* don't increese length!!! */
#define VERIFY_CHIP		"Verify Chip"		/* don't increese length!!! */
#define BLANK_CHECK_CHIP		"Blank Check Chip"	/* don't increese length!!! */
#define READ_INTEL_IDENT		"Read Inteligent ident."	/* don't increese length!!! */
#define SET_PRG_BAUD		"Set Baud Rate"		/* don't increese length!!! */
#define WRITE_POSSIBILITY	"Test Write Possib."	/* don't increese length!!! */
#define PRESS_ESC_KEY_FOR_CANCEL	"Press ESC for cancel"
#define YES_MES			"= YES ="
#define NO_MES			"= NO ="
#define NAK_AT_RX_BLOCK		"\nNAK at RX BLOCKS. Too many errors.\n"
/*#define HEX_ERROR_FORMAT		"Error in HEX format\n"*/
/*#define HEX_ERROR_SUM		"Checksum error in HEX file\n"*/
#define PAR_FORMAT_ERROR		"Format invalid\n"
#define PAR_NUMBER_ERROR		"Number parameters error\n"
#define PAR_ADDRESS_ERROR	"Address in parameter is to big\nValid address 0000 - 7FFF\n"
#define PAR_BYTE_ERROR		"Byte in parameter error\nValid range 00 - FF\n"
#define PAR_DIGIT_ERROR		"Invalid symbol in parameter string\n"
#define PAR_RANGE_ERROR		"Start address Biggest End address\n"
#define PAR_FILE_ERROR		"Parameter file ERROR.\n\nSet default parameter\n"
#define FILE_EXIST		"File exist.\n Rewrite?"
#define FILE_NOT_EXIST		"Device's File not found.\nCreate new file?\n"
#define FILE_OPEN_ERROR		"\nCan't open file\n"
#define FILE_ERROR		"Error processing file\n"
#define FILE_LENGTH_ZERRO	"Sorry Bambino, but Length of file = 0"
#define FILE_LENGTH_BIG		"File length is to big"
#define FILE_SEEK_ERROR		"File seek error"
#define FILE_CLOSE_ERROR		"File close error"
#define FILE_WRITE_ERROR		"Write file error"
#define FILE_NUM_B_BIG_ERROR	"Number bytes for read bigest buffer length"
#define FILE_WRITE_PAR_ERROR	"Write SETUP FILE error"
#define FILE_OPEN_PAR_ERROR	"Read SETUP FILE error"
#define FILE_READ_ERROR		"Read file error"
#define FILE_DEV_ERROR		"File with parameters\nprogramming PROM\nread error"
#define FILE_DEVICE_ERROR     	"Warning!!!\nDevice's file has error."
#define SET_RANGE_ERROR		"Nothing to do. Set rigth address range"
#define COMMAND_ERROR	     	"Invalid command\n"
#define PRG_RX_C_ERROR		"Programmer receive command failed\n"
#define PRG_LINE_ERROR		"Programmer - IBM line failed\n"
#define MALLOC_ERROR	     	"Memory allocate Error"
#define RANGE_PULSE_ERROR     	"Initial Pulse Width Error.\nValue must bettwen 1-50000\n"
#define RANGE_PULSE_COUNT_ERROR	"Pulse Count Error.\nValue must bettwen 1-100\n"
#define RANGE_PULSE_OVER_ERROR	"Overprogram Pulse Width Error.\nValue must bettwen 1-50000\n"
#define D_FIRMA			"                         Company : "
#define D_TYPE			"                   Device's type : "
#define D_PROTOTYPE		"                Device PROTOTYPE : "
#define D_VPP			"             Vpp voltage is (mV) : "
#define D_VCC			"     Vcc during programming (mV) : "
#define D_VENDOR		"              Vendor ID is (HEX) : "
#define D_DEVICE		"              Device ID is (HEX) : "
#define D_ALGORITHM		"   Programming algorithm (N/I/Q) : "
#define D_IPULSE		"        Initial pulse width (us) : "
#define D_CPULSE		"             Maximum pulse count : "
#define D_OPULSE		"    Overprogram pulse width (us) : "
#define PRESS_ESC		"Press ESC to continue"
#define NO_DEVICE		"        No such Chip in Device data base"
#define AUTO			"AutoIdent"
#define WAIT_FOR_SET_POWER	"Please Wait for setting\n power for Verify"
#define COULD_NOT_SPAWN_PROCESS	"Couldn't spawn process"
#define NOT_ENOUGH_MEMORY_FOR_SPAWN "Not Enough Memory for Spawn Process"
#define TYPE_EXIT_TO_RETURN	"\nType EXIT to return to Programmer\n"
#define COM_PORT_NOT_FOUND	"\nCOM PORT NOT FOUND"
#define EXT_EDITOR_NOT_SET	"\nExternal Editor NOT SET"
#define EXT_EDITOR_SET_HELP	"\nPlease set name of External HEX Editor"
#define ERROR_CREATE_TMP_FILE	"\nERROR While Create TEMP File"
#define ERROR_WRITE_TMP_FILE	"\nERROR Write TEMP File"
#define COULD_NOT_LOAD_EXT_EDITOR "\nCouldn't load External HEX Editor"
#define ERROR_CLOSE_TMP_FILE	"\nERROR Close TEMP File"
#define ERROR_LOAD_TMP_FILE	"\nERROR Load TEMP File\nAfter Extern. HEX EDIT Session"


