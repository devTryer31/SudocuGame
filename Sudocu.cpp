#include "Sudocu.h"



Field::Field(uint8_t difficulty) :
	difficulty_(difficulty),
	size_(difficulty * difficulty),
	field_(size_, std::vector<uint8_t>(size_))
{
	if (difficulty == 1)
		throw std::invalid_argument("difficulty == 1 got. This class does't support it.");

	field_.shrink_to_fit();
	for (auto& row : field_)
		row.shrink_to_fit();

	build_basic_field();
}

void Field::build_basic_field() {
	std::vector<uint8_t> pool_nums(size_);
	std::iota(pool_nums.begin(), pool_nums.end(), 1);

	for (size_t i = 0; i < size_; ++i) {
		for (size_t j = 0; j < size_; ++j) {
			field_[i][(j + (i * difficulty_) * (difficulty_-1) + size_ - i/difficulty_) % size_] = pool_nums[j];
		}
	}
}

void Field::swap_two_rows(size_t lhs_idx, size_t rhs_idx) {
	if (lhs_idx > size_ || rhs_idx > size_) {
		std::cerr << "Index out of range." << std::endl;
		return;
	}
	std::swap(field_[lhs_idx], field_[rhs_idx]);
}

void Field::swap_two_columns(size_t lhs_idx, size_t rhs_idx) {
	if (lhs_idx > size_ || rhs_idx > size_) {
		std::cerr << "Index out of range." << std::endl;
		return;
	}
	for (size_t i = 0; i < size_; ++i) {
		std::swap(field_[i][lhs_idx], field_[i][rhs_idx]);
	}
}

void Field::swap_two_rows_blocks(size_t lhs_idx, size_t rhs_idx) {
	if (lhs_idx > difficulty_ || rhs_idx > difficulty_) {
		std::cerr << "lhs_idx > " << difficulty_ 
			<< " or rhs_idx > " << difficulty_ << "." << std::endl;
		return;
	}
	if (std::abs((int64_t)lhs_idx - (int64_t)rhs_idx) != 1) {
		std::cerr << "abs(lhs_idx - rhs_idx) != 1" << std::endl;
		return;
	}
	for (size_t i = 0; i < difficulty_; ++i)
		std::swap(field_[i + difficulty_ * lhs_idx],
		          field_[i + difficulty_ * rhs_idx]);
}

void Field::swap_two_columns_blocks(size_t lhs_idx, size_t rhs_idx) {
	if (lhs_idx > difficulty_ || rhs_idx > difficulty_) {
		std::cerr << "lhs_idx > " << difficulty_ 
			<< " or rhs_idx > " << difficulty_ << "." << std::endl;
		return;
	}
	if (std::abs((int64_t)lhs_idx - (int64_t)rhs_idx) != 1) {
		std::cerr << "abs(lhs_idx - rhs_idx) != 1" << std::endl;
		return;
	}
	for (size_t j = 0; j < difficulty_; ++j)
		swap_two_columns(j + difficulty_ * lhs_idx, j + difficulty_ * rhs_idx);
}

void Field::transposition() {
	for (size_t i = 0; i < size_; ++i)
		for (size_t j = i; j < size_; ++j)
			std::swap(field_[i][j], field_[j][i]);
}

void Field::random_fill(int seed, uint8_t iteration_amount) {
	std::mt19937 rand_g(seed);

	auto rand_gen_close_poss = [&rand_g, this]() {
		size_t r_num1 = rand_g(), r_num2 = rand_g();
		
		size_t first_pos = r_num1 % size_, second_pos;

		if (first_pos == size_ - 1) {
			second_pos = first_pos - (1 + r_num2 % (difficulty_ - 1));
		}
		else if (first_pos == 0) {
			second_pos = first_pos + (1 + r_num2 % (difficulty_ - 1));
		}
		else {
			second_pos = (first_pos / difficulty_) * difficulty_ + r_num2 % difficulty_;
		}

		if (second_pos == first_pos)
			second_pos = (first_pos / difficulty_) * difficulty_ + (1 + r_num2) % difficulty_;

		//For better optimisation and analysis.
		if (first_pos / difficulty_ != second_pos / difficulty_) {
			std::cerr << "Indexes are not in the same area." << std::endl;
			return std::make_pair<size_t, size_t>(0, 1);
		}

		return std::make_pair(first_pos, second_pos);
	};

	auto rand_gen_close_areas = [&rand_g, this]() {
		size_t r_num1 = rand_g(), r_num2 = rand_g();

		size_t first_pos = r_num1 % difficulty_, second_pos;

		if (first_pos == difficulty_ - 1) {
			second_pos = first_pos - 1;
		}
		else if (first_pos == 0) {
			second_pos = first_pos + 1;
		}
		else {
			second_pos = static_cast<size_t>(
				static_cast<int64_t>(first_pos)
							+ std::pow(-1, (r_num2 % 2))
			);
		}

		return std::make_pair(first_pos, second_pos);

	};

	while (iteration_amount--) {
		
		auto poss = rand_gen_close_poss();
		swap_two_columns(poss.first, poss.second);
		
		poss = rand_gen_close_areas();
		swap_two_rows_blocks(poss.first, poss.second);
	
		transposition();

		poss = rand_gen_close_poss();
		swap_two_rows(poss.first, poss.second);

		poss = rand_gen_close_areas();
		swap_two_columns_blocks(poss.first, poss.second);
	}
}

