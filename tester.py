import subprocess
from fcntl import fcntl, F_GETFL, F_SETFL
from os import O_NONBLOCK, read
from time import sleep
import signal
import random
import json


with open("test.list", 'rt') as f:
    urls = f.readlines()

print('Found '+str(len(urls))+" url's for test")


with open('input.json', 'rt') as f:
    mydict = json.loads(f.read())

#print(type(mydict))
#print(len(mydict))

print('Found redirection list with '+str(len(mydict))+" records")


args = ['./urlRewriter', 'input.json']
p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,bufsize=1, universal_newlines=True)

counter = 0
right_ans = 'ERR'

for url in urls:
    p.stdin.write(url)

    url = url[:-1] # strip '\n'
    if url not in mydict:
 #       print('send '+url+' answer in ERR')
        right_ans = 'ERR'
    else:
 #       print('send '+url+' answer is '+mydict[url])
        right_ans = 'OK'

    ans = p.stdout.readline()
   # print(ans)

    if right_ans not in ans:
        print('--------------------------------------------------------------------------------------')
        print('message from application '+ans)
        if url not in mydict:
            print('send '+url+' right answer is ERR')
        else:
            print('send '+url+' right answer is '+mydict[url])

        print('--------------------------------------------------------------------------------------')

    counter = counter +1
    if counter % 1000 == 0:
        print(str(counter)+" records done \r",end='')
        p.send_signal(signal.SIGUSR1)

