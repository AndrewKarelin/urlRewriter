
# эта утилита создает файл формата json заданного вида (см. описание ниже) из файла HOSTS

# Например, имеется список перенаправлений вида "ключ": "значение":
# yandex.ru: google.com
# lurkmore.to: wikipedia.org
# linux.org.ru: kernel.org
# Когда пользователь через Squid обращается на yandex.ru, запрос должен переписываться и перенаправ-
# ляться на google.com.

import json
import sys

input = []

with open("HOSTS",'rt') as f:
    input = f.readlines()


#print(input[:40])

lines = []

for line in input:
    line = line.strip()
    if len(line) == 0:
        continue
    if line[0] == '#':
        continue
    lines.append(line)

#print(lines[:40])

urls = [line.split()[1] for line in lines]


#print(urls[:40])

#for limit size of output file 
#urls = urls[:4000]

half = len(urls)//2

urls1 = urls[:half]
urls2 = urls[half:-1]


result = {url1: url2 for (url1, url2) in zip(urls1, urls2)}


with open("input.json",'wt') as f:
    f.writelines(json.dumps(result))


print('file input.json created with '+str(half)+' records')

