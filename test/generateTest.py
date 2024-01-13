import string
import random
from datetime import datetime, timedelta
import random

def random_date_time():
    # Generate a random date within a reasonable range
    start_date = datetime(2000, 1, 1)
    end_date = datetime(2023, 12, 31)
    random_date = start_date + timedelta(
        seconds=random.randint(0, int((end_date - start_date).total_seconds()))
    )

    # Format the date and time
    formatted_date_time = random_date.strftime("%Y-%m-%d/%H:%M:%S")

    return formatted_date_time

def random_date():
    # Generate a random date within a reasonable range
    start_date = datetime(2000, 1, 1)
    end_date = datetime(2023, 12, 31)
    random_date = start_date + timedelta(
        seconds=random.randint(0, int((end_date - start_date).total_seconds()))
    )

    # Format the date and time
    formatted_date_time = random_date.strftime("%Y-%m-%d")

    return formatted_date_time

# Example usage
existedAccount = {}
# initializing size of string
N = 7
times = 10000
# generate account
f = open("./data/user.txt", "a")
for i in range(times):
    while 1:
        account = ''.join(random.choices(string.ascii_letters, k=N))
        if (account not in existedAccount):
            existedAccount[account] = 1
            break
    password = "longvu"
    amount = "10000"
    name = account
    f.write('+'.join([account, password, amount, name]) + '\n')
f.close()

# generate transfer/withdraw/deposit history
f = open("./data/transmission.txt", 'a')
g = open("./data/deposit.txt", 'a')
h = open("./data/withdrawn.txt", 'a')
s = []
r = []
k = []
for i in range(times * 10):
    data = list(existedAccount.keys())
    fromAccount = random.choice(data)
    data.remove(fromAccount)
    toAccount = random.choice(data)
    date = random_date_time()
    amount = "1"
    s.append((date, ' '.join([fromAccount, "longvu", amount, date]) + '\n'))
    r.append((date, ' '.join([fromAccount,amount, date]) + '\n'))
    k.append((date, ' '.join([fromAccount, toAccount, amount, date]) + '\n'))
s.sort()
r.sort()
k.sort()
for i in range(len(s)):
    g.write(s[i][1])
    h.write(r[i][1])
    f.write(k[i][1])   
    # g.write(' '.join([fromAccount, "longvu", amount, date]) + '\n')
    # h.write(' '.join([fromAccount, amount, date]) + '\n')
    # f.write(' '.join([fromAccount, toAccount, amount, date]) + '\n') 
f.close()
g.close()
h.close()
# generate view[Anything] input
f = open("./input/view.txt", 'a')
for i in range(times * 10):
    fromAccount = random.choice(list(existedAccount.keys()))
    while (1):
        startDate = random_date()
        endDate = random_date()
        if (startDate < endDate):
            break
    f.write(' '.join([fromAccount, startDate, startDate]) + '\n')
f.close()

# generate transferMoney input
f = open("./input/transferMoney.txt", 'a')
for i in range(times * 10):
    while 1:
        fromAccount = random.choice(list(existedAccount.keys()))
        toAccount = random.choice(list(existedAccount.keys()))
        if (fromAccount != toAccount):
            date = random_date_time()
            break
    amount = "1"
    f.write(' '.join([fromAccount, toAccount, amount]) + '\n')
f.close()

# generate withdrawnMoney input
f = open("./input/withdrawMoney.txt", 'a')
for i in range(times * 10):
    amount = "1"
    account = random.choice(list(existedAccount.keys()))
    f.write(' '.join([amount, account]) + '\n')
f.close()

# generate depositMoney input
f = open("./input/depositMoney.txt", 'a')
for i in range(times * 10):
    amount = "1"
    account = random.choice(list(existedAccount.keys()))
    f.write(' '.join([account, "longvu", amount]) + '\n')
f.close()

# generate checkBalance input
f = open("./input/checkBalance.txt", 'a')
for i in range(times * 10):
    amount = "1"
    account = random.choice(list(existedAccount.keys()))
    f.write(account + '\n')

