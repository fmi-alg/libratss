#ifndef LIBRATSS_CGAL_EXTENDED_INT64_Z_TRAITS_H
#define LIBRATSS_CGAL_EXTENDED_INT64_Z_TRAITS_H

#include <CGAL/Gmpz.h>

#include <libratss/types.h>

#include <boost/multiprecision/cpp_int.hpp>

#include <type_traits>

namespace CGAL {
namespace internal {
	
using boost_int128 = boost::multiprecision::int128_t;
using boost_int1024 = boost::multiprecision::int1024_t;

namespace imp {
namespace ExtendedInt64zTraits {
	
	template<typename T_EXTENSION_TYPE>
	struct Make {
		using type = T_EXTENSION_TYPE;
		type operator()(uint64_t value) const;
		type operator()(int64_t value) const;
	};
	
	template<typename T_EXTENSION_TYPE, typename T_INTEGRAL_TYPE>
	struct Fits {
		using type = T_EXTENSION_TYPE;
		using integral_type = T_INTEGRAL_TYPE;
		///return true iff type fits into integral_type 
		bool operator()(const type & value) const;
	};
	
	template<typename T_EXTENSION_TYPE, typename T_INTEGRAL_TYPE>
	struct ToIntegral {
		using type = T_EXTENSION_TYPE;
		using integral_type = T_INTEGRAL_TYPE;
		///convert the given value to integral_type
		integral_type operator()(const type & value) const;
	};

#if defined(__LP64__)
	#if defined(__APPLE__)
		#define EI64ZDT_BINARY_OPERATORS_EE_EI_IE(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			class __NAME { \
			public: \
				using type = T_EXTENSION_TYPE; \
				inline auto operator()(const type & first, const type & second) const { return first __OP second; }; \
				inline auto operator()(const type & first, int64_t second) const { return first __OP GmpTraits::signed_number(second); }; \
				inline auto operator()(const type & first, uint64_t second) const { return first __OP GmpTraits::unsigned_number(second); }; \
				inline auto operator()(int64_t first, const type & second) const { return GmpTraits::signed_number(first) __OP second; }; \
				inline auto operator()(uint64_t first, const type & second) const { return GmpTraits::unsigned_number(first) __OP second; }; \
			};
		#define EI64ZDT_BINARY_OPERATORS_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			class __NAME { \
			public: \
				using type = T_EXTENSION_TYPE; \
				inline auto operator()(const type & first, const type & second) const { return first __OP second; }; \
				inline auto operator()(const type & first, int64_t second) const { return first __OP GmpTraits::signed_number(second); }; \
				inline auto operator()(const type & first, uint64_t second) const { return first __OP GmpTraits::unsigned_number(second); }; \
			};
		#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			class __NAME ## Assign: private Make<T_EXTENSION_TYPE> { \
				using MyBase = Make<T_EXTENSION_TYPE>; \
			public: \
				using type = T_EXTENSION_TYPE; \
				inline void operator()(type & first, const type & first) const { first __OP ## = second; }; \
				inline void operator()(type & first, int64_t second) const { first __OP ## = GmpTraits::signed_number(second); }; \
				inline void operator()(type & first, uint64_t second) const { first __OP ## = GmpTraits::unsigned_number(second); }; \
			};
		#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			class __NAME ## Assign: private Make<T_EXTENSION_TYPE> { \
				using MyBase = Make<T_EXTENSION_TYPE>; \
			public: \
				using type = T_EXTENSION_TYPE; \
				inline void operator()(type & first, int64_t second) const { first __OP ## = GmpTraits::signed_number(second); }; \
				inline void operator()(type & first, uint64_t second) const { first __OP ## = GmpTraits::unsigned_number(second); }; \
			};
	#else
		#define EI64ZDT_BINARY_OPERATORS_EE_EI_IE(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			struct __NAME { \
				using type = T_EXTENSION_TYPE; \
				inline auto operator()(const type & first, const type & second) const { return first __OP second; }; \
				inline auto operator()(const type & first, int64_t second) const { return first __OP second; }; \
				inline auto operator()(const type & first, uint64_t second) const { return first __OP second; }; \
				inline auto operator()(int64_t first, const type & second) const { return first __OP second; }; \
				inline auto operator()(uint64_t first, const type & second) const { return first __OP second; }; \
			};
		#define EI64ZDT_BINARY_OPERATORS_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			struct __NAME { \
				using type = T_EXTENSION_TYPE; \
				inline auto operator()(const type & first, int64_t second) const { return first __OP second; }; \
				inline auto operator()(const type & first, uint64_t second) const { return first __OP second; }; \
			};
		#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			struct __NAME ## Assign { \
				using type = T_EXTENSION_TYPE; \
				inline void operator()(type & first, const type & second) const { first __OP second; }; \
				inline void operator()(type & first, int64_t second) const { first __OP ## = second; }; \
				inline void operator()(type & first, uint64_t second) const { first __OP ## = second; }; \
			};
		#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EI(__NAME, __OP) \
			template<typename T_EXTENSION_TYPE> \
			struct __NAME ## Assign { \
				using type = T_EXTENSION_TYPE; \
				inline void operator()(type & first, int64_t second) const { first __OP ## = second; }; \
				inline void operator()(type & first, uint64_t second) const { first __OP ## = second; }; \
			};
	#endif
#else
	#define EI64ZDT_BINARY_OPERATORS_EE_EI_IE(__NAME, __OP) \
		template<typename T_EXTENSION_TYPE> \
		class __NAME: Make<T_EXTENSION_TYPE> { \
			using MyBase = Make<T_EXTENSION_TYPE>; \
		public: \
			using type = T_EXTENSION_TYPE; \
			inline auto operator()(const type & first, const type & second) const { return first __OP second; }; \
			inline auto operator()(const type & first, int64_t second) const { return first __OP MyBase::operator()(second); }; \
			inline auto operator()(const type & first, uint64_t second) const { return first __OP MyBase::operator()(second); }; \
			inline auto operator()(int64_t first, const type & second) const { return MyBase::operator()(first) __OP second; }; \
			inline auto operator()(uint64_t first, const type & second) const { return MyBase::operator()(first) __OP second; }; \
		};
	#define EI64ZDT_BINARY_OPERATORS_EI(__NAME, __OP) \
		template<typename T_EXTENSION_TYPE> \
		class __NAME: Make<T_EXTENSION_TYPE> { \
			using MyBase = Make<T_EXTENSION_TYPE>; \
		public: \
			using type = T_EXTENSION_TYPE; \
			inline auto operator()(const type & first, int64_t second) const { return first __OP MyBase::operator()(second); }; \
			inline auto operator()(const type & first, uint64_t second) const { return first __OP MyBase::operator()(second); }; \
		};
	#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(__NAME, __OP) \
		template<typename T_EXTENSION_TYPE> \
		class __NAME ## Assign: private Make<T_EXTENSION_TYPE> { \
			using MyBase = Make<T_EXTENSION_TYPE>; \
		public: \
			using type = T_EXTENSION_TYPE; \
			inline void operator()(type & first, const type & second) const { first __OP second; }; \
			inline void operator()(type & first, int64_t second) const { first __OP ## = MyBase::operator()(second); }; \
			inline void operator()(type & first, uint64_t second) const { first __OP ## = MyBase::operator()(second); }; \
		};
	#define EI64ZDT_BINARY_ASSIGN_OPERATORS_EI(__NAME, __OP) \
		template<typename T_EXTENSION_TYPE> \
		class __NAME ## Assign: private Make<T_EXTENSION_TYPE> { \
			using MyBase = Make<T_EXTENSION_TYPE>; \
		public: \
			using type = T_EXTENSION_TYPE; \
			inline void operator()(type & first, int64_t second) const { first __OP ## = MyBase::operator()(second); }; \
			inline void operator()(type & first, uint64_t second) const { first __OP ## = MyBase::operator()(second); }; \
		};
#endif
	
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Equal, ==)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(NotEqual, !=)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Less, <)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(LessOrEqual, <=)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Greater, >)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(GreaterOrEqual, >=)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Add, +)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Sub, -)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Mult, *)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Div, /)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(Mod, %)
	EI64ZDT_BINARY_OPERATORS_EI(LShift, <<)
	EI64ZDT_BINARY_OPERATORS_EI(RShift, >>)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(BitOr, |)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(BitAnd, &)
	EI64ZDT_BINARY_OPERATORS_EE_EI_IE(BitXor, ^)
	
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(Add, +)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(Sub, -)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(Mult, *)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(Div, /)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(Mod, %)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EI(LShift, <<)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EI(RShift, >>)
	
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(BitOr, |)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(BitAnd, &)
	EI64ZDT_BINARY_ASSIGN_OPERATORS_EE_EI(BitXor, ^)

