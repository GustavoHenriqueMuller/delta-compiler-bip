.data
	a_1: 0, 0, 0, 0, 0

.text
	LDI 0
	STO 1000
	LDI 8
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV a_1
	LDI 1
	STO 1000
	LDI 4
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV a_1
	LDI 1
	STO 1000
	LDI 3
	STO 1001
	LDI 0
	STO 1002
	LD 1002
	STO $indr
	LDV a_1
	STO 1002
	LD 1001
	ADD 1002
	STO 1001
	# adsasdasd
	LD 1001
	STO 1002
	LD 1000
	STO $indr
	LDV a_1
	STO 1002
	LD 1001
	ADD 1002
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV a_1
	LDI 0
	STO 1000
	HLT 0
