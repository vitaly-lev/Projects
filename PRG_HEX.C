/*
	<C> <Lion>

	prg_hex.c

	Programm for Programmer on 8048

	HEX Editor

	Wrote by Dovbush Yurij
	Changes Vitaly Lev
	
	920525	v1.1 Beta0
	920225	v1.0 Beta6

	Functions:

*/

#include <scl1.h>
#include <scl1keys.h>
#include <scl1clor.h>
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <string.h>

#include "prg_def.h"

/*
			Functions prototype
*/
void	fedithex(void);
void 	hexasc(uint d, char *ss);
void 	edhex(long);
int 	gethexb(int row, int col, byte *dd);
int 	gethexw(int row, int col, uint *dd);
void 	WriteScreenAtrib(int r, int c, int n, char *buf);

#define posadr 2
#define poshex 9
#define posasc 60
#define strow  3
#define rowpared 1

/*
			Local variable
*/
int	c_hex_border;
int	c_hex_hex;
int	c_hex_asc;
int	wx=1;
char	conhex[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char	buf[160];
long	stadrbuf;			/* start adress buffer data for programing	*/
long	endadrbuf;		/* end adress buffer data for programing	*/
long	stadrdev;			/* start adress device for programing		*/
long	endadrdev;		/* end adress device for programing		*/
uint	csum;			/* Buffer Control Sum					*/
uint	select_b;			/* Select's byte flag 					*/
par	prgpar;
byte	far	*bptr;
char	*buffer_common;
int	col_len=16;
/*
	--- menu system ---
*/
MSBar msb[]=
		{
		  1,  6, ALTF, " File ",
		};

MSOptions mso0[]=
		{
		  2,  4, " Read  ", 'R',
		  3,  4, " Write ", 'W',
		};


char WindowBuf[140];

MSWindow msw[]=
		{
		  1,  3,  4, 11,  2, WindowBuf, mso0,
		};

MSData msd=
		{
			/* bar-menu colors */
		  BLACK_WHITE, WHITE_BLACK+HIGHLIGHT,

			/* pull-down menu colors */
		  WHITE_BLACK, BLACK_WHITE, WHITE_BLACK+HIGHLIGHT,

		  msb, msw, 1, 0, 0, 0
		};

/*
			External functions
*/
extern	void	read_var(char *, char *);
extern	void	read_f(void);
extern	void	write_f(void);
extern	int	wprintf(int, int, int, char *, ...);
extern	int	hi(int);
extern	int	in(int);

/*
			External variable
*/
extern	int	cf;
extern	int	fill_c;
extern	char	flag_common;
extern	char namefile[];
extern	struct device far *prg_dev;


/*----------------------------------------------------------------------*/

void main(int argc, char **argv)
{
long ptr=0l;
uint k;
char key;
uint ptrt;
uint i;
char btmp[]="    ";
uint copyfrom=0;
uint copyend=0;
uint copyto=0;
byte filldata=0;
int currow=strow;
int curcol=posadr+1;
enum {adr, hex, asc} typewin=adr;
long il;
int x;

	x = argc;

	CursorOn();

	read_var(argv[1], argv[2]);

	c_hex_border = prgpar.c_hex_border;
	c_hex_hex	   = prgpar.c_hex_hex;
	c_hex_asc	   = prgpar.c_hex_asc;


	WriteScreen(c_hex_border,  1, 0, "ีอออออออออออออออออออออออออออออออออออออออออออออออออออออออออัออออออออออออออออออออธ");
	WriteScreen(c_hex_border,  2, 0, "ณ         0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F ณ 0123456789ABCDEF   ณ");
	WriteScreen(c_hex_border, 19, 0, "ณ         0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F ณ 0123456789ABCDEF   ณ");
	WriteScreen(c_hex_border, 20, 0, "รฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤมฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤด");
	WriteScreen(c_hex_border, 21, 0, "ณ                                                          Buffer size : 00000 ณ");
	WriteScreen(c_hex_border, 22, 0, "ณ Selected :                                                  Checksum :  0000 ณ");
	WriteScreen(c_hex_border, 23, 0, "ิออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ");

	sprintf(buf, "%05X", endadrbuf+1);
	WriteScreen(c_hex_border, 21, 73, buf);

	/* CSUM Calculate */
	csum = 0;
	

	switch (select_b)
	{
	case 0 : 	for (il=stadrbuf; il<=endadrbuf; il++)
				csum += *(bptr+il);
			break;

	case 1 :	for (il=stadrbuf; il<=endadrbuf; il+=2)
				csum += *(bptr+il);
			break;

	case 2 :	for (il=stadrbuf+1; il<=endadrbuf; il+=2)
				csum += *(bptr+il);
			break;
	}

	wprintf(c_hex_border, 22, 74, "%04X", csum);

	switch (select_b)
	{
		case 0 : WriteScreen(c_hex_border, 22, 13, "ALL");  break;
		case 1 : WriteScreen(c_hex_border, 22, 13, "ODD");  break;
		case 2 : WriteScreen(c_hex_border, 22, 13, "EVEN"); break;
	}

	for (i=2; i<posasc*2-1; i++)
 	{
		buf[i++] = ' ';
		buf[i] = (byte)c_hex_hex;
	}

	for (i=posasc*2; i<posasc*2+16*2; i++)
 	{
		buf[i++] = ' ';
		buf[i] = (byte)c_hex_asc;
	}

	buf[0]				= 'ณ';	buf[0+1]				= (byte)c_hex_border;
								buf[1*2+1]			= (byte)c_hex_border;
								buf[2*2+1]			= (byte)c_hex_border;
								buf[3*2+1]			= (byte)c_hex_border;
								buf[4*2+1]			= (byte)c_hex_border;
								buf[5*2+1]			= (byte)c_hex_border;
								buf[6*2+1]			= (byte)c_hex_border;
	buf[(poshex-2)*2]    	= ':';	buf[(poshex-2)*2+1]    	= (byte)c_hex_border;
								buf[(poshex-2)*2+3]    	= (byte)c_hex_border;
								buf[(posasc-3)*2+1]    	= (byte)c_hex_border;
	buf[(posasc-2)*2]    	= 'ณ';	buf[(posasc-2)*2+1]    	= (byte)c_hex_border;
								buf[(posasc-1)*2+1]    	= (byte)c_hex_border;
								buf[(posasc+16)*2+1] 	= (byte)c_hex_border;
								buf[(posasc+16+1)*2+1] 	= (byte)c_hex_border;
	buf[(posasc+16+2)*2] 	= '0';	buf[(posasc+16+2)*2+1] 	= (byte)c_hex_border;
	buf[(posasc+16+3)*2] 	= 'ณ';	buf[(posasc+16+3)*2+1] 	= (byte)c_hex_border;

	/* --- On Menu System --- */


	msd.BarNColor = prgpar.c_all;
	msd.BarRColor = hi(prgpar.c_all);
	msd.MenuNColor= prgpar.c_all;
	msd.MenuRColor= in(prgpar.c_all);
	msd.MenuHColor= hi(prgpar.c_all);

	MenuSystem(MS_SHADOW_OFF,	(MSData *)0);	/* Draw shadow 	*/
	MenuSystem(MS_ALT_ON,		(MSData *)0);	/* ALT sensitive 	*/
	MenuSystem(MS_DRAW,				&msd);	/* Draw			*/

	SetCurPos(currow,curcol);
	edhex(ptr);
	
	while (wx)
	{
		SetCurPos(currow,curcol);

		if (KeyReady())
		{
		
			k = MenuSystem(MS_KEY, &msd, k);

			if (KeyReady())
				k = GetKey();
			key = (char)k;

			switch (k)
			{
				case UP :
				{
					if (currow==strow ) 
					{
						if (ptr>=col_len)
						{
							ptr -= col_len;
							edhex(ptr);
						}
					}
 					else
						currow--;
					break;
				}

				case DOWN :
				{
					if (currow == strow+15 || typewin == adr)
					{
						if (ptr <= (endadrbuf-(col_len*16)))
						{
							ptr += col_len;
							edhex(ptr);
						}
					}
                 			else
						++currow;
					break;
				}
	
				case LEFT :
				{
					switch (typewin)
					{
						case adr : if (curcol!=posadr+1) curcol--; break;
			
						case hex :	if ((curcol-poshex)==0)
										if (currow==strow)
				 						{ 
											typewin = adr; 
									  		curcol  = posadr+3+1;
										}
										else
				 						{ 
											currow--;
											switch (select_b)
											{
											case 0 :	curcol = poshex+16*3-2; break;
											case 1 :	curcol = poshex+16*3-5; break;
											case 2 :	curcol = poshex+16*3-2; break;
											}
										}
				      				else
						        		{
                                				if (curcol == poshex+8*3+1) 
										{
										 	switch (select_b)
											{
											case 0 : curcol -= 3; break;
											case 1 : curcol -= 6; break;
											case 2 : curcol -= 3; break;
											}
										}
										else
                                					if (curcol < poshex+8*3+1)
											{
												if ((curcol-poshex)%3 == 0) 
												{
												 	switch (select_b)
													{
													case 0 : 	curcol -= 2; break;

													case 1 :	if (((curcol-poshex) % 2) == 0)
																curcol -= 5;
															else
																curcol -= 2;
															break;

													case 2 :	if (((curcol-poshex) % 2) != 0)
															{
																curcol -= 5;	
																if ((curcol-poshex)<=0)
																{
																	if (currow==strow)
				 													{ 
																		typewin = adr; 
									  									curcol  = posadr+3+1;
																	}
																	else
				 													{ 
																		currow--;
																		switch (select_b)
																		{
																		case 0 :	curcol = poshex+16*3-2; break;
																		case 1 :	curcol = poshex+16*3-5; break;
																		case 2 :	curcol = poshex+16*3-2; break;
																		}
																	}
																}
															}
															else
																curcol -= 2;
															break;
													}
												}
												else 
													curcol--;
											}
											else /* curcol > poshex+8*3+1) */
											{
												if ((curcol-(poshex+8*3+1))%3 == 0) 
												{
												 	switch (select_b)
													{
													case 0 : 	curcol -= 2; break;

													case 1 :	if (((curcol-poshex+1) % 2) == 0)
																curcol -= 5;
															else
																curcol -= 2;
															break;

													case 2 :	if (((curcol-poshex+1) % 2) != 0)
																curcol -= 5;
															else
																curcol -= 2;
															break;
													}
												}
												else 
													curcol--;
											}
									}
									break;

						case asc :	if ((curcol-posasc)==0)
									{
										if (currow != strow)
				 						{
											currow--;
											switch (select_b)
											{
											case 0 : curcol=posasc+15; break;
											case 1 : curcol=posasc+15-1; break;
											case 2 : curcol=posasc+15; break;
											}
										}
										break;
									}
			      					else
									{
				 					 	switch (select_b)
										{
										case 0 :  curcol--; break;
										case 1 :	if (((curcol-posasc) % 2) == 0)
													curcol -= 2;
												else
													curcol--;
												break;

										case 2 :	if (((curcol-posasc) % 2) != 0)
												{
													curcol -= 2;
													if ((curcol-posasc) <= 0)
													{
														if (currow != strow)
				 										{
															currow--;
															curcol=posasc+15;
														}
														else
															curcol +=2;
													}
													break;
												}
												else
													curcol--;
												break;

										} /* switch (select_b) */
										
			      					} /* case asc */
									break;

					} /* switch (typewin) */
				     break;

				} /* case LEFT */
			
				case PgDn :
				{
					if (((long)ptr + 256) < ((long)endadrbuf - 255))
						ptr += 256;
					else
						ptr = endadrbuf-255 + (ptr % 16);
				
					edhex(ptr);
					break;
				}
			
				case PgUp :
				{
					if (ptr > 255)
						ptr-=256;
					else
						ptr=ptr % 16;

					edhex(ptr);
					break;
				}
  			
				case HOME :
				{
					switch(typewin)
					{
						case adr : curcol = posadr+1; break;
						case hex : switch (select_b)
								 {
								 	case 0 : curcol = poshex; break;
									case 1 : curcol = poshex; break;
									case 2 : curcol = poshex+3; break;
								 }
								 break;
							
						case asc : switch (select_b)
								 {
								 	case 0 : curcol = posasc; break;
									case 1 : curcol = posasc; break;
									case 2 : curcol = posasc+3; break;
								 }
								 break;
					}
					break;
				}

				case END :
				{
		               switch (typewin)
					{
						case adr : curcol=posadr+3+1; 	break;
						case hex : switch (select_b)
								 {
								 	case 0 : curcol=poshex+16*3-2; break;
									case 1 : curcol=poshex+16*3-2-3; break;
									case 2 : curcol=poshex+16*3-2; break;
								 }
								 break;

						case asc : switch (select_b) 
								 {
									case 0 : curcol=posasc+15; break;
									case 1 : curcol=posasc+15-1; break;
									case 2 : curcol=posasc+15; break;
								 }
								 break;
					}
					break;
				}
			
				case TAB :
				{
					switch(typewin)
					{
						case adr : typewin=hex;
								
								 switch (select_b)
								 {
								 case 0 : curcol=poshex; break;
								 case 1 : curcol=poshex; break;
								 case 2 : curcol=poshex+3; break;
								 }
								 break;
								 
						case hex : typewin=asc; 
								 switch (select_b)
								 {
								 case 0 : curcol=posasc; break;
								 case 1 : curcol=posasc; break;
								 case 2 : curcol=posasc+1; break;
								 }
								 break;
								 
						case asc : typewin=adr; curcol=posadr+1; currow=strow; break;
		                  	}
               			break;
				}
				
				case CTRLRIGHT:
					if (col_len < 16)
						col_len++;
					edhex(ptr);
					break;

				case CTRLLEFT:
					if (col_len > 1)
						col_len--;
					edhex(ptr);
					break;

	/*			case F5 :
				{
					if ((wparedit=wxxopen(8,15,11,56,NULL,CURSOR+BORDER+BD2+CENTER,60,4,-1,32))==-1)
						terror("Error opening First Window");
				
					watsay(wparedit,rowpared,4,"From: ");
					watsay(wparedit,0,8,"Move date in buffer ");
					hexasc(copyfrom,btmp);
					watsay(wparedit,rowpared,10,btmp);
					watsay(wparedit,rowpared,16,"End: ");
					hexasc(copyend,btmp);
					watsay(wparedit,rowpared,21,btmp);
					watsay(wparedit,rowpared,27,"To: ");
					hexasc(copyto,btmp);
					watsay(wparedit,rowpared,31,btmp);
					wshow(wparedit);
					if (gethexw(rowpared,10,&copyfrom)!=ESC)
						if (gethexw(rowpared,21,&copyend)!=ESC)
							if (gethexw(rowpared,31,&copyto)!=ESC) 
							{
								i=copyfrom;
								j=copyto;
								wat(wparedit,rowpared,4);
								while (i<=copyend)
								{
									*(bptr+j++) = *(bptr+i++);
									if (j==0) break;
								}
							}
					wclose(wparedit);
					edhex(ptr);
					break;

				}
			
				case F6 :
				{
					if ((wparedit=wxxopen(8,15,11,62,NULL,CURSOR+BORDER+BD2+CENTER,60,4,-1,32))==-1)
						terror("Error opening First Window");
					watsay(wparedit,0,8,"Fill date in buffer ");
					watsay(wparedit,rowpared,4,"From: ");
					hexasc(copyfrom,btmp);
					watsay(wparedit,rowpared,10,btmp);
					watsay(wparedit,rowpared,16,"End: ");
					hexasc(copyend,btmp);
					watsay(wparedit,rowpared,21,btmp);
					watsay(wparedit,rowpared,27,"Fill date: ");
					btmp[0]=conhex[(filldata>>4) & 0x0f];
					btmp[1]=conhex[filldata & 0x0f];
					btmp[2]=0;
					watsay(wparedit,rowpared,38,btmp);
					wshow(wparedit);
				
					if (gethexw(rowpared,10,&copyfrom)!=ESC)
						if (gethexw(rowpared,21,&copyend)!=ESC)
							if (gethexb(rowpared,38,&filldata)!=ESC) 
							{
								i=copyfrom;
         							if (copyend>endadrbuf)
									copyend=endadrbuf;
								wat(wparedit,rowpared,4);
								while(i<copyend)
									*(bptr+i++) = filldata;
								if (i==copyend)
									*(bptr+i)=filldata;
							}
							wclose(wparedit);
							edhex(ptr);
							break;
				}
	*/
				default :
				{
					if (k!=RIGHT)
						switch (typewin)
						{
							case adr :
							{
								if (key==' ')
									k=RIGHT;
             						
								if ((key>='0' && key<='9') ||
							    	(key>='A' && key<='F') ||
							    	(key>='a' && key<='f'))
								{
									if (key>='a')
										k -= 0x20;
									if (key>'9')
										(k -= ('A'-0xA));
									else k -= '0';
							
									switch (curcol-posadr-1)
									{
										case(0): ptr=(ptr & 0x00000fff) | ((k<<12) & 0xf000);	break;
										case(1): ptr=(ptr & 0x0000f0ff) | ((k<<8)  & 0x0f00);	break;
										case(2): ptr=(ptr & 0x0000ff0f) | ((k<<4)  & 0x00f0);	break;
									   	case(3): ptr=(ptr & 0x0000fff0);					break;
									}
								
									if (ptr>(endadrbuf-255))
										ptr = endadrbuf-255+(ptr % 16);

									edhex(ptr);
									k=RIGHT; 
								} 
								break;
							}
						
							case hex :
							{
								if (key == ' ')
									k = RIGHT;
							
								if ((key>='0' && key<='9') ||
							    	(key>='A' && key<='F') ||
							    	(key>='a' && key<='f'))
								{
									if (key>='a')  
										k -= 0x20;
									/*edhex(bptr+ptr);*/
									/*WriteChar(WHITE_BLACK, currow, curcol, 1, k);*/
								
									if (key>'9')
										k -= 'A'-0x0A;
									else k -= '0';

									if (curcol<poshex+8*3)
									{
										i = (curcol-poshex)%3;
										ptrt = (curcol-poshex)/3;
									}
									else
									{
										i = (curcol-(poshex+8*3+1))%3;
										ptrt = (curcol-(poshex+8*3+1))/3+8;
									}
								
									SetCurPos(currow,posasc+ptrt);
									ptrt+=ptr+(currow-strow)*16;
								
									if (i==0)
									{
								 		csum -= *(bptr+ptrt);
										*(bptr+ptrt) = ((*(bptr+ptrt)) & (byte)0x0f) | (byte)(k<<4);
									}
									else
									{
										csum -= *(bptr+ptrt); 
										*(bptr+ptrt) = ((*(bptr+ptrt)) & (byte)0xf0) | (byte)k;
									}
									csum += *(bptr+ptrt);
									edhex(ptr);
							        
									k=RIGHT;
								} 
								break;
							}
						
							case asc :
							{
								if (k != ESC)
								{
									csum -= *(bptr+ptr+(currow-strow)*16+(curcol-posasc));
									*(bptr+ptr+(currow-strow)*16+(curcol-posasc)) = (char)k;
									csum += (char)k;
									edhex(ptr);
									k=RIGHT;
									break;
								}
							}

						} /* case typewin */
				
					if (k==RIGHT)
						switch (typewin)
						{
							case adr:
							{
								if (curcol != posadr+3+1)
									curcol++;
								else
								{
									curcol = poshex;
									typewin = hex;
								}
								break;
							}

							case hex :
							{
								if (curcol>=poshex+16*3-1)
								{
									if (currow!=strow+15)
									{
										currow++;
										switch (select_b)
										{
										case 0 :	curcol = poshex; break;
										case 1 :	curcol = poshex; break;
										case 2 :	curcol = poshex+3; break;
										}

									}
								}
								else
								{
									if (curcol == poshex+7*3+1)
									{
									 	switch (select_b)
										{
										case 0 : 	curcol+=3; break;
										case 1 : 	curcol+=3; break;
										case 2 :  curcol+=6; break;
										}
									}
									else
										if (curcol<poshex+7*3+1) 
											if ((curcol-poshex)%3 == 0)
												curcol++;
											else
											{
												switch (select_b)
												{
												case 0 :	curcol += 2; break;
												case 1 :	curcol += 5; break;
												case 2 :	curcol += 5; break;
												}
												if (curcol > poshex+7*3+1)
													curcol++;
											}
									else
								    		if ((curcol-(poshex+8*3+1))%3 == 0)
											curcol++;
									else
									{
										switch (select_b)
										{
										case 0 :	curcol += 2; break;
										case 1 :	curcol += 5; break;
										case 2 :	curcol += 5; break;
										}

										if (curcol>=poshex+16*3-1)
										{
											if (currow!=strow+15)
											{
												currow++;
												switch (select_b)
												{
												case 0 :	curcol = poshex; break;
												case 1 :	curcol = poshex; break;
												case 2 :	curcol = poshex+3; break;
												}
										
											}
											else
											{
												switch (select_b)
												{
												case 0 :	curcol -= 2; break;
												case 1 :	curcol -= 5; break;
												case 2 :	curcol -= 5; break;
												}

											}
										}

									}
								}
								break;
							}
						
							case asc :
							{
								if (curcol>=posasc+15)
			    					{
									if (currow!=strow+15)
									{
										currow++;
										switch (select_b)
										{
										case 0 : curcol=posasc; break;
										case 1 : curcol=posasc; break;
										case 2 : curcol=posasc+1; break;
										}
										break;
									}
								}
								else
								{
									switch (select_b)
									{
									case 0 : curcol++; break;
									case 1 : curcol += 2; break;
									case 2 : curcol += 2; break;
									}
	
									if (curcol > posasc+15)
			    						{
										if (currow!=strow+15)
										{
											currow++;
											curcol=posasc;
										}
										else
										{
											switch (select_b)
											{
											case 0 : curcol--; break;
											case 1 : curcol -= 2; break;
											case 2 : curcol -= 2; break;
											}
										}
									}
								}
								break;
							}
						} /* case typewin */
					
					break; /* case default */
				}
				
				case ALTR :
					read_f();
					edhex(ptr);
					break;

				case ALTW :
					write_f();
					break;

			} /* case*/

			if (k==ESC)
				break;

		}
		else
		{
			k = MenuSystem(MS_CHECK, &msd);
			if (k == MS_SELECT)
			{
				switch(msd.Menu)
				{
				case 1:
					switch(msd.Option)
					{	/* Files Menu */

					case 1 : read_f();
						    edhex(ptr);
						    break;

					case 2 : write_f();	break;
					}
					break;

				case 2:
					switch(msd.Option)
					{
					case 1:
						MessageOn(BLACK_WHITE, "Mark");
						WaitTime(100);
						MessageOff();
						break;
					
					case 2:
						MessageOn(BLACK_WHITE, "Cut");
						WaitTime(100);
						MessageOff();
						break;

					case 3:
						MessageOn(BLACK_WHITE, "Copy");
						WaitTime(100);
						MessageOff();
						break;

					case 4:
						MessageOn(BLACK_WHITE, "Paste");
						WaitTime(100);
						MessageOff();
						break;

					}
					break;
				} /* switch msd.Option */
			}
		}
	}

	CursorOff();
}

/*----------------------------------------------------------------------*/
void hexasc(unsigned d,char *ss)
{
	*ss++=conhex[d>>12];
	*ss++=conhex[(d>>8) & 0x000f];
	*ss++=conhex[(d>>4) & 0x000f];
	*ss=conhex[d & 0x000f];
}
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
int gethexw(int row,int col,uint *dd)
{
char buf[]="    ";
int k;
int colptr;
colptr=0;

	hexasc(*dd,buf);
	WriteScreen(0x38, row, col, buf);
	while(wx)
	{
		SetCurPos(row,col+colptr);
		k = GetKey();
		if (k==ESC || k==ENTER)
			break;
		if (k==' ')
		{
			if (colptr < 3)
				colptr++;
		}
		
		if ((k>='0' && k<='9') || (k>='A' && k<='F') || (k>='a' && k<='f'))
		{
			if (k>='a')
				k -= 0x20;
			WriteChar(0x38, row, col+colptr, 1, k);
			if (k>'9')
				k-=('A'-0x0A);
			else
				k-='0';
			
			switch (colptr)
			{
    				case(0): *dd=(*dd & 0x0fff) | (k<<12); break;
    				case(1): *dd=(*dd & 0xf0ff) | (k<<8); break;
    				case(2): *dd=(*dd & 0xff0f) | (k<<4); break;
    				case(3): *dd=(*dd & 0xfff0) | k; break;
			}
			
			if (colptr < 3)
				colptr++;
		}
		else
			if (k==LEFT)
			{
				if (colptr != 0)
					colptr--;
			}
			else
				if (k==RIGHT)
				{
					if (colptr < 3)
						colptr++;
				}
	}

	hexasc(*dd,buf);
	WriteScreen(0x07, row, col, buf);
	return(k);
}

int gethexb(int row, int col, byte *dd)
{
char buf[]="  ";
uint k;
int colptr;
colptr=0;
buf[0]=conhex[(*dd>>4) & 0x0f];
buf[1]=conhex[*dd & 0x0f];

	WriteScreen(0x38, row, col, buf);
	while(wx)
	{
		SetCurPos(row,col+colptr);
		k=GetKey();
		if (k==ESC || k==ENTER)
			break;
		if (k==' ')
		{
			if (colptr==0)
				colptr++;
		}
		if ((k>='0' && k<='9') || (k>='A' && k<='F') || (k>='a' && k<='f'))
   		{
			if (k>='a')
				k -= 0x20;
			WriteChar(0x38, row, col+colptr, 1, k);
		
			if (k>'9')
				k-=('A'-0xA);
			else k -= '0';
		
			switch (colptr)
			{
				case 0 : *dd = ((*dd) & (byte)0x0f) | (byte)(k << 0x04); 	break;
				case 1 : *dd = ((*dd) & (byte)0xf0) | (byte)k;	break;
			}
		
			if (colptr==0)
				colptr++;
		}
		else
			if (k==LEFT)
			{
				if (colptr != 0)
					colptr--;
			}
			else
		
			if (k==RIGHT)
			{
				if (colptr == 0)
					colptr++;
			}
	}

	buf[0] = conhex[(*dd>>4) & 0x000f];
	buf[1] = conhex[*dd & 0x000f];
	WriteScreen(0x07, row, col, buf);
	return(k);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void edhex(long ptr)
{
int i,j,k, j1;
char ss;
byte far *eptr;

	eptr = bptr+ptr;

	for (i=0; i<16; i++) /* 16 stings on screen */
	{

/* Address on screen */

		buf[(posadr+0)*2]    = conhex[(ptr>>16)& 0x000f];
		buf[(posadr+1)*2]    = conhex[(ptr>>12)& 0x000f];
		buf[(posadr+2)*2]    = conhex[(ptr>>8) & 0x000f];
		buf[(posadr+3)*2]    = conhex[(ptr>>4) & 0x000f];
		buf[(posadr+4)*2]    = conhex[ptr      & 0x000f];



		buf[(posasc+16+1)*2] = buf[(posadr+3)*2];

		for (j=0, k=poshex*2; j<col_len; j++) 
		{
			ss=*(eptr++); ptr++;				/* ss - byte from buffer */

			buf[k]=conhex[(ss >> 4) & 0x0f]; k+=2;	/* translate byte in ascii */
			buf[k]=conhex[ss & 0x0f]; k+=2;

			if (k!=poshex*2-2+8*6) 				/* after 8 possition - write space on screen */
				k+=2; 
			else 
				k+=4;
											/* convert hex to ascii */
			if (ss==0) 
				ss=0xff;

			buf[(posasc+j)*2]=ss;
											/* =================== */
		}
		
		for (j1=j; j1<16; j1++)
			{
			buf[k] = ' '; k+=2;
			buf[k] = ' '; k+=2;
			buf[(posasc+j1)*2]=' ';
			}

/* write prepare string on screen */

		WriteScreenAtrib(i+strow, 0, 80, buf);
	}

	wprintf(c_hex_border, 22, 74, "%04X", csum);
}

void WriteScreenAtrib(int r, int c, int n, char *buf)
{
int i;
	for (i=0; i<n; i++)
	{
		WriteChar(*(buf+i*2+1), r, c++, 1, *(buf+i*2));
	}
}


