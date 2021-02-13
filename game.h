#pragma once

#include "Synchronized.h"
#include "Sudocu.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/forward_list.hpp>

#include <Windows.h>
#include <conio.h>

#include <chrono>
#include <iostream>
#include <map>
#include <forward_list>
#include <fstream>

class SerializebleSeeds final {
public:
	SerializebleSeeds() = default;
	SerializebleSeeds(const SerializebleSeeds& source) = default;
	void operator= (const SerializebleSeeds& source) = delete;

	std::map<uint8_t, std::forward_list<int>>& operator[]
	(uint8_t difficult) {
		return data_[difficult];
	}

//For serialization
private:
	friend class boost::serialization::access;

	template <class Archive>
	void serialize(Archive& arch, const unsigned char version) {
		arch & data_;
	}

private:
	std::map<uint8_t, std::map<uint8_t, std::forward_list<int>>> data_;
};

void first_seeds_generate(uint8_t one_pos_seeds_count);

class Game final {

public:
	Game();

	void build_field(uint8_t difficult, uint8_t percent_to_delete );
	void loope();

private:
	void print_field(uint8_t i_idx, uint8_t j_idx);
	bool process_enter_button();

private:
	Field::_field current_field_;
	Field current_solved_field_;

	uint8_t i_pos = 0, j_pos = 0,
		error_count = 0;

	//There are two difficulties: 3 and 4 cells in one block dimantion. 
	//All difficulties have 3 percent rating: 
	//			30, 45 and 75 % deleted cells in field.
	//And then for all of this positions 
	//					we will generate seeds for future sudocu building.
	Synchronized<SerializebleSeeds> seeds_;
};


