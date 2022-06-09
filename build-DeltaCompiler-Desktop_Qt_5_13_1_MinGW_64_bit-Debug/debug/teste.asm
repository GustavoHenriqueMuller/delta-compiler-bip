.data
	i_2: 0

.text
	LDI 0
	STO 1000
	LD 1000
	STO i_2
for_inicio:
	LD i_2
	STO 1000
	LDI 5
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1000
	LD 1000
	BEQ for_final
	LD i_2
	STO 1000
	JMP for_inicio
for_final:
	LDI 0
	STO 1001
	HLT 0