# Device-Mapper-Proxy

## User Guide

1. Клонируем репозиторий
```c
git clone https://github.com/adik-alone/Device-Mapper-Proxy.git
```
2. Собираем модуль
```bash
make
```
3. Выдаём права на исполнение скриптов
```bash
chmod u+x build.sh test.sh clean.sh w_test.sh r_test.sh
```
4. Загружаем модуль
```bash
sudo ./build.sh
```
5. Тестируем модуль
```bash
sudo ./test.sh
sudo ./w_test.sh
sudo ./r_test.sh
```
6. Выгружаем модуль
```bash
sudo ./clean.sh
```
