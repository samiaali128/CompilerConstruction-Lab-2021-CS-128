section .data
    t35_do_while_start dd 0
    !t40 dd 0
    t40 dd 0
    t39 dd 0
    t37 dd 0
    t36 dd 0
    t34 dd 0
    true dd 0
    t33_next_case dd 0
    t30_next_case dd 0
    !t29 dd 0
    t29 dd 0
    L22 dd 0
    t26 dd 0
    t4 dd 0
    t13 dd 0
    t2 dd 0
    L1 dd 0
    t41_do_while_end dd 0
    Samia dd 0
    count dd 0
    result dd 0
    t8 dd 0
    == dd 0
    !t32 dd 0
    > dd 0
    t21 dd 0
    false dd 0
    <= dd 0
    20.09774 dd 0
    != dd 0
    x dd 0
    t6 dd 0
    t3 dd 0
    L17 dd 0
    Liaqat dd 0
    < dd 0
    b dd 0
    t20 dd 0
    t1 dd 0
    y dd 0
    pi dd 0
    name dd 0
    t25 dd 0
    t12 dd 0
    t5 dd 0
    flag dd 0
    L24 dd 0
    sum dd 0
    price dd 0
    a dd 0
    t10 dd 0
    3.14e+2 dd 0
    L2 dd 0
    t7 dd 0
    t9 dd 0
    t11 dd 0
    t27_switch_end dd 0
    t0 dd 0
    t14 dd 0
    L3 dd 0
    t15 dd 0
    t19 dd 0
    L16 dd 0
    t18 dd 0
    t32 dd 0
    i dd 0

section .text
    global _start
_start:
    mov dword [x], 10
    mov dword [y], 20
    mov dword [sum], 40
    mov dword [price], 20.09774
    mov eax, [3.14]
    add eax, []
    mov dword [pi], eax
    mov dword [name], Samia Liaqat
    mov dword [flag], false
    mov dword [t0], x == 10
    mov dword [t1], t0
    cmp dword [t1 ], 1
    je L1
    jmp L2

L1:
    mov dword [y], 999

L2:
    mov dword [t2], x != y
    mov dword [t3], t2
    cmp dword [t3 ], 1
    je L1
    jmp L2

L1:
    mov eax, [price]
    imul eax, [price]
    mov dword [t4], eax
    mov dword [price], t4

L2:
    mov dword [t5], price <= 12
    mov dword [t6], t5
    cmp dword [t6 ], 1
    je L1
    jmp L2

L1:
    mov dword [name], Samia Liaqat

L2:
    mov dword [count], 25
    mov dword [a], 10
    mov dword [b], 20
    mov dword [t7], a < 10
    mov dword [t8], t7
    cmp dword [t8 ], 1
    je L1
    jmp L2

L1:
    mov eax, [a]
    add eax, [b]
    mov dword [t9], eax
    mov eax, [t9]
    add eax, [sum]
    mov dword [t10], eax
    mov dword [sum], t10
    jmp L3

L2:
    mov eax, [x]
    add eax, [y]
    mov dword [t11], eax
    mov eax, [t11]
    add eax, [3]
    mov dword [t12], eax
    mov dword [sum], t12

L3:
    mov dword [t13], x > y
    mov dword [t14], t13
    cmp dword [r t14 ], 1
    je L1
    jmp L2

L1:
    mov eax, [20]
    add eax, [sum]
    mov dword [t15], eax
    mov dword [x], t15
    jmp L3

L2:
    mov dword [sum], y

L3:
    jmp L16

L16:
    mov dword [t18], sum > 5
    mov dword [t19], t18
    cmp dword [t19 ], 1
    je L17
    jmp L16
    mov eax, [x]
    add eax, [30]
    mov dword [t20], eax
    mov dword [x], t20
    mov eax, [sum]
    add eax, [1]
    mov dword [t21], eax
    mov dword [sum], t21
    jmp L16

L17:
    mov dword [i], 0

L22:
    mov dword [t25], i > 5
    cmp dword [t25 ], 1
    je L24
    mov eax, [i]
    add eax, [1]
    mov dword [t26], eax
    mov dword [i], t26

L23:
    mov dword [i], t26
    jmp L22

L24:
    mov dword [t29], x == 1
    cmp dword [!t29 ], 1
    je t30_next_case
    mov dword [result], true
    jmp t27_switch_end

t30_next_case:
    mov dword [t32], x == 2
    cmp dword [!t32 ], 1
    je t33_next_case
    mov dword [result], false
    jmp t27_switch_end

t33_next_case:
    mov dword [t34], x > 0
    mov dword [result], t34

t27_switch_end:

t35_do_while_start:
    mov eax, [x]
    add eax, [1]
    mov dword [t36], eax
    mov dword [x], t36
    mov eax, [y]
    imul eax, [2]
    mov dword [t37], eax
    mov dword [y], t37

t38_do_while_condition:
    mov dword [t39], x < 10
    mov dword [t40], t39
    cmp dword [!t40 ], 1
    je t41_do_while_end
    jmp t35_do_while_start

t41_do_while_end:
