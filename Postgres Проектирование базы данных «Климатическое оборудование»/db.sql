CREATE DATABASE climEquip;
___________________________________________________________________________________________создание таблиц
CREATE TABLE main (id SERIAL PRIMARY KEY, id_Модели int, Дата_выпуска date, Цена int, Дата_продажи date, Наличие_брака boolean);

CREATE TABLE models (id SERIAL PRIMARY KEY, Модель varchar(40), id_Каталога int, id_Поставщика int);

CREATE TABLE catalogs (id SERIAL PRIMARY KEY, Каталог varchar(40));

CREATE TABLE providers (id SERIAL PRIMARY KEY, Фирма varchar(40), id_Страны int);

CREATE TABLE countries (id SERIAL PRIMARY KEY, Страна varchar(40));

___________________________________________________________________________________________связывание и ограничения

ALTER TABLE providers add constraint countriesfk foreign key (id_Страны) references
countries (id) on update no action on delete cascade;

ALTER TABLE models add constraint firmsfk foreign key (id_Поставщика) references providers (id)
match simple on update cascade on delete no action;

ALTER TABLE models add constraint catalogsfk foreign key (id_Каталога) references catalogs (id)
on update no action on delete no action;

ALTER TABLE main add constraint modelsfk foreign key (id_Модели) references
models (id) on update no action on delete no action;

ALTER TABLE main add check (Дата_выпуска < Дата_продажи);

ALTER TABLE main add check (Цена > 0);
____________________________________________________________________________________________функции
1) Для каждого вида КО указать сведения о нем (наименование, дату выпуска, поставщик, цена, дата продажи и т.п.).
CREATE OR REPLACE FUNCTION showEquip() RETURNS TABLE 
(id int, Поставщик varchar(40), Модель varchar(40), Дата_выпуска date, Дата_продажи date, Цена int)
AS $$ BEGIN RETURN QUERY
SELECT main.id, providers.Фирма, models.Модель, main.Дата_выпуска, main.Дата_продажи, main.Цена
FROM main, providers, models
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id); 
return; end; $$ language 'plpgsql';



2) Для каждого КО выдать список, отсортированный:
	2.1) По дате выпуска 
	CREATE OR REPLACE FUNCTION sort_release_date() RETURNS TABLE 
	(id int, Поставщик varchar(40), Модель varchar(40), Дата_выпуска date, Дата_продажи date, Цена int)
	AS $$ BEGIN RETURN QUERY
	SELECT main.id, providers.Фирма, models.Модель, main.Дата_выпуска, main.Дата_продажи, main.Цена
	FROM main, providers, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id) 
	ORDER BY Дата_выпуска; 
	return; end; $$ language 'plpgsql';

	2.2) В алфавитном порядке по поставщику
	CREATE OR REPLACE FUNCTION sort_provider() RETURNS TABLE 
	(id int, Поставщик varchar(40), Модель varchar(40), Дата_выпуска date, Дата_продажи date, Цена int)
	AS $$ BEGIN RETURN QUERY
	SELECT main.id, providers.Фирма, models.Модель, main.Дата_выпуска, main.Дата_продажи, main.Цена
	FROM main, providers, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id) 
	ORDER BY providers.Фирма; 
	return; end; $$ language 'plpgsql';

	2.3) По стоимости
	CREATE OR REPLACE FUNCTION sort_price() RETURNS TABLE 
	(id int, Поставщик varchar(40), Модель varchar(40), Дата_выпуска date, Дата_продажи date, Цена int)
	AS $$ BEGIN RETURN QUERY
	SELECT main.id, providers.Фирма, models.Модель, main.Дата_выпуска, main.Дата_продажи, main.Цена
	FROM main, providers, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id) 
	ORDER BY main.Цена; 
	return; end; $$ language 'plpgsql';
	
	2.4) По дате продажи
	CREATE OR REPLACE FUNCTION sort_sale_date() RETURNS TABLE 
	(id int, Поставщик varchar(40), Модель varchar(40), Дата_выпуска date, Дата_продажи date, Цена int)
	AS $$ BEGIN RETURN QUERY 
	SELECT main.id, providers.Фирма, models.Модель, main.Дата_выпуска, main.Дата_продажи, main.Цена
	FROM main, providers, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id) 
	ORDER BY main.Дата_продажи; 
	return; end; $$ language 'plpgsql';

