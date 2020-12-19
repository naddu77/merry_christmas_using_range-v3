#include "pch.h"
#include <range/v3/all.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <concepts>

namespace rv = ranges::views;
using namespace std::string_literals;

template <std::integral T, std::convertible_to<std::string>... Ts>
auto MakeChristmasTree(T const level, Ts const&... messages)
{
	auto const msg_max_len{
		1 + ranges::accumulate(
			std::vector<std::string>{ messages... },
			int{},
			[](auto const acc, auto const& msg) {
				return std::max(acc, static_cast<int>(ranges::size(msg)));
			})
	};
	auto const center_pos{ 1 + level * 4 };
	auto index{
		rv::iota(3)
		| rv::stride(2)
		| rv::take(level)
	};
	auto rindex{
		index
		| rv::reverse
	};

	return rv::zip(index, rindex)
		| rv::transform([msg_max_len, center_pos](auto const& p) {
			auto make_triangle{ [](auto const n) {
				auto spaces{ rv::iota(1)
					| rv::transform([](auto const e) {
						return rv::repeat_n(' ', e);
					})
					| rv::take(n)
					| rv::reverse
				};
				auto stars{ rv::iota(1)
					| rv::transform([](auto const e) {
						return rv::repeat_n('*', e) | rv::intersperse(' ');
					})
					| rv::take(n)
				};

				return rv::zip_with([](auto const& s1, auto const& s2) { return rv::concat(s1, s2); }, spaces, stars);
			} };
			auto indent{ [msg_max_len, center_pos](auto const n) {
				return ranges::make_pipeable([=](auto&& rng) {
					auto const padding{ std::max<int>(0, (msg_max_len - center_pos) / 2) };
					auto input{ rv::repeat(n)
						| rv::transform([msg_max_len, padding](auto const n) { return rv::repeat_n(' ', n - 3 + padding); })
						| rv::take(ranges::distance(rng))
					};

					return rv::zip_with([](auto const& s1, auto const& s2) { return rv::concat(s1, s2); }, input, std::move(rng));
				});
			} };

			return make_triangle(p.first) | indent(p.second);
		})
		| rv::join
		| ranges::to<std::vector<std::string>>()
		| ranges::actions::push_back(std::vector<std::string>(3, std::string(std::max(msg_max_len / 2 - 1, center_pos / 2), ' ') + "***"))
		| ranges::actions::push_back({
			""s,
			std::string((std::max(msg_max_len, center_pos) - ranges::size(messages)) / 2 + 2, ' ') + messages...
		});
}
int main()
{
	auto n{ 0 };

	do {
		std::cout << "How big would you like to make a Christmas tree? [n >= 3]\n> ";
		
		if (std::string line; std::getline(std::cin, line))
		{
			try
			{
				n = std::stoi(line);
			}
			catch (...)
			{
				std::cerr << "Please enter a number of 3 or more.\n";
				
				n = 0;
			}
		}
	} while (n < 3);

	ranges::copy(MakeChristmasTree(n, "Merry Christmas!!!!", "C++Korea"), ranges::ostream_iterator(std::cout, "\n"));

	return 0;
}