LOAD 2
STORE y
LOAD 1
STORE x
LOAD 1
STORE z
LOAD 0
STORE sum
LOAD 5
STORE c0
LOAD 10
STORE c1
WRITE c1
LOAD y
SUB x
BRPOS L0
LOAD 10000
STORE z
LOAD 8008
STORE y
L0: NOOP
READ y
L1: NOOP
LOAD y
SUB z
BRNEG L2
LOAD sum
ADD z
STORE T0
LOAD T0
STORE sum
LOAD z
ADD 1
STORE T1
LOAD T1
STORE x
BR L1
L2: NOOP
WRITE x
STOP
y 0
x 0
z 0
sum 0
c0 0
c1 0
T0 0
T1 0
