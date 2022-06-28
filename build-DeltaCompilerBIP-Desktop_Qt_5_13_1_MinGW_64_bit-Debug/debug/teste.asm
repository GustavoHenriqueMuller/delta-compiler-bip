.data
	a_1: 0
	b_1: 0
	f_1: 0
	a_3: 0
	b_3: 0

.text
	jmp void_main
	HLT 0

int_soma_int_int:
	LD a_1
	STO 1000
	LD b_1
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	RETURN 0
int_soma_int:
	LD f_1
	STO 1000
	RETURN 0
void_main:
	LDI 2
	STO 1000
	LDI 4
	STO 1001
	LD 1001
	STO b_1
	LD 1000
	STO a_1
	CALL int_soma_int_int
	LD 1000
	STO a_3
	LDI 3
	STO 1000
	LD 1000
	STO f_1
	CALL int_soma_int
	LD 1000
	STO b_3
	LD a_3
	STO 1000
	LD b_3
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	LD 1000
	STO $out_port
	RETURN 0
