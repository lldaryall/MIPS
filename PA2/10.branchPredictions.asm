	.text
	bne	$t0,$t1,2
	add	$t2,$t0,$t1
	bne	$t2,$t1,2
	sub	$t3,$t2,$t1
	bne	$t3,$t2,2
	halt
	.data
data1:	.word	5
data2:	.word	10
data3:	.word	15
data4:	.word	20 