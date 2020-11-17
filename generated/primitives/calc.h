#ifndef calc_H
#define calc_H

#include "../vector_extension_structs.h"

namespace vectorlib{
	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct add {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::vector_t const & p_vec1,
			 typename ProcessingStyle::vector_t const & p_vec2,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t add (
		 typename ProcessingStyle::vector_t const & p_vec1,
		 typename ProcessingStyle::vector_t const & p_vec2,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		add		<
			ProcessingStyle,
			Granularity
		>
		::apply(		p_vec1, 		p_vec2, 		element_count);
	}
	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct hadd {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::base_t 
		apply (
			 typename ProcessingStyle::vector_t const & p_vec1,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::base_t hadd (
		 typename ProcessingStyle::vector_t const & p_vec1,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		hadd		<
			ProcessingStyle,
			Granularity
		>
		::apply(		p_vec1, 		element_count);
	}
	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct sub {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::vector_t const & p_vec1,
			 typename ProcessingStyle::vector_t const & p_vec2,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t sub (
		 typename ProcessingStyle::vector_t const & p_vec1,
		 typename ProcessingStyle::vector_t const & p_vec2,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		sub		<
			ProcessingStyle,
			Granularity
		>
		::apply(		p_vec1, 		p_vec2, 		element_count);
	}
	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct mul {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::vector_t const & p_vec1,
			 typename ProcessingStyle::vector_t const & p_vec2,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t mul (
		 typename ProcessingStyle::vector_t const & p_vec1,
		 typename ProcessingStyle::vector_t const & p_vec2,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		mul		<
			ProcessingStyle,
			Granularity
		>
		::apply(		p_vec1, 		p_vec2, 		element_count);
	}
	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	struct div {
		MSV_CXX_ATTRIBUTE_FORCE_INLINE
		static
		typename ProcessingStyle::vector_t 
		apply (
			 typename ProcessingStyle::vector_t const & p_vec1,
			 typename ProcessingStyle::vector_t const & p_vec2,
			 int element_count = ProcessingStyle::vector_helper_t::element_count::value
		) = delete;
	}

	template<
		class ProcessingStyle,
		int Granularity = ProcessingStyle::vector_helper_t::granularity::value
	>
	MSV_CXX_ATTRIBUTE_FORCE_INLINE
	static
	typename ProcessingStyle::vector_t div (
		 typename ProcessingStyle::vector_t const & p_vec1,
		 typename ProcessingStyle::vector_t const & p_vec2,
		 int element_count = ProcessingStyle::vector_helper_t::element_count::value)
	{
		return 		div		<
			ProcessingStyle,
			Granularity
		>
		::apply(		p_vec1, 		p_vec2, 		element_count);
	}
}
#endif
