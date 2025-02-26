# allocator

Блочный аллокатор памяти. Работает по принципу first-fit.

## Выпуски применявшегося ПО
+ g++: 11.4.0
+ CMake: 3.22.1
+ Doxygen: 1.9.1
+ GoogleTest: 1.11.0-3

## Сборка

### Сборка тестирующей программы

Необходимо установить библиотеку &laquo;GoogleTest&raquo;. Команда установки:
```shell
apt install libgtest-dev
```

Сборка исполняемого файла:
```shell
mkdir ./build && \
cd ./build/ && \
cmake ../ && \
make test
```

### Сборка документации

Настроено создание документации в формате LaTeX.

Команда запуска генерации документации:
```shell
doxygen ./Doxyfile
```

Документация будет находиться в файле &laquo;documentation/latex/refman.tex&raquo;.

Для сборки PDF-документации, нужно установить пакет &laquo;texlive&raquo;.

Команда установки пакета &laquo;texlive&raquo;:
```shell
apt install texlive-full
```

Сборка документации в PDF.

```
cd ./documentation/latex/ && \
make
```

&laquo;./refman.pdf&raquo; &mdash; файл с документацией.
