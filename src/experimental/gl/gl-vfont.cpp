namespace ncpp { namespace GL {
#define _V(PX, PY) v.push(Vertex(PX, PY, 0, color)) //макрос для сокращения длины инструкций.
#define _ASCII(point) _getASCIISymbolVertices(v, point, x, y, size, color, aspect)

	void _getASCIISymbolVertices(Array<Vertex>& v, int symb, float x, float y, float size, unsigned int color, float aspect=1.0f){
        float width = size * 0.6f / aspect; float lowRow = size*0.6f; float desc = size*0.25f;
		switch(symb){ 
			case ' ': break;
			case '!': // 33
                _V(x+width*0.5f, y-size); _V(x+width*0.5f, y-size*0.25f);
                _V(x+width*0.5f, y-size*0.07f); _V(x+width*0.5f, y); break;
            case '"': // 34
                _V(x+width*0.3f, y-size); _V(x+width*0.3f, y-size*0.75f);
                _V(x+width*0.7f, y-size); _V(x+width*0.7f, y-size*0.75f); break;
            case '#': // 35
                _V(x+width*0.3f, y-size); _V(x+width*0.3f, y);
                _V(x+width*0.7f, y-size); _V(x+width*0.7f, y);
                _V(x, y-size*0.65f); _V(x+width, y-size*0.65f);
                _V(x, y-size*0.35f); _V(x+width, y-size*0.35f); break;
            case '$': // 36
                _V(x+width*0.5f, y-size*1.1f); _V(x+width*0.5f, y + size * 0.1f);
                _V(x+width, y-size*0.85f); _V(x, y-size*0.85f);
                _V(x, y-size*0.85f); _V(x, y-size*0.5f);
                _V(x, y-size*0.5f); _V(x+width, y-size*0.5f);
                _V(x+width, y-size*0.5f); _V(x+width, y-size*0.15f);
                _V(x+width, y-size*0.15f); _V(x, y-size*0.15f); break;
            case '%': // 37
                _V(x, y); _V(x+width, y-size);
                _V(x, y-size); _V(x+width*0.2f, y-size);
                _V(x+width*0.8f, y); _V(x+width, y); break;
            case '&': // 38
                _V(x+width, y); _V(x, y-size);
                _V(x, y); _V(x+width, y-size);
                _V(x, y); _V(x+width, y); break;
            case '\'': _V(x+width*0.5f, y-size); _V(x+width*0.5f, y-size*0.75f); break; //39
            case '(': // 40
                _V(x+width*0.8f, y-size); _V(x+width*0.3f, y-size*0.8f);
                _V(x+width*0.3f, y-size*0.8f); _V(x+width*0.3f, y-size*0.2f);
                _V(x+width*0.3f, y-size*0.2f); _V(x+width*0.8f, y); break;
            case ')': // 41
                _V(x+width*0.2f, y-size); _V(x+width*0.7f, y-size*0.8f);
                _V(x+width*0.7f, y-size*0.8f); _V(x+width*0.7f, y-size*0.2f);
                _V(x+width*0.7f, y-size*0.2f); _V(x+width*0.2f, y); break;
            case '*': // 42
                _V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y-lowRow*0.2f);
                _V(x+width*0.1f, y-lowRow*0.8f); _V(x+width*0.9f, y-lowRow*0.4f);
                _V(x+width*0.9f, y-lowRow*0.8f); _V(x+width*0.1f, y-lowRow*0.4f); break;
            case '+': // 43
                _V(x+width*0.5f, y-lowRow*0.8f); _V(x+width*0.5f, y-lowRow*0.2f);
                _V(x+width*0.1f, y-lowRow*0.5f); _V(x+width*0.9f, y-lowRow*0.5f); break;
            case ',': // 44
                _V(x+width*0.5f, y-size*0.1f); _V(x+width*0.5f, y);
                _V(x+width*0.5f, y); _V(x+width*0.3f, y + desc * 0.5f); break;
            case '-': _V(x+width*0.1f, y-lowRow*0.5f); _V(x+width*0.9f, y-lowRow*0.5f); break; // 45
            case '.':  _V(x+width*0.5f, y-size*0.07f); _V(x+width*0.5f, y); break; // 46
            case '/': _V(x, y); _V(x+width, y-size); break; // 47
			// --- цифры ---
			case '0':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x+width, y-size); _V(x, y); break;
			case '1':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-size);
				_V(x+width*0.2f, y-size*0.7f); _V(x+width*0.5f, y-size); break;
			case '2':
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case '3':
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f); break;
			case '4':
				_V(x, y-size); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f);
				_V(x+width, y-size); _V(x+width, y); break;
			case '5':
				_V(x+width, y-size); _V(x, y-size);
				_V(x, y-size); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case '6':
				_V(x+width, y-size); _V(x, y-size);
				_V(x, y-size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f); break;
			case '7': _V(x, y-size); _V(x+width, y-size); _V(x+width, y-size); _V(x+width*0.5f, y); break;
			case '8':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f); break;
			case '9':
				_V(x+width, y); _V(x+width, y-size);
				_V(x+width, y-size); _V(x, y-size);
				_V(x, y-size); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f); break;
			// --- ---
			case ':': // 58
                _V(x+width*0.5f, y-lowRow*0.8f); _V(x+width*0.5f, y-lowRow*0.7f);
                _V(x+width*0.5f, y-size*0.07f); _V(x+width*0.5f, y); break;
            case ';': // 59
                _V(x+width*0.5f, y-lowRow*0.8f); _V(x+width*0.5f, y-lowRow*0.7f);
                _V(x+width*0.5f, y-size*0.07f); _V(x+width*0.5f, y);
                _V(x+width*0.5f, y); _V(x+width*0.3f, y + desc * 0.5f); break;
            case '<': // 60
                _V(x+width*0.9f, y-lowRow); _V(x+width*0.1f, y-lowRow*0.5f);
                _V(x+width*0.1f, y-lowRow*0.5f); _V(x+width*0.9f, y); break;
            case '=': // 61
                _V(x, y-lowRow*0.7f); _V(x+width, y-lowRow*0.7f);
                _V(x, y-lowRow*0.3f); _V(x+width, y-lowRow*0.3f); break;
            case '>': // 62
                _V(x+width*0.1f, y-lowRow); _V(x+width*0.9f, y-lowRow*0.5f);
                _V(x+width*0.9f, y-lowRow*0.5f); _V(x+width*0.1f, y); break;
            case '@': // 64
                _V(x+width, y); _V(x+width, y-lowRow);
                _V(x+width, y-lowRow); _V(x, y-lowRow);
                _V(x, y-lowRow); _V(x, y);
                _V(x, y); _V(x+width*0.7f, y);
                _V(x+width*0.7f, y); _V(x+width*0.7f, y-lowRow*0.5f);
                _V(x+width*0.7f, y-lowRow*0.5f); _V(x+width*0.3f, y-lowRow*0.5f); break;
			// --- прописные ---
			case 'A': _V(x, y); _V(x+width*0.5f, y-size); _V(x+width*0.5f, y-size); _V(x+width, y); 
				_V(x+width*0.2f, y-size*0.4f); _V(x+width*0.8f, y-size*0.4f); break;
			case 'B':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size*0.75f);
				_V(x+width, y-size*0.75f); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.25f);
				_V(x+width, y-size*0.25f); _V(x, y); break;
			case 'C': _V(x+width, y-size); _V(x, y-size); _V(x, y-size); _V(x, y); _V(x, y); _V(x+width, y); break;
			case 'E':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x, y-size*0.5f); _V(x+width*0.7f, y-size*0.5f);
				_V(x, y); _V(x+width, y); break;
			case 'F': _V(x, y); _V(x, y-size); _V(x, y-size); _V(x+width, y-size); _V(x, y-size*0.5f); _V(x+width*0.7f, y-size*0.5f); break;
			case 'D':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width*0.8f, y-size*0.8f);
				_V(x+width*0.8f, y-size*0.8f); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x+width*0.8f, y-size*0.2f);
				_V(x+width*0.8f, y-size*0.2f); _V(x, y); break;
			case 'G':
				_V(x+width, y-size); _V(x, y-size);
				_V(x, y-size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size*0.4f);
				_V(x+width, y-size*0.4f); _V(x+width*0.5f, y-size*0.4f); break;
			case 'H': _V(x, y); _V(x, y-size); _V(x+width, y); _V(x+width, y-size); _V(x, y-size*0.5f); _V(x+width, y-size*0.5f); break;
			case 'I': _V(x+width*0.5f, y); _V(x+width*0.5f, y-size); _V(x+width*0.2f, y-size); _V(x+width*0.8f, y-size); 
				_V(x+width*0.2f, y); _V(x+width*0.8f, y); break;
			case 'J': _V(x+width, y-size); _V(x+width, y); _V(x+width, y); _V(x, y); _V(x, y); _V(x, y-size*0.3f); break;
			case 'K': _V(x, y); _V(x, y-size); _V(x, y-size*0.5f); _V(x+width, y-size); _V(x, y-size*0.5f); _V(x+width, y); break;
			case 'L': _V(x, y-size); _V(x, y); _V(x, y); _V(x+width, y); break;
			case 'M':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width*0.5f, y-size*0.5f);
				_V(x+width*0.5f, y-size*0.5f); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y); break;
			case 'N': _V(x, y); _V(x, y-size); _V(x, y-size); _V(x+width, y); _V(x+width, y); _V(x+width, y-size); break;
			case 'O':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'P':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f); break;
			case 'Q':
				_V(x, y-size*0.2f); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y-size*0.2f);
				_V(x+width, y-size*0.2f); _V(x, y-size*0.2f);
				_V(x+width*0.5f, y-size*0.4f); _V(x+width, y); break;
			case 'R':
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y); break;
			case 'S':
				_V(x+width, y-size); _V(x, y-size);
				_V(x, y-size); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'T': _V(x+width*0.5f, y); _V(x+width*0.5f, y-size); _V(x, y-size); _V(x+width, y-size); break;
			case 'U': _V(x, y-size); _V(x, y); _V(x, y); _V(x+width, y); _V(x+width, y); _V(x+width, y-size); break;
			case 'V': _V(x, y-size); _V(x+width*0.5f, y); _V(x+width*0.5f, y); _V(x+width, y-size); break;
			case 'W':
				_V(x, y-size); _V(x+width*0.25f, y);
				_V(x+width*0.25f, y); _V(x+width*0.5f, y-size*0.4f);
				_V(x+width*0.5f, y-size*0.4f); _V(x+width*0.75f, y);
				_V(x+width*0.75f, y); _V(x+width, y-size); break;
			case 'X': _V(x, y); _V(x+width, y-size); _V(x, y-size); _V(x+width, y); break;
			case 'Y':
				_V(x, y-size); _V(x+width*0.5f, y-size*0.5f);
				_V(x+width, y-size); _V(x+width*0.5f, y-size*0.5f);
				_V(x+width*0.5f, y-size*0.5f); _V(x+width*0.5f, y); break;
			case 'Z': _V(x, y-size); _V(x+width, y-size); _V(x+width, y-size); _V(x, y); _V(x, y); _V(x+width, y); break;
			// --- ---
			case '[': // 91
				_V(x+width*0.8f, y-size); _V(x+width*0.2f, y-size);
				_V(x+width*0.2f, y-size); _V(x+width*0.2f, y);
				_V(x+width*0.2f, y); _V(x+width*0.8f, y); break;
			case '\\': _V(x, y-size); _V(x+width, y); break; // 92
			case ']': // 93
				_V(x+width*0.2f, y-size); _V(x+width*0.8f, y-size);
				_V(x+width*0.8f, y-size); _V(x+width*0.8f, y);
				_V(x+width*0.8f, y); _V(x+width*0.2f, y); break;
			case '^': // 94
				_V(x+width*0.2f, y-size*0.7f); _V(x+width*0.5f, y-size);
				_V(x+width*0.5f, y-size); _V(x+width*0.8f, y-size*0.7f); break;
			case '_': _V(x, y + desc * 0.5f); _V(x+width, y + desc * 0.5f); break; // 95
			case '`': _V(x+width*0.3f, y-size); _V(x+width*0.6f, y-size*0.8f); break; // 96
			// --- строчные ---	
			case 'a':
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x, y-lowRow);
				_V(x+width, y-lowRow*0.5f); _V(x, y-lowRow*0.5f);
				_V(x, y-lowRow*0.5f); _V(x, y); break;
			case 'b':
				_V(x, y); _V(x, y-size);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x, y-lowRow); break;
			case 'c':
				_V(x+width, y); _V(x, y);
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width, y-lowRow); break;
			case 'd':
				_V(x+width, y); _V(x+width, y-size);
				_V(x+width, y); _V(x, y);
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width, y-lowRow); break;
			case 'e':
                _V(x, y); _V(x+width, y);
                _V(x, y); _V(x, y-lowRow);
                _V(x, y-lowRow); _V(x+width, y-lowRow);
                _V(x+width, y-lowRow); _V(x+width, y-lowRow*0.5f);
                _V(x+width, y-lowRow*0.5f); _V(x, y-lowRow*0.5f); break;
			case 'f':
                _V(x+width*0.5f, y); _V(x+width*0.5f, y-size);
                _V(x+width*0.5f, y-size); _V(x+width, y-size);
                _V(x, y-lowRow); _V(x+width, y-lowRow); break;
            case 'g':
				_V(x+width, y-lowRow); _V(x+width, y+desc);
				_V(x+width, y+desc); _V(x, y+desc);
				_V(x, y); _V(x+width, y);
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width, y-lowRow); break;
            case 'h':
                _V(x, y); _V(x, y-size);
                _V(x, y-lowRow); _V(x+width, y-lowRow);
                _V(x+width, y-lowRow); _V(x+width, y); break;
			case 'i':
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-lowRow);
				_V(x+width*0.3f, y); _V(x+width*0.7f, y);
				_V(x+width*0.3f, y-lowRow*0.85f); _V(x+width*0.5f, y-lowRow);
				_V(x+width*0.5f, y-lowRow - size * 0.15f); _V(x+width*0.5f, y-lowRow - size * 0.25f); break;
            case 'j':
				_V(x+width*0.7f, y + desc); _V(x+width*0.7f, y-lowRow*0.8f);
				_V(x+width*0.7f, y + desc); _V(x, y + desc);
				_V(x+width*0.7f, y-lowRow); _V(x+width*0.7f, y-lowRow - size*0.07f); break;
            case 'k':
                _V(x, y); _V(x, y-size);
                _V(x, y-lowRow*0.4f); _V(x+width, y-lowRow);
                _V(x, y-lowRow*0.4f); _V(x+width, y); break;
			case 'l': 
				_V(x+width*0.45f, y-size*0.05f); 
				_V(x+width*0.45f, y-size); 
				_V(x+width*0.25f, y-size*0.9f); 
				_V(x+width*0.45f, y-size);
				_V(x+width*0.45f, y); 
				_V(x+width*0.75f, y); 
				break;
            case 'm':
                _V(x, y); _V(x, y-lowRow);
                _V(x, y-lowRow); _V(x+width*0.5f, y-lowRow);
                _V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y);
                _V(x+width*0.5f, y-lowRow); _V(x+width, y-lowRow);
                _V(x+width, y-lowRow); _V(x+width, y); break;
            case 'n':
                _V(x, y); _V(x, y-lowRow);
                _V(x, y-lowRow); _V(x+width, y-lowRow);
                _V(x+width, y-lowRow); _V(x+width, y); break;
			case 'o':
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'p':
				_V(x, y-lowRow); _V(x, y + desc);
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 'q':
				_V(x+width, y-lowRow); _V(x+width, y + desc);
				_V(x+width, y-lowRow); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			case 'r': _V(x, y); _V(x, y-lowRow); _V(x, y-lowRow); _V(x+width, y-lowRow); break;
			case 's':
				_V(x+width, y-lowRow); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x, y-lowRow*0.5f);
				_V(x, y-lowRow*0.5f); _V(x+width, y-lowRow*0.5f);
				_V(x+width, y-lowRow*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 't':
				_V(x+width*0.4f, y-size*0.1f); _V(x+width*0.4f, y-size*0.9f);
				_V(x+width*0.4f, y); _V(x+width*0.8f, y);
				_V(x+width*0.4f, y-size*0.1f); _V(x+width*0.4f, y);
				_V(x+width*0.1f, y-lowRow); _V(x+width*0.8f, y-lowRow); break;
			case 'u':
				_V(x, y-lowRow); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow); break;
			case 'v': _V(x, y-lowRow); _V(x+width*0.5f, y); _V(x+width*0.5f, y); _V(x+width, y-lowRow); break;
			case 'w':
				_V(x, y-lowRow); _V(x+width*0.25f, y);
				_V(x+width*0.25f, y); _V(x+width*0.5f, y-lowRow*0.5f);
				_V(x+width*0.5f, y-lowRow*0.5f); _V(x+width*0.75f, y);
				_V(x+width*0.75f, y); _V(x+width, y-lowRow); break;
			case 'x': _V(x, y); _V(x+width, y-lowRow); _V(x, y-lowRow); _V(x+width, y); break;
			case 'y': _V(x, y-lowRow); _V(x+width*0.5f, y); _V(x+width, y-lowRow); _V(x, y + desc); break;
			case 'z':
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x, y);
				_V(x, y); _V(x+width, y); break;
			// --- ---
			case '{': // 123
				_V(x+width*0.8f, y-size); _V(x+width*0.5f, y-size*0.9f);
				_V(x+width*0.5f, y-size*0.9f); _V(x+width*0.5f, y-size*0.55f);
				_V(x+width*0.5f, y-size*0.55f); _V(x+width*0.2f, y-size*0.5f);
				_V(x+width*0.2f, y-size*0.5f); _V(x+width*0.5f, y-size*0.45f);
				_V(x+width*0.5f, y-size*0.45f); _V(x+width*0.5f, y-size*0.1f);
				_V(x+width*0.5f, y-size*0.1f); _V(x+width*0.8f, y); break;
			case '|': _V(x+width*0.5f, y-size); _V(x+width*0.5f, y); break; // 124
			case '}': // 125
				_V(x+width*0.2f, y-size); _V(x+width*0.5f, y-size*0.9f);
				_V(x+width*0.5f, y-size*0.9f); _V(x+width*0.5f, y-size*0.55f);
				_V(x+width*0.5f, y-size*0.55f); _V(x+width*0.8f, y-size*0.5f);
				_V(x+width*0.8f, y-size*0.5f); _V(x+width*0.5f, y-size*0.45f);
				_V(x+width*0.5f, y-size*0.45f); _V(x+width*0.5f, y-size*0.1f);
				_V(x+width*0.5f, y-size*0.1f); _V(x+width*0.2f, y); break;
			case '~': // 126
				_V(x, y-lowRow*0.5f); _V(x+width*0.25f, y-lowRow*0.8f);
				_V(x+width*0.25f, y-lowRow*0.8f); _V(x+width*0.75f, y-lowRow*0.2f);
				_V(x+width*0.75f, y-lowRow*0.2f); _V(x+width, y-lowRow*0.5f); break;
			default: case '?': // 63
				_V(x, y-size*0.7f); _V(x+width*0.5f, y-size);
				_V(x+width*0.5f, y-size); _V(x+width, y-size*0.7f);
				_V(x+width, y-size*0.7f); _V(x+width*0.5f, y-size*0.4f);
				_V(x+width*0.5f, y-size*0.4f); _V(x+width*0.5f, y-size*0.2f);
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-size*0.07f); break;
	    } }
	
	void _getUTF8SymbolVertices(Array<Vertex>& v, int point, float x, float y, float size, unsigned int color, float aspect=1.0f){
		if(point<128){ return _ASCII(point); } float width = size * 0.6f / aspect; float lowRow = size*0.6f; float desc = size*0.25f;
		
		switch(point){ 
			// --- Кирилица: Прописные ---
			case 1040: return _ASCII('A'); //А
			case 1041: // Б
				_V(x, y); _V(x, y-size);
				_V(x, y-size*0.6f); _V(x+width, y-size*0.3f);
				_V(x+width, y-size*0.3f); _V(x, y);
				_V(x, y-size); _V(x+width, y-size); break;
				break;
			case 1042: return _ASCII('B'); // В
			case 1043: // Г
				_V(x, y-size); _V(x+width, y-size); _V(x, y-size); _V(x, y); break;
			case 1044: // Д
				_V(x, y); _V(x+width, y);
				_V(x + width * 0.1f, y); _V(x + width * 0.5f, y-size);
				_V(x + width * 0.9f, y); _V(x + width * 0.5f, y-size);
				_V(x, y); _V(x, y+desc*0.8f); _V(x+width, y); _V(x+width, y+desc*0.8f); break;
			case 1045: return _ASCII('E'); // Е
			case 1025: // Ё
				_V(x, y); _V(x, y-size);
				_V(x, y-size); _V(x+width, y-size);
				_V(x, y-size*0.5f); _V(x+width*0.7f, y-size*0.5f);
				_V(x, y); _V(x+width, y);
				_V(x+width*0.3f, y-size*1.2f); _V(x+width*0.3f, y-size*1.1f);
				_V(x+width*0.7f, y-size*1.2f); _V(x+width*0.7f, y-size*1.1f); break;
			case 1046: // Ж
				_V(x, y-size); _V(x+width, y);
				_V(x, y); _V(x+width, y-size);
				_V(x+width*0.5f, y-size); _V(x+width*0.5f, y); break;
            case 1047: // З
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width*0.5f, y-size*0.5f);
				_V(x+width*0.5f, y-size*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 1048: // И
				_V(x, y); _V(x, y-size); _V(x, y); _V(x+width, y-size); _V(x+width, y-size); _V(x+width, y); break;
			case 1049: // Й
				_V(x, y); _V(x, y-size); _V(x, y); _V(x+width, y-size);	_V(x+width, y-size); _V(x+width, y);
				_V(x+width*0.2f, y-size*1.2f); _V(x+width*0.5f, y-size*1.0f);
				_V(x+width*0.5f, y-size*1.0f); _V(x+width*0.8f, y-size*1.2f); break;
			case 1050: return _ASCII('K'); // К
			case 1051: // Л
				_V(x+width*0.5f, y-size); _V(x, y); _V(x+width*0.5f, y-size); _V(x+width, y); break;
			case 1052: return _ASCII('M'); // М
			case 1053: return _ASCII('H'); // Н
			case 1054: return _ASCII('O'); // О
			case 1055: // П
				_V(x, y); _V(x, y-size);
				_V(x+width, y); _V(x+width, y-size);
				_V(x, y-size); _V(x+width, y-size); break;
			case 1056: return _ASCII('P'); // Р
			case 1057: return _ASCII('C'); // С
			case 1058: return _ASCII('T'); // Т
			case 1059: // У
				_V(x, y-size); _V(x+width*0.5f, y-size*0.4f); _V(x+width*0.2f, y); _V(x+width, y-size); break;
			case 1060: // Ф
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-size);
				_V(x, y-size*0.4f); _V(x+width, y-size*0.4f);
				_V(x, y-size*0.8f); _V(x+width, y-size*0.8f);
				_V(x, y-size*0.4f); _V(x, y-size*0.8f); 
				_V(x+width, y-size*0.4f); _V(x+width, y-size*0.8f); break;
			case 1061: return _ASCII('X'); // Х
			case 1062: // Ц
				_V(x, y-size); _V(x, y);
				_V(x+width, y-size); _V(x+width, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y + desc * 0.5f); break;
			case 1063: // Ч
				_V(x, y-size); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x+width, y-size*0.5f);
				_V(x+width, y-size); _V(x+width, y); break;
			case 1064: // Ш
				_V(x, y-size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size);
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-size); break;
			case 1065: // Щ
				_V(x, y-size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size);
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-size);
				_V(x+width, y); _V(x+width, y+desc*0.8f); break;
			case 1066: // Ъ
				_V(x+width*0.2f, y); _V(x+width*0.2f, y-size);
				_V(x+width*0.2f, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x+width*0.2f, y-size*0.5f); _V(x+width*0.2f, y-size); _V(x, y-size*0.9f); break;
			case 1067: // Ы
				_V(x, y); _V(x, y-size);
				_V(x, y); _V(x+width*0.8f, y);
				_V(x+width*0.8f, y); _V(x+width*0.8f, y-size*0.5f);
				_V(x+width*0.8f, y-size*0.5f); _V(x, y-size*0.5f); _V(x+width, y); _V(x+width, y-size); break;
			case 1068: // Ь
				_V(x, y); _V(x, y-size);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-size*0.5f);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f); break;
			case 1069: // Э
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x+width, y-size*0.5f); _V(x+0.3f*width, y-size*0.5f); break;
			case 1070: // Ю
				_V(x, y-size); _V(x, y);
				_V(x, y-size*0.5f); _V(x+width*0.5f, y-size*0.5f);
				_V(x+width*0.5f, y-size); _V(x+width*0.5f, y);
				_V(x+width*0.5f, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y); _V(x+width*0.5f, y); break;
			case 1071: // Я
				_V(x, y-size); _V(x+width, y-size);
				_V(x+width, y-size); _V(x+width, y);
				_V(x+width, y-size*0.5f); _V(x, y-size*0.5f);
				_V(x, y-size*0.5f); _V(x, y-size);
				_V(x+width, y-size*0.5f); _V(x, y); break;
			// --- Кирилица: Строчные ---
			case 1072: return _ASCII('a'); // а
			case 1073: // б
				_V(x, y-size); _V(x+width, y-size);
				_V(x, y-size); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x, y-lowRow); break;
			case 1074: // в
				_V(x, y-lowRow); _V(x, y);
				_V(x, y-lowRow); _V(x+width, y-lowRow*0.75f);
				_V(x+width, y-lowRow*0.75f); _V(x, y-lowRow*0.5f);
				_V(x, y-lowRow*0.5f); _V(x+width, y-lowRow*0.25f);
				_V(x+width, y-lowRow*0.25f); _V(x, y); break;
			case 1075: // г
				_V(x, y-lowRow); _V(x+width, y-lowRow); _V(x, y-lowRow); _V(x, y); break;
			case 1076: // д
				_V(x, y); _V(x+width, y);
				_V(x+width*0.1f, y); _V(x+width*0.5f, y-lowRow);
				_V(x+width*0.9f,y); _V(x+width*0.5f, y-lowRow);
				_V(x, y); _V(x, y+desc*0.8f); _V(x+width, y); _V(x+width, y+desc*0.8f); break;
			case 1077: return _ASCII('e'); // е
			case 1105: // ё
				_V(x, y); _V(x+width, y);
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y-lowRow*0.5f);
				_V(x+width, y-lowRow*0.5f); _V(x, y-lowRow*0.5f);
				_V(x+width*0.3f, y-lowRow*1.3f); _V(x+width*0.3f, y-lowRow*1.2f);
				_V(x+width*0.7f, y-lowRow*1.3f); _V(x+width*0.7f, y-lowRow*1.2f); break;
			case 1078: // ж
				_V(x, y-lowRow); _V(x+width, y);
				_V(x, y); _V(x+width, y-lowRow);
				_V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y); break;
			case 1079: // з
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width*0.5f, y-lowRow*0.5f);
				_V(x+width*0.5f, y-lowRow*0.5f); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 1080: // и
				_V(x, y); _V(x, y-lowRow); _V(x, y); _V(x+width, y-lowRow); _V(x+width, y-lowRow); _V(x+width, y); break;
			case 1081: // й
				_V(x, y); _V(x, y-lowRow); _V(x, y); _V(x+width, y-lowRow); _V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width*0.2f, y-lowRow*1.2f); _V(x+width*0.5f, y-lowRow*1.0f);
				_V(x+width*0.5f, y-lowRow*1.0f); _V(x+width*0.8f, y-lowRow*1.2f); break;
			case 1082: // к
				_V(x, y); _V(x, y-lowRow); _V(x, y-lowRow*0.5f); _V(x+width, y-lowRow); _V(x, y-lowRow*0.5f); _V(x+width, y); break;
			case 1083: // л
				_V(x+width*0.5f, y-lowRow); _V(x, y); _V(x+width*0.5f, y-lowRow); _V(x+width, y); break;
			case 1084: // м
				_V(x, y); _V(x, y-lowRow);
				_V(x, y-lowRow); _V(x+width*0.5f, y-lowRow*0.5f);
				_V(x+width*0.5f, y-lowRow*0.5f); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y); break;			
			case 1085: // н
				_V(x, y); _V(x, y-lowRow); _V(x+width, y); _V(x+width, y-lowRow); _V(x, y-lowRow*0.5f); _V(x+width, y-lowRow*0.5f); break;
			case 1086: return _ASCII('o'); // о
			case 1087: return _ASCII('n'); // п (похоже на n)
			case 1088: // р
				_V(x, y-lowRow); _V(x, y + desc);
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y); break;
			case 1089: return _ASCII('c'); // с
			case 1090: // т
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-lowRow); _V(x, y-lowRow); _V(x+width, y-lowRow); break;
			case 1091: // у
				_V(x, y-lowRow); _V(x+width*0.5f, y-lowRow*0.2f);
				_V(x+width*0.2f, y+desc*0.8f); _V(x+width, y-lowRow);
				break;
			case 1092: // ф
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-lowRow);
				_V(x, y-lowRow*0.3f); _V(x+width, y-lowRow*0.3f);
				_V(x, y-lowRow*0.7f); _V(x+width, y-lowRow*0.7f);
				_V(x, y-lowRow*0.3f); _V(x, y-lowRow*0.7f);
				_V(x+width, y-lowRow*0.3f); _V(x+width, y-lowRow*0.7f); break;
			case 1093: return _ASCII('x'); // х
			case 1094: // ц
				_V(x, y-lowRow); _V(x, y);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y + desc); break;
			case 1095: // ч
				_V(x, y-lowRow); _V(x, y-lowRow*0.5f);
				_V(x, y-lowRow*0.5f); _V(x+width, y-lowRow*0.5f);
				_V(x+width, y-lowRow); _V(x+width, y); break;
			case 1096: // ш
				_V(x, y-lowRow); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow);
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-lowRow); break;
			case 1097: // щ
				_V(x, y-lowRow); _V(x, y);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow);
				_V(x+width*0.5f, y); _V(x+width*0.5f, y-lowRow);
				_V(x+width, y); _V(x+width, y+desc*0.8f); break;
			case 1098: // ъ
				_V(x+width*0.2f, y); _V(x+width*0.2f, y-lowRow);
				_V(x+width*0.2f, y); _V(x+width, y); 
				_V(x+width, y); _V(x+width, y-lowRow*0.5f);
				_V(x+width, y-lowRow*0.5f); _V(x+width*0.2f, y-lowRow*0.5f); _V(x+width*0.2f, y-lowRow); _V(x, y-lowRow*0.9f); break;
			case 1099: // ы
				_V(x, y); _V(x, y-lowRow);
				_V(x, y); _V(x+width*0.8f, y);
				_V(x+width*0.8f, y); _V(x+width*0.8f, y-lowRow*0.5f);
				_V(x+width*0.8f, y-lowRow*0.5f); _V(x, y-lowRow*0.5f); _V(x+width, y); _V(x+width, y-lowRow); break;
			case 1100: // ь
				_V(x, y); _V(x, y-lowRow);
				_V(x, y); _V(x+width, y);
				_V(x+width, y); _V(x+width, y-lowRow*0.5f);
				_V(x+width, y-lowRow*0.5f); _V(x, y-lowRow*0.5f); break;
			case 1101: // э
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y); _V(x, y);
				_V(x+width, y-lowRow*0.5f); _V(x+0.3f*width, y-lowRow*0.5f); break;
			case 1102: // ю
				_V(x, y-lowRow); _V(x, y);
				_V(x, y-lowRow*0.5f); _V(x+width*0.5f, y-lowRow*0.5f);
				_V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y);
				_V(x+width*0.5f, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y); _V(x+width*0.5f, y); break;
			case 1103: // я
				_V(x, y-lowRow); _V(x+width, y-lowRow);
				_V(x+width, y-lowRow); _V(x+width, y);
				_V(x+width, y-lowRow*0.5f); _V(x, y-lowRow*0.5f);
				_V(x, y-lowRow*0.5f); _V(x, y-lowRow);
				_V(x+width, y-lowRow*0.5f); _V(x, y); break;
			// --- Кирилица: Украинские: Прописные ---
			case 1028: return _ASCII('E'); // Є
			case 1030: return _ASCII('I'); // І
			case 1031: // Ї
				_V(x+width*0.5f, y-size); _V(x+width*0.5f, y);
				_V(x+width*0.3f, y-size*1.2f); _V(x+width*0.3f, y-size*1.1f);
				_V(x+width*0.7f, y-size*1.2f); _V(x+width*0.7f, y-size*1.1f); break;
			case 1168: // Ґ
				_V(x, y-size*0.9f); _V(x+width, y-size*0.9f); _V(x, y-size*0.9f); _V(x, y); 
				_V(x+width, y-size*0.9f); _V(x+width, y-size*1.1f);
				break;
			// --- Кирилица: Украинские: Строчные ---
			case 1108: return _ASCII('e'); // є
			case 1110: return _ASCII('i'); // і
				_V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y);
				_V(x+width*0.5f, y-lowRow*1.2f); _V(x+width*0.5f, y-lowRow*1.15f); break;
			case 1111: // ї
				_V(x+width*0.5f, y-lowRow); _V(x+width*0.5f, y);
				_V(x+width*0.3f, y-lowRow*1.3f); _V(x+width*0.3f, y-lowRow*1.2f);
				_V(x+width*0.7f, y-lowRow*1.3f); _V(x+width*0.7f, y-lowRow*1.2f); break;
			case 1169: // ґ
				_V(x, y-lowRow); _V(x+width, y-lowRow); _V(x, y-lowRow); _V(x, y); 
				_V(x+width, y-lowRow); _V(x+width, y-lowRow-desc*0.8f); break;
			// --- Прочее ---
			default: return _ASCII('?');
		} }
		
