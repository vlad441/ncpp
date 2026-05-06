namespace ncpp { namespace GL {
#define _V(PX, PY) v.push(Vertex(PX, PY, 0, color)) //макрос для сокращения длины инструкций.
//#define _ASCII() _getASCIISymbolVertexes(v, point, x, y, size, color, aspect)
	
	 void _getASCIISymbolVertexes(Array<Vertex>& v, int symb, float x, float y, float size, unsigned int color, float aspect=1.0f){
        float width = size * 0.6f / aspect; float lowRow = size*0.6f; float desc = size*0.25f;
		switch(symb){ 
			case ' ': break;
			case '!': // 33
                _V(x+width*0.5f, y+size); _V(x+width*0.5f, y+size*0.25f); // Палка
                _V(x+width*0.5f, y+size*0.07f); _V(x+width*0.5f, y); break; // Точка
            case '"': // 34
                _V(x+width*0.3f, y+size); _V(x+width*0.3f, y+size*0.75f);
                _V(x+width*0.7f, y+size); _V(x+width*0.7f, y+size*0.75f); break;
            case '#': // 35
                _V(x+width*0.3f, y+size); _V(x+width*0.3f, y);
                _V(x+width*0.7f, y+size); _V(x+width*0.7f, y);
                _V(x, y+size*0.65f); _V(x+width, y+size*0.65f);
                _V(x, y+size*0.35f); _V(x+width, y+size*0.35f); break;
            case '$': // 36
                _V(x+width*0.5f, y+size*1.1f); _V(x+width*0.5f, y - size * 0.1f); // Вертикаль
                _V(x+width, y+size*0.85f); _V(x, y+size*0.85f); // S-образные линии
                _V(x, y+size*0.85f); _V(x, y+size*0.5f);
                _V(x, y+size*0.5f); _V(x+width, y+size*0.5f);
                _V(x+width, y+size*0.5f); _V(x+width, y+size*0.15f);
                _V(x+width, y+size*0.15f); _V(x, y+size*0.15f); break;
            case '%': // 37
                _V(x, y); _V(x+width, y+size); // Диагональ
                _V(x, y+size); _V(x+width*0.2f, y+size); // Точка 1
                _V(x+width*0.8f, y); _V(x+width, y); break; // Точка 2
            case '&': // 38
                _V(x+width, y); _V(x, y+size);
                _V(x, y); _V(x+width, y+size);
                _V(x, y); _V(x+width, y); break;
            case '\'': _V(x+width*0.5f, y+size); _V(x+width*0.5f, y+size*0.75f); break; //39
            case '(': // 40
                _V(x+width*0.8f, y+size); _V(x+width*0.3f, y+size*0.8f);
                _V(x+width*0.3f, y+size*0.8f); _V(x+width*0.3f, y+size*0.2f);
                _V(x+width*0.3f, y+size*0.2f); _V(x+width*0.8f, y); break;
            case ')': // 41
                _V(x+width*0.2f, y+size); _V(x+width*0.7f, y+size*0.8f);
                _V(x+width*0.7f, y+size*0.8f); _V(x+width*0.7f, y+size*0.2f);
                _V(x+width*0.7f, y+size*0.2f); _V(x+width*0.2f, y); break;
            case '*': // 42
                _V(x+width*0.5f, y+lowRow); _V(x+width*0.5f, y+lowRow*0.2f);
                _V(x+width*0.1f, y+lowRow*0.8f); _V(x+width*0.9f, y+lowRow*0.4f);
                _V(x+width*0.9f, y+lowRow*0.8f); _V(x+width*0.1f, y+lowRow*0.4f); break;
            case '+': // 43
                _V(x+width*0.5f, y+lowRow*0.8f); _V(x+width*0.5f, y+lowRow*0.2f);
                _V(x+width*0.1f, y+lowRow*0.5f); _V(x+width*0.9f, y+lowRow*0.5f); break;
            case ',': // 44
                _V(x+width*0.5f, y+size*0.1f); _V(x+width*0.5f, y);
                _V(x+width*0.5f, y); _V(x+width*0.3f, y - desc * 0.5f); break;
            case '-': _V(x+width*0.1f, y+lowRow*0.5f); _V(x+width*0.9f, y+lowRow*0.5f); break; // 45
            case '.':  _V(x+width*0.5f, y+size*0.07f); _V(x+width*0.5f, y); break; // 46
            case '/': _V(x, y); _V(x+width, y+size); break; // 47
			// --- цифры ---
			case '0':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x+width, y+size); _V(x, y); break;
			case '1':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y+size);
				_V(x+width*0.2f, y+size*0.7f); _V(x+width*0.5f, y+size); break;
			case '2':
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case '3':
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f); break;
			case '4':
				_V(x, y+size); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f);
				_V(x+width, y+size); _V(x+width, y); break;
			case '5':
				_V(x+width, y+size); _V(x, y+size);
				_V(x, y+size); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case '6':
				_V(x+width, y+size); _V(x, y+size);
				_V(x, y+size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x, y+size*0.5f); break;
			case '7':
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width*0.5f, y); break;
			case '8':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f); break;
			case '9':
				_V(x+width, y); _V(x+width, y+size);
				_V(x+width, y+size); _V(x, y+size);
				_V(x, y+size); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f); break;
			// --- ---
			case ':': // 58
                _V(x+width*0.5f, y+lowRow*0.8f); _V(x+width*0.5f, y+lowRow*0.7f);
                _V(x+width*0.5f, y+size*0.07f); _V(x+width*0.5f, y); break;
            case ';': // 59
                _V(x+width*0.5f, y+lowRow*0.8f); _V(x+width*0.5f, y+lowRow*0.7f);
                // Запятая снизу
                _V(x+width*0.5f, y+size*0.07f); _V(x+width*0.5f, y);
                _V(x+width*0.5f, y); _V(x+width*0.3f, y - desc * 0.5f); break;
            case '<': // 60
                _V(x+width*0.9f, y+lowRow); _V(x+width*0.1f, y+lowRow*0.5f);
                _V(x+width*0.1f, y+lowRow*0.5f); _V(x+width*0.9f, y); break;
            case '=': // 61
                _V(x, y+lowRow*0.7f); _V(x+width, y+lowRow*0.7f);
                _V(x, y+lowRow*0.3f); _V(x+width, y+lowRow*0.3f); break;
            case '>': // 62
                _V(x+width*0.1f, y+lowRow); _V(x+width*0.9f, y+lowRow*0.5f);
                _V(x+width*0.9f, y+lowRow*0.5f); _V(x+width*0.1f, y); break;
            case '@': // 64 (упрощенная "собака")
                _V(x+width, y); _V(x+width, y+lowRow); // Внешний контур
                _V(x+width, y+lowRow); _V(x, y+lowRow);
                _V(x, y+lowRow); _V(x, y);
                _V(x, y); _V(x+width*0.7f, y);
                _V(x+width*0.7f, y); _V(x+width*0.7f, y+lowRow*0.5f); // Внутренняя "a"
                _V(x+width*0.7f, y+lowRow*0.5f); _V(x+width*0.3f, y+lowRow*0.5f); break;
			// --- прописные ---
			case 'A':
				_V(x, y); _V(x+width*0.5f, y+size); // Линия 1: левая палка (снизу вверх)
				_V(x+width*0.5f, y+size); _V(x+width, y); // Линия 2: правая палка (сверху вниз)
				_V(x+width*0.2f, y+size*0.4f); _V(x+width*0.8f, y+size*0.4f); break; // Линия 3: перекладина
			case 'B':
				_V(x, y); _V(x, y+size); // Вертикаль
				_V(x, y+size); _V(x+width, y+size*0.75f);
				_V(x+width, y+size*0.75f); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.25f);
				_V(x+width, y+size*0.25f); _V(x, y); break;
			case 'C':
				_V(x+width, y+size); _V(x, y+size);
				_V(x, y+size); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case 'E':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x, y+size*0.5f); _V(x+width*0.7f, y+size*0.5f);
				_V(x, y); _V(x+width, y); break;
			case 'F':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x, y+size*0.5f); _V(x+width*0.7f, y+size*0.5f); break;
			case 'D':
				_V(x, y); _V(x, y+size); // Спинка
				_V(x, y+size); _V(x+width*0.8f, y+size*0.8f);
				_V(x+width*0.8f, y+size*0.8f); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x+width*0.8f, y+size*0.2f);
				_V(x+width*0.8f, y+size*0.2f); _V(x, y); break;
			case 'G':
				_V(x+width, y+size); _V(x, y+size); // Верх
				_V(x, y+size); _V(x, y); // Лево
				_V(x, y); _V(x+width, y); // Низ
				_V(x+width, y); _V(x+width, y+size*0.4f); // Хвостик вверх
				_V(x+width, y+size*0.4f); _V(x+width*0.5f, y+size*0.4f); break;
			case 'H':
				_V(x, y); _V(x, y+size);
				_V(x+width, y); _V(x+width, y+size);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f); break;
			case 'I':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y+size);
				_V(x+width*0.2f, y+size); _V(x+width*0.8f, y+size);
				_V(x+width*0.2f, y); _V(x+width*0.8f, y); break;
			case 'J':
				_V(x+width, y+size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x, y); _V(x, y+size*0.3f); break;
			case 'K':
				_V(x, y); _V(x, y+size); // Спинка
				_V(x, y+size*0.5f); _V(x+width, y+size); // Вверх-право
				_V(x, y+size*0.5f); _V(x+width, y); break; // Вниз-право
			case 'L':
				_V(x, y+size); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case 'M':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width*0.5f, y+size*0.5f);
				_V(x+width*0.5f, y+size*0.5f); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y); break;
			case 'N':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y);
				_V(x+width, y); _V(x+width, y+size); break;
			case 'O':
				_V(x, y); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'P':
				_V(x, y); _V(x, y+size); // Спинка
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x, y+size*0.5f); break;
			case 'Q':
				_V(x, y+size*0.2f); _V(x, y+size);
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y+size*0.2f);
				_V(x+width, y+size*0.2f); _V(x, y+size*0.2f);
				_V(x+width*0.5f, y+size*0.4f); _V(x+width, y); break;
			case 'R':
				_V(x, y); _V(x, y+size); // Спинка
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y); break;
			case 'S':
				_V(x+width, y+size); _V(x, y+size);
				_V(x, y+size); _V(x, y+size*0.5f);
				_V(x, y+size*0.5f); _V(x+width, y+size*0.5f);
				_V(x+width, y+size*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'T':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y+size);
				_V(x, y+size); _V(x+width, y+size); break;
			case 'U':
				_V(x, y+size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y+size); break;
			case 'V':
				_V(x, y+size); _V(x+width*0.5f, y);
				_V(x+width*0.5f, y); _V(x+width, y+size); break;
			case 'W':
				_V(x, y+size); _V(x+width*0.25f, y);
				_V(x+width*0.25f, y); _V(x+width*0.5f, y+size*0.4f);
				_V(x+width*0.5f, y+size*0.4f); _V(x+width*0.75f, y);
				_V(x+width*0.75f, y); _V(x+width, y+size); break;
			case 'X':
				_V(x, y); _V(x+width, y+size);
				_V(x, y+size); _V(x+width, y); break;
			case 'Y':
				_V(x, y+size); _V(x+width*0.5f, y+size*0.5f);
				_V(x+width, y+size); _V(x+width*0.5f, y+size*0.5f);
				_V(x+width*0.5f, y+size*0.5f); _V(x+width*0.5f, y); break;
			case 'Z':
				_V(x, y+size); _V(x+width, y+size);
				_V(x+width, y+size); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			// --- ---
			case '[': // 91
				_V(x+width*0.8f, y+size); _V(x+width*0.2f, y+size);
				_V(x+width*0.2f, y+size); _V(x+width*0.2f, y);
				_V(x+width*0.2f, y); _V(x+width*0.8f, y); break;
			case '\\': _V(x, y+size); _V(x+width, y); break; // 92
			case ']': // 93
				_V(x+width*0.2f, y+size); _V(x+width*0.8f, y+size);
				_V(x+width*0.8f, y+size); _V(x+width*0.8f, y);
				_V(x+width*0.8f, y); _V(x+width*0.2f, y); break;
			case '^': // 94
				_V(x+width*0.2f, y+size*0.7f); _V(x+width*0.5f, y+size);
				_V(x+width*0.5f, y+size); _V(x+width*0.8f, y+size*0.7f); break;
			case '_': _V(x, y - desc * 0.5f); _V(x+width, y - desc * 0.5f); break; // 95
			case '`': _V(x+width*0.3f, y+size); _V(x+width*0.6f, y+size*0.8f); break; // 96
			// --- строчные ---	
			case 'a': // Коробочка строчной 'a'
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y+lowRow);
				_V(x+width, y+lowRow); _V(x, y+lowRow);
				_V(x+width, y+lowRow*0.5f); _V(x, y+lowRow*0.5f);
				_V(x, y+lowRow*0.5f); _V(x, y); break;
			case 'b':
				_V(x, y); _V(x, y+size); // Вертикаль во всю высоту (size)
				_V(x, y); _V(x+width, y); // Брюшко в нижнем ряду (lowRow)
				_V(x+width, y); _V(x+width, y+lowRow);
				_V(x+width, y+lowRow); _V(x, y+lowRow); break;
			case 'c': // Только в нижнем ряду
				_V(x+width, y); _V(x, y);
				_V(x, y); _V(x, y+lowRow);
				_V(x, y+lowRow); _V(x+width, y+lowRow); break;
			case 'd': // Для примера, как работает хвост справа
				_V(x+width, y); _V(x+width, y+size);
				_V(x+width, y); _V(x, y);
				_V(x, y); _V(x, y+lowRow);
				_V(x, y+lowRow); _V(x+width, y+lowRow); break;
			case 'e':
                _V(x, y); _V(x+width, y); // Нижняя база
                _V(x, y); _V(x, y+lowRow); // Левая стенка (основной ствол строчной буквы)
                _V(x, y+lowRow); _V(x+width, y+lowRow); // Верхняя "шапка"
                _V(x+width, y+lowRow); _V(x+width, y+lowRow*0.5f); // Правая верхняя стенка (закрывает глаз буквы)
                _V(x+width, y+lowRow*0.5f); _V(x, y+lowRow*0.5f); break; // Средняя перекладина
			case 'f':
                _V(x+width*0.5f, y); _V(x+width*0.5f, y+size); // Ствол
                _V(x+width*0.5f, y+size); _V(x+width, y+size); // Крючок сверху
                _V(x, y+lowRow); _V(x+width, y+lowRow); break; // Перекладина
            case 'g': // С нижним хвостом
				_V(x+width, y+lowRow); _V(x+width, y - desc); // Ствол вниз
				_V(x+width, y - desc); _V(x, y - desc);      // Заворот хвоста
				_V(x, y); _V(x+width, y);                   // Тело (низ)
				_V(x, y); _V(x, y+lowRow);                      // Тело (лево)
				_V(x, y+lowRow); _V(x+width, y+lowRow); break;  // Тело (верх)
            case 'h':
                _V(x, y); _V(x, y+size); // Высокая палка
                _V(x, y+lowRow); _V(x+width, y+lowRow); // Плечо
                _V(x+width, y+lowRow); _V(x+width, y); break; // Ножка
			case 'i':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y+lowRow);  // 1. Тело буквы (основной штрих до уровня lowRow)
				_V(x+width*0.3f, y); _V(x+width*0.7f, y); // 2. Нижняя засечка (горизонтальная линия на базовой линии)
				_V(x+width*0.3f, y+lowRow*0.85f); _V(x+width*0.5f, y+lowRow); // 3. Верхняя засечка (небольшой «носик» или плечо слева)
				_V(x+width*0.5f, y+lowRow + size * 0.15f); _V(x+width*0.5f, y+lowRow + size * 0.25f); break; // 4. Точка (вынесена выше уровня строчных букв)
            case 'j': // С хвостом вниз
				_V(x+width*0.7f, y - desc); _V(x+width*0.7f, y+lowRow*0.8f);
				_V(x+width*0.7f, y - desc); _V(x, y - desc);
				_V(x+width*0.7f, y+lowRow); _V(x+width*0.7f, y+lowRow + size*0.07f); break; // Точка над j
            case 'k':
                _V(x, y); _V(x, y+size); // Спинка
                _V(x, y+lowRow*0.4f); _V(x+width, y+lowRow); // Верхняя ножка
                _V(x, y+lowRow*0.4f); _V(x+width, y); break; // Нижняя ножка
            //case 'l': _V(x+width*0.5f, y); _V(x+width*0.5f, y+size); break;
			case 'l': 
				// 1. Основное вертикальное тело (от низа до самого верха size)
				_V(x+width*0.45f, y+size*0.05f); 
				_V(x+width*0.45f, y+size); 
				// 2. Верхний "носик" (небольшая засечка влево)
				_V(x+width*0.25f, y+size*0.9f); 
				_V(x+width*0.45f, y+size);
				// 3. Нижний "хвостик" (закругление или засечка вправо)
				_V(x+width*0.45f, y); 
				_V(x+width*0.75f, y); 
				break;
            case 'm':
                _V(x, y); _V(x, y+lowRow); // Лево
                _V(x, y+lowRow); _V(x+width*0.5f, y+lowRow); // Плечо 1
                _V(x+width*0.5f, y+lowRow); _V(x+width*0.5f, y); // Середина
                _V(x+width*0.5f, y+lowRow); _V(x+width, y+lowRow); // Плечо 2
                _V(x+width, y+lowRow); _V(x+width, y); break; // Право
            case 'n':
                _V(x, y); _V(x, y+lowRow); // Лево
                _V(x, y+lowRow); _V(x+width, y+lowRow); // Плечо
                _V(x+width, y+lowRow); _V(x+width, y); break; // Право
			case 'o':
				_V(x, y); _V(x, y+lowRow);
				_V(x, y+lowRow); _V(x+width, y+lowRow);
				_V(x+width, y+lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'p':
				_V(x, y+lowRow); _V(x, y - desc); // Ствол вниз
				_V(x, y+lowRow); _V(x+width, y+lowRow);
				_V(x+width, y+lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'q':
				_V(x+width, y+lowRow); _V(x+width, y - desc); // Ствол вниз справа
				_V(x+width, y+lowRow); _V(x, y+lowRow);
				_V(x, y+lowRow); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case 'r':
				_V(x, y); _V(x, y+lowRow); // Ствол
				_V(x, y+lowRow); _V(x+width, y+lowRow); break; // Плечо
			case 's':
				_V(x+width, y+lowRow); _V(x, y+lowRow);
				_V(x, y+lowRow); _V(x, y+lowRow*0.5f);
				_V(x, y+lowRow*0.5f); _V(x+width, y+lowRow*0.5f);
				_V(x+width, y+lowRow*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 't':
				_V(x+width*0.4f, y+size*0.1f); _V(x+width*0.4f, y+size*0.9f);
				_V(x+width*0.4f, y); _V(x+width*0.8f, y);
				_V(x+width*0.4f, y+size*0.1f); _V(x+width*0.4f, y);
				_V(x+width*0.1f, y+lowRow); _V(x+width*0.8f, y+lowRow); break;
			case 'u':
				_V(x, y+lowRow); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y+lowRow); break;
			case 'v':
				_V(x, y+lowRow); _V(x+width*0.5f, y);
				_V(x+width*0.5f, y); _V(x+width, y+lowRow); break;
			case 'w':
				_V(x, y+lowRow); _V(x+width*0.25f, y);
				_V(x+width*0.25f, y); _V(x+width*0.5f, y+lowRow*0.5f);
				_V(x+width*0.5f, y+lowRow*0.5f); _V(x+width*0.75f, y);
				_V(x+width*0.75f, y); _V(x+width, y+lowRow); break;
			case 'x':
				_V(x, y); _V(x+width, y+lowRow);
				_V(x, y+lowRow); _V(x+width, y); break;
			case 'y':
				_V(x, y+lowRow); _V(x+width*0.5f, y);
				_V(x+width, y+lowRow); _V(x, y - desc); break;
			case 'z':
				_V(x, y+lowRow); _V(x+width, y+lowRow);
				_V(x+width, y+lowRow); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			// --- ---
			case '{': // 123
				_V(x+width*0.8f, y+size); _V(x+width*0.5f, y+size*0.9f);
				_V(x+width*0.5f, y+size*0.9f); _V(x+width*0.5f, y+size*0.55f);
				_V(x+width*0.5f, y+size*0.55f); _V(x+width*0.2f, y+size*0.5f); // Носик
				_V(x+width*0.2f, y+size*0.5f); _V(x+width*0.5f, y+size*0.45f);
				_V(x+width*0.5f, y+size*0.45f); _V(x+width*0.5f, y+size*0.1f);
				_V(x+width*0.5f, y+size*0.1f); _V(x+width*0.8f, y); break;
			case '|': _V(x+width*0.5f, y+size); _V(x+width*0.5f, y); break; // 124
			case '}': // 125
				_V(x+width*0.2f, y+size); _V(x+width*0.5f, y+size*0.9f);
				_V(x+width*0.5f, y+size*0.9f); _V(x+width*0.5f, y+size*0.55f);
				_V(x+width*0.5f, y+size*0.55f); _V(x+width*0.8f, y+size*0.5f); // Носик
				_V(x+width*0.8f, y+size*0.5f); _V(x+width*0.5f, y+size*0.45f);
				_V(x+width*0.5f, y+size*0.45f); _V(x+width*0.5f, y+size*0.1f);
				_V(x+width*0.5f, y+size*0.1f); _V(x+width*0.2f, y); break;
			case '~': // 126
				_V(x, y+lowRow*0.5f); _V(x+width*0.25f, y+lowRow*0.8f);
				_V(x+width*0.25f, y+lowRow*0.8f); _V(x+width*0.75f, y+lowRow*0.2f);
				_V(x+width*0.75f, y+lowRow*0.2f); _V(x+width, y+lowRow*0.5f); break;
			default: case '?': // 63
				_V(x, y+size*0.7f); _V(x+width*0.5f, y+size);
				_V(x+width*0.5f, y+size); _V(x+width, y+size*0.7f);
				_V(x+width, y+size*0.7f); _V(x+width*0.5f, y+size*0.4f);
				_V(x+width*0.5f, y+size*0.4f); _V(x+width*0.5f, y+size*0.2f);
				// Точка
				_V(x+width*0.5f, y); _V(x+width*0.5f, y+size*0.07f); break;
        } }
	
	/*void _getUTF8SymbolVertexes(Array<Vertex>& v, int point, float x, float y, float size, unsigned int color, float aspect=1.0f){
		float width = size * 0.6f / aspect; float lowRow = size*0.6f; float desc = size*0.25f;
		switch(point){ case ' ': break; } }
	
	void _getSymbolVertexes(Array<Vertex>& v, int point, float x, float y, float size, unsigned int color, float aspect=1.0f);*/
		
#undef _V
	void RenderString(const CString& cs, float x, float y, float size=0.1f, unsigned int color=0xFFFFFFFF, float aspect=1.0f,
		const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef)
	{   Array<Vertex> vertices(cs.size()*4); float cX=x; //x+=(size*0.02f); size_t pos=0,point=0; 
		for(size_t i=0; i<cs.size(); ++i){ if(cs[i]=='\n'){ cX=x; y-=size+(size*0.40f); continue; }
			_getASCIISymbolVertexes(vertices, cs[i], cX, y, size, color, aspect); cX += size*0.8f/aspect; }
		//while((point=UTF8NextPoint(cs.c_str(), cs.size(), pos))!=-1){ _getSymbolVertexes(vertices, cs[i], cX, y, size, color); cX += size*0.8f/aspect; }
        RenderLines(vertices, mvp, shinfo);
	}
	
} }