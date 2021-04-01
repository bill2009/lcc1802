;macros inserted between assembler passes for pixie accommodation.
;they get substituted for long branch instructions so as not to upset 1861 video timing
;a cmpanion code module pxbrcode.inc is required
;21-03-08 added pagefit nn to ensure there are nn bytes left in a 256 byte page
  IFNDEF	pagefit
pagefit: macro bytesneeded	
	if ($#256)>(255-bytesneeded)
	    align 256,0xe2
	endif
	endm
  ENDIF	
xbr:	macro	target
	sep	RCALL
	dw	xbrsub
	dw	target
	endm
xbz:	macro	target
	sep	RCALL
	dw	xbzsub
	dw	target
	endm
xbnz:	macro	target
	sep	RCALL
	dw	xbnzsub
	dw	target
	endm
xbdf:	macro	target
	sep	RCALL
	dw	xbdfsub
	dw	target
	endm
xbnf:	macro	target
	sep	RCALL
	dw	xbnfsub
	dw	target
	endm
	