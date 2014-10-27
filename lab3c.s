.text

main:	
	li $1, 30
	li $2, 10
	li $3, 100
	li $4, 1000
	li $5, 7

loop:	
	subi $5, $5, 1
	
	# bge  $5, $0, loop
	# nop
	beqz $5, loop # if $5 == 0, branch out of loop.

	add $4, $0, $1
	li $2, 1
	syscall
	
	li $2, 10
	syscall


.data