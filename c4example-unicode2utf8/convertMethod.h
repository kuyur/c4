#include <string>

bool match(const char *src, unsigned int src_length)
{
	if (NULL == src)
		return false;
	if (src_length == 0)
		return false;
	if ((src_length&1) != 0)
		return false;

	return true;
}

unsigned int calcUtf8StringLength(const char *src, unsigned int src_length, bool is_little_endian)
{
	if (NULL == src)
		return 0;
	if (src_length == 0)
		return 0;
	// if src_length is an odd number
	if ((src_length&1) != 0)
		return 0;

	const unsigned char *unsignedSrc = (unsigned char *)src;
	wchar_t chr = 0;
	unsigned int dest_len = 0;
	for (unsigned int i=0;i<src_length;)
	{
		if (is_little_endian)
			chr = *(unsignedSrc+i) + (*(unsignedSrc+i+1))*256;
		else
			chr = *(unsignedSrc+i+1) + (*(unsignedSrc+i))*256;
		i+=2;

		if (chr <= 0x007F)  // U-0000 - U-007F
		{
			dest_len += 1;
		} else if (chr <= 0x07FF) {  // U-0080 - U-07FF
			dest_len += 2;
		} else if (chr >= 0xD800 && chr <= 0xDFFF) {  // U-D800 - U-DFFF
			dest_len += 0;
		} else {  // U-0800 - UD7FF and UE000 - UFFFF
			dest_len += 3;
		}
	}

	return dest_len;
}

std::string convert2utf8_pushback(const char *src, unsigned int src_length, bool is_little_endian)
{
	std::string utf8str;
	utf8str.clear();

	if (NULL == src)
		return utf8str;
	if (src_length == 0)
		return utf8str;
	// if src_length is an odd number
	if ((src_length&1) != 0)
		return utf8str;

	const unsigned char *unsignedSrc = (unsigned char *)src;
	wchar_t chr = 0;
	char dest_chars[3];
	memset((void*)dest_chars, 0, 3);
	unsigned int dest_len;
	for (unsigned int i=0;i<src_length;)
	{
		dest_len = 0;
		if (is_little_endian)
			chr = *(unsignedSrc+i) + (*(unsignedSrc+i+1))*256;
		else
			chr = *(unsignedSrc+i+1) + (*(unsignedSrc+i))*256;
		i+=2;

		if (chr <= 0x007F)  // U-0000 - U-007F
		{
			dest_len = 1;
			dest_chars[0] = (char)chr;
		} else if (chr <= 0x07FF) {  // U-0080 - U-07FF
			dest_len = 2;
			dest_chars[0] = (char)(0xC0 | (chr>>6));
			dest_chars[1] = (char)(0x80 | (chr&0x003F));
		} else if (chr >= 0xD800 && chr <= 0xDFFF) {  // U-D800 - U-DFFF
			// ignore this unicode character
			dest_len = 0;
		} else {  // U-0800 - UD7FF and UE000 - UFFFF
			dest_len = 3;
			dest_chars[0] = (char)(0xE0 | (chr>>12));
			dest_chars[1] = (char)(0x80 | ((chr>>6) & 0x003F));
			dest_chars[2] = (char)(0x80 | (chr & 0x003F));
		}
		for (unsigned int j=0;j<dest_len;j++)
		{
			utf8str.push_back(dest_chars[j]);
		}
	}

	return utf8str;
}

std::string convert2utf8_copy(const char *src, unsigned int src_length, bool is_little_endian)
{
	if (NULL == src)
		return std::string();
	if (src_length == 0)
		return std::string();
	// if src_length is an odd number
	if ((src_length&1) != 0)
		return std::string();

	unsigned int need_length = calcUtf8StringLength(src, src_length, is_little_endian);
	char* dest = new char[need_length+1];
	char* offset = dest;
	memset((void*)dest, 0, need_length+1);

	const unsigned char *unsignedSrc = (unsigned char *)src;
	wchar_t chr = 0;
	char dest_chars[3];
	memset((void*)dest_chars, 0, 3);
	unsigned int dest_len;
	for (unsigned int i=0;i<src_length;)
	{
		dest_len = 0;
		if (is_little_endian)
			chr = *(unsignedSrc+i) + (*(unsignedSrc+i+1))*256;
		else
			chr = *(unsignedSrc+i+1) + (*(unsignedSrc+i))*256;
		i+=2;

		if (chr <= 0x007F)  // U-0000 - U-007F
		{
			dest_len = 1;
			dest_chars[0] = (char)chr;
		} else if (chr <= 0x07FF) {  // U-0080 - U-07FF
			dest_len = 2;
			dest_chars[0] = (char)(0xC0 | (chr>>6));
			dest_chars[1] = (char)(0x80 | (chr&0x003F));
		} else if (chr >= 0xD800 && chr <= 0xDFFF) {  // U-D800 - U-DFFF
			// ignore this unicode character
			dest_len = 0;
		} else {  // U-0800 - UD7FF and UE000 - UFFFF
			dest_len = 3;
			dest_chars[0] = (char)(0xE0 | (chr>>12));
			dest_chars[1] = (char)(0x80 | ((chr>>6) & 0x003F));
			dest_chars[2] = (char)(0x80 | (chr & 0x003F));
		}
		if (dest_len>0)
		{
			memcpy(offset, dest_chars, dest_len);
			offset+=dest_len;
		}
	}

	std::string utf8str(dest);
	delete []dest;
	return utf8str;
}

