.data
	a_1: 0
	c_1: 0
	n_1: 0

.text
	CALL void_main
	HLT 0

void_main:
	LDI 3
	STO 1000
	LD 1000
	STO a_1
	LD a_1
	STO 1000
	LD 1000
	STO $out_port
	LDI 102
	STO 1000
	LD 1000
	STO c_1
	LD c_1
	STO 1000
	LD 1000
	STO $out_port
	LD $in_port
	STO 1000
	LD 1000
	STO n_1
	RETURN 0
