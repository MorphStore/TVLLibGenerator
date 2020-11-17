#ifndef calc_sse_H
#define calc_sse_H

#include "../../../preprocessor.h"
#include "../extension_sse.h
#include "../../../primitives/calc.h"
#include <functional>

namespace vectorlib{
	template<
	>
	struct add <sse<v128<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename sse<v128<uint64_t>>::vector_t apply (
			 typename sse<v128<uint64_t>>::vector_t const & p_vec1,
			 typename sse<v128<uint64_t>>::vector_t const & p_vec2,
			 int element_count = sse<v128<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _mm_add_epi64( p_vec1, p_vec2);
		}
	}

	template<
	>
	struct add <sse<v128<uint32_t>>,  32> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename sse<v128<uint32_t>>::vector_t apply (
			 typename sse<v128<uint32_t>>::vector_t const & p_vec1,
			 typename sse<v128<uint32_t>>::vector_t const & p_vec2,
			 int element_count = sse<v128<uint32_t>>::vector_helper_t::element_count::value)
		{
			return _mm_add_epi32( p_vec1, p_vec2);
		}
	}

}
#endif
