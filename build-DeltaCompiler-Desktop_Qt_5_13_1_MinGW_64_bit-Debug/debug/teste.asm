.data
	i_2: 0

.text
	LDI 5
	STO 1000
	LD 1000
	STO i_2
for_start_0:
	LD i_2
	STO 1000
	LDI 7
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1000
	LD 1000
	BEQ for_end_0
	LD i_2
	STO 1000
	LD 1000
	STO $out_port
	LD i_2
	STO 1000
	ADDI 1
	STO i_2
	JMP for_start_0
for_end_0:
	HLT 0