import random


with open("HOSTS", 'rt') as f:
    myinput = f.readlines()

lines = []
for line in myinput:
    line = line.strip()
    if len(line) == 0:
        continue
    if line[0] == '#':
        continue
    lines.append(line)

urls = [line.split()[1] for line in lines]

random.shuffle(urls)
print('Found '+str(len(urls))+" url's for test")


with open("test.list",'wt') as f:
    for url in urls:
        f.write(url+'\n')


print('File test.list created')

