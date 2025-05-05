#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

// Объединение для хранения различных типов данных
union PropertyValue {
    int intValue;
    double doubleValue;
    std::string* stringValue;
    std::vector<int>* vecIntValue;
    std::vector<double>* vecDoubleValue;
    std::vector<std::string>* vecStringValue;
    std::map<std::string, PropertyValue>* objectValue;

    PropertyValue() : intValue(0) {}  // Инициализация по умолчанию

    ~PropertyValue() {}  // Явный деструктор
};

// Перечисление типов данных
enum PropertyType {
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_VECTOR_INT,
    TYPE_VECTOR_DOUBLE,
    TYPE_VECTOR_STRING,
    TYPE_OBJECT
};

// Структура Property для хранения значения и типа
struct Property {
    PropertyType type;
    PropertyValue value;

    // Конструкторы для различных типов
    Property(int v) : type(TYPE_INT) { value.intValue = v; }
    Property(double v) : type(TYPE_DOUBLE) { value.doubleValue = v; }
    Property(const std::string& v) : type(TYPE_STRING) { value.stringValue = new std::string(v); }
    Property(const std::vector<int>& v) : type(TYPE_VECTOR_INT) { value.vecIntValue = new std::vector<int>(v); }
    Property(const std::vector<double>& v) : type(TYPE_VECTOR_DOUBLE) { value.vecDoubleValue = new std::vector<double>(v); }
    Property(const std::vector<std::string>& v) : type(TYPE_VECTOR_STRING) { value.vecStringValue = new std::vector<std::string>(v); }
    Property(const std::map<std::string, PropertyValue>& v) : type(TYPE_OBJECT) { value.objectValue = new std::map<std::string, PropertyValue>(v); }

    // Копирующий конструктор
    Property(const Property& other) : type(other.type) {
        switch (type) {
            case TYPE_INT:
                value.intValue = other.value.intValue;
                break;
            case TYPE_DOUBLE:
                value.doubleValue = other.value.doubleValue;
                break;
            case TYPE_STRING:
                value.stringValue = new std::string(*other.value.stringValue);
                break;
            case TYPE_VECTOR_INT:
                value.vecIntValue = new std::vector<int>(*other.value.vecIntValue);
                break;
            case TYPE_VECTOR_DOUBLE:
                value.vecDoubleValue = new std::vector<double>(*other.value.vecDoubleValue);
                break;
            case TYPE_VECTOR_STRING:
                value.vecStringValue = new std::vector<std::string>(*other.value.vecStringValue);
                break;
            case TYPE_OBJECT:
                value.objectValue = new std::map<std::string, PropertyValue>(*other.value.objectValue);
                break;
        }
    }

    // Оператор присваивания
    Property& operator=(const Property& other) {
        if (this != &other) {
            // Удаляем старое значение
            this->~Property();

            // Копируем новое значение
            new(this) Property(other);
        }
        return *this;
    }

    // Деструктор для корректного освобождения памяти
    ~Property() {
        switch (type) {
            case TYPE_STRING:
                delete value.stringValue;
                break;
            case TYPE_VECTOR_INT:
                delete value.vecIntValue;
                break;
            case TYPE_VECTOR_DOUBLE:
                delete value.vecDoubleValue;
                break;
            case TYPE_VECTOR_STRING:
                delete value.vecStringValue;
                break;
            case TYPE_OBJECT:
                delete value.objectValue;
                break;
            default:
                break;
        }
    }
};

struct Object {
    std::map<std::string, PropertyValue> properties;

    void setProperty(const std::string& key, const Property& value) {
        properties[key] = value.value;
    }

    bool hasProperty(const std::string& key) const {
        return properties.find(key) != properties.end();
    }

    PropertyValue getProperty(const std::string& key) const {
        std::map<std::string, PropertyValue>::const_iterator it = properties.find(key);
        if (it != properties.end()) {
            return it->second;
        } else {
            throw std::runtime_error("Property not found");
        }
    }

    void removeProperty(const std::string& key) {
        properties.erase(key);
    }
};

// Функция для вывода значений свойств
void printPropertyValue(const PropertyValue& value, PropertyType type) {
    switch (type) {
        case TYPE_INT:
            std::cout << value.intValue;
            break;
        case TYPE_DOUBLE:
            std::cout << value.doubleValue;
            break;
        case TYPE_STRING:
            std::cout << *value.stringValue;
            break;
        case TYPE_VECTOR_INT: {
            std::cout << "[";
            const std::vector<int>& vec = *value.vecIntValue;
            for (size_t i = 0; i < vec.size(); ++i) {
                std::cout << vec[i];
                if (i != vec.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
            break;
        }
        case TYPE_VECTOR_DOUBLE: {
            std::cout << "[";
            const std::vector<double>& vec = *value.vecDoubleValue;
            for (size_t i = 0; i < vec.size(); ++i) {
                std::cout << vec[i];
                if (i != vec.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
            break;
        }
        case TYPE_VECTOR_STRING: {
            std::cout << "[";
            const std::vector<std::string>& vec = *value.vecStringValue;
            for (size_t i = 0; i < vec.size(); ++i) {
                std::cout << vec[i];
                if (i != vec.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
            break;
        }
        case TYPE_OBJECT: {
            std::cout << "{";
            const std::map<std::string, PropertyValue>& obj = *value.objectValue;
            for (std::map<std::string, PropertyValue>::const_iterator it = obj.begin(); it != obj.end(); ++it) {
                std::cout << it->first << ": ";
                printPropertyValue(it->second, TYPE_OBJECT);  // Assuming all nested objects are of TYPE_OBJECT
                if (it != --obj.end()) std::cout << ", ";
            }
            std::cout << "}";
            break;
        }
    }
}

int main() {
    Object obj;
    obj.setProperty("age", Property(30));
    obj.setProperty("name", Property(std::string("John Doe")));
    obj.setProperty("height", Property(5.9));
    std::vector<int> scores;
    scores.push_back(90);
    scores.push_back(85);
    scores.push_back(88);
    obj.setProperty("scores", Property(scores));

    // Вложенный объект
    std::map<std::string, PropertyValue> address;
    address["city"] = PropertyValue();
    address["city"].stringValue = new std::string("New York");
    address["zip"] = PropertyValue();
    address["zip"].intValue = 10001;
    obj.setProperty("address", Property(address));

    if (obj.hasProperty("name")) {
        std::cout << "Name: ";
        printPropertyValue(obj.getProperty("name"), TYPE_STRING);
        std::cout << std::endl;
    }

    if (obj.hasProperty("address")) {
        std::cout << "Address: ";
        printPropertyValue(obj.getProperty("address"), TYPE_OBJECT);
        std::cout << std::endl;
    }

    return 0;
}