std::string convert2utf8_allocate(const char *src, unsigned int src_length, bool is_little_endian)
{
	if (NULL == src)
		return std::string();
	if (src_length == 0)
		return std::string();
	// if src_length is an odd number
	if ((src_length&1) != 0)
		return std::string();

	unsigned int need_length = calcUtf8StringLength(src, src_length, is_little_endian);
	std::string utf8str(need_length, 0);
	unsigned int offset = 0;

	const unsigned char *unsignedSrc = (unsigned char *)src;
	wchar_t chr = 0;
	char dest_chars[3];
	memset((void*)dest_chars, 0, 3);
	unsigned int dest_len;
	for (unsigned int i=0;i<src_length;)
	{
		dest_len = 0;
		if (is_little_endian)
			chr = *(unsignedSrc+i) + (*(unsignedSrc+i+1))*256;
		else
			chr = *(unsignedSrc+i+1) + (*(unsignedSrc+i))*256;
		i+=2;

		if (chr <= 0x007F)  // U-0000 - U-007F
		{
			dest_len = 1;
			dest_chars[0] = (char)chr;
		} else if (chr <= 0x07FF) {  // U-0080 - U-07FF
			dest_len = 2;
			dest_chars[0] = (char)(0xC0 | (chr>>6));
			dest_chars[1] = (char)(0x80 | (chr&0x003F));
		} else if (chr >= 0xD800 && chr <= 0xDFFF) {  // U-D800 - U-DFFF
			// ignore this unicode character
			dest_len = 0;
		} else {  // U-0800 - UD7FF and UE000 - UFFFF
			dest_len = 3;
			dest_chars[0] = (char)(0xE0 | (chr>>12));
			dest_chars[1] = (char)(0x80 | ((chr>>6) & 0x003F));
			dest_chars[2] = (char)(0x80 | (chr & 0x003F));
		}
		for (unsigned int j=0;j<dest_len;j++)
			utf8str[offset++] = dest_chars[j];
	}

	return utf8str;
}

bool convert2utf8(const char *src, unsigned int src_length, char *dest, unsigned int dest_length, bool is_little_endian, bool check_dest_length=false)
{
	if (NULL == src || NULL == dest)
		return false;
	if (0 == src_length || 0 == dest_length)
		return false;
	if (!(dest>src+src_length || src>dest+dest_length))
		return false;
	if ((src_length&1) != 0)
		return false;
	if (check_dest_length)
	{
		if (dest_length < calcUtf8StringLength(src, src_length, is_little_endian))
			return false;
	}

	unsigned int offset = 0;
	const unsigned char *unsignedSrc = (unsigned char *)src;
	wchar_t chr = 0;
	memset((void*)dest, 0, dest_length);
	for (unsigned int i=0;i<src_length;)
	{
		if (is_little_endian)
			chr = *(unsignedSrc+i) + (*(unsignedSrc+i+1))*256;
		else
			chr = *(unsignedSrc+i+1) + (*(unsignedSrc+i))*256;
		i+=2;

		if (chr <= 0x007F)  // U-0000 - U-007F
		{
			*(dest + offset++) = (char)chr;
		} else if (chr <= 0x07FF) {  // U-0080 - U-07FF
			*(dest + offset++) = (char)(0xC0 | (chr>>6));
			*(dest + offset++) = (char)(0x80 | (chr&0x003F));
		} else if (chr >= 0xD800 && chr <= 0xDFFF) {  // U-D800 - U-DFFF
			// ignore this unicode character
		} else {  // U-0800 - UD7FF and UE000 - UFFFF
			*(dest + offset++) = (char)(0xE0 | (chr>>12));
			*(dest + offset++) = (char)(0x80 | ((chr>>6) & 0x003F));
			*(dest + offset++) = (char)(0x80 | (chr & 0x003F));
		}
	}

	return true;
}