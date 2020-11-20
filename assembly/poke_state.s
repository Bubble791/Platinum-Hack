.text
.align 2
.thumb
.thumb_func

.global ev_screen
ev_screen:
mov r1, #0x20
ldr r5, [r2,#0x4C]
mov r6, r5
tst r6, r1
beq check
sub r1, #0x21
bl PageChange
mov r0, #2
pop {r4-r6,pc}

/*L Button*/
check:
mov r1, #0x2
lsl r1, r1, #8
tst r6, r1
beq check2
sub r1, r3, #3
ldrsb r1, [r4, r1]
cmp r1, #2
bne check2
mov r1, #1
bl ChangeState
mov r0, #2
pop {r4-r6,pc}

/*R Button*/
check2:
mov r1, #0x1
lsl r1, r1, #8
tst r6, r1
beq check3
sub r1, r3, #3
ldrsb r1, [r4, r1]
cmp r1, #2
bne check3
mov r1, #2
bl ChangeState
mov r0, #2
pop {r4-r6,pc}

/*Select Button*/
check3:
mov r1, #0x4
tst r6, r1
beq return
sub r1, r3, #3
ldrsb r1, [r4, r1]
cmp r1, #2
bne return
mov r1, #0
bl ChangeState
mov r0, #2
pop {r4-r6,pc}


return:
ldr r1, =0x0208C1B8 + 1
bx r1

PageChange:
ldr r2, =0x0208D01C + 1
bx r2