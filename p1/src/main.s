	.file	"main.c"
	.text
	.section	.rodata
	.align 8
.LC0:
	.string	"Voc\303\252 deve passar o nome do arquivo como par\303\242metro!"
.LC1:
	.string	"r"
.LC2:
	.string	"Error ao abrir arquivo!\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$4160, %rsp
	movl	%edi, -4148(%rbp)
	movq	%rsi, -4160(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -4132(%rbp)
	cmpl	$1, -4148(%rbp)
	jg	.L2
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L8
.L2:
	movq	-4160(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -4120(%rbp)
	movq	$0, -4128(%rbp)
	cmpq	$0, -4120(%rbp)
	je	.L4
	movq	-4120(%rbp), %rdx
	leaq	-4112(%rbp), %rax
	movq	%rdx, %rcx
	movl	$4096, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	%rax, -4128(%rbp)
	movq	-4120(%rbp), %rax
	movq	%rax, %rdi
	call	ferror@PLT
	testl	%eax, %eax
	je	.L5
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$24, %edx
	movl	$1, %esi
	leaq	.LC2(%rip), %rdi
	call	fwrite@PLT
	jmp	.L6
.L5:
	movq	-4128(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -4128(%rbp)
	movb	$0, -4112(%rbp,%rax)
.L6:
	movq	-4120(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
.L4:
	movq	-4128(%rbp), %rax
	movl	%eax, %edx
	leaq	-4112(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	processarEntrada@PLT
	movl	%eax, -4132(%rbp)
	cmpl	$0, -4132(%rbp)
	je	.L7
	movl	-4132(%rbp), %eax
	jmp	.L8
.L7:
	movl	$0, %eax
	call	imprimeListaTokens@PLT
	movl	$0, %eax
	call	emitirMapaDeMemoria@PLT
.L8:
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
