
.text

main:	
	li $2, 32
    li $1, 0
    nop
    nop

loop:
	subi $2, $2, 1
	nop
	nop
	nop
	bge  $2, $1, loop
	nop
	nop
	li $2, 10
	syscall
	