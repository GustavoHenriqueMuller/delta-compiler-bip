.data
	a_1: 0, 0, 0, 0, 0
	b_1: 0

.text
	LDI 2
	STO 1000
	LDI 4
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV a_1
	LDI 2
	STO 1000
	LD 1000
	STO $indr
	LDV a_1
	ADDI 1
	STOV 1000
	STOV a_1
	LD 1000
	STO b_1
	HLT 0