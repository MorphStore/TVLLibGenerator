#ifndef io_H
#define io_H

#include "../vector_extension_structs.h"

#include "../../header/iov.h"

namespace vectorlib{
	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct load {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::::base_t * p_DataPtr,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t load (
		 typename ProcessingStyle::::base_t * p_DataPtr,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		load		<
			ProcessingStyle,
			IOVariant,
			Granularity
		>
		::apply(		p_DataPtr, 		element_count);
	}
	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct store {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		void 
		apply (
			 typename ProcessingStyle::::base_t * p_DataPtr,
			 typename ProcessingStyle::::vector_t * p_vec,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	void store (
		 typename ProcessingStyle::::base_t * p_DataPtr,
		 typename ProcessingStyle::::vector_t * p_vec,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		store		<
			ProcessingStyle,
			IOVariant,
			Granularity
		>
		::apply(		p_DataPtr, 		p_vec, 		element_count);
	}
	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct gather {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::::base_t * p_DataPtr,
			 typename ProcessingStyle::::vector_t * p_vec,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		iov IOVariant,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t gather (
		 typename ProcessingStyle::::base_t * p_DataPtr,
		 typename ProcessingStyle::::vector_t * p_vec,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		gather		<
			ProcessingStyle,
			IOVariant,
			Granularity
		>
		::apply(		p_DataPtr, 		p_vec, 		element_count);
	}
}
#endif
