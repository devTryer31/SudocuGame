#include "Game.h"

#include "profile.h"
#include <iostream>

#pragma warning (disable : 26812)

int main() {
	//first_seeds_generate(5);
	Game game;
	game.build_field(3u, 45u);
	game.loope();

	//Field f2(3), f3(3), f4(4);
	//std::random_device rd;
	//std::mt19937 gen(rd());
	////size_t cnt = 20000

	//LOG_DURATION("1 times");
	//size_t cnt = 5000*500;

	//size_t cnt_error = 0, cnt_success = 0;
	//while (cnt--) {
	//	auto seed = gen();
	//	f2.random_fill(seed);

	//	auto sudocu = f2.delete_random_cells(75, seed);
	//	auto [sol_cnt, sol_table ] = f2.get_solution(sudocu);

	//	if (sol_cnt == 1) {
	//		bool is_equal = std::equal(f2.field_.begin(), f2.field_.end(), sol_table.begin());
	//		if (!is_equal)
	//			++cnt_error;
	//		
	//		++cnt_success;
	//		std::cout << std::boolalpha<< "is equal "<< cnt_success <<": " << is_equal << std::endl;
	//	}
	//}
	//std::cout << "cnt errors: " << cnt_error << std::endl;
	//std::cout << "cnt success: " << cnt_success << std::endl;
	//while (cnt--)
		//for (Field& f : std::vector{ f2,f3, f4 }) {
		//	f.random_fill(1);
		//	std::cout << f << std::endl;
		//	for (const auto& vec : f.build_sudocu(45, 1)) {
		//		for (uint16_t num : vec)
		//			std::cout << num << ' ';
		//		std::cout << '\n';
		//	}
		//	std::cout << std::endl;
		//}
	
	return 0;
}