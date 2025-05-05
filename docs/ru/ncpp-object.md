[![doc-link](https://img.shields.io/badge/Введение%20main-gray)](introduction.md)

- [ncpp::Object](#ncppobject)
	- [Object.keys()](#objectkeys)
	- [Object.has()](#objecthas)
	- [Object.cout()](#objectcout)
	- [Object.toString()](#objecttostring)
	- [Object.type()](#objecttype)
	- [Object::getNestedValue()](#objectgetnestedvalue)
	- [Object::setNestedValue()](#objectsetnestedvalue)
- [ncpp::Variant](#ncppvariant)
- [ncpp::VArray](#ncppvarray)
- [ncpp::ObjectValue](#ncppobjectvalue)
	- [ObjectValue.clear()](#objectvalueclear)
	- [ObjectValue.set()](#objectvalueset)
	- [ObjectValue.isInt32()](#objectvalueisint32)
	- [ObjectValue.isInt64()](#objectvalueisint64)
	- [ObjectValue.isDouble()](#objectvalueisdouble)
	- [ObjectValue.isNumber()](#objectvalueisnumber)
	- [ObjectValue.isBool()](#objectvalueisbool)
	- [ObjectValue.isString()](#objectvalueisstring)
	- [ObjectValue.isBuff()](#objectvalueisbuff)
	- [ObjectValue.isObj()](#objectvalueisobj)
	- [ObjectValue.isArray()](#objectvalueisarray)
	- [ObjectValue.isNull()](#objectvalueisnull)
	- [ObjectValue.isUndefined()](#objectvalueisundefined)
	- [ObjectValue.asDouble()](#objectvalueasdouble)
	- [ObjectValue.asBool()](#objectvalueasbool)
	- [ObjectValue.asString()](#objectvalueasstring)
	- [ObjectValue.asBuff()](#objectvalueasbuff)
	- [ObjectValue.asObj()](#objectvalueasobj)
	- [ObjectValue::compare()](#objectvaluecompare)
	- [ObjectValue.type()](#objectvaluetype)
	- [ObjectValue.toString()](#objectvaluetostring)
	- [ObjectValue.toNumber()](#objectvaluetonumber)
	- [ObjectValue.toInt32()](#objectvaluetoint32)
	- [ObjectValue.toInt64()](#objectvaluetoint64)
	- [ObjectValue.toBuff()](#objectvaluetobuff)
	- [ObjectValue.toBool()](#objectvaluetobool)
- [ncpp::JSON::stringify()](#ncppjsonstringify)
- [ncpp::JSON::parse()](#ncppjsonparse)
- [ncpp::BSON::serialize()](#ncppbsonserialize)
- [ncpp::BSON::parse()](#ncppbsonparse)
- [ncpp::CBOR::serialize()](#ncppcborserialize)
- [ncpp::CBOR::parse()](#ncppcborparse)

## ncpp::Object
```cpp
Object();
Object(String json);
```
Структура динамического вложенного объекта, которая может вместить JSON или иное представление вложенного объекта, основаного на `std::map<std::string, ObjectValue>`.

### Object.keys()
```cpp
Array<String> keys() const;
```
Вернуть массив с ключами.

### Object.has()
```cpp
bool has(std::string key) const;
```
Проверить наличие этого ключа в объекте.

### Object.cout()
```cpp
std::string cout(int depth=2, int bias=2) const;
```
Вывести строковое представление объекта на указанную глубину.

### Object.toString()
```cpp
String toString(int depth=2) const;
```
Вывести строковое представление объекта на указанную глубину.

### Object.type()
```cpp
std::string type();
```
Вывести тип текущего объекта.

### Object::getNestedValue()
```cpp
static const ObjectValue& getNestedValue(const Object* obj, const Array<String>& path);
static const ObjectValue& getNestedValue(const Object& obj, const String& path);
static ObjectValue& getNestedValue(Object* obj, Array<String> path);
static ObjectValue& getNestedValue(Object& obj, String path);
```
Достает вложенное значение. Если передан `String`, разбивает ее по разделителю `.` в массив `Array<String>` и рекурсивно берет значение.

### Object::setNestedValue()
```cpp
static void setNestedValue(Object* obj, const Array<String>& path, const ObjectValue& value);
static void setNestedValue(Object& obj, const String& path, const ObjectValue& value);
```
Устанавливает вложенное значение. Если передан `String`, разбивает ее по разделителю `.` в массив `Array<String>` и рекурсивно устанавливает значение.

## ncpp::Variant
Является `typedef ObjectValue Variant;`

## ncpp::VArray
Является `typedef Array<Variant> VArray;` 

## ncpp::ObjectValue
Структура динамического значения, которое может хранить в себе одновременно 1 тип данных, имея возможность динамически его изменять.
Имеет перегрузки оператора `operator=`, `operator==`, `operator ()` и прочие аналогичные типам в `set()`.

### ObjectValue.clear()
```cpp
void clear();
```
Очистить и сброить текущее значение.

### ObjectValue.set()
```cpp
ObjectValue& set(double d);
ObjectValue& set(const std::string& s);
ObjectValue& set(const Buffer& bf);
ObjectValue& set(const char* c);
ObjectValue& set(const Object& o);
ObjectValue& set(const StringMap& map);
ObjectValue& set(const DoubleMap& map);
ObjectValue& set(const Array<T>& arr);
ObjectValue& setNull();
ObjectValue& setUndefined();
```
Установить новое значение, заменяет старое.

### ObjectValue.isInt32()
```cpp
bool isInt32() const;
```
Проверяет, является ли тип значения `int32`.

### ObjectValue.isInt64()
```cpp
bool isInt64() const;
```
Проверяет, является ли тип значения `int64`.

### ObjectValue.isDouble()
```cpp
bool isDouble() const;
```
Проверяет, является ли тип значения `double`.

### ObjectValue.isNumber()
```cpp
bool isNumber() const;
```
Проверяет, является ли численным типом в целом. (`double`/`int32`/`int64`)

### ObjectValue.isBool()
```cpp
bool isBool() const;
```
Проверяет, является ли тип значения `bool`.

### ObjectValue.isString()
```cpp
bool isString() const;
```
Проверяет, является ли тип значения `String`.

### ObjectValue.isBuff()
```cpp
bool isBuff() const;
```
Проверяет, является ли тип значения `Buffer`.

### ObjectValue.isObj()
```cpp
bool isObj() const;
```
Проверяет, является ли тип значения `Object`.

### ObjectValue.isArray()
```cpp
bool isArray() const;
```
Проверяет, является ли тип значения `Array`.

### ObjectValue.isNull()
```cpp
bool isNull() const;
```
Проверяет, является ли встроенный тип значения NIL.

### ObjectValue.isUndefined()
```cpp
bool isUndefined() const;
```
Проверяет, является ли встроенный тип значения Undefined.

### ObjectValue.asDouble()
```cpp
double& asDouble();
const double& asDouble() const;
```
Получить ссылку на значение, если тип строго совпадает с `double`. Иначе вернет исключение.

### ObjectValue.asBool()
```cpp
bool& asBool();
const bool& asBool() const;
```
Получить ссылку на значение, если тип строго совпадает с `bool`. Иначе вернет исключение.

### ObjectValue.asString()
```cpp
String& asString();
const String& asString() const;
```
Получить ссылку на значение, если тип строго совпадает с `String`. Иначе вернет исключение.

### ObjectValue.asBuff()
```cpp
Buffer& asBuff();
const Buffer& asBuff() const;
```
Получить ссылку на значение, если тип строго совпадает с `Buffer`. Иначе вернет исключение.

### ObjectValue.asObj()
```cpp
Object& asObj();
const Object& asObj() const;
```
Получить ссылку на значение, если тип строго совпадает с `Object`. Иначе вернет исключение.

### ObjectValue::compare()
```cpp
static bool compare(const ObjectValue& val1, const ObjectValue& val2, bool strong=false);
```
Сравнить 2 `ObjectValue` между собой.

### ObjectValue.type()
```cpp
std::string type() const noexcept;
```
Вернуть тип текущего значения.

### ObjectValue.toString()
```cpp
String toString() const noexcept;
```
Переобразовать значаение в `String` и вернуть его.

### ObjectValue.toNumber()
```cpp
double toNumber() const noexcept;
```
Переобразовать значаение в `double` и вернуть его.

### ObjectValue.toInt32()
```cpp
int toInt32() const noexcept; 
```
Переобразовать значаение в `int32` и вернуть его.

### ObjectValue.toInt64()
```cpp
long long toInt64() const noexcept;
```
Переобразовать значаение в `int64` и вернуть его.

### ObjectValue.toBuff()
```cpp
Buffer toBuff() const noexcept;
```
Переобразовать значаение в `Buffer` и вернуть его.

### ObjectValue.toBool()
```cpp
bool toBool() const noexcept;
```
Переобразовать значаение в `bool` и вернуть его.

## ncpp::JSON::stringify()
```cpp
String stringify(const Object& obj);
String stringify(const StringMap& map);
```
Получить сериализованную JSON строку.

## ncpp::JSON::parse()
```cpp
Object parse(String jsonstr);
```
Распарсить JSON строку в `Object`.

## ncpp::BSON::stringify()
```cpp
Buffer serialize(const Object& obj, bool ex=false);
Buffer serialize(const StringMap& map, bool ex=false);
```
Получить сериализованный BSON буфер. Параметр `ex` означает использование расширенния спецификации с 4-х до 8-байт для хранения размера элементов.

## ncpp::BSON::parse()
```cpp
Object parse(const Buffer& bson, bool ex=false, size_t offset=0);
```
Распарсить BSON буфер в `Object`.

## ncpp::CBOR::serialize()
```cpp
Buffer serialize(const Object& obj);
Buffer serialize(const StringMap& map);
```
Получить сериализованный CBOR буфер.

## ncpp::CBOR::parse()
```cpp
Object parse(const Buffer& cbor);
```
Распарсить CBOR буфер в `Object`.








































 
		 


