#ifndef NIRVANA_STATIC_H_
#define NIRVANA_STATIC_H_

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/list/size.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/list/first_n.hpp>
#include <boost/preprocessor/list/fold_left.hpp>
#include <boost/preprocessor/list/at.hpp>
#include <boost/preprocessor/list/adt.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/logical/or.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include "OLF.h"

#define NIRVANA_STATIC_NS_B(r, data, elem) namespace elem {
#define NIRVANA_STATIC_NS_E(z, n, data) }
#define NIRVANA_STATIC_CAT_NS(s, state, elem) state::elem
#define NIRVANA_STATIC_CAT_ID(s, state, elem) state "/" BOOST_PP_STRINGIZE(elem)
#define NIRVANA_STATIC_CAT_U(s, state, elem) BOOST_PP_CAT(state##_, elem)

#define NIRVANA_CHECK_VERSION(ma, mi) static_assert(ma >= 0 && ma <= 0xFFFF && mi >= 0 && mi <= 0xFFFF, "Invalid version number");
#define NIRVANA_VERSION(ma, mi) BOOST_PP_IF(BOOST_PP_OR(ma, mi), ":" #ma "." #mi,)

#define NIRVANA_STATIC_IMPL(ids, nscnt, Primary, ver) BOOST_PP_LIST_FOR_EACH(NIRVANA_STATIC_NS_B,, BOOST_PP_LIST_FIRST_N(nscnt, ids))\
class BOOST_PP_LIST_AT(ids, nscnt);\
BOOST_PP_REPEAT(nscnt, NIRVANA_STATIC_NS_E,)\
namespace Nirvana {\
template <> const char StaticId <BOOST_PP_LIST_FOLD_LEFT(NIRVANA_STATIC_CAT_NS, , ids)>::static_id_ [] =\
BOOST_PP_LIST_FOLD_LEFT(NIRVANA_STATIC_CAT_ID, BOOST_PP_STRINGIZE(BOOST_PP_LIST_FIRST(ids)), BOOST_PP_LIST_REST(ids)) ver;\
template <> struct PrimaryInterface <BOOST_PP_LIST_FOLD_LEFT(NIRVANA_STATIC_CAT_NS, , ids)> { typedef Primary Itf; };}

#define NIRVANA_STATIC_LIST(ids, Primary, ver) NIRVANA_STATIC_IMPL(ids, BOOST_PP_DEC(BOOST_PP_LIST_SIZE(ids)), Primary, ver)

#define NIRVANA_STATIC(ids, Primary) NIRVANA_STATIC_LIST(BOOST_PP_TUPLE_TO_LIST(ids), Primary,)

#define NIRVANA_STATIC_VER(tup, Primary, ma, mi) NIRVANA_CHECK_VERSION(ma, mi)\
NIRVANA_STATIC_LIST(BOOST_PP_TUPLE_TO_LIST(tup), Primary, NIRVANA_VERSION (ma, mi))

#define NIRVANA_STATIC_EXP_LIST_ID(ids) BOOST_PP_LIST_FOLD_LEFT(NIRVANA_STATIC_CAT_U, exp, ids)
#define NIRVANA_STATIC_EXP_LIST(ids) extern "C" const void* NIRVANA_STATIC_EXP_LIST_ID(ids) = (const void*)&BOOST_PP_LIST_FOLD_LEFT(NIRVANA_STATIC_CAT_NS, , ids)::export_struct_;\
__pragma(comment (linker, "/include:" C_NAME_PREFIX BOOST_PP_STRINGIZE(NIRVANA_STATIC_EXP_LIST_ID(ids))))

#define NIRVANA_STATIC_EXP(...) NIRVANA_STATIC_EXP_LIST(BOOST_PP_VARIADIC_TO_LIST(__VA_ARGS__))

#endif
