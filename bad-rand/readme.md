# Bad Rand

Это задача типа [crashme](../docs/crash_readme.md).

Исходный код находится в файле `run.cpp`, программа собирается с помощью команды:
```sh
$ g++-12 -O3 run.cpp -o bad_rand
```

Вам нужно заполнить файл input.txt, в проверяющей системе он будет использоваться в качестве входа для программы
```sh
$ ./bad_rand <seed> < input.txt
```
где `<seed>` - некоторое число

Обратите внимание, что исполняемый файл собирается в Linux,
поэтому вас интересует решение этой задачи для данной ОС (т.е. вас интересует библиотека glibc).
Под macOS решение будет другим.

Вы также должны отправить генератор, который вы использовали для получения решения.
Для этого пишите код генератора в файле solution/main.cpp.
