Hello, World example in Marsh assembly
5/29/2020 moxniso

$START
DUMP
MOVI Y, 40
PUSH Y
POP S

MOVC X, ~
SYSCALL
MOVC X, H
SYSCALL
MOVC X, e
SYSCALL
MOVC X, l
SYSCALL
SYSCALL
MOVC X, o
SYSCALL
MOVC X, ~
SYSCALL

DUMP
END