#undef EI64ZDT_BINARY_OPERATORS_IMP
#undef EI64ZDT_BINARY_ASSIGN_OPERATORS_IMP

//BEGIN CGAL::Gmpz specializations
template<>
struct Make<CGAL::Gmpz> {
	using type = CGAL::Gmpz;
	type operator()(int64_t v) const;
	type operator()(uint64_t v) const;
};

template<>
struct Fits<CGAL::Gmpz, uint64_t> {
	using type = CGAL::Gmpz;
	using integral_type = uint64_t;
	bool operator()(const type & value) const;
};

template<>
struct Fits<CGAL::Gmpz, int64_t> {
	using type = CGAL::Gmpz;
	using integral_type = int64_t;
	bool operator()(const type & value) const;
};

template<>
struct ToIntegral<CGAL::Gmpz, uint64_t> {
	using type = CGAL::Gmpz;
	using integral_type = uint64_t;
	integral_type operator()(const type & value) const;
};

template<>
struct ToIntegral<CGAL::Gmpz, int64_t> {
	using type = CGAL::Gmpz;
	using integral_type = int64_t;
	integral_type operator()(const type & value) const;
};

#if not defined(__LP64__)
	template<>
	class LShift<CGAL::Gmpz>: private Make<CGAL::Gmpz> {
		using MyBase = Make<CGAL::Gmpz>;
	public:
		using type = CGAL::Gmpz;
		inline type operator()(const type & first, int64_t second) const {assert(uint32_t(second) == second); return first << int32_t(second);};
		inline type operator()(const type & first, uint64_t second) const {assert(uint32_t(second) == second); return first << uint32_t(second); };
	};
	template<>
	class RShift<CGAL::Gmpz>: private Make<CGAL::Gmpz> {
		using MyBase = Make<CGAL::Gmpz>;
	public:
		using type = CGAL::Gmpz;
		inline type operator()(const type & first, int64_t second) const {assert(uint32_t(second) == second); return first >> int32_t(second);};
		inline type operator()(const type & first, uint64_t second) const {assert(uint32_t(second) == second); return first >> uint32_t(second); };
	};
	template<>
	class LShiftAssign<CGAL::Gmpz>: private Make<CGAL::Gmpz> {
		using MyBase = Make<CGAL::Gmpz>;
	public:
		using type = CGAL::Gmpz;
		inline void operator()(type & first, int64_t second) const {assert(uint32_t(second) == second); first <<= int32_t(second);};
		inline void operator()(type & first, uint64_t second) const {assert(uint32_t(second) == second); first <<= uint32_t(second); };
	};
	template<>
	class RShiftAssign<CGAL::Gmpz>: private Make<CGAL::Gmpz> {
		using MyBase = Make<CGAL::Gmpz>;
	public:
		using type = CGAL::Gmpz;
		inline void operator()(type & first, int64_t second) const {assert(uint32_t(second) == second); first >>= int32_t(second);};
		inline void operator()(type & first, uint64_t second) const {assert(uint32_t(second) == second); first >>= uint32_t(second); };
	};
