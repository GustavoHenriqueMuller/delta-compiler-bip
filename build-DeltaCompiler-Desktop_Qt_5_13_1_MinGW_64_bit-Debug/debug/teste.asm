.data
	a_1: 0

.text
	LDI 8
	STO 1000
	LD 1000
	STO a_1
	LD a_1
	STO 1000
	LDI 5
	STO 1001
	LD 1001
	BEQ when_is_0_end_0
	LDI 50
	STO 1000
	LD 1000
	STO $out_port
	JMP when_end_0
when_is_0_end_0:
	LDI 8
	STO 999
	LD 999
	BEQ when_is_1_end_0
	LDI 100
	STO 998
	LD 998
	STO $out_port
	JMP when_end_0
when_is_1_end_0:
	LDI 200
	STO 997
	LD 997
	STO $out_port
when_end_0:
	HLT 0