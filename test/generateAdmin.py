import string
import random
 
existedAccount = {}
# initializing size of string
N = 7
 
# generate account
f = open("./data/admin.txt", "a")
for i in range(10000):
    while 1:
        account = ''.join(random.choices(string.ascii_letters, k=N))
        if (account not in existedAccount):
            existedAccount[account] = 1
            break
    password = "longvu"
    f.write(' '.join([account, password]) + '\n')
f.close()

# generate transferMoney input
f = open("./input/transferMoney.txt")
for i in range(10000):
    while 1:
        fromAccount = random.choice(existedAccount.keys())
        toAccount = random.choice(existedAccount.keys())
        if (fromAccount != toAccount):
            break
    amount = "1"
    f.write(' '.join([fromAccount, toAccount, ])) 

