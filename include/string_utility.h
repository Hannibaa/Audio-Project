/*
 *****************************************************************************
 *                                                                           *
 *                          Platform Independent                             *
 *                        String  function  Library                          *
 *                                                                           *
 * Author: KADDA Aoues - haoues_k@yahoo.fr - 2023                            *
 * Under namespace : sfl                                                     *
 * URL :                                                                     *
 *                                                                           *
 * Copyright notice:                                                         *
 * Free use of the Library CFile                                             *
 * is permitted under the guidelines and in accordance with the most current *
 * version of the MIT License.                                               *
 * http://www.opensource.org/licenses/MIT                                    *
 *                                                                           *
 *****************************************************************************
*/

/*
     namesspaces :
          conv : convertion between strings type, types ...
          splite : put string in some order, splite it ...
          type   : types defined for string


*/


#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sstream>
#include <utility>
#include <algorithm>
#include <chrono>
#include <vector>
#include <array>
#include <memory>
#include <filesystem>
#include <fstream>
#include <random>
#include <map>
#include <unordered_set>
#include <cwctype>


namespace fs = std::filesystem;

// MACROS

#ifndef CHAR_DEFINITION
#define CHAR_DEFINITION

#define KA_END             '\n'
#define KA_BELL            '\a'
#define KA_TAB             '\t'
#define KA_VTAB            '\v'
#define KA_BACKSPACE       '\b'
#define KA_NEWPAGE         '\f'
#define KA_CR              '\r'
#define KA_LF			   '\n'
#define KA_ESCAPE          '\x1b'
#define KA_DELETE          '\x7F'
#define KA_CTRL_C          (char(3))
#define KA_SPACE		   32

#endif // !CHAR_DEFINITION

#ifdef END
#undef END
#endif // END

#define sfl_splitString		sfl::make_vecLines

namespace sfl::conv{
	std::string wstr_to_str(const std::wstring & wstr);
}

namespace sfl {

	// convert string to uppper case
	std::string to_upper(std::string str)
	{
		for (int i = 0; i < str.size(); ++i) str[i] = std::toupper(str[i]);
		return str;
	}
	std::wstring to_upper(std::wstring str)
	{
		for (int i = 0; i < str.size(); ++i) str[i] = std::toupper(str[i]);
		return str;

	}
	// convert string to lower case
	std::string to_lower(std::string str)
	{
		for (int i = 0; i < str.size(); ++i) str[i] = std::tolower(str[i]);
		return str;
	}

	enum STR_FORMAT {
		STR_SEC_MS,
		STR_MIN_SEC_MS,
		STR_HOUR_MIN_SEC_MS
	};
	// convert milliseconds to hours:minutes:seconds:millis
	std::string to_string(std::chrono::milliseconds ms) {

		namespace chr = std::chrono;

		// Break downd into compounents
		auto hours = chr::duration_cast<chr::hours>(ms);
		ms -= hours;

		auto minites = chr::duration_cast<chr::minutes>(ms);
		ms -= minites;

		auto seconds = chr::duration_cast<chr::seconds>(ms);
		ms -= seconds;

		// Format using stringstream
		std::stringstream ss;
		ss << std::setfill('0')
			<< std::setw(2) << hours.count() << ":"
			<< std::setw(2) << minites.count() << ":"
			<< std::setw(2) << seconds.count() << ":"
			<< std::setw(3) << ms.count();

		return ss.str();
	}

	std::string to_string(std::chrono::milliseconds ms, STR_FORMAT format) {
		namespace chr = std::chrono;
		std::stringstream ss;
		auto seconds = chr::duration_cast<chr::seconds>(ms);

		switch (format) {
		case STR_SEC_MS:
			ms -= seconds;

			// Format using stringstream
			ss << std::setfill('0')
				<< std::setw(2) << seconds.count() << ":"
				<< std::setw(3) << ms.count();

			return ss.str();

		case STR_MIN_SEC_MS:
			auto minites = chr::duration_cast<chr::minutes>(ms);
			ms -= minites;

			seconds = chr::duration_cast<chr::seconds>(ms);
			ms -= seconds;

			// Format using stringstream
			ss << std::setfill('0')
				<< std::setw(2) << minites.count() << ":"
				<< std::setw(2) << seconds.count() << ":"
				<< std::setw(3) << ms.count();

			return ss.str();

		case STR_HOUR_MIN_SEC_MS:
			return to_string(ms);
		}
	}

