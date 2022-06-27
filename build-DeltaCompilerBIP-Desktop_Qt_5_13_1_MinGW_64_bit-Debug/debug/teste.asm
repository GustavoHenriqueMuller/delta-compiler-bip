.data
	a_3: 0
	b_3: 0

.text
int_soma_int_int:
	LD a_1
	STO 1000
	LD b_1
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	RETURN 0
int_soma_char:
	LDI 1
	STO 1001
	RETURN 0
void_main:
	LDI 2
	STO 1002
	LDI 4
	STO 1003
	LD 1003
	STO b_1
	LD 1002
	STO a_1
	CALL int_soma_int_int
	LD 1001
	STO a_3
	LD 1000
	STO c_1
	CALL int_soma_char
	LD 999
	STO b_3
	LD a_3
	STO 999
	LD b_3
	STO 1000
	LD 999
	ADD 1000
	STO 999
	LD 999
	STO $out_port
	HLT 0