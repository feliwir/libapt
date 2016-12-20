#pragma once
#include <algorithm>
#include <string>
#include <functional> 
#include <cctype>
#include <locale>

namespace libapt
{

	template <class T>
	inline T read(uint8_t *&buf)
	{
		T result;
		result = *(T *)buf;
		buf += sizeof(T);
		return result;
	}

	template <class T>
	inline void skip(uint8_t *&buf)
	{
		buf += sizeof(T);
	}

	inline char getc(uint8_t*& buf)
	{
		char r = *buf;
		buf++;
		return r;
	}

	inline std::string readString(uint8_t*& buf)
	{
		std::string result;
		char c;
		while (c = getc(buf))
			result.push_back(c);

		return result;
	}

	inline std::string readString(const uint8_t* buf)
	{
		std::string result;
		int i = 0;
		char c;
		while (c = static_cast<char>(buf[i]))
		{
			result.push_back(c);
			++i;
		}
			

		return result;
	}

	inline std::string readString(uint8_t*& buf, int size)
	{
		std::string result;
		result.reserve(size);
		for (int i = 0;i < size;++i)
			result.push_back(getc(buf));

		return result;
	}

	inline void reverse(uint32_t &v)
	{
		v = (v << 24) | (v << 8 & 0xff0000) | (v >> 8 & 0xff00) | (v >> 24);
	}


	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}


	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}
}