import random
import matplotlib.pyplot as plt

physical_size = 16 * 1024
address_space_size = 1024

def random_generate_va(num):
    ret_list = [0 for i in range(address_space_size)]
    for i in range(num):
        random.seed(i)
        for limit in range(address_space_size):
            va = address_space_size * random.random()
            if (va < limit):
                ret_list[limit] += 1
    return ret_list

num = 1000
ret_list = random_generate_va(num)
fraction_list = [(tmp / num) for tmp in ret_list]
address_list = [i for i in range(address_space_size)]

plt.plot(address_list, fraction_list)
plt.ylim(0, 1)
plt.xlabel('limit(bound) size')
plt.ylabel('The fraction of va valid')
plt.title('Fraction of VA valid with limit(bound) size')
plt.savefig('15-5-fraction.png')
plt.show()
