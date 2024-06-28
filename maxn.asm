call main
done: HALT

main:
    adc 1
    stl 0
    adj -2
    ldc num
    ldnl 0
    stl 0
    adj 1
    ldc 0
    stl 0
    adj -1
    loop:
        ldl 0
        ldl 1
        sub
        brz exit
        ldl 1
        ldc array
        add
        ldnl 0
        stl -1
        ldc cur
        ldnl 0
        ldl -1
        sub
        brlz swap
        ldc 1
        sp2a
        ldnl 1
        add
        stl 1
        br loop
    swap:
        ldl 1
        ldc array
        add
        ldnl 0
        ldc cur
        stnl 0
        ldc 1
        sp2a
        ldnl 1
        add
        stl 1
        br loop
    exit:
        adj 2
        ldl 0
        br done

cur: data 0
num: data 7
array: data 6
data 7
data 100
data 85
data 53
data 42
data 117