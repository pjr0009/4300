
	.text
main:
li $2, 10
subi $1, $2, 3
addi $1, $2, 3
li $3, 5
add  $1, $2, $3

# main:	li $1, 0
# 	li $2, 32

# loop:	subi $2, $2, 1
# 	nop
# 	nop
# 	nop
# 	nop
# 	nop
# 	bge  $2, $1, loop
# 	nop

# 	li $2, 10
# 	syscall
	