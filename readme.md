# allocator

Блочный аллокатор памяти. Работает по принципу first-fit.

## Выпуски применявшегося ПО
+ g++: 11.4.0
+ CMake: 3.22.1
+ Doxygen: 1.9.1
+ GoogleTest: 1.11.0 (автоматически устанавливается при сборке)
+ ClangFormat: 14.0.0
+ TexLive: 2021.20220204-1

## Сборка

```shell
git clone https://github.com/stepan-dolgorukov/allocator.git && \
cd ./allocator/ && \
cmake -S ./ -B ./build/ && \
cd ./build/ && \
make solution
```

./liballocator.so &mdash; модуль, содержащий реализацию аллокатора. \
./test &mdash; тестирующая программа. \
./documentation/latex/refman.pdf &mdash; документация.

## Форматирование исходного текста

Применяется программа &laquo;ClangFormat&raquo;. Команда для установки:
```shell
apt install clang-format
```

В репозитории есть shell-программа &laquo;format-source-text.sh&raquo;, которая вызывает программу &laquo;clang-format&raquo; с нужными аргументами. Выдача владельцу права выполнения shell-программы:
```shell
chmod u+x ./format-source-text.sh
```

Запуск:
```shell
./format-source-text.sh
```
