/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/

// Based on  https://github.com/tobiaslocker/base64

#ifndef BASE_64_H_
#define BASE_64_H_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>

namespace base64 {

inline constexpr const char base64_chars [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                              "abcdefghijklmnopqrstuvwxyz"
                                              "0123456789+/";

template<class OutputBuffer, class InputIterator>
inline OutputBuffer encode_into(InputIterator begin, InputIterator end) {
	static_assert(std::is_same_v<std::decay_t<decltype(*begin)>, char>
		|| std::is_same_v<std::decay_t<decltype(*begin)>, unsigned char>
		|| std::is_same_v<std::decay_t<decltype(*begin)>, std::byte>);

  size_t counter = 0;
  uint32_t bit_stream = 0;
  size_t offset = 0;
  OutputBuffer encoded;
	encoded.reserve(static_cast<size_t>(1.5 * static_cast<double>(std::distance(begin, end))));
  while(begin != end) {
		auto const num_val = static_cast<unsigned char>(*begin);
    offset = 16 - counter % 3 * 8;
    bit_stream += num_val << offset;
    if (offset == 16) {
      encoded.push_back(base64_chars[bit_stream >> 18 & 0x3f]);
    }
    if (offset == 8) {
      encoded.push_back(base64_chars[bit_stream >> 12 & 0x3f]);
    }
    if (offset == 0 && counter != 3) {
      encoded.push_back(base64_chars[bit_stream >> 6 & 0x3f]);
      encoded.push_back(base64_chars[bit_stream & 0x3f]);
      bit_stream = 0;
    }
    ++counter;
		++begin;
  }
  if (offset == 16) {
    encoded.push_back(base64_chars[bit_stream >> 12 & 0x3f]);
    encoded.push_back('=');
		encoded.push_back('=');
  }
  if (offset == 8) {
    encoded.push_back(base64_chars[bit_stream >> 6 & 0x3f]);
		encoded.push_back('=');
  }
  return encoded;
}

template<class OutputBuffer, class InputIterator>
inline OutputBuffer decode_into(InputIterator begin, InputIterator end) {
	using value_type = typename OutputBuffer::value_type;
	static_assert(std::is_same_v<value_type, char>
		|| std::is_same_v<value_type, unsigned char>
		|| std::is_same_v<value_type, std::byte>);

  size_t counter = 0;
  uint32_t bit_stream = 0;
  OutputBuffer decoded;
	decoded.reserve(std::distance (begin, end));
  while (begin != end) {
    unsigned char c = *begin;
    auto const num_val = std::find (base64_chars, std::end (base64_chars), c);
    if (num_val != std::end (base64_chars)) {
      auto const offset = 18 - counter % 4 * 6;
      bit_stream += static_cast<uint32_t>(num_val - base64_chars) << offset;
      if (offset == 12) {
        decoded.push_back(static_cast<value_type>(bit_stream >> 16 & 0xff));
      }
      if (offset == 6) {
        decoded.push_back(static_cast<value_type>(bit_stream >> 8 & 0xff));
      }
      if (offset == 0 && counter != 4) {
        decoded.push_back(static_cast<value_type>(bit_stream & 0xff));
        bit_stream = 0;
      }
    } else if (c != '=') {
      throw std::runtime_error{"Invalid base64 encoded data"};
    }
    ++counter;
    ++begin;
  }
  return decoded;
}

} // namespace base64

#endif // BASE_64_H_
