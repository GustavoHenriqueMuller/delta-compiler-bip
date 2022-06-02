.data
	pog_1: 0, 0, 0, 0, 0

.text
	LDI 0
	STO 1000
	LD 1000
	STO $indr
	LDV pog_1
	STO 1000
	LD 1000
	STO $out_port
	LDI 0
	STO 1000
	HLT 0
