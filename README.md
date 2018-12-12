# urlRewriter
Take url from stdin, search it in redirect list and answer ERR or OK to stdout

Краткое описание содержимого:

project  - каталог с проектом urlRewriter, созданным в
		Eclipse IDE for C/C++ Developers Version: 2018-09 (4.9.0) Build id: 20180917-1800
		OS: Linux, v.4.15.0-39-generic, x86_64 / gtk 3.22.30 Java version: 1.8.0_191

HOSTS.zip - файл с большим количеством реально существующих url для проверки работы urlRewriter

produceFileTestStdin.py - скрипт создающий из файла HOSTS файл test.list, содержайщий url для отправки в stdin

produceInputJson.py  - скрипт создающий из файла HOSTS файл input.json, содержащий список перенаправления в формате json

test1.sh - скрипт для тестирования работы urlRewriter

tester.py - скрипт для более полного тестирования работы urlRewriter

testFileInputJson.py - скрипт для проверки файла input.json


Последовательность запуска:

1 В Eclipse выбрать File  тестировании работы urlRewriter

6 Запустив test1.sh или набрав time ./urlRewriter input.json <test.list >result.txt, проверить работу приложения urlRewriter. Результат будет сохранен в result.txt, в котором ответов ERR и OK должно быть поровну, так как ровно половина url из файла HOSTS была использована как ключи при формировании списка перенаправления.

7 Запустив python3 tester.py можно провести более полное тестирование с проверкой ответов и любыми другими тестами, которые прийдут в голову.

Конец
