/*
				Title		:  BOOL OPTION TO MAP ALL BOOLEAN USED IN ONE APPLICATION WITH NAME AND COMMENT 

				Name		:
				Author		:	Kadda Aoues
				Date		:
				Statu		:	On Development
				Version		:
				Copyright	:

				Discription :
				This program is made in profite to test diffirente algorithm and functions
				of selection of object and groupe selection

				TODO :
				------

*/
#pragma once
#include <map>
#include <vector>
#include <string>


namespace tool {

	template<typename Tchar>
	struct _Option {
		bool							action;
		std::basic_string<Tchar>		comment;

		void not_action() {
			action = !action;
		}

		bool operator()() const {
			return action;
		}

		bool& operator()() {
			return action;
		}
	};

	using Option = _Option<wchar_t>;
	using mOption = std::map<std::string, Option>;
	using msOption = std::map<std::string, _Option<char>>;
}