# Webserv
School 21 (Ecole 42) learning project about HTTP and web connection based on Berkeley sockets.

## How to run project
```
Clone and build project:
1. git clone https://github.com/auhoris/webserv.git
2. cd webserv
3. make webserv
```

## Usage
There is config file like in Ngnix. You can use provided test config or created by yourself.
Webserv supports only basic parameters:
- host:port;
- error pages;
- locations with
    - root
    - autoinedx (on/off)
    - index pages
    - methods (GET, POST, DELETE)


```
1. ./webserv config_nginx
2. Connect from browser or any web client to server
```

## Team
Intra logins:
- auhoris;
- cmarguer;
- skitsch.

## TODO
- [ ] Странички ошибок, их вывод на проблему; -> skitsch
- [ ] Обработать длинное тело post-запроса; -> skitsch
- [ ] setup routes in a server to different directories; -> cmarguer : skitsch
- [ ] setup a default file to search for if you ask for a directory; -> skitsch
- [ ] попытаться удалить что-то с разрешением и без; -> skitsch
- [ ] upload some file to the server and get it back; -> 
- [x] убрать errno; -> auhoris
- [ ] Try to list a directory; -> cmarguer
- [ ] Try a redirected URL; -> cmarguer
- [x] Сделать запись ответа в файл и отправлять ответ из файла клиенту; -> auhoris
- [ ] Обработать chunked request; -> auhoris : cmarguer
- [ ] Запускать разные(?) CGI в зависимости от расширения файла; -> ???
- [ ] Забиндить имя серверу; -> auhoris
- [ ] Разобраься с CGI; -> всем
