.data
	a_1: 0, 0, 0, 0, 0

.text
	LDI 0
	STO 1000
	LD 1000
	STO $indr
	LDV a_1
	STO 1000
	LDI 0
	STO 1001
	HLT 0