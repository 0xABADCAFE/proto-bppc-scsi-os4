VERSION = 0
REVISION = 1

.macro DATE
.ascii "14.5.2017"
.endm

.macro VERS
.ascii "blizzppc.device 0.1"
.endm

.macro VSTRING
.ascii "blizzppc.device 0.1 (14.5.2017)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: blizzppc.device 0.1 (14.5.2017)"
.byte 0
.endm
