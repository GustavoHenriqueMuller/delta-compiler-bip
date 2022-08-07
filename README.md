# DeltaCompilerBIP
Compiler for the Delta toy programming language to BIP assembly.

Syntax: ```delta [options] [filepath]```

```[options]```  
```-watch```: Prints Bhaskara (IDE) information in console;  
```-debug```: Prints debugging information in console;

```[filepath]```: Relative or absolute path of .delta file to be compiled.

## Features
Variables and datatypes - Integer (int), Character (char), Boolean (bool) and Void (void):
```c
void main() {
  int a = 5;
  char b = 'x';
  boolean c = true;
  
  return;
}
```
Single-line and multi-line comments:
```c
void main() {
  // I'm a single-lined comment
  
  /*
    I'm a
    multi-lined
    comment
  */
  
  return;
}
```
Expressions - Addition, subtraction, bit OR, bit XOR, bit AND, bit LS, bit RS (usable with assignment operator):
```c
void main() {
  int value = 4 + 4 - 2;
  int dividedByTwo = value << 2;
  int negated = ~value;

  int incrementAssign = 4;
  incrementAssign += 2; // "incrementAssign" now contains 6
}
```
Unary operators:
```c
void main() {
  int unary = 0;
  unary--; // expression returns 0 and "unary" contains -1
  
  unary = 0;
  --unary; // expression returns -1 and "unary" contains -1
  
  unary = 0;
  unary++; // expression returns 0 and "unary" contains 1
  
  unary = 0;
  ++unary; // expression returns 1 and "unary" contains 1
  
  return;
}
```
Single-dimension arrays for any datatype:
```c
void main() {
  int[3] array;
  array[0] = 10;
  array[1] = array[0] + 15;
    
  return;
}
```
Control structures: if, elseif, else, while, for, do while and when-is (switch-case statement):
```c
void main() {
  int[3] array;
  array[0] = 20;
  array[1] = 15;
  array[2] = 10;
  
  // Bubblesort
  for (int i = 0; i < 3 - 1; i++) {
    for (int j = 0; j < 3 - i - 1; j++) {
      if (array[j] > array[j + 1]) {
        int temp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = temp;
      }
    }
  }
  
  // Print all elements
  for (int i = 0; i < 3; i++) {
    print(array[i]);
  }
  
  return;
}
```
```c
void main() {
  int number = 42;
  int choice = 0;

  while (true) {
    choice = input();
    
    if (choice > number) {
      print(1); // lower!
    } elseif (choice < number) {
      print(2); // higher!
    } else {
      print(3); // you won!
      return;
    }
  }
  
  return;
}
```
Input and output:
```c
void main() {
  int a = 3;
  print(a); // prints 3
  
  char c = 'f';
  print(c); // prints 102 (ASCII equivalent)
  
  int n = input(); // takes integer number from user
  
  return;
}
```
Assembly generated for bubblesort:
```
.data
	array_1: 0, 0, 0
	i_2: 0
	j_3: 0
	temp_4: 0
	i_5: 0

.text
	CALL void_main
	HLT 0

void_main:
	LDI 0
	STO 1000
	LDI 20
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV array_1
	LDI 1
	STO 1000
	LDI 15
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV array_1
	LDI 2
	STO 1000
	LDI 10
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV array_1
	LDI 0
	STO 1000
	LD 1000
	STO i_2
for_start_0:
	LD i_2
	STO 1000
	LDI 3
	STO 1001
	LDI 1
	STO 1002
	LD 1001
	SUB 1002
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
	LDI 0
	STO 1000
	LD 1000
	STO j_3
for_start_1:
	LD j_3
	STO 1000
	LDI 3
	STO 1001
	LD i_2
	STO 1002
	LD 1001
	SUB 1002
	STO 1001
	LDI 1
	STO 1002
	LD 1001
	SUB 1002
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1000
	LD 1000
	BEQ for_end_1
	LD j_3
	STO 1000
	LD 1000
	STO $indr
	LDV array_1
	STO 1000
	LD j_3
	STO 1001
	LDI 1
	STO 1002
	LD 1001
	ADD 1002
	STO 1001
	LD 1001
	STO $indr
	LDV array_1
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1002
	LD 1001
	SRL 10
	ANDI 1
	STO 1003
	LD 1001
	NOT 0
	ADDI 1
	SRL 10
	ANDI 1
	OR 1003
	XORI 1
	STO 1003
	LD 1002
	XORI 1
	STO 1002
	LD 1003
	XORI 1
	AND 1002
	STO 1000
	LD 1000
	BEQ if_end_2
	LD j_3
	STO 1000
	LD 1000
	STO $indr
	LDV array_1
	STO 1000
	LD 1000
	STO temp_4
	LD j_3
	STO 1000
	LD j_3
	STO 1001
	LDI 1
	STO 1002
	LD 1001
	ADD 1002
	STO 1001
	LD 1001
	STO $indr
	LDV array_1
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV array_1
	LD j_3
	STO 1000
	LDI 1
	STO 1001
	LD 1000
	ADD 1001
	STO 1000
	LD temp_4
	STO 1001
	LD 1000
	STO $indr
	LD 1001
	STOV array_1
	JMP if_stmt_end_2
if_end_2:
if_stmt_end_2:
	LD j_3
	STO 1000
	ADDI 1
	STO j_3
	JMP for_start_1
for_end_1:
	LD i_2
	STO 1000
	ADDI 1
	STO i_2
	JMP for_start_0
for_end_0:
	LDI 0
	STO 1000
	LD 1000
	STO i_5
for_start_3:
	LD i_5
	STO 1000
	LDI 3
	STO 1001
	LD 1000
	SUB 1001
	STO 1001
	LD 1001
	SRL 10
	ANDI 1
	STO 1000
	LD 1000
	BEQ for_end_3
	LD i_5
	STO 1000
	LD 1000
	STO $indr
	LDV array_1
	STO 1000
	LD 1000
	STO $out_port
	LD i_5
	STO 1000
	ADDI 1
	STO i_5
	JMP for_start_3
for_end_3:
	RETURN 0
```
