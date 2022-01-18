/// \file
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
#ifndef NIRVANA_SIMPLELIST_H_
#define NIRVANA_SIMPLELIST_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <iterator>
#include <assert.h>

namespace Nirvana {

/// Simplified list.
/// This is intrusive list container implementation.
/// This means that container does not copy the elements but just links them.
/// 
/// On SimpleList copy we get the empty SimpleList.
/// On SimpleList destruction, elements are not destructed but remain linked into a ring.
/// To destruct elements, the clear() must be called explicitly.
/// On the element destruction it will be unlinked from the list.
/// 
/// \tparam T   Element type. T must derive SimpleList <T>::Element.
/// \tparam Tag Optional tag. If T can be listed in multiple SimpleList <T> lists,
///                           each list must have unique tag.
template <class T, int Tag = 0>
class SimpleList
{
public:
	class Element
	{
	public:
		Element () NIRVANA_NOEXCEPT
		{
			m_next = m_prev = this;
		}

		/// On destruction, element removed from list automatically.
		~Element ()
		{
			remove ();
		}

		/// Copied element is not included in list.
		Element (const Element& src) NIRVANA_NOEXCEPT
		{
			m_next = m_prev = this;
		}

		/// Copied element is not included in list.
		Element& operator = (const Element& src) NIRVANA_NOEXCEPT
		{
			m_next = m_prev = this;
			return *this;
		}

		/// Insert element after this element.
		/// 
		/// \param next Next element in list.
		///             If next element included in some list,
		///             it will be removed from it first.
		void insert (Element& next) NIRVANA_NOEXCEPT
		{
			next.__check ();
			remove ();
			m_next = &next;
			(m_prev = next.m_prev)->m_next = this;
			next.m_prev = this;
			__check ();
		}

		/// Remove element from list.
		void remove () NIRVANA_NOEXCEPT
		{
			__check ();
			m_next->m_prev = m_prev;
			m_prev->m_next = m_next;
			m_next = m_prev = this;
		}

		/// \returns `true` if element is included in list.
		bool listed () const NIRVANA_NOEXCEPT
		{
			return m_next != this;
		}

		Element* next () const NIRVANA_NOEXCEPT
		{
			return m_next;
		}

		Element* prev () const NIRVANA_NOEXCEPT
		{
			return m_prev;
		}

		void swap (Element& rhs) NIRVANA_NOEXCEPT
		{
			__check ();
			rhs.__check ();
			std::swap (m_next, rhs.m_next);
			std::swap (m_prev, rhs.m_prev);
		}

	private:
		void __check () const NIRVANA_NOEXCEPT
		{
			assert ((m_next != m_prev) || ((m_next->m_next == this) && (m_next->m_prev == this)));
		}

	private:
		Element* m_next, * m_prev;
	};

	typedef T value_type;
	typedef const T& const_reference;
	typedef T& reference;

	typedef std::iterator <std::bidirectional_iterator_tag, T> _It;

	class iterator;

	class const_iterator : public _It
	{
	public:
		const_iterator () NIRVANA_NOEXCEPT
		{}

		const_iterator (const Element* p) NIRVANA_NOEXCEPT
			: ptr_ (const_cast <Element*> (p))
		{}

		bool operator == (const const_iterator& rhs) const NIRVANA_NOEXCEPT
		{
			return ptr_ == rhs.ptr_;
		}

		bool operator != (const const_iterator& rhs) const NIRVANA_NOEXCEPT
		{
			return ptr_ != rhs.ptr_;
		}

		const_reference operator * () const NIRVANA_NOEXCEPT
		{
			return *static_cast <T*> (ptr_);
		}

		const T* operator -> () const NIRVANA_NOEXCEPT
		{
			return static_cast <T*> (ptr_);
		}

		const_iterator& operator ++ () NIRVANA_NOEXCEPT
		{
			ptr_ = ptr_->next ();
			return *this;
		}

		const_iterator operator ++ (int) NIRVANA_NOEXCEPT
		{
			const_iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		const_iterator& operator -- () NIRVANA_NOEXCEPT
		{
			ptr_ = ptr_->prev ();
			return *this;
		}

		const_iterator operator -- (int) NIRVANA_NOEXCEPT
		{
			const_iterator tmp = *this;
			operator -- ();
			return tmp;
		}

	protected:
		Element* ptr_;
	};

	class iterator : public const_iterator
	{
	public:
		iterator () NIRVANA_NOEXCEPT :
			const_iterator ()
		{}

		iterator (Element* p) NIRVANA_NOEXCEPT :
			const_iterator (p)
		{}

		reference operator * () const NIRVANA_NOEXCEPT
		{
			return const_cast <T&> (const_iterator::operator* ());
		}

		T* operator -> () const NIRVANA_NOEXCEPT
		{
			return const_cast <T*> (const_iterator::operator-> ());
		}

		iterator& operator ++ () NIRVANA_NOEXCEPT
		{
			const_iterator::operator++ ();
			return *this;
		}

		iterator operator ++ (int) NIRVANA_NOEXCEPT
		{
			iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		iterator& operator -- () NIRVANA_NOEXCEPT
		{
			const_iterator::operator-- ();
			return *this;
		}

		iterator operator -- (int) NIRVANA_NOEXCEPT
		{
			iterator tmp = *this;
			operator -- ();
			return tmp;
		}
	};

	const_iterator begin () const NIRVANA_NOEXCEPT
	{
		return const_iterator (root_.next ());
	}

	iterator begin () NIRVANA_NOEXCEPT
	{
		return iterator (root_.next ());
	}

	const_iterator end () const NIRVANA_NOEXCEPT
	{
		return const_iterator (root_);
	}

	iterator end () NIRVANA_NOEXCEPT
	{
		return iterator (&root_);
	}

	const_reference front () const NIRVANA_NOEXCEPT
	{
		assert (!empty ());
		return *begin ();
	}

	reference front () NIRVANA_NOEXCEPT
	{
		assert (!empty ());
		return *begin ();
	}

	const_reference back () const NIRVANA_NOEXCEPT
	{
		assert (!empty ());
		return *const_iterator ((end ()->prev ()));
	}

	reference back () NIRVANA_NOEXCEPT
	{
		assert (!empty ());
		return *iterator ((end ()->prev ()));
	}

	void clear () NIRVANA_NOEXCEPT
	{
		while (!empty ())
			delete (&*begin ());
	}

	bool empty () const NIRVANA_NOEXCEPT
	{
		return !root_.listed ();
	}

	iterator insert (iterator it, T& elem) NIRVANA_NOEXCEPT
	{
		elem.insert (*it);
		return iterator (&elem);
	}

	void push_back (T& elem) NIRVANA_NOEXCEPT
	{
		insert (end (), elem);
	}

	void push_front (T& elem) NIRVANA_NOEXCEPT
	{
		insert (begin (), elem);
	}

	void remove (iterator it) NIRVANA_NOEXCEPT
	{
		it->remove ();
	}

	void swap (SimpleList& rhs) NIRVANA_NOEXCEPT
	{
		root_.swap (rhs.root_);
	}

	void transfer (SimpleList& rhs) NIRVANA_NOEXCEPT
	{
		if (rhs.root_.listed ()) {
			Element* pnext = rhs.root_.next ();
			rhs.root_.remove ();
			root_.insert (*pnext);
		} else
			root_.remove ();
	}

private:
	Element root_;
};

}

#endif