3) Найти КО:
	3.1) Самое дорогое
	CREATE OR REPLACE FUNCTION max_price() RETURNS TABLE
	(Фирма varchar(40), Каталог varchar(40), Модель varchar(40), Цена int) 
	AS $$ BEGIN RETURN QUERY 
	SELECT providers.Фирма, catalogs.Каталог, models.Модель, main.Цена
	FROM providers, catalogs, models, main
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id
	AND main.Цена = (SELECT MAX(main.Цена) FROM main));
	return; end; $$ language 'plpgsql';

	3.2) Самое дешевое
	CREATE OR REPLACE FUNCTION min_price() RETURNS TABLE
	(Фирма varchar(40), Каталог varchar(40), Модель varchar(40), Цена int) 
	AS $$ BEGIN RETURN QUERY 
	SELECT providers.Фирма, catalogs.Каталог, models.Модель, main.Цена
	FROM providers, catalogs, models, main
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id
	AND main.Цена = (SELECT MIN(main.Цена) FROM main));
	return; end; $$ language 'plpgsql';

	3.3) Средняя стоимость в целом
	CREATE OR REPLACE FUNCTION avg_price() RETURNS numeric 
	AS $$ DECLARE n numeric; 
	BEGIN n:=(SELECT AVG(main.Цена) FROM main);
	return n; end; $$ language 'plpgsql';

	3.4) Средняя стоимость по типам
	CREATE OR REPLACE FUNCTION avg_price_in() RETURNS TABLE
	(Каталог varchar(40), Средняя_стоимость numeric)
	AS $$ BEGIN RETURN QUERY
	SELECT catalogs.Каталог AS Каталог, AVG(main.Цена) AS Средняя_стоимость
	FROM catalogs, models, main
	WHERE(main.id_Модели = models.id AND models.id_Каталога = catalogs.id) 
	GROUP BY catalogs.Каталог;
	return; end; $$ language 'plpgsql';

4)Найти КО с ценой в заданных пределах (предусмотреть ввод цены с клавиатуры)
	4.1) По каждому виду
	CREATE OR REPLACE FUNCTION price_between_for_each_catalog(int, int) RETURNS TABLE
	(Фирма varchar(40), Каталог varchar(40), Модель varchar(40), Цена int) 
	AS $$ BEGIN RETURN QUERY 
	SELECT providers.Фирма, catalogs.Каталог, models.Модель, main.Цена
	FROM providers, catalogs, models, main
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id
	AND main.Цена > $1 AND main.Цена < $2);
	return; end; $$ language 'plpgsql';
	4.2) В целом
	CREATE OR REPLACE FUNCTION price_between(int, int) RETURNS TABLE
	(Фирма varchar(40), Каталог varchar(40), Модель varchar(40), Цена int) 
	AS $$ BEGIN RETURN QUERY 
	SELECT providers.Фирма, catalogs.Каталог, models.Модель, main.Цена
	FROM providers, catalogs, models, main
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id
	AND main.Цена > $1 AND main.Цена < $2);
	return; end; $$ language 'plpgsql';

5) Найти все КО заданного производителя (выбор производителя)
CREATE OR REPLACE FUNCTION all_equip_of_provider(varchar) RETURNS TABLE
(Фирма varchar(40), Каталог varchar(40), Модель varchar(40), Цена int) 
AS $$ BEGIN RETURN QUERY 
SELECT providers.Фирма, catalogs.Каталог, models.Модель, main.Цена
FROM providers, catalogs, models, main
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id
AND providers.Фирма = $1);
return; end; $$ language 'plpgsql';

