import math

amount_of_numbers = 0

while True:
    try:
        amount_of_numbers = int(input("how many numbers to check? :"))
        break
    except:
        print("invalid input, try again")

wrong = False
cubed_arr = []
for i in range(1, amount_of_numbers + 1):
    cubed = (i * i * i)
    j = i - 1
    while True:
        if j == 0:
            break
        cubed += j*j*j
        j -= 1
    result1 = math.sqrt(cubed)
    result2 = 0.0
    j = i
    while True:
        result2 += j
        j -= 1
        if j == 0:
            break
    if result1 != result2:
        print(result1, " != ", result2)
        wrong = True
        break

if not wrong:
    print("everything matched")