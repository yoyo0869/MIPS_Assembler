00000000 ; A program to value of two data locations
00000000 
00000000 00000d00 ldc a       ; get address of a
00000001 00000004 ldnl 0      ; store a in A
00000002 
00000002 00000e00 ldc b       ; get address of b ; shift A's into B
00000003 00000004 ldnl 0      ; store b in A
00000004 
00000004 ffffff0a adj -1      ; make space on stack
00000005 00000003 stl 0       ; put A's on stack
00000006 
00000006 00000e00 ldc b       ; get address of b
00000007 00000005 stnl 0      ; store value of a at b
00000008 
00000008 00000002 ldl 0       ; get value from stack
00000009 00000d00 ldc a       ; get address of a
0000000a 00000005 stnl 0      ; store value of b at a
0000000b 
0000000b 0000010a adj 1       ; put SP to original location
0000000c 
0000000c 00000012 HALT        ; end the program
0000000d 
0000000d 
0000000d 00000017 a:  data 23      ; first data
0000000e fffff111 b:  data 17      ; second data
0000000f 
