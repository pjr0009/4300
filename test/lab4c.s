
	.text

main:	
lid $f0, 1.65
lid $f1, -0.57
lid $f2, 0.0
lid $f3, 0.80
lid $f4, -0.92	
lid $f5, 0.0
li $5, 100
li $0, 0

loop:	
fmul $f8, $f0, $f2
fsub $f6, $f8, $f1
fmul $f9, $f3, $f5
fsub $f7, $f9, $f4
sd $f2, ($f10)
ld $f1, ($f10)
sd $f6, ($f11)
ld $f2, ($f11)
sd $f5, ($f12)
ld $f4, ($f12)
sd $f7, ($f13)
ld $f5, ($f13)
sd $f6, ($f14)
sd $f7, ($f15)
subi $5, $5, 1
bge $5, $0, loop
	