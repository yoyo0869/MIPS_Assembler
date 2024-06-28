; Simple While Loop , Checking By adding 1, condition i>0 to stop
; looping from -500 to 501 and using brlz to branch
ldc -500
loop:
adc 1
brlz loop
HALT