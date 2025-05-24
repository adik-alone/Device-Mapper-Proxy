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
3. Выдать права на исполнение
```bash
chmod u+x build.sh test.sh clean.sh w_test.sh r_test.sh
```
3. Сетапим модуль
```bash
sudo ./build.sh
```
4. Тестирование
```bash
sudo ./test.sh
sudo ./w_test.sh
sudo ./r_test.sh
```
5. Выгрузка
```bash
sudo ./clean.sh
```
