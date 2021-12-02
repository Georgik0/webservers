# Webservers

## ***Multithreading asynchronous server***
_Сервер работет следующим образом:_
- _главный процесс запускает пул воркеров (потоков), которые начинают ожидають своих клиентов_
- _как только подключается клиент, select отлавливает подключение, главный процесс дает сигнал и клиента забирает один из воркеров_
- _чтение и запись каждого врокера работает в неблокирующем режиме_
```sh
# собираем сервер
make server_multithreading_select
# появился исполняемый файл ./server
```
_Как запускаем:_
```sh
./server ip port количество_воркеров
# например
./server 127.0.0.1 8000 10
```


## ***Multithreading echo server***
После каждого нового подключения создается поток, в котором будет обрабатываться клиент
```sh
#соберет многопоточный сервер
make server_multithreading_echo
# запустить
./server 127.0.0.1 8000
```

## ***Select server***
Сервер на одном select-е
```sh
#соберет сервер на select-е
make server_select
# запустить
./server 127.0.0.1 8000
```

<!-- ## ***data_time_server.cpp***
```sh
#соберет сервер на одном потоке
make server_multithreading_echo
``` -->

