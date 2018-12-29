/*
	<C> <Lion>

	prg_def.h

	Programm for Programmer on 8048

	Windows Defines

	911020	v1.0

*/
#define RETRYMAX 	256
#define SECSIZ 		128
#define TIMEOUT 	-1
#define usrint 		-5       /*If ESC-key insert - return value = -5*/
#define length_name 	80
#define YES 		1
#define NO 		0

/*
	Window for programming PROM (2 windows: 	first  - IBM
						second - PROGRAMMER
*/
/*
	- 1 -
*/
#define prgm_ur 2
#define prgm_lc 0
#define prgm_lr 7
#define prgm_rc prg_rc+1
/*
	- 2 -
*/
#define all_ur prgm_lr+2
#define all_lc 0
#define all_lr all_ur+6
#define all_rc prgm_rc

/*	
	- 1 -	
*/
#define ibm_ur prgm_ur+1
#define ibm_lc prgm_lc+1
#define ibm_lr prgm_lr-1
#define ibm_rc ibm_lc+22
/*	
	- 2 -	
*/
#define prg_ur prgm_ur+1
#define prg_lc ibm_rc+2
#define prg_lr prgm_lr-1
#define prg_rc prg_lc+11
/*
	- 3 -
*/
#define errm_ur prgm_ur
#define errm_lc prg_rc+3
#define errm_lr all_lr
#define errm_rc errm_lc+13

#define err_ur prgm_ur+1
#define err_lc errm_lc+1
#define err_lr errm_lr-1
#define err_rc errm_rc-1

/*
	Window for Device Name
*/
#define device_ur	17
#define device_lc	0
#define device_lr	device_ur+7
#define device_rc	device_lc+23
/*
	Window for Set Range Address
*/
#define address_ur	17
#define address_lc	device_rc+1
#define address_lr	address_ur+7
#define address_rc	address_lc+29
/*
	Window for PROGRAMMING messages
*/
/*
	Window for "PROGRAMMING COMPLATE" messages
*/
/*
	Window for HELP (about ALT key) messages
*/
#define alt_ur	4
#define alt_lc	17
#define alt_lr	alt_ur+17
#define alt_rc	alt_lc+44
/*
	Window for SETUP PROCEDURE 
*/
#define set_ur	10
#define set_lc	30
#define set_lr	set_ur+6
#define set_rc	set_lc+20
/*
	Window for Select Device
*/
/*
	- 1 -
*/
#define selfi_ur 5
#define selfi_lc 35
#define selfi_lr 25
#define selfi_rc 60
/*
	- 2 -
*/
#define seld_ur selfi_ur
#define seld_lc selfi_lc+17+3
#define seld_lr 25
#define seld_rc 80
/*
	Window for Save main Window before call second process
*/
#define h_ur 0
#define h_lc 0
#define h_lr 24
#define h_rc 79
/*
	Window for procedure "Clear buffer bytes:"
*/
#define cl_ur	10
#define cl_lc	20
#define cl_lr	20
#define cl_rc	50
/*
	Window for procedure "Set color"
*/
/*
	- 1 -
*/
#define c_ur	11
#define c_lc	40
#define c_lr	18
#define c_rc	68
/*
	- 2 -
*/
#define s_ur	20
#define s_lc	30
#define s_lr	23
#define s_rc	78
/*
	Window for procedure "Set Prg Parameters Device"
*/
/*
	- 1 -
*/
#define selss_ur 1
#define selss_lc 50
#define selss_lr 25
#define selss_rc 79
/*
	- 2 -
*/
#define selq_ur 18
#define selq_lc 35
#define selq_lr selq_ur+5 
#define selq_rc selq_lc+12 
/*
	- 3 -
*/
#define selmn_ur 18
#define selmn_lc 35
#define selmn_lr 25
#define selmn_rc selmn_lc+20 
/*
	- 4 -
*/
#define selst_ur 0
#define selst_lc 36
#define selst_lr 0
#define selst_rc selst_lc+40 
/*
	Window for File manipulation procedure
*/
/*
	- 1 -
*/
#define f_ur 3
#define f_lc 20
#define f_lr 17
#define f_rc 59
/*
	- 2 -
*/
#define fw_ur 7
#define fw_lc 15
#define fw_lr 18
#define fw_rc 62
/*
	Window for "TESTs" procedure
*/
/*
	- 1 -
*/
#define vpp_ur	3
#define vpp_lc	3
#define vpp_lr	20
#define vpp_rc	70
/*
	- 2 -
*/
#define pins_ur	0
#define pins_lc	0
#define pins_lr	25
#define pins_rc	79
/*
	Window for PRG STATISTICS
*/

#define prgs_ur	2
#define prgs_lc	54
#define prgs_lr	23
#define prgs_rc	79
/*
	ALL Center Message window
*/
#define ac_ur 10
#define ac_lc 20
#define ac_lr ac_ur+7
#define ac_rc 80-ac_lc

