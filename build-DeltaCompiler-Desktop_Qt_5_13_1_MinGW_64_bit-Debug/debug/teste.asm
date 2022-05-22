.data
	a_1: 0

.text
	LD a_1
	STO 1000
	LD $in_port
	STO a_1
	LD a_1
	STO 1000
	LDI 10
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	LD 1000
	STO $out_port
	LDI 0
	STO 1000
	HLT 0
