#Эта утилита проверяет формат файла input.json

# Например, имеется список перенаправлений вида "ключ": "значение":
# yandex.ru: google.com
# lurkmore.to: wikipedia.org
# linux.org.ru: kernel.org
# Когда пользователь через Squid обращается на yandex.ru, запрос должен переписываться и перенаправ-
# ляться на google.com.

import json
import re


with open("input.json", 'rt') as f:
    in_data = f.read()


print(type(in_data))
print(len(in_data))

print(in_data[:40])


#try to parse
my_dict = json.loads(in_data)


print(type(my_dict))
print(len(my_dict))

#print(my_dict)
#print(list(my_dict))


regex = re.compile(
        r'^(?:http|ftp)s?://|' # http:// or https://
        r'(?:(?:[A-Z0-9_](?:[A-Z0-9_-]{0,61}[A-Z0-9_])?\.)+(?:[A-Z]{2,6}\.?|[A-Z0-9_-]{2,}\.?)|' #domain...
        r'localhost|' #localhost...
        r'\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})' # ...or ip
        r'(?::\d+)?' # optional port
        r'(?:/?|[/?]\S+)$', re.IGNORECASE)


badcount = 0;

for url in list(my_dict):
    if re.match(regex, url) is None:
        print("not good "+url)
        badcount += 1
    if len(url) > 2048:
        print("too long "+url)
        badcount += 1

print("total url's "+str(len(list(my_dict)))+' bad '+str(badcount))





