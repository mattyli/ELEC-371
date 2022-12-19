#### START PSEUDOCODE ####
SelectLarger(src_list1 r4, src_list2 r3, destList r2, N r5)::
    for int i in range N:
        if list1[i] > list2[i]:
            destList[i] = list1[i]:
        else:
            destList[i] = list2[i]
        endif
    endfor

    return destList

# reserving the amount of space in memory for all the lists
.skip 16    X
.skip 16    Y
.skip 16    Z

# word sized = 4 bytes
# 4 bytes * 4 elements = reserve 16 bytes
#### END PSEUDOCODE ####

#### START SUBROUTINE ####
select_larger:
    subi    sp, sp, 24
    stw r2, 16(sp)          # dest_list
    stw r3, 12(sp)          # list_1    
    stw r4, 8(sp)           # list_2
    stw r5, 4(sp)           # N
    stw r7, 0(sp)           # for indexing list_1
    stw r8, 20(sp)          # for indexing list_2

sl_loop:
    ldw r7, 0(r3)           # first element of list_1
    ldw r8, 0(r4)           # first element of list_2

sl_if:
    bgt r7, r8, sl_else     # if list1[i] > list2[i]
    stw r8, 0(r2)
    br sl_end

sl_else:
    stw r8, 0(r2)           # storing list2[i] into r2 (because it is larger)

sl_end:
    addi r3, r3, 4          # incrementing list1 pointer
    addi r4, r4, 4          # incrementing list2 pointer
    subi r5, r5, 1

    bne r5, r0, sl_loop     # if still have elements, keep looping

    # restore the stack
ret