6) Найти долю дешевого КО (меньше заданного, ввод ограничения) от общего числа КО 
	6.1) Для каждого вида (каталога)


	CREATE OR REPLACE FUNCTION low_part_in_catalog(varchar, int) RETURNS numeric
	AS $$ DECLARE n numeric; 
	BEGIN
	n:=((SELECT COUNT(*) FROM main, models, catalogs
	WHERE(main.id_Модели = models.id AND models.id_Каталога = catalogs.id AND main.Цена <=$2 AND catalogs.Каталог = $1))::numeric
	/(SELECT COUNT(*) FROM main,models, catalogs
	WHERE(main.id_Модели = models.id AND models.id_Каталога = catalogs.id AND catalogs.Каталог = $1)))::numeric;
	return n; end; $$ language 'plpgsql';

	6.2) В целом
	CREATE OR REPLACE FUNCTION low_part(int) RETURNS numeric 
	AS $$ DECLARE n numeric; 
	BEGIN n:=(SELECT COUNT(*) FROM main 
	WHERE main.Цена <=$1)/(SELECT COUNT(*) FROM main)::numeric;
	return n; end; $$ language 'plpgsql';

SELECT AVG(main.Цена) FROM main, models,providers WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND providers.Фирма = 'Oasis');

7) Найти все КО с заданной датой выпуска
CREATE OR REPLACE FUNCTION find_by_release_date(date) RETURNS TABLE
(id int, Цена int,  Дата_выпуска date, Фирма varchar(40),  Каталог varchar(40), Модель varchar(40)) 
AS $$ BEGIN RETURN QUERY 
SELECT main.id, main.Цена,main.Дата_выпуска, providers.Фирма, catalogs.Каталог, models.Модель
FROM main, providers, catalogs, models
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id 
AND main.Дата_выпуска = $1);
return; end; $$ language 'plpgsql';

8) Найти все КО, чья дата продажи находится в заданных пределах 
	8.1) В целом
CREATE OR REPLACE FUNCTION get_between(date, date) RETURNS TABLE
(id int, name varchar(40), surname varchar(40), dob date, dex int)
AS $$ BEGIN RETURN QUERY
SELECT driver.id, driver.name, driver.surname, driver.dob, driver.dex
FROM driver 
WHERE driver.dob >= $1 AND driver.dob <= $2;
return; end; $$ language 'plpgsql';

	CREATE OR REPLACE FUNCTION find_if_sale_date_between(date, date) RETURNS TABLE
	(id int, Цена int,  Дата_продажи date, Фирма varchar(40),  Каталог varchar(40), Модель varchar(40)) 
	AS $$ BEGIN RETURN QUERY 
	SELECT main.id, main.Цена,main.Дата_продажи, providers.Фирма, catalogs.Каталог, models.Модель
	FROM main, providers, catalogs, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id 
	AND main.Дата_продажи > $1 AND main.Дата_продажи < $2);
	return; end; $$ language 'plpgsql';
	8.2) Для заданного производителя
	CREATE OR REPLACE FUNCTION find_if_sale_date_between_of_provider(date, date, varchar) RETURNS TABLE
	(id int, Цена int,  Дата_продажи date, Фирма varchar(40),  Каталог varchar(40), Модель varchar(40)) 
	AS $$ BEGIN RETURN QUERY 
	SELECT main.id, main.Цена,main.Дата_продажи, providers.Фирма, catalogs.Каталог, models.Модель
	FROM main, providers, catalogs, models
	WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id 
	AND main.Дата_продажи > $1 AND main.Дата_продажи < $2 AND providers.Фирма = $3);
	return; end; $$ language 'plpgsql';

9) Найти долю КО, проданного за определенный период времени (ввод периода) от общего времени продажи
CREATE OR REPLACE FUNCTION sale_date_part(date, date) RETURNS numeric
AS $$ DECLARE n numeric; 
BEGIN n:=(SELECT COUNT(*) FROM main 
WHERE main.Дата_выпуска > $1 AND main.Дата_выпуска < $2)/(SELECT COUNT(*) FROM main)::numeric;
return n; end; $$ language 'plpgsql';

