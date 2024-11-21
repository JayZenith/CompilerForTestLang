global _start
_start:
    char DB 'E'
    MOV rax, [char]
    push rax
    mov rax, 60
    pop rdi
    syscall
    mov rax, 60
    mov rdi, 0
    syscall
