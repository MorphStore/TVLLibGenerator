#ifndef io_tsubasa_H
#define io_tsubasa_H

#include "../../../preprocessor.h"
#include "../extension_tsubasa.h
#include "../../../primitives/io.h"
#include <functional>

namespace vectorlib{
	template<
	>
	struct load <tsubasa<v16384<uint64_t>>, iov::ALIGNED,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::::base_t * p_DataPtr,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			 return  _vel_vst_vssl(p_vec, 8, reinterpret_cast<void*> (p_DataPtr), element_count);
		}
	}

	template<
	>
	struct store <tsubasa<v16384<uint64_t>>, iov::ALIGNED,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		void apply (
			 typename tsubasa<v16384<uint64_t>>::::base_t * p_DataPtr,
			 typename tsubasa<v16384<uint64_t>>::::vector_t * p_vec,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			return _vel_vld_vssl(8, reinterpret_cast< void const * >(p_DataPtr), element_count);
		}
	}

	template<
	>
	struct gather <tsubasa<v16384<uint64_t>>, iov::ALIGNED,  64> {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename tsubasa<v16384<uint64_t>>::vector_t apply (
			 typename tsubasa<v16384<uint64_t>>::::base_t * p_DataPtr,
			 typename tsubasa<v16384<uint64_t>>::::vector_t * p_vec,
			 int element_count = tsubasa<v16384<uint64_t>>::vector_helper_t::element_count::value)
		{
			typename tsubasa<v16384<uint64_t>>::vector_t vy = _vel_vsfa_vvssl(p_vec, 3, reinterpret_cast<uint64_t>(p_DataPtr), element_count); // shift left by 3 and add a, should depend from scale
         return _vel_vgt_vvssl(vy, 
                              0, //lowest address reinterpret_cast<uint64_t> (p_DataPtr)
                              0, //highest address
                              element_count);
		}
	}

}
#endif
