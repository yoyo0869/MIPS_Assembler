00000000 ; Simple While Loop , Checking By adding 1, condition i>0 to stop
00000000 ; looping from -500 to 501 and using brlz to branch
00000000 FFFE0C00 ldc -500
00000001 loop:
00000001 00000101 adc 1
00000002 FFFFFE10 brlz loop
00000003 00000012 HALT
