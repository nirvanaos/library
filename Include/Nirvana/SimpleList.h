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
		Element () noexcept
		{
			m_next = m_prev = this;
		}

		/// On destruction, element removed from list automatically.
		~Element ()
		{
			remove ();
		}

		/// Copied element is not included in list.
		Element (const Element& src) noexcept
		{
			m_next = m_prev = this;
		}

		/// Copied element is not included in list.
		Element& operator = (const Element& src) noexcept
		{
			m_next = m_prev = this;
			return *this;
		}

		/// Insert element after this element.
		/// 
		/// \param next Next element in list.
		///             If next element included in some list,
		///             it will be removed from it first.
		void insert (Element& next) noexcept
		{
			next.__check ();
			remove ();
			m_next = &next;
			(m_prev = next.m_prev)->m_next = this;
			next.m_prev = this;
			__check ();
		}

		/// Remove element from list.
		void remove () noexcept
		{
			__check ();
			m_next->m_prev = m_prev;
			m_prev->m_next = m_next;
			m_next = m_prev = this;
		}

		/// \returns `true` if element is included in list.
		bool listed () const noexcept
		{
			return m_next != this;
		}

		Element* next () const noexcept
		{
			return m_next;
		}

		Element* prev () const noexcept
		{
			return m_prev;
		}

		void swap (Element& rhs) noexcept
		{
			__check ();
			rhs.__check ();
			std::swap (m_next, rhs.m_next);
			std::swap (m_prev, rhs.m_prev);
		}

	private:
		void __check () const noexcept
		{
			assert ((m_next != m_prev) || ((m_next->m_next == this) && (m_next->m_prev == this)));
		}

	private:
		Element* m_next, * m_prev;
	};

	typedef T value_type;
	typedef const T& const_reference;
	typedef T& reference;

	class _It
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;
	};

	class iterator;

	class const_iterator : public _It
	{
	public:
		const_iterator () noexcept
		{}

		const_iterator (const Element* p) noexcept
			: ptr_ (const_cast <Element*> (p))
		{}

		bool operator == (const const_iterator& rhs) const noexcept
		{
			return ptr_ == rhs.ptr_;
		}

		bool operator != (const const_iterator& rhs) const noexcept
		{
			return ptr_ != rhs.ptr_;
		}

		const_reference operator * () const noexcept
		{
			return *static_cast <T*> (ptr_);
		}

		const T* operator -> () const noexcept
		{
			return static_cast <T*> (ptr_);
		}

		const_iterator& operator ++ () noexcept
		{
			ptr_ = ptr_->next ();
			return *this;
		}

		const_iterator operator ++ (int) noexcept
		{
			const_iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		const_iterator& operator -- () noexcept
		{
			ptr_ = ptr_->prev ();
			return *this;
		}

		const_iterator operator -- (int) noexcept
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
		iterator () noexcept :
			const_iterator ()
		{}

		iterator (Element* p) noexcept :
			const_iterator (p)
		{}

		reference operator * () const noexcept
		{
			return const_cast <T&> (const_iterator::operator* ());
		}

		T* operator -> () const noexcept
		{
			return const_cast <T*> (const_iterator::operator-> ());
		}

		iterator& operator ++ () noexcept
		{
			const_iterator::operator++ ();
			return *this;
		}

		iterator operator ++ (int) noexcept
		{
			iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		iterator& operator -- () noexcept
		{
			const_iterator::operator-- ();
			return *this;
		}

		iterator operator -- (int) noexcept
		{
			iterator tmp = *this;
			operator -- ();
			return tmp;
		}
	};

	iterator begin () const noexcept
	{
		return iterator (root_.next ());
	}

	const_iterator cbegin () const noexcept
	{
		return const_iterator (root_.next ());
	}

	iterator end () const noexcept
	{
		return iterator (const_cast <Element*> (&root_));
	}

	const_iterator cend () const noexcept
	{
		return const_iterator (&root_);
	}

	const_reference front () const noexcept
	{
		assert (!empty ());
		return *begin ();
	}

	reference front () noexcept
	{
		assert (!empty ());
		return *begin ();
	}

	const_reference back () const noexcept
	{
		assert (!empty ());
		return *const_iterator ((end ()->prev ()));
	}

	reference back () noexcept
	{
		assert (!empty ());
		return *iterator ((end ()->prev ()));
	}

	void clear () noexcept
	{
		while (!empty ())
			delete (&*begin ());
	}

	bool empty () const noexcept
	{
		return !root_.listed ();
	}

	iterator insert (iterator it, T& elem) noexcept
	{
		static_cast <Element&> (elem).insert (*it);
		return iterator (&elem);
	}

	void push_back (T& elem) noexcept
	{
		insert (end (), elem);
	}

	void push_front (T& elem) noexcept
	{
		insert (begin (), elem);
	}

	void remove (iterator it) noexcept
	{
		it->remove ();
	}

	void swap (SimpleList& rhs) noexcept
	{
		root_.swap (rhs.root_);
	}

	void transfer (SimpleList& rhs) noexcept
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
