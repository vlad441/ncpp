#ifndef NCPP_H_HEX_COLORS_FUNCS
#define NCPP_H_HEX_COLORS_FUNCS //Y = 0.2126R + 0.7152G + 0.0722B
unsigned int HexReverse(unsigned int hex){ return ((hex & 0xFF000000) >> 24) | ((hex & 0x00FF0000) >> 8) | ((hex & 0x0000FF00) << 8)  | ((hex & 0x000000FF) << 24); }
// --- RGB --- Вход: 0xRRGGBB -> Выход: [R, G, B]
void HexToRGB(unsigned char rgb[3], unsigned int hex){ rgb[0] = (hex >> 16) & 0xFF; rgb[1] = (hex >> 8) & 0xFF; rgb[2] = hex & 0xFF; }
unsigned int RGBToHex(unsigned char rgb[3]){ return ((unsigned int)rgb[0] << 16) | ((unsigned int)rgb[1] << 8) | (unsigned int)rgb[2]; }

void HexToRGBf(float rgb[3], unsigned int hex){ rgb[0] = ((hex >> 16) & 0xFF) / 255.0f; rgb[1] = ((hex >> 8) & 0xFF) / 255.0f; rgb[2] = (hex & 0xFF) / 255.0f; }
unsigned int RGBfToHex(float rgb[3]){ return ((unsigned int)(rgb[0] * 255.0f) << 16) | ((unsigned int)(rgb[1] * 255.0f) << 8) | (unsigned int)(rgb[2] * 255.0f); }
#ifdef __cplusplus
unsigned int RGBfToHex(float r, float g, float b){ float rgb[3]={r,g,b}; return RGBfToHex(rgb); }
#endif
// --- RGBA --- Вход: 0xAARRGGBB -> Выход: [R, G, B, A]
void HexToRGBA(unsigned char rgba[4], unsigned int hex){ rgba[0] = (hex >> 16) & 0xFF; rgba[1] = (hex >> 8) & 0xFF; rgba[2] = hex & 0xFF; rgba[3] = (hex >> 24) & 0xFF; }
unsigned int RGBAToHex(unsigned char rgba[4]){ return ((unsigned int)rgba[3] << 24) | ((unsigned int)rgba[0] << 16) | ((unsigned int)rgba[1] << 8) | (unsigned int)rgba[2]; }

void HexToRGBAf(float rgba[4], unsigned int hex){ rgba[0] = ((hex >> 16) & 0xFF) / 255.0f; rgba[1] = ((hex >> 8) & 0xFF) / 255.0f; 
	rgba[2] = (hex & 0xFF) / 255.0f; rgba[3] = ((hex >> 24) & 0xFF) / 255.0f; }
unsigned int RGBAfToHex(float rgba[4]){ return ((unsigned int)(rgba[3] * 255.0f + 0.5f) << 24) | ((unsigned int)(rgba[0] * 255.0f + 0.5f) << 16) |
	((unsigned int)(rgba[1] * 255.0f + 0.5f) << 8) | (unsigned int)(rgba[2] * 255.0f + 0.5f); }
#ifdef __cplusplus
unsigned int RGBAfToHex(float r, float g, float b, float a = 1.0f){ float rgba[4] = {r, g, b, a}; return RGBAfToHex(rgba); }
#endif
#endif