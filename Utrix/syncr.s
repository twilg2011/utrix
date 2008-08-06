

.text
.globl _lock

_lock:	movl 4(%esp),%eax
	movl (%eax),%ecx
	movl %eax,%edx
	movl $0,%eax
	movl $1,%ebx
	cmpxchg %ebx,%ecx
	jne cambio
	movl %ecx,(%edx)
	movl %edx,4(%esp)
	ret

cambio: jmp _lock	
        ret	 

.globl _unlock
_unlock: movl 4(%esp),%eax
	 movl (%eax),%ebx
	 movl $0,%ebx
	 movl %ebx,(%eax)
	 movl %eax,4(%esp)
	 ret