std::set<uint8_t> Field::get_cell_possible_nums
	(const _field& field_, size_t i_idx, size_t j_idx) 
{
	size_t size = field_.size(), 
		difficukty = static_cast<size_t>(std::sqrt(size));

	std::vector<uint8_t> prohibited_nums(size+1);
	for (int16_t idx = static_cast<int16_t>( size)-1; idx >= 0; --idx) {
		
		uint8_t num1 = field_[idx][j_idx];
		if (idx != i_idx)
			prohibited_nums[num1] = num1;

		uint8_t num2 = field_[i_idx][idx];
		if (idx != j_idx)
			prohibited_nums[num2] = num2;
	}

	for (
		size_t i = (i_idx / difficukty) * difficukty; 
		i < (i_idx / difficukty + 1) * difficukty;
		++i)
		for (
			size_t j = (j_idx / difficukty) * difficukty; 
			j < (j_idx / difficukty + 1) * difficukty;
			++j)
			if (i != i_idx && j != j_idx)
				prohibited_nums[field_[i][j]] = field_[i][j];

	std::set<uint8_t> result;
	for (uint8_t i = 1; i < size+1; ++i)
		if (prohibited_nums[i] != i)
			result.insert(i);

	return result;
}

Field::_field Field::delete_random_cells
		(uint8_t percent_to_delete, int seed) const 
{
	_field result = field_; //Field copying.

#pragma warning (disable : 26451)
#pragma warning (disable : 4244)
	size_t quant_nums_to_delete = 
		size_ * size_ * 
		(percent_to_delete / 100.0) + 1;

	std::mt19937 gen(seed);
	while (quant_nums_to_delete--) {
		result[gen() % size_][gen() % size_] = 0;
	}

	return result;
}

std::pair<int8_t, Field::_field> Field::get_solution
(_field sudocu_table, size_t i_start, size_t j_start) {
	_field result;

	for (size_t i = i_start; i < sudocu_table.size(); ++i) {
		for (size_t j = j_start; j < sudocu_table.size(); ++j) {
			if (sudocu_table[i][j] == 0) {
				const auto 
					possible_nums = Field::get_cell_possible_nums(sudocu_table, i, j);

				if (possible_nums.size() == 1) {
					sudocu_table[i][j] = *possible_nums.begin();
				}
				else if (possible_nums.size() == 0) {
					return { 0, sudocu_table };
				}
				else {
					int8_t solutions_count = 0;
					for (uint8_t num : possible_nums) {
						sudocu_table[i][j] = num;

						//Copiing sudocu_table.
						auto [sol_cnt, table] = get_solution(sudocu_table, i+1,j+1);
						result = std::move(table);

						solutions_count += sol_cnt;
						if (solutions_count >= 2) {
							return {solutions_count, result};
						}
					}
					if (solutions_count == 0)
						return{ 0, result };
					else
						sudocu_table = std::move(result);
				}
			}
		}
	}

	return { 1, sudocu_table };
}


std::ostream& operator <<(std::ostream& os, const Field& rhs) {
	for (const auto& line : rhs.field_) {
		for (uint16_t num : line)
			os << num << ' ';
		os << std::endl;
	}
	return os;
}

std::vector<uint8_t>& Field::operator[](size_t idx) {
	return field_[idx];
}

//SUDOCU SOLVER

