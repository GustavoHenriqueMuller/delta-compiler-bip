.data
	f_1: 0
	a_2: 0
	b_2: 0
	pog1_2: 0
	pog2_2: 0
	a_3: 0
	b_3: 0

.text
	CALL void_main
	HLT 0

int_soma_const int:
	LD f_1
	STO 1000
	RETURN 0
int_soma_int_int:
	LD a_2
	STO 1000
	LD 1000
	STO f_1
	CALL int_soma_const int
	LD 1000
	STO pog1_2
	LD b_2
	STO 1000
	LD 1000
	STO f_1
	CALL int_soma_const int
	LD 1000
	STO pog2_2
	LD pog1_2
	STO 1000
	LD pog2_2
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	RETURN 0
void_main:
	LDI 2
	STO 1000
	LDI 4
	STO 1001
	LD 1001
	STO b_2
	LD 1000
	STO a_2
	CALL int_soma_int_int
	LD 1000
	STO a_3
	LDI 3
	STO 1000
	LD 1000
	STO f_1
	CALL int_soma_const int
	LD 1000
	STO b_3
	LD b_3
	STO 1000
	LDI 2
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1002
	LD 1001
	SRL 10
	ANDI 1
	STO 1003
	LD 1001
	NOT 0
	ADDI 1
	SRL 10
	ANDI 1
	OR 1003
	XORI 1
	STO 1003
	LD 1002
	XORI 1
	STO 1002
	LD 1003
	XORI 1
	AND 1002
	STO 1000
	LD 1000
	BEQ if_end_0
	LDI 2
	STO 1000
	LD 1000
	STO $out_port
	JMP if_stmt_end_0
if_end_0:
if_stmt_end_0:
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