	std::string seconds_to_string(double timesec) {
		namespace chr = std::chrono;
		return to_string(chr::duration_cast<chr::milliseconds>(chr::duration<double>(timesec)));
	}

	std::string to_string(const std::wstring& wstr) {
		return sfl::conv::wstr_to_str(wstr);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		CHECK IF FILE IS INCLUDED IN LIST TYPE
// 	    LIST STYPE : EXEMPLE : "*.exe|*.com|*.dll|*.lib|*.o|*.obj"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool is_file_extension_in_list(const fs::path& filename, std::string_view ext_list) {
		if (!filename.has_extension()) return false;
		std::string ext = filename.extension().string();
		// normalize ot lower case
		ext = sfl::to_lower(ext);
		return ext_list.find(ext) != std::string::npos;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// 
	/////////////////    TYPES AND CONSTANT PREDIFINED IN STRING LIBRARY
	///////////////// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

    inline namespace type {
        
        template<typename Char = char>
        using vecString = std::vector<std::basic_string<Char>>;
        
        using vecPath   = std::vector<fs::path>;
        
        using uchar = unsigned char;
        using byte = unsigned char;
        using uint = unsigned int;
        using ushort = unsigned short;

		// enum define just from where we can applay 
		enum FROM {
			BEGINING,
			END,
			BEGINING_AND_END,
			ANY
		};

		// Templated functions :
		enum class CONV_OPTION {
			IGNOR_HI_CHAR,
			ADD_SIGNEFICANT_CHAR,
			ADD_ALL_CHAR
		};


    } // namespace type


	namespace var {

		const char* alphabet = "";
		const char* numbers = "0123456789";
		const char* alphanum = "";

	}




    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////// 
    /////////////////    CONVERTION FUNCTION   
    ///////////////// 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace conv {

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 
		//    Convert String element to vector element
		// 
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		// function that take string<Char> and converted to vector<Char> :
		// using std::copy function
		template<typename TChar>
		std::vector<TChar>  to_vector(const std::basic_string<TChar>& String) {

			std::vector<TChar> vec(String.size());
			std::copy(String.begin(), String.end(), vec.begin());
			return vec;
		}

		// function that take string<Char> and converted to vector<Char> :
		// using memcpy function
		template<typename TChar>
		std::vector<TChar>  to_vector_memcpy(const std::basic_string<TChar>& String) {

			std::vector<TChar> vec(String.size());
			std::memcpy(vec.data(), String.data(), String.size() * sizeof(TChar));
			return vec;
		}

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 
        //    CONVERTION BETWEEN CHAR TO WCHAR AND WCHAR TO CHAR FUNCTIONS    
        // 
        //////////////////////////////////////////////////////////////////////////////////////////////////////////

		auto wchart_to_char(wchar_t wch) {

			std::array<char, 2> ch;

			ch[0] = static_cast<char>(wch & 0x00ff);
			ch[1] = static_cast<char>((wch & 0xff00) >> 8);

			return ch;
		}

		bool is_wchart_fit_char(wchar_t wch) {
			return (wch & 0xff00) == 0;
		}

		// this function return true if inverse convertible between char and wchar.
		bool is_wstr_convertible_str(const std::wstring& wstr) {

			for (const auto& w : wstr)
			{
				if (!is_wchart_fit_char(w)) {
					return false;
				}
			}

			return true;
		}

		// this here will ignore hi char in wchar_t. 
		std::string wstr_to_str(const std::wstring& wstr) {

			std::string str(wstr.size(), '\0');

			for (int i = 0; i < wstr.size(); ++i) {
				str[i] = static_cast<char>(wstr[i] & 0x00ff);
			}
			return str;
		}

		std::string wstr_to_str2(const std::wstring& wstr) {
			return std::string(wstr.begin(), wstr.end());
		}

		// string to wstring
		std::wstring str_to_wstr(const std::string& str) {
			std::wstring wstr(str.size(), L'\0');

			for (int i = 0; i < str.size(); ++i) {
				wstr[i] = static_cast<wchar_t>(str[i]);
			}
			return wstr;
		}

		std::string wstr_to_str3(const std::wstring& wstr) {
			std::string str;
			str.reserve(wstr.size() * sizeof(std::wstring::value_type));

			for (const auto& e : wstr) {
				str.push_back(static_cast<char>(e & 0x00ff));

				if ((e & 0xff00) != 0x0000)
					str.push_back(static_cast<char>((e & 0xff00) >> 8));
			}

			return str;
		}

		std::string wstr_to_str4(const std::wstring& wstr) {
			std::string str;
			str.reserve(wstr.size() * sizeof(std::wstring::value_type));

			for (const auto& e : wstr) {
				str.push_back(static_cast<char>(e & 0x00ff));
				str.push_back(static_cast<char>((e & 0xff00) >> 8));
			}
			return str;
		}


		template<CONV_OPTION Version>
		constexpr std::string wstr_to_str(const std::wstring& wstr) {
			std::string str;
			if constexpr (Version == CONV_OPTION::IGNOR_HI_CHAR)
				str = wstr_to_str(wstr);
			else if constexpr (Version == CONV_OPTION::ADD_SIGNEFICANT_CHAR)
				str = wstr_to_str2(wstr);
			else if constexpr (Version == CONV_OPTION::ADD_ALL_CHAR)
				str = wstr_to_str3(wstr);
			else {
				std::cerr << "Error of version";
				return {};
			}

			return str;
		}


		// conversion of vector of Tchar to basic_string<Tchar>
		template<typename Tchar>
		std::basic_string<Tchar> vector2string(const std::vector<Tchar>& buffer)
		{
			return std::basic_string<Tchar>(buffer.begin(), buffer.end());
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 
		//     CONVERSION FROM STRING TO OTHER TYPES
		// 
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		float toFloat(const std::string& str)
		{
			return std::stof(str);
		}

		int toInt(const std::string& str)
		{
			return std::stoi(str);
		}

		bool toBool(const std::string& str)
		{
			
			if (str == "true" || str == "True" || str == "TRUE")
				return true;
			return false;
		}


	}// namespace conv : conversion




	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// 
	/////////////////       FUNCTIONS OF STRING TRAITMENTS
	///////////////// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace trait {
		// duplicate character 

		template<typename STRING>
		STRING remove_all_duplicated_char(const STRING& text, const STRING& set_chars)
		{
			std::unordered_set<typename STRING::value_type>     seen;
			STRING temptext;

			for (auto c : text) {
				if (set_chars.find(c) != STRING::npos) {
					if (seen.find(c) == seen.end()) {
						temptext.push_back(c);
						seen.insert(c);
					}
				}
				else
				{
					temptext.push_back(c);
				}
			}

			return temptext;
		}

		template<typename STRING>
		STRING remove_successiveDup(const STRING& text, const STRING& set_chars)
		{
			if (text.empty()) return{};

			STRING tempstr;
			tempstr.push_back(text[0]);

			for (size_t i = 1; i < text.size(); ++i)
			{
				// Only add char if it's different from the last one added
				if (set_chars.find(text[i]) != STRING::npos)
				{
					if (text[i] != tempstr.back()) {
						tempstr.push_back(text[i]);
					}
				}
				else
				{
					tempstr.push_back(text[i]);
				}
			}

			return tempstr;
		}

		template<typename STRING>
		STRING remove_successiveDuplicate(const STRING& text)
		{
			if (text.empty()) return{};

			STRING tempstr;
			tempstr.push_back(text[0]);

			for (size_t i = 1; i < text.size(); ++i)
			{
				// Only add char if it's different from the last one added
				if (text[i] != tempstr.back()) {
					tempstr.push_back(text[i]);
				}
			}

			return tempstr;
		}



		// general function
		template<typename Char, FROM from_where>
		void remove_char(std::basic_string<Char>& str, int (*is_function)(int))
		{
			if (str.empty()) return;
			int i{};

			if constexpr (from_where == FROM::BEGINING)
			{
				while (is_function(str[i])) {
					++i;
					if (i > str.size()) break;
				}
				str = str.substr(i);
			}
			else
				if constexpr (from_where == FROM::END)
				{
					while (is_function(str[str.size() - 1])) {
						str.pop_back();
						if (str.empty()) return;
					}
				}
				else
					if constexpr (from_where == FROM::BEGINING_AND_END) 
					{
						remove_char<Char, FROM::BEGINING>(str, is_function);
						remove_char<Char, FROM::END>(str, is_function);
					}
					else
						if constexpr (from_where == FROM::ANY)
						{
							std::basic_string<Char> tempstr = str;
							str.clear(); str.reserve(tempstr.size());
							for (auto& e : tempstr) {
								if (!is_function(e)) str.push_back(e);
							}
							str.shrink_to_fit();
						}


		}

		// returning a string 
		template<typename Char, FROM from_where>
		constexpr std::basic_string<Char> remove_char2(std::basic_string<Char> str, int (*is_function)(int))
		{
			if (str.empty()) return {};
			int i{};

			if constexpr (from_where == FROM::BEGINING)
			{
				while (is_function(str[i])) {
					++i;
					if (i > str.size()) break;
				}
				str = str.substr(i);
			}
			else
				if constexpr (from_where == FROM::END)
				{
					while (is_function(str[str.size() - 1])) {
						str.pop_back();
						if (str.empty()) return;
					}
				}
				else
					if constexpr (from_where == FROM::BEGINING_AND_END)
					{
						remove_char<Char, FROM::BEGINING>(str, is_function);
						remove_char<Char, FROM::END>(str, is_function);
					}
					else
						if constexpr (from_where == FROM::ANY)
						{
							std::basic_string<Char> tempstr = str;
							str.clear(); str.reserve(tempstr.size());
							for (auto& e : tempstr) {
								if (!is_function(e)) str.push_back(e);
							}
							str.shrink_to_fit();
						}

			return str;
		}


		// remove char from string 
		void remove_char_any(std::string& str, char c = ' ') {
			std::string tempstr = str;

			str.clear(); str.reserve(tempstr.size());

			for (auto& e : str) {
				if (e == c) str.push_back(e);
			}

			str.shrink_to_fit();
		}

		void remove_char_any(std::string& str, int(*is_function)(int)) {
			std::string tempstr = str;

			str.clear(); str.reserve(tempstr.size());

			for (auto& e : tempstr) {
				if (is_function(e)) str.push_back(e);
			}

			str.shrink_to_fit();

		}

		// romove successive char(blank) from end of string
		void remove_char_end(std::string& str, char Nchar = ' ') {
			if (str.empty()) return;

			while (str[str.size() - 1] == Nchar) {
				str.pop_back();
				if (str.empty()) return;
			}
		}

		template<typename STRING>
		void remove_char_end(STRING& str, int(*is_function)(int))
		{
			if (str.empty()) return;

			while ( is_function(str[str.size() - 1]) ) {
				str.pop_back();
				if (str.empty()) return;
			}
		}

		// remove successive char(blank) from the begining of string
		void remove_char_beg(std::string& str, char Nchar = ' ') {
			int i = 0;

			while (str[i] == Nchar) {
				++i;
				if (i > str.size()) break;
			}

			str = str.substr(i);
		}

		template<typename STRING>
		void remove_char_beg(STRING& str, int (*is_function)(int))
		{
			int i = 0;

			while ( is_function(str[i]) ) {
				++i;
				if (i > str.size()) break;
			}

			str = str.substr(i);
		}

		// remove sccessive char(blank) from end and begining of string
		void remove_char_eb(std::string& str, char Nchar = ' ') {
			remove_char_beg(str,Nchar);
			remove_char_end(str,Nchar);
		}

		template<typename STRING>
		void remove_char_eb(STRING& str, int(*is_function)(int))
		{
			remove_char_beg(str, is_function);
			remove_char_end(str, is_function);
		}

		// general function that remove all blank like CR LF Space Tab 

		template<FROM where> 
		void remove_space(std::string& str) {

			if constexpr (where == FROM::BEGINING) {
				remove_char_beg(str, std::isspace);
			}
			else
				if constexpr (where == FROM::END) {
					remove_char_end(str, std::isspace);
				}
				else
					if constexpr (where == FROM::BEGINING_AND_END) {
						remove_char_eb(str, std::isspace);
					}
					else
						if constexpr (where == FROM::ANY) {
							remove_char_any(str, std::isspace);
						}

		}


		// remove blank from begining and end of strings

		void remove_blank_eb(std::string& str) {
			remove_char_eb(str, std::isblank);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//                   REVERSE STRING AND RANDOM ARRANGEMENT OF STRING
		//
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename Tchar>
		std::basic_string<Tchar> reverse_string(std::basic_string<Tchar> str) {
			int l = str.size();
			for (int i = 0; i < l / 2; i++) {
				std::swap(str[i], str[l - i - 1]);
			}

			return str;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 
		//    convert flaot to string and wstring 
		// 
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		std::wstring float_format(float f, const int decimal)
		{
			std::wstring strf = std::to_wstring(f);
			size_t size = strf.size();
			std::wstring format = L"%." + std::to_wstring(decimal) + L"f";
			swprintf_s(strf.data(), size, format.c_str(), f);

			sfl::trait::remove_char<wchar_t, sfl::BEGINING_AND_END>(strf,
				+[](int x)->int {return !std::iswprint(x) | std::iswblank(x); });

			return strf;
		}


	} // namespace trait

	// TODO : string transformation and input and outpu
	template<typename STRING>
	auto make_vecLines(const STRING& text, const typename STRING::value_type& dilemiter) {
		std::vector<STRING> vlines;
		std::basic_istringstream<typename STRING::value_type>  iss(text);
		STRING str;

		while (std::getline(iss, str, dilemiter)) {
			vlines.push_back(str);
		}

		return vlines;
	}

	template<typename STRING>
	std::pair<size_t, size_t> get_line_pos(const STRING& text, size_t nline, const STRING cr_line)
	{
		size_t pos{};
		size_t npos{};
		int i{};
		while (true) {
			npos = text.find_first_of(cr_line, pos + 1);

			//std::cout << i << " line coordinate " << pos << "|" << npos << end_;

			if (npos == STRING::npos)
			{
				return { pos + (i != 0), text.size() };
			}

			if (i == nline)
			{
				return { pos + (i != 0) ,npos };
			}

			pos = npos;

			++i;
		}
	}

	template<typename STRING>
	STRING get_line(const STRING& text, int nline, const STRING& cr_char)
	{
		auto pair = get_line_pos(text, nline, cr_char);
		return text.substr(pair.first, pair.second - pair.first);
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// 
	/////////////////    LOGIC AND BINARY RELATION  
	///////////////// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace check {

		template<char ...ch>
		constexpr int is_one_of(int c) {
			return ((c == ch) || ...);
		}
		
		template<typename Char, Char...ch>
		constexpr int is_one_of(int c) {
			return ((c == static_cast<int>(ch)) || ...);
		}


	} // namespace check : checking of string 

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////// 
    /////////////////    COMPARAISON 
    ///////////////// 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace comp {



	} // namespace comp : comparaison

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////// 
    /////////////////    STRING GENERATOR  
    ///////////////// 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace generator {



		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 
		//    RANDOM GENERATOR
		// 
		//////////////////////////////////////////////////////////////////////////////////////////////////////////

		namespace rand {


		} // namespace rand : random


	} // namespace generator
    
}// namespace stl : string function library