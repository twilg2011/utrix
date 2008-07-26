	.file	"main.c"
	.text
.globl _init
_init:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$_globalSp
	call	_getPrisp
	addl	$16, %esp
	leave
	ret
	.data
LC0:
	.ascii "f:\0"
	.text
.globl _f
_f:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$LC0
	call	_puts
	addl	$16, %esp
	movl	_tcw, %eax
	movl	16(%eax), %edx
	movl	_tcf, %eax
	movl	16(%eax), %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	_pth_start
	addl	$16, %esp
	leave
	ret
	.data
LC1:
	.ascii "w:\0"
	.text
.globl _w
_w:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	subl	$12, %esp
	pushl	$LC1
	call	_puts
	addl	$16, %esp
	movl	_mctx, %eax
	movl	_tcw, %edx
	movl	16(%edx), %edx
	subl	$8, %esp
	pushl	%eax
	pushl	%edx
	call	_pth_start
	addl	$16, %esp
	leave
	ret
.globl _main
_main:
	leal	4(%esp), %ecx
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	subl	$32, %esp
	call	___main
	call	_init
	movl	$0, -12(%ebp)
	subl	$12, %esp
	pushl	$20
	call	_malloc
	addl	$16, %esp
	movl	%eax, _tcf
	subl	$12, %esp
	pushl	$20
	call	_malloc
	addl	$16, %esp
	movl	%eax, _tcw
	movl	_tcf, %ebx
	subl	$8, %esp
	pushl	$0
	pushl	$_f
	call	_pth_init
	addl	$16, %esp
	movl	%eax, 16(%ebx)
	movl	_tcw, %ebx
	subl	$8, %esp
	pushl	$0
	pushl	$_w
	call	_pth_init
	addl	$16, %esp
	movl	%eax, 16(%ebx)
	subl	$12, %esp
	pushl	$84
	call	_malloc
	addl	$16, %esp
	movl	%eax, _mctx
	movl	_tcf, %eax
	movl	16(%eax), %eax
	movl	_mctx, %edx
	subl	$8, %esp
	pushl	%eax
	pushl	%edx
	call	_pth_start
	addl	$16, %esp
	leal	-8(%ebp), %esp
	popl	%ecx
	popl	%ebx
	leave
	leal	-4(%ecx), %esp
	ret
.comm _tcf,16
.comm _tcw,16
.comm _mctx,16
