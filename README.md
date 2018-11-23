# urlRewriter
Take url from stdin, search it in redirect list and answer ERR or OK to stdout
краткое описание содержимого:

project  - каталог с проектом urlRewriter, созданным в
		Eclipse IDE for C/C++ Developers Version: 2018-09 (4.9.0) Build id: 20180917-1800
		OS: Linux, v.4.15.0-39-generic, x86_64 / gtk 3.22.30 Java version: 1.8.0_191

HOSTS.zip - файл с большим количеством реально существующих url для проверки работы urlRewriter

produceFileTestStdin.py - скрипт создающий из файла HOSTS файл test.list, содержайщий url для отправки в stdin

produceInputJson.py  - скрипт создающий из файла HOSTS файл input.json, содержащий список перенаправления в формате json

test1.sh - скрипт для тестирования работы urlRewriter

tester.py - скрипт для более полного тестирования работы urlRewriter

testFileInputJson.py - скрипт для проверки файла input.json



последовательность запуска:

1 В Eclipse выбрать File - Open Projects from File System . Импортировать каталог project, при необходимости Build All. Скопировать скомпилированное приложение urlRewriter в каталог result.

2 Распаковать HOSTS.zip. При помощи python3 produceInputJson.py создать файл input.json, в котором будет список перенаправления в виде словаря, сохраненного в формате json, состоящего из всех найденных url из HOSTS. Первая половина url используется как ключи словаря, вторая половина как значения. Этот файл для работы urlRewriter надо указывать в качестве первого аргумента в командной строке.

3 Запустить ./urlRewriter input.json , проверить работу приложения. Набирая в консоли адреса можно увидеть ERR, если такого адреса нет в списке перенаправления, или OK rewrite-url=... если адрес в списке есть. (local, localhost, fileserver, sql-po). все события пишутся в syslog

4 Если послать приложению сигнал USR1 или изменить содержание файла input.json, то произойдет перезагрузка списка перенаправления. Для этого нужно найти PID в syslog или ps -a | grep urlRewriter, а затем послать сигнал командой kill -s USR1 <найденный pid>. или любым редактором (который не зависнет) отредактировать и сохранить файл указанный как аргумент при запуске (input.json).

5 При помощи python3 produceFileTestStdin.py создать файл test.list, в котором будут все найденные url из HOSTS по одному в строке. Этот файл будет использован для отправки на stdin при тестировании работы urlRewriter

6 Запустив test1.sh или набрав time ./urlRewriter input.json <test.list >result.txt, проверить работу приложения urlRewriter. Результат будет сохранен в result.txt, в котором ответов ERR и OK должно быть поровну, так как ровно половина url из файла HOSTS была использована как ключи при формировании списка перенаправления.

7 Запустив python3 tester.py можно провести более полное тестирование с проверкой ответов и любыми другими тестами, которые прийдут в голову.


