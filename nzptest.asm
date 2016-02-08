.ORIG x1234
ADD R0, R0, #2
BRp pos
pos LEA R3, addr
neg LDW R3, R3, #-1
ADD R0, R0, #-7
BRn neg
ADD R1, R1, #-4
ADD R2, R2, #-2
ADD R3, R1, R2
LDW R1, R1, #0 ; R1 = addr
LEA R0, b0ss
LDW R0, R0, #0 ; R0 = b0ss
STW R0, R1, #0 ;
end HALT
b0ss .FILL  x1337 
addr .FILL x004A
.END

