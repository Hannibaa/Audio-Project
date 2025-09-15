/*
				Title		:	Audio Play List WMP only

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
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "../../third_party/json/nlohmann_json/json.hpp"



using   json = nlohmann::json;
namespace fs = std::filesystem;


namespace audio {

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				GET FILE FROM FOLDER 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<fs::path> get_filesDirectory(const fs::path& directory) {

		// check if directory exist and is directory
		if (!std::filesystem::is_directory(directory)) {
			std::cerr << "[" << directory << "] is not directory \n";
			return{};
		}

		// making a list of every thing in directory
		std::vector<fs::path> filenames;

		for (auto& dir : std::filesystem::directory_iterator{ directory })
		{
			if (!std::filesystem::is_directory(dir))
				filenames.push_back(dir.path());
		}

		return filenames;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//			PLAYLIST CLASS 
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
	class PlayList {
		std::vector<std::wstring>		_list;
		std::wstring					_current_play;
		std::wstring					_music_folder;

		size_t							_current_index;
		size_t							_max_lines{ 5 };
		size_t							_max_size{ 20 };
		const char*						_INIT_FILE{ "audio.json" };
		const char*						_AUDIO_EXTENTION{"*.mp3|*.wav"};

	public:
		PlayList()
			:_current_index{ 0 }
		{
			_list.reserve(_max_size + 1);
		}

		void clear() {
			_list.clear();
			_current_index = 0;
			_current_play.clear();
			save_playlist();
		}

		void load_playlist() {
			std::ifstream	ifs(_INIT_FILE);

			if (!ifs.is_open()) {
				std::cerr << "file not found " << _INIT_FILE << '\n';
				return;
			}

			// Perse json
			json j;
			try {
				ifs >> j;
			}
			catch (const json::parse_error& e) {
				std::cerr << "JSON parse error : " << e.what() << '\n';
				return;
			}

			_list.clear();
			_list = j["list"].get<std::vector<std::wstring>>();
			if (_list.size() > _max_size)
				_list.resize(_max_size);

			_current_play = j["current"].get<std::wstring>();
			_current_index = j["cur_index"].get<size_t>();
			_music_folder = j["music_folder"].get<std::wstring>();
		}

		void save_playlist() {
			json j;
			j["author"] = "KADDA AOUES";
			j["list"] = _list;
			j["string--current"] = sfl::to_string(_current_play);
			j["current"] = _current_play;
			j["cur_index"] = _current_index;
			j["music_folder"] = _music_folder;

			std::ofstream ofs(_INIT_FILE);
			if (!ofs.is_open()) {
				std::cerr << "Failed to open file for writing audio.json \n";
				return;
			}

			ofs << j.dump(4);
		}

		void set_music_folder() {
			if (!empty()) {
				fs::path folder = _list[0];
				_music_folder = folder.parent_path().wstring();
			}
		}

		std::wstring music_folder() const {
			return _music_folder;
		}

		void load_list_from_folder(const fs::path& folder) {
			if (folder.empty()) return;

			auto temp = get_filesDirectory(folder);
			std::for_each(temp.begin(), temp.end(), [&](const fs::path& file) {
				add_item(file.wstring());
				});
		}

		void current_item(const std::wstring& file) {

			if (file == L"")
				_current_play = L"";

			if (exit(file)) {
				_current_play = file;
				return;
			}

			if (!_list.empty())
				_current_play = _list[0];
			else
				_current_play = L"";

			_current_index = 0;
		}

		std::wstring current_item() const {
			return _current_play;
		}

		std::wstring next_item() {
			if (_current_index + 1 < _list.size()) return _list[_current_index + 1];
			return _list[0];
		}

		std::wstring previous_item() {
			if (_current_index - 1 < _list.size()) return _list[_current_index - 1];
			return _list[_list.size() - 1];
		}

		constexpr size_t size() const noexcept {
			return _list.size();
		}

		constexpr bool empty() const noexcept {
			return _list.empty();
		}

		void set_size(size_t size) {
			_max_size = size;
			_list.reserve(_max_size + 1);
		}

		void set_lineShow(size_t lines) {
			_max_lines = lines;
		}

		std::wstring	at(size_t i) {
			return _list.at(i);
		}

		void print_list(size_t from_index) {
			if (from_index < _max_lines) from_index = 0;
			for (size_t i = from_index; i < _list.size() && i < from_index + _max_lines; ++i) {
				std::wcout << _list[i] << '\n';
			}
		}

		bool exit(const std::wstring file) {
			size_t i{};
			for (const auto& f : _list) {
				if (f == file) {
					_current_index = i;
					return true;
				}
				++i;
			}

			return false;
		}

		void add_item(const std::wstring& file_name) {

			// check if is audio file
			if (!sfl::is_file_extension_in_list(file_name, _AUDIO_EXTENTION))
				return;
			// check file is exit in list
			if (exit(file_name)) return;

			// check the maximum size of list allowed
			if (_list.size() > _max_size) return;

			_list.push_back(file_name);

			if (_list.size() == 1) {
				_current_index = 0;
				_current_play = _list[0];
			}
		}

		void remove_item(const std::wstring& file_name) {

			int i{};
			for (auto it = _list.begin(); it != _list.end();) {
				if (*it == file_name) {
					it = _list.erase(it);

					if (_current_play == file_name) {
						_current_play = *it;
						_current_index = i;
					}

					break;
				}
				else ++it;

				++i;
			}
		}

		void remove_item(size_t i) {
			if (i < _list.size())
				remove_item(_list[i]);
		}
	};


}