10) Найти количество бракованного КО, поступившего из заданной страны (выбор страны) для заданного поставщика (выбор поставщика).
CREATE OR REPLACE FUNCTION defective(varchar, varchar) RETURNS int
AS $$ DECLARE n int; 
BEGIN n:=(SELECT COUNT(*) FROM main, models, providers, countries
WHERE (main.Наличие_брака = true AND main.id_Модели = models.id AND models.id_Поставщика = providers.id
AND providers.id_Страны = countries.id AND countries.Страна = $1 AND providers.Фирма = $2));
return n; end; $$ language 'plpgsql';

11) Найти все КО, поступившее от заданного поставщика, чья стоимость больше, чем заданная стоимость
КО (выбор наименования КО), поступившего из заданной страны (выбор страны).
CREATE OR REPLACE FUNCTION find_by_provider_price_country(varchar, varchar, varchar) RETURNS TABLE
(id int, Цена int,  Дата_продажи date, Фирма varchar(40),  Каталог varchar(40), Модель varchar(40)) 
AS $$ BEGIN RETURN QUERY SELECT main.id, main.Цена, main.Дата_продажи, providers.Фирма, catalogs.Каталог, models.Модель
FROM main, providers, catalogs, models, countries
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id 
	AND providers.id_Страны = countries.id AND  AND providers.Фирма = $1 AND countries.Страна = $3);
return; end; $$ language 'plpgsql';

12) Найти долю дешевого КО (чья стоимость меньше заданной, ввод стоимости), поступившего от заданного поставщика (выбор поставщика) и в целом
CREATE OR REPLACE FUNCTION low_part_from_provider(int, varchar) RETURNS numeric 
AS $$ DECLARE n numeric; 
BEGIN n:=(SELECT COUNT(*) FROM main, providers 
WHERE (main.Цена <=$1 AND providers.Фирма = $2))/(SELECT COUNT(*) FROM main)::numeric;
return n; end; $$ language 'plpgsql';
	В целом
CREATE OR REPLACE FUNCTION low_part_from(int) RETURNS numeric 
AS $$ DECLARE n numeric; 
BEGIN n:=(SELECT COUNT(*) FROM main, providers 
WHERE (main.Цена <=$1 AND providers.Фирма = $2))/(SELECT COUNT(*) FROM main)::numeric;
return n; end; $$ language 'plpgsql';

13) Найти среднюю стоимость КО, проданного за определенный промежуток времени (ввод интервала).
CREATE OR REPLACE FUNCTION avg_price_between(date,date) RETURNS numeric
AS $$ DECLARE n numeric;
BEGIN n:=(SELECT AVG(main.Цена) FROM main 
WHERE (main.Дата_продажи>$1 AND main.Дата_продажи<$2))::numeric;
return n; end; $$ language 'plpgsql';

14) Найти все КО, чья стоимость выше, чем средняя стоимость КО заданного производителя (выбор производителя).
CREATE OR REPLACE FUNCTION price_more_avg_of(varchar)
RETURNS TABLE(id int, Модель varchar, Фирма varchar, Каталог varchar, Цена int)
AS $$ BEGIN RETURN QUERY 
SELECT main.id, models.Модель, providers.Фирма, catalogs.Каталог, main.Цена
FROM main, models, providers, catalogs 
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id 
AND main.Цена > (SELECT AVG(main.Цена)
FROM main, models, providers 
WHERE (main.id_Модели = models.id
AND models.id_Поставщика = providers.id AND providers.Фирма = $1)));
return; end; $$ language 'plpgsql';

15) Определить долю регулярных поставок заданного КО (выбор) по заданной дате отгрузки (ввод) от общего числа КО.
CREATE OR REPLACE FUNCTION part_of_provider_by_date(varchar, date) RETURNS numeric
AS $$ DECLARE n numeric;
BEGIN n:=((SELECT COUNT(*) 
FROM main, models 
WHERE main.id_Модели = models.id AND models.Модель = $1 AND main.Дата_выпуска = $2))::numeric /
(SELECT COUNT(*) FROM main);
return n; end; $$ language 'plpgsql';

