	.text
	andi	$s1,$s0,15
	ori	$t1,$t0,15
	andi	$s2,$s1,7
	ori	$t2,$t1,8
	halt
	.data
data1:	.word	15
data2:	.word	30
data3:	.word	45
data4:	.word	60 