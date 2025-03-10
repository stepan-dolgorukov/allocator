# allocator

Блочный аллокатор памяти. Работает по принципу first-fit.

## Выпуски применявшегося ПО
+ g++: 11.4.0
+ CMake: 3.22.1
+ Doxygen: 1.9.1
+ GoogleTest: 1.11.0-3
+ ClangFormat: 14.0.0

## Сборка

```shell
git clone https://github.com/stepan-dolgorukov/allocator.git && \
cd allocator
```

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
