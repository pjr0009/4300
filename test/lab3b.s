## Daniel J. Ellard -- 02/21/94
## palindrome.asm -- read a line of text and test if it is a palindrome.
##
## A palindrome is a string that is exactly the same when read backwards
## as when it is read forward (e.g. anna).
##
## Whitespace is considered significant in this version, so "ahahaha"
## is a palindrome while "aha haha" is not.
##
## Register usage:
## $9 - A - initially, the address of the first character in the string S.
## $10 - B - initially, the address of the the last character in S.
## $11 - the character at address A.
## $12 - the character at address B.
## $2 - syscall parameter / return values.
## $4 - syscall parameters.
## $5 - syscall parameters.

	.text
main:
	
## read the string S:
	#address of write
	la $4, 200
	li $5, 1024
	li $2, 8		# load "read_string" code into $2.
	nop  #stall so that write back stage is executed before calling syscall
	nop
	nop
	syscall
	la $9, 200	# A = S.
	la $10, 200	# we need to move B to the end
	nop
	nop
	nop

length_loop:			# length of the string
	lb $11, ($10)		# load the byte at addr B into $11.
	nop
	nop  
	nop               # stall beq three cycles so that lb can load the byte into $11
	beqz $11, end_length_loop # if $11 == 0, branch out of loop.
	nop
	nop
	nop
	nop
	
	addi $10, $10, 1	# otherwise, increment B,
	b length_loop		# and repeat the loop.
	nop
	
end_length_loop:
	subi $10, $10, 2	# subtract 2 to move B back past # the '\0' 

test_loop:
	bge $9, $10, is_palin	# if A >= B, it's a palindrome.
	nop
	
	lb $11, ($9)		# load the byte at addr A into $11,
	lb $12, ($10)		# load the byte at addr B into $12.
	nop
	nop
	nop
	nop
	nop
	bne $11, $12, not_palin # if $11 != $12, not a palindrome.
	nop
	
# Otherwise,

	addi $9, $9, 1	# increment A,
	subi $10, $10, 1	# decrement B,
	b test_loop		# and repeat the loop.
	nop
	
is_palin:			# print the is_palin_msg, and exit.

	la $4, 0
	li $2, 4
	nop  #stall so that write back stage is executed before calling syscall
	nop
	nop
	syscall
	b exit
	nop
	
not_palin:
	la $4, 100	## print the not_palin_msg, and exit.
	li $2, 4
	nop  #stall so that write back stage is executed before calling syscall
	nop
	nop
	syscall
	b exit
	nop
	
exit:				# exit the program
	li $2, 10		# load "exit" code into $2.
	nop  #stall so that write back stage is executed before calling syscall
	nop
	nop
	syscall			# make the system call.


.data
	0:  .asciiz "The string is a palindrome.\n"
	100:  .asciiz "The string is not a palindrome.\n"
 	200:  .space 1024	# reserve 1024 bytes for the string.
