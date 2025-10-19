	.text
	lw	$t0,0($s0)
	sw	$t0,4($s0)
	lw	$t1,4($s0)
	halt
	.data
data1:	.word	15
data2:	.word	25
data3:	.word	35
data4:	.word	45