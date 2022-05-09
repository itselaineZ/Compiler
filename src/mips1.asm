main:
addi $1 $0 3
addi $2 $0 4
addi $3 $0 2
add $4 $0 $1
add $5 $0 $2
add $6 $0 $3
jal demo
add $7 $0 $30
jal program
j END
program:
addi $8 $0 0
add $9 $5 $7
sub $10 $4 $9
bgtz $10 J0
j J1
J0:
mul $11 $5 $7
addi $12 $11 1
add $11 $4 $12
add $13 $0 $11
j J5
J1:
add $13 $0 $4
J5:
addi $14 $0 100
sub $10 $8 $14
blez $10 J3
j J4
J3:
addi $15 $0 2
mul $14 $13 $15
add $8 $0 $14
j J5
J4:
add $30 $0 $8
jr $31
demo:
addi $15 $4 2
add $4 $0 $15
addi $17 $0 2
mul $16 $4 $17
add $30 $0 $16
jr $31
END: