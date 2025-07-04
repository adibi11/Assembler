MAIN: mov @r7 ,LOOP
LOOP: jmp L1
prn 5
mcro m1
bne END
L1: inc K
endmcro
m1
bne LOOP
END: stop
STR: .string "abcdef"
LENGTH: .data 6 , -9,15,235,235,3,5,3,2,3,4,5,6,7
K: .data 1


