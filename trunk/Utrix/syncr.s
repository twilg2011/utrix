

.text
.globl _lock

_lock: 	movl $0,%eax
	movl $1,%ebx
	cmpxchg %ebx,val
	jne _lock	
        ret	 

.globl _unlock
_unlock: movl $0,val
	 ret