16) Найти КО, которое лучше всего раскупают, указать параметры (стоимость, производитель, страна и т.д.).
CREATE OR REPLACE FUNCTION popular_equip() RETURNS TABLE(Модель varchar, Фирма varchar, Страна varchar, Цена int, Количество bigint)
AS $$ BEGIN RETURN QUERY 
SELECT models.Модель, providers.Фирма, countries.Страна, main.Цена, COUNT(main.id_Модели) AS cnt 
FROM providers, models, countries, main, catalogs
WHERE (main.id_Модели = models.id AND models.id_Поставщика = providers.id AND models.id_Каталога = catalogs.id AND providers.id_Страны = countries.id)
GROUP BY main.id_Модели, providers.Фирма, countries.Страна, main.Цена, models.Модель 
ORDER BY cnt desc limit 1;
return; end; $$ language 'plpgsql';
____________________________________________________________________________________________роли
CREATE ROLE operator WITH PASSWORD '00000' NOSUPERUSER NOCREATEDB LOGIN;

CREATE ROLE userdb WITH PASSWORD '00000' NOSUPERUSER NOCREATEDB LOGIN;

CREATE ROLE admin with PASSWORD '00000' SUPERUSER CREATEDB LOGIN;

GRANT INSERT, UPDATE, DELETE, SELECT ON models, catalogs, providers, countries TO operator;

GRANT SELECT ON models, catalogs, providers, countries TO userdb;
____________________________________________________________________________________________заполнение
INSERT INTO countries (Страна) VALUES ('Россия');

INSERT INTO catalogs (Каталог) VALUES
('Кондиционеры'),
('Очистители воздуха'),
('Котлы'),
('Вентиляторы'),
('Камины'),
('Обогреватели');

INSERT INTO providers (Фирма, id_Страны) VALUES 
('Timberk',3), 
('Hyundai',5),
('Noirot Spot',6),
('NeoClima',3),
('Winzel Expert',6),
('Эван',1),
('Oasis',2),
('Master',4),
('Alex Bauman',6),
('RealFlame',1),
('SCOOLE',5);

INSERT INTO models (Модель, id_Каталога, id_Поставщика) VALUES
('SC AC SP8 09',1,11),
('SC AC SP8 24',1,11),
('SC AC SP6 24',1,11),
('AC TIM 09H S22',1,1),
('AC TIM 09H S20',1,1),
('H-ARI17-07H',1,2),
('H-ARI17-18H',1,2),
('RB1-50',2,5),
('RA1-50 P',2,5),
('WiFi RW1-50 P',2,5),
('E-3 Plus 1500',6,3),
('E-5 1000',6,3),
('E-3 Plus 1000',6,3),
('Fast 2000 w',3,4),
('ЭПО 25',3,6),
('ЭПО 4',3,6),
('ЭПО 6',3,6),
('Warmos 24',3,6),
('BM-20',3,7),
('BM-16',3,7),
('DF 20',4,8),
('DF 30',4,8),
('BL 6800',4,8),
('Camilla C 18',5,9),
('Tango Premium Style V 25FX ',5,9),
('Lion V 34FX',5,9),
('Colorado Rock 3D',5,10),
('Lilviana DN+Majestic BL',5,10);

