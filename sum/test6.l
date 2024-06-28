00000000 ; Name: M.Maheeth Reddy
00000000 ; Roll: 1801CS31
00000000 ; I hereby declare that this test file is my own unaided work.
00000000 ; test06.asm
00000000 ; program to find sum of first n natural numbers
00000000 0000010d call sumN
00000001 00000012 HALT
00000002 
00000002 00000003 sumN:       stl 0       ; store return address (from A) into stack
00000003 ffffff0a             adj -1      ; adjust stack pointer
00000004 00001500     loop:   ldc num
00000005 00000004             ldnl 0      ; load num into A
00000006 00001600             ldc sum
00000007 00000004             ldnl 0      ; load sum into A, B = num
00000008 00000006             add         ; A = B + A
00000009 00001600             ldc sum
0000000a 00000005             stnl 0      ; store new value of sum
0000000b 00001500             ldc num
0000000c 00000004             ldnl 0      ; load num into A again
0000000d ffffff01             adc -1      ; decrement A
0000000e 0000030f             brz done    ; if A == 0, stop sum calculation
0000000f 00001500             ldc num
00000010 00000005             stnl 0      ; otherwise update num value
00000011 fffff211             br loop     ; repeat calculation
00000012     done:   
00000012 0000010a             adj 1       ; if done, adjust stack pointer
00000013 00000002             ldl 0       ; load calling address address to A
00000014 0000000e             return      ; return to calling address
00000015 
00000015 00000064 num: data 100       ; number upto which sum is calculated
00000016 00000000 sum: data 0         ; sum is stored here
