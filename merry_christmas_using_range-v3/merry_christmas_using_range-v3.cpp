// merry_christmas_using_range-v3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <range/v3/all.hpp>

int main()
{
	auto make_christmas_tree = [](auto const level, auto const& message) {
		auto index = ranges::view::iota(3)
			| ranges::view::filter([](auto const e) { return e % 2 == 1; })
			| ranges::view::take(level)
			;
		auto rindex = index
			| ranges::view::reverse
			;

		auto zipped = ranges::view::zip(index, rindex);

		std::vector<std::vector<std::string>> christmas_tree = zipped
			| ranges::view::transform([](auto const& n) {
				auto make_triangle = [](auto const n) {
					auto spaces = ranges::view::iota(1)
						| ranges::view::transform([](auto const e) {
							return std::string(e - 1, ' ');
						})
						| ranges::view::take(n)
						| ranges::view::reverse
						;

					auto stars = ranges::view::iota(1)
						| ranges::view::transform([](auto const e) {
							auto stars = std::string(e, '*');
							std::string stars_with_space = stars | ranges::view::intersperse(' ');

							return stars_with_space;
						})
						| ranges::view::take(n)
						;

					std::vector<std::string> result = ranges::view::zip_with([](auto const& s1, auto const& s2) { return s1 + s2; }, spaces, stars);

					return result;
				};

				auto indent = [](auto const n) {
					return ranges::make_pipeable([=](auto&& rng) {
						auto input = ranges::view::repeat(n)
							| ranges::view::transform([](auto const n) { return std::string(n, ' '); })
							| ranges::view::take(ranges::distance(rng))
							;

						std::vector<std::string> result = ranges::view::zip_with([](auto const& s1, auto const& s2) { return s1 + s2; }, input, rng);

						return result;
					});
				};

				return make_triangle(n.first) | indent(n.second);
			})
			;
		std::vector<std::string> result = christmas_tree 
			| ranges::view::join
			;

		return result |= ranges::action::push_back("")
			| ranges::action::push_back(std::string(ranges::max(1, (static_cast<int>(std::size(result.back())) - static_cast<int>(std::size(message))) / 2 + 2), ' ') + message);;
	};

	ranges::copy(make_christmas_tree(3, "Merry Christmas!!!!"), ranges::ostream_iterator<std::string>(std::cout, "\n"));

	return 0;
}