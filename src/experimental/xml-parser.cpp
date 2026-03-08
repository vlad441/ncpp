namespace ncpp { namespace XML {

String serialize(const StringMap& map, const CString rootname="xml"){
    String xml; xml << "<" << rootname << ">\n";

    for(StringMap::ConstIter it=map.begin(); it!=map.end(); ++it){
        xml << "  <" << it->first;
        if(it->second.size() == 0){ xml+="/>\n"; }
		else{ xml << ">" << it->second << "</" << it->first << ">\n"; }
    } xml << "</" << rootname << ">\n"; return xml; }

/*StringMap parseSimple(const CString& xml){
    StringMap map; const char* curr = xml.data(); const char* end = xml.ptr() + xml.size();

    while(curr < end){
        const char* openTagStart = strchr(curr, '<'); if(!openTagStart) break;
        const char* openTagEnd = strchr(openTagStart + 1, '>'); if(!openTagEnd) break;

        CString keyName(openTagStart + 1, openTagEnd - (openTagStart + 1)); // Извлекаем имя ключа
        
        // Проверяем на самозакрывающийся тег
        if(openTagEnd[-1] == '/'){
            CString realKey(openTagStart + 1, (openTagEnd - 1) - (openTagStart + 1));
            map[realKey]=""; curr = openTagEnd + 1; continue; }

        // Ищем закрывающий тег
        char closeTagStr[256]; // Буфер для закрывающего тега
        ncpp::snprintf(closeTagStr, sizeof(closeTagStr), "</%s>", keyName.ptr());
        const char* closeTagStart = strstr(openTagEnd + 1, closeTagStr);
        
        if (!closeTagStart){ break; }

        CString value(openTagEnd + 1, closeTagStart - (openTagEnd + 1)); // Извлекаем значение
        
        map[keyName]=value; curr = closeTagStart + ncpp::strlen(closeTagStr);
    } return map; }*/

} }
