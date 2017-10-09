#include "defs.h"

.data
.global _data
_data:
.long

.data
.global _output
_output:
.long

.text

.global _asm_inb
_asm_inb:
xor %eax,%eax
inb $RTC_DATA_REG, %al
mov %eax, _data

ret

.end _asm_inb
