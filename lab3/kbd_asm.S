#include "i8254.h"

.data
.global _codeAss
_codeAss:
.long

.text

.global _asm_handler
_asm_handler:
xor %eax,%eax
inb $OUT_BUFF, %al
mov %eax, _codeAss

ret

.end _asm_handler