#undef _V
#undef _ASCII
	void genString(Array<Vertex>& l, const CString& cs, float x, float y, float size=0.1f, unsigned int color=0xFFFFFFFF, float aspect=1.0f)
	{   float cX=x; int point=0; size_t pos=0;
		//for(size_t i=0; i<cs.size(); ++i){ if(cs[i]=='\n'){ cX=x; y+=size+(size*0.40f); continue; }
			//_getASCIISymbolVertices(l, cs[i], cX, y, size, color, aspect); cX += size*0.8f/aspect; }
		while((point=UTF8Next(cs.c_str(), cs.size(), pos))!=-1){ if(point=='\n'){ cX=x; y+=size+(size*0.40f); continue; }
			_getUTF8SymbolVertices(l, point, cX, y, size, color, aspect); cX += size*0.8f/aspect; }
	}
	
	void RenderString(const CString& cs, float x, float y, float size=0.1f, unsigned int color=0xFFFFFFFF, float bold=0.0f, float aspect=1.0f,
		const Matrix4& mvp=Matrix4().setMatrix2DPreset(), const ShaderInfo& shinfo=Shaders::shDef)
	{	Array<Vertex> v; v.reserve(cs.size()*4); genString(v, cs, x, y, size, color, aspect);
		if(bold>0.0f){ Array<Vertex> boldLn; genLines(boldLn, v, bold); RenderTriangles(boldLn, mvp, shinfo); }
		else{ RenderLines(v, mvp, shinfo); } }
	
} }