
global _mul_vecs: label;


const TIMER_SET_ADDR = 20802h;
const TIMER_VAL_ADDR = 20800h;

const IB2 = 0_8000h;
const IB3 = 0_C000h;
const IB4 = 1_0000h;
const IB5 = 1_4000h;
const IB6 = 1_8000h;
const IB7 = 1_C000h;


const CYCLIC_MODE = 1;	//cyclic processing first 128 32-words again and again
//max value : 7fff_ffffh : about 2m18s

// w        was 007f_ffffh
const REPEATS = 007f_ffffh;	//128 = process entire bank, 1 = process 128 words (256 total fot both duets)
						//DO NOT set > 128 if not using CYCLIC_MODE



begin ".text_asm_timer"
<_mul_vecs>
	push ar0, gr0;
	push ar1, gr1;
	push ar2, gr2;
	push ar3, gr3;
	push ar4, gr4;
	push ar5, gr5;

	set fp_branch;

	//duet number one(@): fpu 0 + fpu 1
	//loads data from IB2(ar0), IB3(ar1) and IB4(ar2) (for IB4(ar2) once, first 64 32-words)
	//stores result in IB4 (ar2)

	//duet number two(#): fpu 2 + fpu 3
	//loads data from IB5(ar3), IB6(ar4) and IB7(ar5) (for IB7(ar5) once, first 64 32-words)
	//stored result in IB7 (ar5)

	//two duets works in parallel independently 

	//for @ duet one
	ar0 = IB2;
	ar1 = IB3;
	ar2 = IB4;

	//for # duet two
	ar3 = IB5;
	ar4 = IB6;
	ar5 = IB7;

	//initial fill
	fpu 0 rep 32 vreg0 = [ar0++];
	fpu 2 rep 32 vreg0 = [ar1++];

	fpu 0 rep 32 vreg3 = [ar3++];
	fpu 2 rep 32 vreg3 = [ar4++];


	fpu 0 rep 32 vreg6 = [ar0++];
	fpu 2 rep 32 vreg6 = [ar1++];

	fpu 0 rep 32 vreg7 = [ar3++];
	fpu 2 rep 32 vreg7 = [ar4++];



	fpu 1 rep 32 vreg6 = [ar0++];
	fpu 1 rep 32 vreg7 = [ar1++];

	fpu 3 rep 32 vreg6 = [ar3++];
	fpu 3 rep 32 vreg7 = [ar4++];



	ar0 = IB2;
	ar1 = IB3;
	ar2 = IB4;
	ar3 = IB5;
	ar4 = IB6;
	ar5 = IB7;





	gr3 = REPEATS;

	gr3;
	if <= goto _skip;

	<cycle_start>

	//process is divided in two identical halves, except using different sets of fpu registers to avoid idle

	//@read 0 - even half
	fpu 0 rep 32 vreg1 = [ar0++];
	//#read 2 - even half
	fpu 2 rep 32 vreg1 = [ar3++];
	//@exec 0 - even half
	fpu 0 .matrix vreg2 = vreg0 * (vreg6, vreg7) + vreg1;
	//#exec 2 - even half
	fpu 2 .matrix vreg2 = vreg0 * (vreg6, vreg7) + vreg1;
	//@read 1 - even half
	fpu 1 rep 32 vreg1 = [ar1++];
	//#read 3 - even half
	fpu 3 rep 32 vreg1 = [ar4++];
	//@tr 0-1 - even half
	fpu 1 vreg0 = fpu 0 vreg2;
	//#tr 2-3 - even half
	fpu 3 vreg0 = fpu 2 vreg2;
	//@exec 1 - even half
	fpu 1 .matrix vreg2 = vreg0 * (vreg6, vreg7) + vreg1;
	//#exec 3 - even half
	fpu 3 .matrix vreg2 = vreg0 * (vreg6, vreg7) + vreg1;
	//@write 1 - even half
	fpu 1 rep 32 [ar2++] = vreg2;
	//#write 3 - even half
	fpu 3 rep 32 [ar5++] = vreg2;
	
	//@read 0 - odd half
	fpu 0 rep 32 vreg4 = [ar0++];
	//#read 2 - odd half
	fpu 2 rep 32 vreg4 = [ar3++];
	//@exec 0 - odd half
	fpu 0 .matrix vreg5 = vreg3 * (vreg6, vreg7) + vreg4;
	//#exec 2 - odd half
	fpu 2 .matrix vreg5 = vreg3 * (vreg6, vreg7) + vreg4;
	//@read 1 - odd half
	fpu 1 rep 32 vreg4 = [ar1++];
	//#read 3 - odd half
	fpu 3 rep 32 vreg4 = [ar4++];
	//@tr 0-1 - odd half
	fpu 1 vreg3 = fpu 0 vreg5;
	//#tr 2-3 - odd half
	fpu 3 vreg3 = fpu 2 vreg5;
	//@exec 1 - odd half
	fpu 1 .matrix vreg5 = vreg3 * (vreg6, vreg7) + vreg4;
	//#exec 3 - odd half
	fpu 3 .matrix vreg5 = vreg3 * (vreg6, vreg7) + vreg4;
	//@write 1 - odd half
	fpu 1 rep 32 [ar2++] = vreg5;
	//#write 3 - odd half
	fpu 3 rep 32 [ar5++] = vreg5;

.if CYCLIC_MODE;
	ar0 -= 128;
	ar1 -= 128;
	ar2 -= 128;
	ar3 -= 128;
	ar4 -= 128;
	ar5 -= 128;
.endif;

	gr3--;
	if > goto cycle_start;

	<_skip>

	gr7 = 777;

	pop ar5, gr5;
	pop ar4, gr4;
	pop ar3, gr3;
	pop ar2, gr2;
	pop ar1, gr1;
	pop ar0, gr0;
	return;

end ".text_asm_timer";
