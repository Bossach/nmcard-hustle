

global ___cp0_prim_rc_ihandler: label;
global ___cp1_prim_rc_ihandler: label;
global ___cp2_prim_rc_ihandler: label;
global ___cp3_prim_rc_ihandler: label;

global ___cp0_prim_tr_ihandler: label;
global ___cp1_prim_tr_ihandler: label;
global ___cp2_prim_tr_ihandler: label;
global ___cp3_prim_tr_ihandler: label;

extern ___cp_rc_ihandler : label;
extern ___cp_tr_ihandler : label;


begin ".text_i"
<___cp0_prim_rc_ihandler>
	push ar5, gr5;
	gr5 = 0;
	goto ___cp_sec_rc_ihandler;
<___cp1_prim_rc_ihandler>
	push ar5, gr5;
	gr5 = 1;
	goto ___cp_sec_rc_ihandler;
<___cp2_prim_rc_ihandler>
	push ar5, gr5;
	gr5 = 2;
	goto ___cp_sec_rc_ihandler;
<___cp3_prim_rc_ihandler>
	push ar5, gr5;
	gr5 = 3;
	goto ___cp_sec_rc_ihandler;





<___cp0_prim_tr_ihandler>
	push ar5, gr5;
	gr5 = 0;
	goto ___cp_sec_tr_ihandler;
<___cp1_prim_tr_ihandler>
	push ar5, gr5;
	gr5 = 1;
	goto ___cp_sec_tr_ihandler;
<___cp2_prim_tr_ihandler>
	push ar5, gr5;
	gr5 = 2;
	goto ___cp_sec_tr_ihandler;
<___cp3_prim_tr_ihandler>
	push ar5, gr5;
	gr5 = 3;
	goto ___cp_sec_tr_ihandler;	



<___cp_sec_rc_ihandler>
	// xxx push ar5,gr5 pushed by prim handler
	push ar6,gr6;	// prevent spoil by _cp_rc_ihandler 
	push ar7,gr7;
	ar7 += 4;

	[ar7 - 1] = gr5;	// passing port number
	call ___cp_rc_ihandler;

	ar7 -= 4;
	pop ar7,gr7;
	pop ar6,gr6;
	pop ar5,gr5;

	ireturn;

<___cp_sec_tr_ihandler>
	// xxx push ar5,gr5 pushed by prim handler
	push ar6,gr6;	// prevent spoil by _cp_tr_ihandler 
	push ar7,gr7;
	ar7 += 4;

	[ar7 - 1] = gr5;	// passing port number
	call ___cp_tr_ihandler;

	ar7 -= 4;
	pop ar7,gr7;
	pop ar6,gr6;
	pop ar5,gr5;
	
	ireturn;




end ".text_i";