INSERT INTO main (id_Модели , Дата_выпуска , Дата_продажи , Цена , Наличие_брака) VALUES
(1,'2018-11-20','2018-11-27',51,'false'),
(2,'2016-11-20','2018-11-27',650,'false'),
(2,'2016-11-20','2018-05-12',650,'true'),
(2,'2016-11-20','2018-11-12',650,'false'),
(2,'2016-11-20','2018-11-12',650,'false'),
(2,'2016-11-20','2018-11-12',650,'false'),
(2,'2016-11-20','2018-11-12',650,'false'),
(3,'2015-03-01','2017-03-05',1125,'false'),
(3,'2015-03-01','2017-03-05',1125,'false'),
(3,'2015-03-01','2017-03-05',1125,'true'),
(3,'2015-03-01','2017-03-05',1125,'false'),
(4,'2011-11-01','2016-08-07',4648,'false'),
(4,'2011-11-01','2016-08-07',4648,'false'),
(4,'2011-11-01','2016-08-07',4648,'false'),
(4,'2011-11-01','2016-08-07',4648,'true'),
(4,'2011-11-01','2016-08-07',4648,'false'),
(5,'2003-12-12','2005-03-04',468,'false'),
(5,'2003-12-12','2005-03-04',468,'false'),
(6,'2003-12-12','2005-03-04',7221,'true'),
(7,'2003-12-12','2005-03-04',453,'true'),
(8,'2003-12-12','2005-03-04',367,'false'),
(9,'2003-12-12','2005-03-04',2221,'true'),
(9,'2003-12-12','2005-03-04',2221,'false'),
(10,'2011-05-07','2011-05-08',617,'false'),
(10,'2011-05-07','2011-05-08',617,'false'),
(10,'2011-05-07','2011-05-09',617,'false'),
(11,'2013-02-01','2013-04-01',379,'false'),
(11,'2013-02-01','2013-04-01',379,'false'),
(11,'2013-02-01','2013-04-01',379,'false'),
(11,'2013-02-01','2013-04-01',379,'true'),
(12,'2013-02-01','2013-04-01',431,'false'),
(12,'2013-02-01','2013-04-01',431,'false'),
(12,'2013-02-01','2013-04-01',431,'false'),
(13,'2006-07-08','2006-07-11',9616,'false'),
(13,'2006-07-08','2006-07-11',9616,'false'),
(13,'2006-07-08','2006-07-11',9616,'false'),
(13,'2006-07-08','2006-07-12',9616,'true'),
(13,'2006-07-08','2006-07-12',9616,'false'),
(13,'2006-07-08','2006-07-13',9616,'true'),
(13,'2006-07-08','2006-07-13',9616,'true'),
(13,'2006-07-08','2006-07-14',9616,'false'),
(14,'2017-02-22','2017-03-24',348,'false'),
(15,'2017-02-22','2017-03-24',3484,'false'),
(16,'2017-02-22','2017-03-24',314,'false'),
(17,'2017-02-22','2017-03-24',999,'true'),
(18,'2017-02-22','2017-03-24',114,'false'),
(18,'2017-02-22','2017-03-24',114,'true'),
(19,'2017-02-22','2017-03-24',767,'false'),
(20,'2017-02-22','2017-03-24',6761,'true'),
(20,'2017-02-22','2017-03-24',6761,'true'),
(20,'2014-05-22','2017-04-27',6761,'true'),
(21,'2012-08-03','2014-06-06',333,'true'),
(22,'2012-08-03','2014-11-05',335,'false'),
(23,'2012-07-04','2014-12-16',324,'false'),
(24,'2012-05-07','2014-02-09',951,'false'),
(25,'2012-04-09','2014-06-07',1324,'false'),
(25,'2012-04-09','2014-06-07',1324,'true'),
(26,'2014-01-01','2015-08-10',648,'true'),
(26,'2014-01-01','2015-08-11',648,'true'),
(26,'2014-01-01','2015-08-12',648,'false'),
(26,'2014-01-01','2015-08-12',648,'true'),
(26,'2014-01-01','2015-08-13',648,'true'),
(26,'2014-01-01','2015-08-13',648,'false'),
(27,'1968-12-11','2018-11-20',8999,'true'),
(28,'2003-06-13','2003-08-21',2697,'false'),
(28,'2003-06-13','2003-08-21',2697,'false'),
(28,'2003-06-13','2003-08-22',2697,'false'),
(28,'2003-06-13','2003-08-23',2697,'true'),
(28,'2003-06-13','2003-09-12',2697,'false'),
(28,'2003-06-14','2003-09-14',2697,'true'),
(28,'2003-06-14','2003-09-15',2697,'true'),
(28,'2003-06-14','2003-10-01',2697,'false');