#endif

//END CGAL::Gmpz specializations
//BEGIN boost_int1024 specializations
template<>
struct Make<boost_int1024> {
	using type = boost_int1024;
	type operator()(int64_t v) const;
	type operator()(uint64_t v) const;
};
//END boost_int1024 specializations

}}

template<typename T_EXTENSION_TYPE>
struct ExtendedInt64zTraits {
	using type = T_EXTENSION_TYPE;
	
	template<typename T_INTEGRAL_TYPE>
	using Fits = imp::ExtendedInt64zTraits::Fits<type, T_INTEGRAL_TYPE>;
	
	template<typename T_INTEGRAL_TYPE>
	Fits<T_INTEGRAL_TYPE> fits() const { return Fits<T_INTEGRAL_TYPE>(); }
	
	template<typename T_INTEGRAL_TYPE>
	using ToIntegral = imp::ExtendedInt64zTraits::ToIntegral<type, T_INTEGRAL_TYPE>;
	
	template<typename T_INTEGRAL_TYPE>
	ToIntegral<T_INTEGRAL_TYPE> toIntegral() const { return ToIntegral<T_INTEGRAL_TYPE>(); }
	
#define EI64T_USE(__NAME, __FNAME) \
	using __NAME = imp::ExtendedInt64zTraits:: __NAME <type>; \
	__NAME __FNAME() const { return __NAME(); };
	
	EI64T_USE(Make, make)
	
	EI64T_USE(Equal, equal)
	EI64T_USE(NotEqual, notEqual)
	EI64T_USE(Less, less)
	EI64T_USE(LessOrEqual, lessOrEqual)
	EI64T_USE(Greater, greater)
	EI64T_USE(GreaterOrEqual, greatorOrEqual)
	EI64T_USE(Add, add)
	EI64T_USE(Sub, sub)
	EI64T_USE(Mult, mult)
	EI64T_USE(Div, div)
	EI64T_USE(Mod, mod)
	EI64T_USE(LShift, lShift)
	EI64T_USE(RShift, rShift)
	EI64T_USE(BitOr, bitOr)
	EI64T_USE(BitAnd, bitAnd)
	EI64T_USE(BitXor, bitXor)
	
	EI64T_USE(AddAssign, addAssign)
	EI64T_USE(SubAssign, subAssign)
	EI64T_USE(MultAssign, multAssign)
	EI64T_USE(DivAssign, divAssign)
	EI64T_USE(ModAssign, modAssign)
	EI64T_USE(LShiftAssign, lShiftAssign)
	EI64T_USE(RShiftAssign, rShiftAssign)
	EI64T_USE(BitOrAssign, bitOrAssign)
	EI64T_USE(BitAndAssign, bitAndAssign)
	EI64T_USE(BitXorAssign, bitXorAssign)
#undef EI64T_USE
};

} //end namespace iternal

} //namespace CGAL

#endif // LIBRATSS_CGAL_EXTENDED_INT64_Z_H
