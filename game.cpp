#include "game.h"

void first_seeds_generate(uint8_t one_pos_seeds_count) {
	SerializebleSeeds data;
	std::random_device rd;
	std::mt19937 gen(rd());

	uint16_t cnt_error = 0, cnt_mb_success = 0;
	for (uint16_t diffic : {3}) {
		std::cerr << "difficult: " << diffic << '\n';

		for (uint16_t percent_to_delete : {30, 45, 70}) {
			std::cerr << "percent_to_delete: " << percent_to_delete << '\n';

			uint16_t current_cnt = 0;
			while (current_cnt != one_pos_seeds_count) {
				int seed = gen();
				Field fd(diffic);
				fd.random_fill(seed);

				auto sudocu = fd.delete_random_cells(percent_to_delete, seed);
				auto [sol_cnt, sol_table] = Field::get_solution(sudocu);

				if (sol_cnt == 1) {
					bool is_equal = std::equal(fd.field_.begin(),
						fd.field_.end(), sol_table.begin());
					if (!is_equal)
						++cnt_error;
					else {
						++current_cnt;
						data[diffic][percent_to_delete].push_front(seed);
					}
					++cnt_mb_success;
					std::cerr << std::boolalpha
						<< "is equal " << cnt_mb_success << ": " << is_equal << std::endl;
				}
			}
		}
	}

	std::ofstream ofs("seeds.dump");
	boost::archive::text_oarchive(ofs) << data;
}

//class Game 

Game::Game() : seeds_([] {
	SerializebleSeeds data;
	std::ifstream ifs("seeds.dump");
	boost::archive::text_iarchive(ifs) >> data;
	return data;
	}()), current_solved_field_(0){}

void Game::build_field(uint8_t difficult, uint8_t percent_to_delete) {
	int valid_seed;

	{auto access = seeds_.get_access();

		auto& current_seeds = access.value_[difficult][percent_to_delete];
		valid_seed = current_seeds.front();
		current_seeds.pop_front();
	}

	current_solved_field_ = Field(difficult);
	current_solved_field_.random_fill(valid_seed);
	current_field_ = std::move(
		current_solved_field_.delete_random_cells(percent_to_delete, valid_seed));
}

//Change text and background color.
template<int txt = 7, int bg = 0>
std::ostream& color(std::ostream & os) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | txt));
	return os;
}

//os = std::cout in current emplementation.
void Game::print_field(uint8_t i_idx, uint8_t j_idx) {
	uint8_t difficulty = current_solved_field_.get_difficulty();
	uint8_t current_num = current_field_[i_pos][j_pos];
		auto print_line = [this]() {
			uint8_t cnt = current_field_.size() + 1;
			while (cnt--)
				std::cout << "--";
			std::cout << "-\n";
		};

	for (uint8_t i = 0; i < current_field_.size(); ++i) {

		if (i % difficulty == 0) {
			print_line();
		}
		
		for (uint8_t j = 0; j < current_field_.size(); ++j) {
			//Make block sep.
			if (j % difficulty == 0)
				std::cout << '|';

			//Marking if current cell or current num != 0.
			if ( (i == i_idx && j == j_idx)	|| 
				(current_num != 0 && current_field_[i][j] == current_num)) 
			{	
				std::cout << color<0, 7>;
				if (current_field_[i][j] != 0)
					std::cout << static_cast<uint16_t>(current_field_[i][j]);
				else
					std::cout << '#';

					std::cout << color << ' ';
			}
			//Deafult cell.
			else
				if (current_field_[i][j] != 0)
					std::cout << static_cast<uint16_t>(current_field_[i][j]) << ' ';
				else 
					std::cout << "# ";
		}
		std::cout << "\b|\n";
	}
	print_line();
}

bool Game::process_enter_button() {
	std::cout << "Number: ";
	uint16_t entered_num = static_cast<uint8_t>(-1);
	std::cin >> entered_num;
	while (!std::cin) {
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Invalid input." << std::endl;
		std::cout << "\nNumber: ";
		std::cin >> entered_num;
	}
	if (entered_num != current_solved_field_[i_pos][j_pos]) {
		std::cout << ">>Wrong<<" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	else {
		current_field_[i_pos][j_pos] = entered_num;
		std::cout << "<<Correct>>" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1)*0.7);
		if (std::equal(current_field_.begin(), current_field_.end(),
			current_solved_field_.begin()))
		{
			std::cout 
				<< "\n====\\Congratulations/====\n"
				  << "        You've won!        "
				<< std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return false;
		}
	}
	return true;
}

Game::MENUOPT Game::menu() {
	uint8_t pos = 0;
	const char* pos_s[]  { 
R"(
|========MENU========|
|     <Continue>     |
|    Change  game    |
|        Exit        |
|====================|)",
R"(
|========MENU========|
|      Continue      |
|   <Change  game>   |
|        Exit        |
|====================|)",
R"(
|========MENU========|
|      Continue      |
|    Change  game    |
|       <Exit>       |
|====================|)"};


	std::cout << pos_s[0];

	bool is_in_menu = true;
	while (is_in_menu) {
		switch (_getch()) {
		case 72://up
			if (pos != 0)
				--pos;
			break;
		case 80://down
			if (pos != 2)
				++pos;
			break;
		case 13://enter
			return MENUOPT(pos);
		}
		std::system("cls");
		std::cout << pos_s[pos];
	}

	return {};
}


void Game::loope() {
	bool is_in_game = true;
	MENUOPT menu_returning;

	while (is_in_game) {
		//std::cout << current_solved_field_ << std::endl << std::endl;
		print_field(i_pos, j_pos);

		switch (_getch()) {
		case 72://up
			if (i_pos != 0)
				--i_pos;
			break;
		case 80://down
			if (i_pos != current_field_.size() - 1)
				++i_pos;
			break;
		case 75://left
			if (j_pos != 0)
				--j_pos;
			break;
		case 77://right
			if (j_pos != current_field_.size() - 1)
				++j_pos;
			break;
		case 13://enter
			if (current_field_[i_pos][j_pos] == 0)
				is_in_game = process_enter_button();
			break;
		case 27://escape
			std::system("cls");
			menu_returning = menu();

			switch (menu_returning) {
			case MENUOPT::Exit:
				is_in_game = false;
				break;
			case MENUOPT::Change:
				break;
			case MENUOPT::Continue:
				break;
			}

			break;
		}

		std::system("cls");
	}

	std::cout << "Game end." << std::endl;
}

//end class Game