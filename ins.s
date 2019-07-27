.data
aa:
	.long 0x1234

.text
.global main

main:
	rep movsb
	rep scasb
	in %dx, %al
	in %dx, %eax
	int3
	int $0x80
	cld
	std
	cli
	sti
	movsb
	movsw
	movsl
	movsq
	cmpsb 
	cmpsw 
	cmpsl
	cmpsq
	lodsb
	lodsw
	lodsl
	lodsq
	stosb
	stosw
	stosl
	stosq
	scasb
	scasw
	scasl
	scasq
	pop 4(%rax)
	xchg %ax, %ax
	xchg %eax, %eax
	xchg %rbx, %rax
	xchg %bx, %ax
	xchg aa, %ax
	xchg aa, %eax
	xchg aa, %rax
	xchg 0x2019(%rax, %rbx, 8), %rbx
	push %fs
	pop %fs
	push %gs
	pop %gs
	imul %ebx
	imul %fs:0x1234, %ax
	imul %fs:0x1234, %eax
	imul $0x1234, %ax
	imul $0x1234, %eax
	imul $0x1234, %rax
	imul $0x1234, %eax, %ebx
	imul $aa, %eax, %ebx
	imul $aa, %ecx, %r8d
	and $0x12, %al
	xor $0x12, %al
	cmp $0x12, %al
	cmp $0x1234, %eax
	test $0x1234, %rax
	sub $0x12, %al
	sub $0x1234, %ax
	sub $0x1234, %eax
	sub $0x1234, %rax
	add $0x12, %al
	add $0x1234, %ax
	add $0x1234, %eax
	add $0x1234, %rax
	add %rax, %rbx
	add 0x16(%rax), %rbx
	add 0x16(%rax, %rcx, 8), %rbx
	add %r8, %r15
	add %rdx, %r13
	add %r11, %rbx
	add %r8, (%r15)
	add %r8, 0x12345678(%r15, %rax, 4)
	add 0x1234(%rip), %r9
	add 0x1234(%rbp), %r9
	add 0x1234(%rbp, %r15), %r9
	mov 8(%rbp, %rax, 2), %rax
	leaq 8(%rbp, %rax, 2), %rax
	leaq 8(%rbp), %rax
	leaq -8(%rbp), %rax
	leaq -201008(%rbp), %r13
	mov $0x28, %rax
	mov %fs:0x28, %rax
	mov %fs:(0x28), %rax
	mov %ds:(%rax), %rbx
	mov %ds:aa, %rbx
	mov %gs:(%rax), %rcx
	mov %rax, %fs:(%rcx)
	mov %rax, %fs:0x8(%rcx)
	movb $1, %al
	movb $-1, %cl
	movb $12, %r12b
	movw $123, %r8w
	movw $123, %ax
	movw 0x4(%rbx), %ax
	mov -0x4(%rbx), %rax
	mov %rsp, 0x8(%rax,%rbx,4)
	mov %r12, 0x8(%r12,%rbx,4)
	mov %r12, 0x8(%r12,%r8,1)
	mov %rsp, 0x20188(%r12,%r8,1)
	mov %rdx, 0x8(%rbp,%rax,2)
	mov %rdx, 0x20188(%rbp,%rax,2)
	mov %rdx, 0x8(%rbp, %rax)
	mov %rdx, (%r13, %rax)
	mov %rdx, (%r13, %rbp, 8)
	mov %rbp, 0x20188(%r13, %rbp, 8)
	mov %rbp, (%rax, %rax, 1)
	mov %rbp, (%rax, 1)
	push %fs 
	push %bp
	pushq %rbp
        pop %rdi
        pop %rsi
        pop %rbp
        pop %rsp
        pop %rbx
        pop %rdx
        pop %rcx
        pop %rax
        pop %r15
        pop %r14
        pop %r13
        pop %r12
        pop %r11
        pop %r10
        pop %r9
        pop %r8
	nop

        push %rdi
        push %rsi
        push %rbp
        push %rsp
        push %rbx
        push %rdx
        push %rcx
        push %rax
        push %r15
        push %r14
        push %r13
        push %r12
        push %r11
        push %r10
        push %r9
        push %r8
	nop

        dec %rdi
        dec %rsi
        dec %rbp
        dec %rsp
        dec %rbx
        dec %rdx
        dec %rcx
        dec %rax
        dec %r15
        dec %r14
        dec %r13
        dec %r12
        dec %r11
        dec %r10
        dec %r9
        dec %r8
        nop

	inc %rdi
	inc %rsi
	inc %rbp
	inc %rsp
	inc %rbx
	inc %rdx
	inc %rcx
	inc %rax
	inc %r15
	inc %r14
	inc %r13
	inc %r12
	inc %r11
	inc %r10
	inc %r9
	inc %r8
	nop

        not %rdi
        not %rsi
        not %rbp
        not %rsp
        not %rbx
        not %rdx
        not %rcx
        not %rax
        not %r15
        not %r14
        not %r13
        not %r12
        not %r11
        not %r10
        not %r9
        not %r8
        nop


        neg %rdi
        neg %rsi
        neg %rbp
        neg %rsp
        neg %rbx
        neg %rdx
        neg %rcx
        neg %rax
        neg %r15
        neg %r14
        neg %r13
        neg %r12
        neg %r11
        neg %r10
        neg %r9
        neg %r8
        nop

	xor %rdi, %rdi
	xor %rsi, %rsi
	xor %rbp, %rbp
	xor %rsp, %rsp
	xor %rbx, %rbx
	xor %rdx, %rdx
	xor %rcx, %rcx
	xor %rax, %rax
	xor %r15, %r15
	xor %r14, %r14
	xor %r13, %r13
	xor %r12, %r12
	xor %r11, %r11
	xor %r10, %r10
	xor %r9, %r9
	xor %r8, %r8
	nop

