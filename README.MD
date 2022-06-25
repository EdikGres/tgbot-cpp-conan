## apt get install libmysqlclient-dev
message->from->id - узнать ид юзера
message->chat->id - то же самое надо изучать код чтобы найти разницу

ALTER TABLE users AUTO_INCREMENT = 1 - сброс автоинкремента ид
mysql_errno() - узнать ид ошибки


запрос SELECT * FROM users WHERE id = ...;

SELECT Name FROM users WHERE Name LIKE 'ed%' - примерный поиск, ed....