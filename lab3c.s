
.text
main:	
	li $1, 1
	li $2, 1
	li $3, 1
	li $4, 4
	nop
	li $5, 1 
	li $6, 1
loop:	
	add $1, $2, $3
	add $2, $2, $4
.data
	0: .asciiz "The string is a palindrome.\n"
	1: .asciiz "The string is not a palindrome.\n"
	2: .space 1024