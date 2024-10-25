# pimpl

Вам предстоит познакомиться с
идиомой [pimpl](http://en.cppreference.com/w/cpp/language/pimpl).

`pimpl` используют тогда, когда хотят достичь большей "приватности",
чем дают атрибуты доступа `private` и `protected`. `private` скрывает
поля класса от программиста, а `pimpl` --- от компилятора.

При реализации `pimpl` приходится дублировать код. Это оправдано в
больших проектах, когда вы хотите создать сильную изоляцию между
реализацией и пользователями API.

В директории есть 3 файла.
  * `bad.h` --- библиотека, функциональность которой вам очень хочется
    использовать. Одна беда --- библиотека написана на C. Вы очень не
    хотите `include`-ить этот код в собственный h файл, потому что в
    долгосрочной перспективе это приведёт к куче проблем.
  * `ugly.h`, `ugly.cpp` --- стена, которую вы должны построить, чтобы
    изолировать себя от ужасов языка C (или красоты, как посмотреть).
  * `good.cpp` --- ваш чистый код, который не догадывается о
    существовании `bad.h`.

В этой задаче можно менять только файл `ugly.cpp`.

## Примечания
 * Вам необходимо превращать коды возврата в исключения.
   В какие именно - можно увидеть в тестах.
 * В задаче жёсткие ограничения на объём памяти, который можно выделить
   при конструировании объекта класса Spline. Вам необходимо обойтись без лишних
   копирований вектора и без лишних аллокаций. Запускайте тесты на проверку памяти
   не в Asan сборке (Debug и Release подойдут).
 * На самом деле ничего ужасного конкретно в файле `bad.h` нет. Но
   поверьте, в мире достаточно кода, который стоит упрятать за pimpl.
