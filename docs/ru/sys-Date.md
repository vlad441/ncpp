[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Date](#ncppdate)
    - [Date.now()](#datenow)
    - [Date.getYear()](#dategetyear)
    - [Date.getDay()](#dateday)
    - [Date.getDate()](#dategetdate)
    - [Date.getDayWeek()](#datedayweek)
    - [Date.toDirectDate()](#datetodirectdate)
    - [Date.toReverseDate()](#datetoreversedate)
    - [Date.toString()](#datetostring)
    - [Date.toISOString()](#datetoisostring)
    - [Date.toIMFDate()](#datetoimfdate)
    - [Date.toTime()](#datetotime)

## ncpp::Date
`Date` — работа с датой и временем. Хранит время в формате UNIX-timestamp (секунды с 1 января 1970 года)

```cpp
Date();
Date(long long secs, char type='s');
Date(const CString& dateStr);
Date(const DValue& dv);
```

### Date::DValue
```cpp
struct DValue { long long year; char month, day, hour, minute; float seconds; };
```
Вспомогательная структура для прямого доступа к компонентам.

### Date::now()
```cpp
static long long now();
```
Возвращает текущий UNIX-timestamp в секундах.

### Date.getYear()
```cpp
long long getYear() const;
```
Возвращает год.

### Date.getMonth()
```cpp
int getMonth() const;
```
Возвращает месяц (1–12).

### Date.getDay()
```cpp
int getDay() const;
```
Возвращает день месяца (1–31).

### Date.getDate()
```cpp
int getDate() const;
```
Синоним для `getDay()`.

### Date.getDayWeek()
```cpp
int getDayWeek() const;
```
Возвращает день недели (0–6), где `0` — понедельник

### Date.toDirectDate()
```cpp
String toDirectDate(char sp1='.', bool toshort=false) const;
```
Возвращает строку в формате `ДД.ММ.ГГГГ ЧЧ:ММ:СС`. Параметр `sp1` меняет разделитель даты.

### Date.toReverseDate()
```cpp
String toReverseDate(char sp1='.', bool toshort=false) const;
```
Возвращает строку в формате `ГГГГ.ММ.ДД ЧЧ:ММ:СС`.

### Date.toString()
```cpp
String toString(char sp='.') const;
String toString(const CString& mode, char sp='.') const;
```
Универсальный метод вывода. Режимы `mode`: `"reverse"`, `"direct"`, `"iso"`, `"imf"`. По умолчанию — `"reverse"`.

### Date.toISOString()
```cpp
String toISOString() const;
```
Возвращает строку в стандарте ISO 8601: `YYYY-MM-DDTHH:MM:SSZ`

### Date.toIMFDate()
```cpp
String toIMFDate() const;
```
Возвращает дату в формате `IMF-fixdate` (RFC 7231), используемом в HTTP заголовках: `Sun, 06 Nov 1994 08:49:37 GMT`

### Date.toTime()
```cpp
String toTime(bool toshort=false) const;
```
Возвращает только строковое представление времени `ЧЧ:ММ:СС` (или `ЧЧ:ММ`, если `toshort` равен `true`).