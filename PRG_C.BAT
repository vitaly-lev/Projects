del prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c async.asm       >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c timer.asm       >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c mnpdrvr.c       >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_main.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_port.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_menu.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_fun.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_ibm.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_file.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_dev.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_hex.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_set.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_test.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_clr.c	 >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_colr.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_varr.c      >>prg_c.err
cl /AS /Fc /Zp1 /W4 /Owzsrca /G0 /Gs /c prg_varw.c      >>prg_c.err
rem
rem	link
rem
call pl	 >>prg_c.err
call pd	 >>prg_c.err
call ps	 >>prg_c.err
call ph	 >>prg_c.err
rem

