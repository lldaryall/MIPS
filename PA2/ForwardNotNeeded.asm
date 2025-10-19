	.text
	ori	$s0,$0,40
	noop
	noop
	lw	$t1,0($s0)
	lw	$t2,4($s0)
	andi	$t3,$s0,30
	ori	$t4,$s0,14
	sub	$s2,$t1,$t2
	sw	$t3,8($s0)
	halt
	.data
Thing:	.word	12
Whatever:	.word	 25
blue:	.word	19