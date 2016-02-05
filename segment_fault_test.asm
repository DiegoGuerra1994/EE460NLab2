.ORIG x1234
ADD R0, R0, #2
ADD R1, R1, #4
BRp eyB0ss
STB R0, R1, #1
eyB0ss LDB R1, R1, #0 ;LDB causes segmentation fault
.FILL  x1337 
.END

