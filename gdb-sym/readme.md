# gdb-sym

В этой задаче вам предстоит поработать с [GDB](https://www.gnu.org/software/gdb/) (GNU debugger)
и поковырять бинарники (как собранные с дебажными символами, так и без оных), чтобы
извлечь из них некоторую информацию.

Если вы считаете, что для задачи `xyz` ответ `123`, то напишите его в файл `xyz/flag.txt`.
Также заполните файл `xyz/report.md`, описав в нём как вы получили ответ.

### Задачи

1. `gdb-sym`: Вам дана программа `with_symbols`, собранная без оптимизации и с дебажными
символами.
__Ответ__: значение переменной `x` после 10000-ой итерации цикла в функции `f()`.
1. `gdb-nosym`: На этот раз программа (`without_symbols`) собрана с оптимизацией и без дебажной информации.
Придётся немного освежить знание языка ассемблера.
__Ответ__: значение регистра `rax` после 10000-ой итерации цикла (прямо перед `jne`) в функции `f()`.
1. `coredump`: Дана программа `with_core`. Вам предстоит покопаться в кишках стандартного контейнера,
используя оставшийся после выполнения программы сore dump. К сожалению, сам контейнер частично побился
из-за того же бага, что уронил программу, но гарантируется, что данные извлечь можно (см. [ссылку](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/hashtable.h)).
__Ответ__: `hashmap[8888][1]`.
1. `flaky`: Эта программа пытается посчитать ответ для вас, но с очень большой вероятностью падает
на полпути.
__Ответ__: то, что напечатает программа, если добежит до конца.

### Используем docker

Все бинарники были собраны на ubuntu 18.04. Если у вас стоит
достаточно свежий дистрибутив linux, то задачи можно решать на нём.

Для тех, у кого OSX, Windows или какой-то странный linux, мы собрали
докер образ с правильным окружением.

```bash
$ docker run --privileged -it darthslon/shad-cpp:gdb /bin/bash
root@a9bc09439be8:/# cd opt
root@a9bc09439be8:/opt# ls
coredump  flaky  with_core  with_symbols  without_symbols
```

### Полезные ссылки
* https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/hashtable.h
* https://www.gnu.org/software/gdb/
* https://en.wikibooks.org/wiki/X86_Assembly/GAS_Syntax
