VERSION		EQU	0
REVISION	EQU	1

DATE	MACRO
		dc.b '14.5.2017'
		ENDM

VERS	MACRO
		dc.b 'blizzppc.device 0.1'
		ENDM

VSTRING	MACRO
		dc.b 'blizzppc.device 0.1 (14.5.2017)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: blizzppc.device 0.1 (14.5.2017)',0
		ENDM
