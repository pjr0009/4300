
	.text
main:
li $2, 10
subi $1, $2, 3
lid $f4, 3.5
addi $1, $2, 3
li $3, 5
lid $f2, 1.2
add  $1, $2, $3
fmul $f3, $f4, $f2
fsub $f5, $f4, $f2
fadd $f6, $f4, $f2
li $5, 4
sd $f2, ($f13)
ld $f5, ($f13)
nop

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
	