#-------------------------------
	xor %rdi, %rdi
	xor %rdi, %rsi
	xor %rdi, %rbp
	xor %rdi, %rsp
	xor %rdi, %rbx
	xor %rdi, %rdx
	xor %rdi, %rcx
	xor %rdi, %rax
	nop

        xor %rsi, %rdi
        xor %rsi, %rsi
        xor %rsi, %rbp
        xor %rsi, %rsp
        xor %rsi, %rbx
        xor %rsi, %rdx
        xor %rsi, %rcx
        xor %rsi, %rax
	nop

        xor %rbp, %rdi
        xor %rbp, %rsi
        xor %rbp, %rbp
        xor %rbp, %rsp
        xor %rbp, %rbx
        xor %rbp, %rdx
        xor %rbp, %rcx
        xor %rbp, %rax
        nop

        xor %rsp, %rdi
        xor %rsp, %rsi
        xor %rsp, %rbp
        xor %rsp, %rsp
        xor %rsp, %rbx
        xor %rsp, %rdx
        xor %rsp, %rcx
        xor %rsp, %rax
        nop

        xor %rbx, %rdi
        xor %rbx, %rsi
        xor %rbx, %rbp
        xor %rbx, %rsp
        xor %rbx, %rbx
        xor %rbx, %rdx
        xor %rbx, %rcx
        xor %rbx, %rax
        nop


        xor %rax, %rdi
        xor %rax, %rsi
        xor %rax, %rbp
        xor %rax, %rsp
        xor %rax, %rbx
        xor %rax, %rdx
        xor %rax, %rcx
        xor %rax, %rax
        nop

	xor %rdi, %r15
	xor %rdi, %r14
	xor %rdi, %r13
	xor %rdi, %r12
	xor %rdi, %r10
	xor %rdi, %r9
	xor %rdi, %r8
	nop

        xor %rsi, %r15
        xor %rsi, %r14
        xor %rsi, %r13
        xor %rsi, %r12
        xor %rsi, %r10
        xor %rsi, %r9
        xor %rsi, %r8
	nop
#------------------------------
        or %rdi, %rdi
        or %rsi, %rsi
        or %rbp, %rbp
        or %rsp, %rsp
        or %rbx, %rbx
        or %rdx, %rdx
        or %rcx, %rcx
        or %rax, %rax
        or %r15, %r15
        or %r14, %r14
        or %r13, %r13
        or %r12, %r12
        or %r11, %r11
        or %r10, %r10
        or %r9, %r9
        or %r8, %r8
        nop

        and %rdi, %rdi
        and %rsi, %rsi
        and %rbp, %rbp
        and %rsp, %rsp
        and %rbx, %rbx
        and %rdx, %rdx
        and %rcx, %rcx
        and %rax, %rax
        and %r15, %r15
        and %r14, %r14
        and %r13, %r13
        and %r12, %r12
        and %r11, %r11
        and %r10, %r10
        and %r9, %r9
        and %r8, %r8
        nop


        sub $5, %rdi
        sub $5, %rsi
        sub $5, %rbp
        sub $5, %rsp
        sub $5, %rbx
        sub $5, %rdx
        sub $5, %rcx
        sub $5, %rax
        sub $5, %r15
        sub $5, %r14
        sub $5, %r13
        sub $5, %r12
        sub $5, %r11
        sub $5, %r10
        sub $5, %r9
        sub $5, %r8
	nop

        add $5, %rdi
        add $5, %rsi
        add $5, %rbp
        add $5, %rsp
        add $5, %rbx
        add $5, %rdx
        add $5, %rcx
        add $5, %rax
        add $5, %r15
        add $5, %r14
        add $5, %r13
        add $5, %r12
        add $5, %r11
        add $5, %r10
        add $5, %r9
        add $5, %r8
        nop

        mov $0x1234, %rdi
        mov $0x1234, %rsi
        mov $0x1234, %rbp
        mov $0x1234, %rsp
        mov $0x1234, %rbx
        mov $0x1234, %rdx
        mov $0x1234, %rcx
        mov $0x1234, %rax
        mov $0x1234, %r15
        mov $0x1234, %r14
        mov $0x1234, %r13
        mov $0x1234, %r12
        mov $0x1234, %r11
        mov $0x1234, %r10
        mov $0x1234, %r9
        mov $0x1234, %r8
        nop

	ret
