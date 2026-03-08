namespace ncpp { namespace GL {
	
	 void _getSymbolVertexes(Array<Vertex>& v, int symb, float x, float y, float size, unsigned int color, float aspect=1.0f){
        float width = size * 0.6f / aspect; float lowRow = size*0.6f; float desc = size*0.25f; //float z=0;
		switch(symb){ 
			case ' ': break;
			case '!': // 33
                v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.25f, 0, color)); // Палка
                v.push(Vertex(x + width * 0.5f, y + size * 0.07f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break; // Точка
            case '"': // 34
                v.push(Vertex(x + width * 0.3f, y + size, 0, color)); v.push(Vertex(x + width * 0.3f, y + size * 0.75f, 0, color));
                v.push(Vertex(x + width * 0.7f, y + size, 0, color)); v.push(Vertex(x + width * 0.7f, y + size * 0.75f, 0, color)); break;
            case '#': // 35
                v.push(Vertex(x + width * 0.3f, y + size, 0, color)); v.push(Vertex(x + width * 0.3f, y, 0, color));
                v.push(Vertex(x + width * 0.7f, y + size, 0, color)); v.push(Vertex(x + width * 0.7f, y, 0, color));
                v.push(Vertex(x, y + size * 0.65f, 0, color)); v.push(Vertex(x + width, y + size * 0.65f, 0, color));
                v.push(Vertex(x, y + size * 0.35f, 0, color)); v.push(Vertex(x + width, y + size * 0.35f, 0, color)); break;
            case '$': // 36
                v.push(Vertex(x + width * 0.5f, y + size * 1.1f, 0, color)); v.push(Vertex(x + width * 0.5f, y - size * 0.1f, 0, color)); // Вертикаль
                v.push(Vertex(x + width, y + size * 0.85f, 0, color)); v.push(Vertex(x, y + size * 0.85f, 0, color)); // S-образные линии
                v.push(Vertex(x, y + size * 0.85f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
                v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
                v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.15f, 0, color));
                v.push(Vertex(x + width, y + size * 0.15f, 0, color)); v.push(Vertex(x, y + size * 0.15f, 0, color)); break;
            case '%': // 37
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); // Диагональ
                v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.2f, y + size, 0, color)); // Точка 1
                v.push(Vertex(x + width * 0.8f, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Точка 2
            case '&': // 38
                v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
            case '\'': v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.75f, 0, color)); break; //39
            case '(': // 40
                v.push(Vertex(x + width * 0.8f, y + size, 0, color)); v.push(Vertex(x + width * 0.3f, y + size * 0.8f, 0, color));
                v.push(Vertex(x + width * 0.3f, y + size * 0.8f, 0, color)); v.push(Vertex(x + width * 0.3f, y + size * 0.2f, 0, color));
                v.push(Vertex(x + width * 0.3f, y + size * 0.2f, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color)); break;
            case ')': // 41
                v.push(Vertex(x + width * 0.2f, y + size, 0, color)); v.push(Vertex(x + width * 0.7f, y + size * 0.8f, 0, color));
                v.push(Vertex(x + width * 0.7f, y + size * 0.8f, 0, color)); v.push(Vertex(x + width * 0.7f, y + size * 0.2f, 0, color));
                v.push(Vertex(x + width * 0.7f, y + size * 0.2f, 0, color)); v.push(Vertex(x + width * 0.2f, y, 0, color)); break;
            case '*': // 42
                v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow * 0.2f, 0, color));
                v.push(Vertex(x + width * 0.1f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.9f, y + lowRow * 0.4f, 0, color));
                v.push(Vertex(x + width * 0.9f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.1f, y + lowRow * 0.4f, 0, color)); break;
            case '+': // 43
                v.push(Vertex(x + width * 0.5f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow * 0.2f, 0, color));
                v.push(Vertex(x + width * 0.1f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.9f, y + lowRow * 0.5f, 0, color)); break;
            case ',': // 44
                v.push(Vertex(x + width * 0.5f, y + size * 0.1f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color));
                v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.3f, y - desc * 0.5f, 0, color)); break;
            case '-': v.push(Vertex(x + width * 0.1f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.9f, y + lowRow * 0.5f, 0, color)); break; // 45
            case '.':  v.push(Vertex(x + width * 0.5f, y + size * 0.07f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break; // 46
            case '/': v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break; // 47
			// --- цифры ---
			case '0':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case '1':
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color));
				v.push(Vertex(x + width * 0.2f, y + size * 0.7f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color)); break;
			case '2':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case '3':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color)); break;
			case '4':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case '5':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case '6':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color)); break;
			case '7':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break;
			case '8':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color)); break;
			case '9':
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color)); break;
			// --- ---
			case ':': // 58
                v.push(Vertex(x + width * 0.5f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow * 0.7f, 0, color));
                v.push(Vertex(x + width * 0.5f, y + size * 0.07f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break;
            case ';': // 59
                v.push(Vertex(x + width * 0.5f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow * 0.7f, 0, color));
                // Запятая снизу
                v.push(Vertex(x + width * 0.5f, y + size * 0.07f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color));
                v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.3f, y - desc * 0.5f, 0, color)); break;
            case '<': // 60
                v.push(Vertex(x + width * 0.9f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.1f, y + lowRow * 0.5f, 0, color));
                v.push(Vertex(x + width * 0.1f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.9f, y, 0, color)); break;
            case '=': // 61
                v.push(Vertex(x, y + lowRow * 0.7f, 0, color)); v.push(Vertex(x + width, y + lowRow * 0.7f, 0, color));
                v.push(Vertex(x, y + lowRow * 0.3f, 0, color)); v.push(Vertex(x + width, y + lowRow * 0.3f, 0, color)); break;
            case '>': // 62
                v.push(Vertex(x + width * 0.1f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.9f, y + lowRow * 0.5f, 0, color));
                v.push(Vertex(x + width * 0.9f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.1f, y, 0, color)); break;
            case '@': // 64 (упрощенная "собака")
                v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Внешний контур
                v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x, y, 0, color));
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width * 0.7f, y, 0, color));
                v.push(Vertex(x + width * 0.7f, y, 0, color)); v.push(Vertex(x + width * 0.7f, y + lowRow * 0.5f, 0, color)); // Внутренняя "a"
                v.push(Vertex(x + width * 0.7f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.3f, y + lowRow * 0.5f, 0, color)); break;
			// --- прописные ---
			case 'A':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color)); // Линия 1: левая палка (снизу вверх)
				v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color)); // Линия 2: правая палка (сверху вниз)
				v.push(Vertex(x + width * 0.2f, y + size * 0.4f, 0, color)); v.push(Vertex(x + width * 0.8f, y + size * 0.4f, 0, color)); break; // Линия 3: перекладина
			case 'B':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Вертикаль
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.75f, 0, color));
				v.push(Vertex(x + width, y + size * 0.75f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.25f, 0, color));
				v.push(Vertex(x + width, y + size * 0.25f, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'C':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'E':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.7f, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'F':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.7f, y + size * 0.5f, 0, color)); break;
			case 'D':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Спинка
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.8f, y + size * 0.8f, 0, color));
				v.push(Vertex(x + width * 0.8f, y + size * 0.8f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.8f, y + size * 0.2f, 0, color));
				v.push(Vertex(x + width * 0.8f, y + size * 0.2f, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'G':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Верх
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y, 0, color)); // Лево
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); // Низ
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size * 0.4f, 0, color)); // Хвостик вверх
				v.push(Vertex(x + width, y + size * 0.4f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color)); break;
			case 'H':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color)); break;
			case 'I':
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color));
				v.push(Vertex(x + width * 0.2f, y + size, 0, color)); v.push(Vertex(x + width * 0.8f, y + size, 0, color));
				v.push(Vertex(x + width * 0.2f, y, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color)); break;
			case 'J':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size * 0.3f, 0, color)); break;
			case 'K':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Спинка
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); // Вверх-право
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Вниз-право
			case 'L':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'M':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'N':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break;
			case 'O':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'P':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Спинка
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color)); break;
			case 'Q':
				v.push(Vertex(x, y + size * 0.2f, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.2f, 0, color));
				v.push(Vertex(x + width, y + size * 0.2f, 0, color)); v.push(Vertex(x, y + size * 0.2f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'R':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Спинка
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'S':
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y + size * 0.5f, 0, color));
				v.push(Vertex(x, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size * 0.5f, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'T':
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break;
			case 'U':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break;
			case 'V':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color));
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break;
			case 'W':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.25f, y, 0, color));
				v.push(Vertex(x + width * 0.25f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color)); v.push(Vertex(x + width * 0.75f, y, 0, color));
				v.push(Vertex(x + width * 0.75f, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); break;
			case 'X':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'Y':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.5f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break;
			case 'Z':
				v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y + size, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			// --- ---
			case '[': // 91
				v.push(Vertex(x + width * 0.8f, y + size, 0, color)); v.push(Vertex(x + width * 0.2f, y + size, 0, color));
				v.push(Vertex(x + width * 0.2f, y + size, 0, color)); v.push(Vertex(x + width * 0.2f, y, 0, color));
				v.push(Vertex(x + width * 0.2f, y, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color)); break;
			case '\\': v.push(Vertex(x, y + size, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // 92
			case ']': // 93
				v.push(Vertex(x + width * 0.2f, y + size, 0, color)); v.push(Vertex(x + width * 0.8f, y + size, 0, color));
				v.push(Vertex(x + width * 0.8f, y + size, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color));
				v.push(Vertex(x + width * 0.8f, y, 0, color)); v.push(Vertex(x + width * 0.2f, y, 0, color)); break;
			case '^': // 94
				v.push(Vertex(x + width * 0.2f, y + size * 0.7f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width * 0.8f, y + size * 0.7f, 0, color)); break;
			case '_': v.push(Vertex(x, y - desc * 0.5f, 0, color)); v.push(Vertex(x + width, y - desc * 0.5f, 0, color)); break; // 95
			case '`': v.push(Vertex(x + width * 0.3f, y + size, 0, color)); v.push(Vertex(x + width * 0.6f, y + size * 0.8f, 0, color)); break; // 96
			// --- строчные ---	
			case 'a': // Коробочка строчной 'a'
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x, y + lowRow * 0.5f, 0, color));
				v.push(Vertex(x, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'b':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Вертикаль во всю высоту (size)
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); // Брюшко в нижнем ряду (lowRow)
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow, 0, color)); break;
			case 'c': // Только в нижнем ряду
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;
			case 'd': // Для примера, как работает хвост справа
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + size, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;
			case 'e':
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); // Нижняя база
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color)); // Левая стенка (основной ствол строчной буквы)
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Верхняя "шапка"
                v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color)); // Правая верхняя стенка (закрывает глаз буквы)
                v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x, y + lowRow * 0.5f, 0, color)); break; // Средняя перекладина
			case 'f':
                v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color)); // Ствол
                v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width, y + size, 0, color)); // Крючок сверху
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break; // Перекладина
            case 'g': // С нижним хвостом
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y - desc, 0, color)); // Ствол вниз
				v.push(Vertex(x + width, y - desc, 0, color)); v.push(Vertex(x, y - desc, 0, color));      // Заворот хвоста
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color));                   // Тело (низ)
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));                      // Тело (лево)
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;  // Тело (верх)
            case 'h':
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Высокая палка
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Плечо
                v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Ножка
			case 'i':
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color));  // 1. Тело буквы (основной штрих до уровня lowRow)
				v.push(Vertex(x + width * 0.3f, y, 0, color)); v.push(Vertex(x + width * 0.7f, y, 0, color)); // 2. Нижняя засечка (горизонтальная линия на базовой линии)
				v.push(Vertex(x + width * 0.3f, y + lowRow * 0.85f, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color)); // 3. Верхняя засечка (небольшой «носик» или плечо слева)
				v.push(Vertex(x + width * 0.5f, y + lowRow + size * 0.15f, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow + size * 0.25f, 0, color)); break; // 4. Точка (вынесена выше уровня строчных букв)
            case 'j': // С хвостом вниз
				v.push(Vertex(x + width * 0.7f, y - desc, 0, color)); v.push(Vertex(x + width * 0.7f, y + lowRow * 0.8f, 0, color));
				v.push(Vertex(x + width * 0.7f, y - desc, 0, color)); v.push(Vertex(x, y - desc, 0, color));
				v.push(Vertex(x + width * 0.7f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.7f, y + lowRow + size*0.07f, 0, color)); break; // Точка над j
            case 'k':
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + size, 0, color)); // Спинка
                v.push(Vertex(x, y + lowRow * 0.4f, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Верхняя ножка
                v.push(Vertex(x, y + lowRow * 0.4f, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Нижняя ножка
            //case 'l': v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color)); break;
			case 'l': 
				// 1. Основное вертикальное тело (от низа до самого верха size)
				v.push(Vertex(x + width * 0.45f, y + size * 0.05f, 0, color)); 
				v.push(Vertex(x + width * 0.45f, y + size, 0, color)); 

				// 2. Верхний "носик" (небольшая засечка влево)
				v.push(Vertex(x + width * 0.25f, y + size * 0.9f, 0, color)); 
				v.push(Vertex(x + width * 0.45f, y + size, 0, color)); 

				// 3. Нижний "хвостик" (закругление или засечка вправо)
				v.push(Vertex(x + width * 0.45f, y, 0, color)); 
				v.push(Vertex(x + width * 0.75f, y, 0, color)); 
				break;
            case 'm':
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color)); // Лево
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color)); // Плечо 1
                v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); // Середина
                v.push(Vertex(x + width * 0.5f, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Плечо 2
                v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Право
            case 'n':
                v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color)); // Лево
                v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); // Плечо
                v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color)); break; // Право
			case 'o':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'p':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x, y - desc, 0, color)); // Ствол вниз
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 'q':
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x + width, y - desc, 0, color)); // Ствол вниз справа
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'r':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x, y + lowRow, 0, color)); // Ствол
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break; // Плечо
			case 's':
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x, y + lowRow * 0.5f, 0, color));
				v.push(Vertex(x, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color));
				v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x, y, 0, color)); break;
			case 't':
				v.push(Vertex(x + width * 0.4f, y + size * 0.1f, 0, color)); v.push(Vertex(x + width * 0.4f, y + size * 0.9f, 0, color));
				v.push(Vertex(x + width * 0.4f, y, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color));
				v.push(Vertex(x + width * 0.4f, y + size * 0.1f, 0, color)); v.push(Vertex(x + width * 0.4f, y, 0, color));
				v.push(Vertex(x + width * 0.1f, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.8f, y + lowRow, 0, color)); break;
			case 'u':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color));
				v.push(Vertex(x + width, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;
			case 'v':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color));
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;
			case 'w':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.25f, y, 0, color));
				v.push(Vertex(x + width * 0.25f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + lowRow * 0.5f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.75f, y, 0, color));
				v.push(Vertex(x + width * 0.75f, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color)); break;
			case 'x':
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			case 'y':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y - desc, 0, color)); break;
			case 'z':
				v.push(Vertex(x, y + lowRow, 0, color)); v.push(Vertex(x + width, y + lowRow, 0, color));
				v.push(Vertex(x + width, y + lowRow, 0, color)); v.push(Vertex(x, y, 0, color));
				v.push(Vertex(x, y, 0, color)); v.push(Vertex(x + width, y, 0, color)); break;
			// --- ---
			case '{': // 123
				v.push(Vertex(x + width * 0.8f, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.9f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.9f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.55f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.55f, 0, color)); v.push(Vertex(x + width * 0.2f, y + size * 0.5f, 0, color)); // Носик
				v.push(Vertex(x + width * 0.2f, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.45f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.45f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.1f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.1f, 0, color)); v.push(Vertex(x + width * 0.8f, y, 0, color)); break;
			case '|': v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y, 0, color)); break; // 124
			case '}': // 125
				v.push(Vertex(x + width * 0.2f, y + size, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.9f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.9f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.55f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.55f, 0, color)); v.push(Vertex(x + width * 0.8f, y + size * 0.5f, 0, color)); // Носик
				v.push(Vertex(x + width * 0.8f, y + size * 0.5f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.45f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.45f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.1f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.1f, 0, color)); v.push(Vertex(x + width * 0.2f, y, 0, color)); break;
			case '~': // 126
				v.push(Vertex(x, y + lowRow * 0.5f, 0, color)); v.push(Vertex(x + width * 0.25f, y + lowRow * 0.8f, 0, color));
				v.push(Vertex(x + width * 0.25f, y + lowRow * 0.8f, 0, color)); v.push(Vertex(x + width * 0.75f, y + lowRow * 0.2f, 0, color));
				v.push(Vertex(x + width * 0.75f, y + lowRow * 0.2f, 0, color)); v.push(Vertex(x + width, y + lowRow * 0.5f, 0, color)); break;
			default: case '?': // 63
				v.push(Vertex(x, y + size * 0.7f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size, 0, color)); v.push(Vertex(x + width, y + size * 0.7f, 0, color));
				v.push(Vertex(x + width, y + size * 0.7f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color));
				v.push(Vertex(x + width * 0.5f, y + size * 0.4f, 0, color)); v.push(Vertex(x + width * 0.5f, y + size * 0.2f, 0, color));
				// Точка
				v.push(Vertex(x + width * 0.5f, y, 0, color)); v.push(Vertex(x + width * 0.5f, y + size*0.07f, 0, color)); break;
        } }
	
	void DrawString(const CString& cs, float x, float y, float size=0.1f, unsigned int color=0xFFFFFF, float aspect=1.0f,
		const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef)
	{   Array<Vertex> vertices(cs.size()*4); float cX=x; //x+=(size*0.02f); size_t pos=0,point=0; 
		for(size_t i = 0; i < cs.size(); ++i){ 
			if(cs[i]=='\n'){ cX=x; y-=size+(size*0.40f); continue; }
			_getSymbolVertexes(vertices, cs[i], cX, y, size, color, aspect); cX += size * 0.8f / aspect; }
		//while((point=UTF8NextPoint(cs.c_str(), cs.size(), 0))!=-1){ _getSymbolVertexes(vertices, cs[i], cX, y, size, color); cX += (size * 0.7f)/aspect; }
        RenderLines(vertices, mvp, shinfo);
	}
	
} }