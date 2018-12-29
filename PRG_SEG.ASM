	title		70_async.asm
	page	60,132
;
;	<C> <Lion>
;
;	prg_seg.asm
;
;	Segment manipulations Functions
;
;	921201	V1.0 Alfa1
;
;	Functions:	
;
;!!! not switch stack for number channel > 1
;
ic_imr	equ	21h			;i/o address of interrupt mask reg
;
;
; Public declarations
;
	public	_save_main_seg
	public	_load_main_seg
	public	_load_old_seg
;
	.model	small
	dosseg
;
	.code
;
_main_ds_s	dw	0
_main_es_s	dw	0
_main_si_s	dw	0
_main_di_s	dw	0
_main_bp_s	dw	0
_main_bx_s	dw	0
_main_cx_s	dw	0
_main_dx_s	dw	0

_main_ss_s	dw	0
_main_sp_s	dw	0

_old_ds_s	dw	0
_old_es_s	dw	0
_old_si_s	dw	0
_old_di_s	dw	0
_old_bp_s	dw	0
_old_bx_s	dw	0
_old_cx_s	dw	0
_old_dx_s	dw	0

_old_ss_s	dw	0
_old_sp_s	dw	0
;

_load_main_seg	proc	far
;
		mov	ax,ds
		mov	cs:_old_ds_s,ax
		mov	ax,es
		mov	cs:_old_es_s,ax
		mov	ax,si
		mov	cs:_old_si_s,ax
		mov	ax,di
		mov	cs:_old_di_s,ax
		mov	ax,bp
		mov	cs:_old_bp_s,ax
		mov	ax,bx
		mov	cs:_old_bx_s,ax
		mov	ax,cx
		mov	cs:_old_cx_s,ax
		mov	ax,dx
		mov	cs:_old_dx_s,ax

		mov	ax,ss
		mov	cs:_old_ss_s,ax
		mov	ax,sp
		add	ax,4
		mov	cs:_old_sp_s,ax


		mov	bp,sp
		mov	bx,[bp]
		mov	cx,[bp+2]

		mov	ax,cs:_main_ss_s
		mov	ss,ax
		mov	ax,cs:_main_sp_s
		sub	ax,1500
		mov	sp,ax
		push	cx
		push	bx

		mov	ax,cs:_main_ds_s
		mov	ds,ax
		mov	ax,cs:_main_es_s
		mov	es,ax
		mov	ax,cs:_main_si_s
		mov	si,ax
		mov	ax,cs:_main_di_s
		mov	di,ax
		mov	ax,cs:_main_bp_s
		mov	bp,ax

		mov	ax,cs:_main_bx_s
		mov	bx,ax
		mov	ax,cs:_main_cx_s
		mov	cx,ax
		mov	ax,cs:_main_dx_s
		mov	dx,ax

		ret
;
_load_main_seg	endp

_load_old_seg	proc	far

		mov	bp,sp
		mov	bx,[bp]
		mov	cx,[bp+2]

		mov	ax,cs:_old_ss_s
		mov	ss,ax
		mov	ax,cs:_old_sp_s
		mov	sp,ax
		push	cx
		push	bx

		mov	ax,cs:_old_ds_s
		mov	ds,ax
		mov	ax,cs:_old_es_s
		mov	es,ax
		mov	ax,cs:_old_si_s
		mov	si,ax
		mov	ax,cs:_old_di_s
		mov	di,ax
		mov	ax,cs:_old_bp_s
		mov	bp,ax

		mov	ax,cs:_old_bx_s
		mov	bx,ax
		mov	ax,cs:_old_cx_s
		mov	cx,ax
		mov	ax,cs:_old_dx_s
		mov	dx,ax

		ret
;
_load_old_seg	endp

_save_main_seg	proc	far
;
		mov	ax,ds
		mov	cs:_main_ds_s,ax
		mov	ax,es
		mov	cs:_main_es_s,ax
		mov	ax,si
		mov	cs:_main_si_s,ax
		mov	ax,di
		mov	cs:_main_di_s,ax
		mov	ax,bp
		mov	cs:_main_bp_s,ax

		mov	ax,bx
		mov	cs:_main_bx_s,ax
		mov	ax,cx
		mov	cs:_main_cx_s,ax
		mov	ax,dx
		mov	cs:_main_dx_s,ax

		mov	ax,ss
		mov	cs:_main_ss_s,ax
		mov	ax,sp
		mov	cs:_main_sp_s,ax

		ret
;
_save_main_seg	endp

		end

