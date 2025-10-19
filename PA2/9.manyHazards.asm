	.text
	add	$t2,$t0,$t1
	lw	$t0,0($s0)
	sub	$t3,$t0,$t2
	add	$t4,$t3,$t2
	halt
	.data
data1:	.word	5
data2:	.word	10
data3:	.word	15
data4:	.word	20 