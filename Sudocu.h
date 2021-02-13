#pragma once
#include "Synchronized.h"

#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <set>
#include <iostream>

//Need Sudocu solver
class Field final{

public:
	using _field = std::vector<std::vector<uint8_t>>;

	Field() = delete;

	Field(uint8_t difficulty);
	void random_fill(int seed, uint8_t iteration_amount = 4);
	_field delete_random_cells(uint8_t percent_to_delete, int seed) const;

	friend std::ostream& operator <<(std::ostream& os, const Field& rhs);
	std::vector<uint8_t>& operator[](size_t idx);

	friend void first_seeds_generate(uint8_t one_pos_seeds_count);//TODO: delete this

	static std::set<uint8_t> get_cell_possible_nums
	(const _field& field_, size_t i_idx, size_t j_idx);

	static std::pair<int8_t, _field> get_solution
	(_field sudocu_table, size_t i_start = 0, size_t j_start = 0);

	auto begin() {
		return field_.begin();
	}

	auto end() {
		return field_.end();
	}

	size_t get_difficulty() const {
		return difficulty_;
	}

private:

	void build_basic_field();

	//Modificators:
	void swap_two_rows(size_t lhs_idx, size_t rhs_idx);
	void swap_two_columns(size_t lhs_idx, size_t rhs_idx);
	void swap_two_rows_blocks(size_t lhs_idx, size_t rhs_idx);
	void swap_two_columns_blocks(size_t lhs_idx, size_t rhs_idx);
	void transposition();

private:
	size_t difficulty_;
	size_t size_;
	std::vector<std::vector<uint8_t>> field_;
};
