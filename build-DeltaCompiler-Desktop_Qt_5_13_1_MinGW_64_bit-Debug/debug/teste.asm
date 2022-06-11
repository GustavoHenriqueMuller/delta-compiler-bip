.data
	i_1: 0

.text
	LDI 0
	STO 1000
	LD 1000
	STO i_1
while_start_0:
	LDI 1
	STO 1000
	LD 1000
	BEQ while_end_0
	LD i_1
	STO 1000
	LD 1000
	STO $out_port
	LD i_1
	STO 999
	ADDI 1
	STO i_1
	JMP while_start_0
while_end_0:
	LDI 0
	STO 999
	HLT 0