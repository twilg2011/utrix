.text 
.globl _getContext
_getContext: 
    popl %edi
	popl %esi
	pushl %esi
	pushl %edi
    movl %esp,4(%eax)
	movl %ebp,8(%eax)
	movl %esi,16(%eax)
	movl %ebx,20(%eax)
	movl %ecx,24(%eax)
	movl %edx,28(%eax)
	movl %ss,32(%eax)
	ret 
	
.globl _getIp
_getIp: popl %edi
        movl %edi,(%eax)
	    push %edi
		ret
	   


.globl _setContext

_setContext:
	movl 4(%eax) , %esp
	movl 8(%eax) , %ebp
	movl 20(%eax), %ebx
	movl 24(%eax), %ecx
	movl 28(%eax), %edx
	movl 32(%eax), %ss 
	movl 16(%eax), %eax
	ret 

	
.globl _swich
_swich: movl (%eax),%edi
        //movl %edi,(%esp)
		movl 16(%eax),%eax
		push %eax
		call *%edi 
        
.comm _eax,8,2
.comm _ip,4,2

