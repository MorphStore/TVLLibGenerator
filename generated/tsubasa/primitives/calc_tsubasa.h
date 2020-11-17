#ifndef calc_tsubasa_H
#define calc_tsubasa_H

#include "../../../preprocessor.h"
#include "../extension_tsubasa.h
#include "../../../primitives/calc.h"
#include <functional>

namespace vectorlib{
	template<
	>
	struct add <tsubasa<v16384<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec1,
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec2,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _vel_vaddul_vvvl(p_vec1, p_vec2, element_count);
		}
	}

	template<
	>
	struct hadd <tsubasa<v16384<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::base_t apply (
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec1,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			_vel_lvsl_svs(_vel_vsuml_vvl(p_vec1, element_count), 0);
		}
	}

	template<
	>
	struct sub <tsubasa<v16384<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec1,
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec2,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _vel_vsubul_vvvl(p_vec1, p_vec2, element_count);
		}
	}

	template<
	>
	struct mul <tsubasa<v16384<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec1,
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec2,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _vel_vmulul_vvvl(p_vec1, p_vec2, element_count);
		}
	}

	template<
	>
	struct div <tsubasa<v16384<uint64_t>>,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec1,
			 typename tsubasa<v16384<uint64_t>>::vector_t const & p_vec2,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _vel_vdivul_vvvl(p_vec1, p_vec2, element_count);
		}
	}

}
#endif
