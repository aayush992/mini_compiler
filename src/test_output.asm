section .data
true_val: dd 1
false_val: dd 0

section .text
global main

main:
    push ebp
    mov ebp, esp
arithmetic_test:
    mov dword [ebp-4], 42
    mov dword [ebp-4], 10
    mov eax, [ebp-4]
    add eax, [ebp-8]
    mov [ebp-12], eax
    mov eax, [ebp-4]
    sub eax, [ebp-8]
    mov [ebp-12], eax
    mov eax, [ebp-4]
    imul eax, [ebp-8]
    mov [ebp-12], eax
    push ebx
    mov eax, [ebp-4]
    cdq
    mov ebx, [ebp-8]
    idiv ebx
    mov [ebp-12], eax
    pop ebx
boolean_test:
    mov dword [ebp-4], 1
    mov dword [ebp-4], 0
    mov eax, [ebp-4]
    mov ebx, [ebp-8]
    and eax, ebx
    mov [ebp-12], eax
    mov eax, [ebp-4]
    mov ebx, [ebp-8]
    or eax, ebx
    mov [ebp-12], eax
    mov eax, [ebp-4]
    mov ebx, [ebp-8]
    xor eax, 1
    mov [ebp-12], eax
comparison_test:
    mov dword [ebp-4], 5
    mov dword [ebp-4], 10
    mov eax, [ebp-4]
    cmp eax, [ebp-8]
    je .L0_true
    mov dword [ebp-12], 0
    jmp .L1_end
.L0_true:
    mov dword [ebp-12], 1
.L1_end:
    mov eax, [ebp-4]
    cmp eax, [ebp-8]
    jne .L2_true
    mov dword [ebp-12], 0
    jmp .L3_end
.L2_true:
    mov dword [ebp-12], 1
.L3_end:
    mov eax, [ebp-4]
    cmp eax, [ebp-8]
    jl .L4_true
    mov dword [ebp-12], 0
    jmp .L5_end
.L4_true:
    mov dword [ebp-12], 1
.L5_end:
    mov eax, [ebp-4]
    cmp eax, [ebp-8]
    jg .L6_true
    mov dword [ebp-12], 0
    jmp .L7_end
.L6_true:
    mov dword [ebp-12], 1
.L7_end:
entry:
    mov dword [ebp-4], 1
    mov eax, [ebp-4]
    test eax, eax
    jz else
then:
    mov dword [ebp-4], 1
    jmp exit
else:
    mov dword [ebp-4], 0
exit:
    mov esp, ebp
    pop ebp
    ret
