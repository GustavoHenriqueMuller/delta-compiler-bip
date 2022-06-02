.data
	pog_1: 0, 0, 0, 0, 0

.text
	LDI 2
	STO 1000
	LD $in_port
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV pog_1
	LDI 0
	STO 1000
	HLT 0
