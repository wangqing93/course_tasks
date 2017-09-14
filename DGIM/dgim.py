#encoding=UTF-8
import random

#最大值为2^8所以需要9个桶队列
m = 9
currentTimeStamp = -1
N = 100000000

AllbucketList = []
for c in range(m):
    AllbucketList.append([])

def removeOldestValue(bucketList):
    if (len(bucketList) > 0):
        leftBarrel = bucketList[0]
        if (currentTimeStamp - leftBarrel[1] + N) % N == 0:
            bucketList.remove(leftBarrel)


def mergeBuckets(bucketList):
    still = True
    p = -1   # 最后一个和倒数第二个；最后一个和倒数第三个
    while still:
        still = False
        if abs(p - 2) > len(bucketList):
            break
        if bucketList[p][0] == bucketList[p-1][0] & bucketList[p][0] == bucketList[p-2][0]:
            bucketList[p-2][0] = bucketList[p-1][0] + bucketList[p-2][0]
            bucketList[p-2][1] = bucketList[p-1][1]
            bucketList.remove(bucketList[p-1])
            p = p - 1
            still = True

def generateBuckets(num):
    global currentTimeStamp
    currentTimeStamp = (currentTimeStamp + 1) % N
    for c in range(m):
        removeOldestValue(AllbucketList[c])
    for c in range(m):
        bucketList = AllbucketList[c]
        bit = num % 2
        num = num / 2
        if bit == 1:
            barrel = [1, currentTimeStamp]
            bucketList.append(barrel)
            mergeBuckets(bucketList)

def calcResult(k):
    result = 0
    for c in range(m):
        s = 0
        bucketList = AllbucketList[c]
        for i in range(1, len(bucketList) + 1):
            barrel = bucketList[-i]  #从最近的时间开始，满足条件的汇总相加
            if ((currentTimeStamp - barrel[1] + N) % N) <= k:
                s = s + barrel[0]
            else:
                if i > 1:
                    s = s - (bucketList[1 - i][0] / 2) #上面的那个值肯定不行了，但是下面那个值可以，但有可能时使用了下下面的时间戳然后可以了，所以要减掉一半的值
                break
        print(str(s) + ' * 2^' + str(c),end='')
        if c != (m - 1):
            print( ' + ', end=''),
    print('')

#先产生100，000，000个随机数
for i in range(100000000):
    if i % 1000000 == 0:
        print(i)
    generateBuckets(random.randint(1, 256))

for i in range(5):
    #每新产生1000000需要重新计算。
    for j in range(1000000):
        generateBuckets(random.randint(1, 256))
    calcResult(50000000)

