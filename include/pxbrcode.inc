	align 32
xbrsub:
	lda	r6
	phi	memaddr
	lda	r6
	plo	r6
	ghi	memaddr
	phi	r6
	sep	rret
xbzsub:
	bz	xbrsub
nbrsub:
	inc r6
	inc r6
	sep	rret
xbnzsub:
	bnz	xbrsub
	br	nbrsub
xbdfsub:
	bdf	xbrsub
	br	nbrsub
xbnfsub:
	bnf	xbrsub
	br	nbrsub

