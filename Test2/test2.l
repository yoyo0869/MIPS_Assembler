00000000 ; test2.asm
00000000 ; Test error handling
00000000 label:
00000000 label: ; duplicate label definiton
00000000 FFFFFF11 br nonesuch ; no such label
00000001 00000000 ldc 08ge ; not a number
00000002 00000000 ldc ; missing operand
00000003 00000006 add 5 ; unexpected operand
00000004 00000500 ldc 5, 6; extra on end of line
00000005 0def: ; bogus label name
00000005 fibble; bogus mnemonic
00000005 0def ; bogus mnemonic
