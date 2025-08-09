	.text
	lw	$t0,0($s0)
	add	$t2,$t0,$t1
	lw	$t3,4($s0)
	sub	$t4,$t3,$t2
	halt
	.data
data1:	.word	10
data2:	.word	20
data3:	.word	30
data4:	.word	40 