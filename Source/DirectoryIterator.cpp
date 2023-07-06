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
#include <Nirvana/DirectoryIterator.h>

namespace Nirvana {

DirectoryIterator::DirectoryIterator (Dir::_ptr_type dir, uint_fast16_t flags,
	const std::string& regexp, unsigned block_size) :
	block_size_ (block_size ? block_size : 1),
	cur_pos_ (0)
{
	dir->opendir (regexp, flags, block_size, block_, iterator_);
}

const DirEntry* DirectoryIterator::readdir ()
{
	if (cur_pos_ >= block_.size ()) {
		cur_pos_ = 0;
		if (!iterator_)
			return nullptr;
		bool result;
		if (block_size_ > 1)
			result = iterator_->next_n (block_size_, block_);
		else {
			block_.resize (1);
			result = iterator_->next_one (block_.front ());
		}
		if (!result) {
			iterator_ = nullptr;
			block_.clear ();
			return nullptr;
		}
	}
	return block_.data () + (cur_pos_++);
}

}
