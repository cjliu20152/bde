// bslmf_isbitwisemoveable.t.cpp                                      -*-C++-*-
#include <bslmf_isbitwisemoveable.h>

#include <bslmf_addpointer.h>
#include <bslmf_addconst.h>
#include <bslmf_addcv.h>
#include <bslmf_addlvaluereference.h>
#include <bslmf_addvolatile.h>
#include <bslmf_nestedtraitdeclaration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

using namespace BloombergLP;
using namespace bsl;

// BDE_VERIFY pragma : -IND01

//=============================================================================
//                                TEST PLAN
//-----------------------------------------------------------------------------
//                                Overview
//                                --------
// The component under test defines a meta-function,
// 'bslmf::IsBitwiseMoveable', that determines whether a template parameter
// type is bitwise movable.  By default, the meta-function supports a
// restricted set of type categories and can be extended to support other types
// through either template specialization or use of the
// 'BSLMF_NESTED_TRAIT_DECLARATION' macro.
//
// Thus, we need to ensure that the natively supported types are correctly
// identified by the meta-function by testing the meta-function with each of
// the supported type categories.  We also need to verify that the
// meta-function can be correctly extended to support other types through
// either of the two supported mechanisms.  Finally, we need to test correct
// support for cv-qualified and array types, where the underlying type may be
// bitwise movable.
//
// ----------------------------------------------------------------------------
// PUBLIC CLASS DATA
// [ 1] bslmf::IsBitwiseMoveable::value
//
// ----------------------------------------------------------------------------
// [ 3] USAGE EXAMPLE
// [ 2] Extending bslmf::IsBitwiseMoveable

//-----------------------------------------------------------------------------

//==========================================================================
//                  STANDARD BDE ASSERT TEST MACRO
//--------------------------------------------------------------------------
// NOTE: THIS IS A LOW-LEVEL COMPONENT AND MAY NOT USE ANY C++ LIBRARY
// FUNCTIONS, INCLUDING IOSTREAMS.

namespace {

int testStatus = 0;

int verbose = 0;
int veryVerbose = 0;
int veryVeryVerbose = 0;

void aSsErT(int c, const char *s, int i) {
    if (c) {
        printf("Error " __FILE__ "(%d): %s    (failed)\n", i, s);
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

}  // close unnamed namespace

# define ASSERT(X) { aSsErT(!(X), #X, __LINE__); }

//=============================================================================
//                       STANDARD BDE TEST DRIVER MACROS
//-----------------------------------------------------------------------------
#define LOOP_ASSERT  BSLS_BSLTESTUTIL_LOOP_ASSERT
#define LOOP2_ASSERT BSLS_BSLTESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLS_BSLTESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLS_BSLTESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLS_BSLTESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLS_BSLTESTUTIL_LOOP6_ASSERT

#define Q   BSLS_BSLTESTUTIL_Q   // Quote identifier literally.
#define P   BSLS_BSLTESTUTIL_P   // Print identifier and value.
#define P_  BSLS_BSLTESTUTIL_P_  // P(X) without '\n'.
#define T_  BSLS_BSLTESTUTIL_T_  // Print a tab (w/o newline).
#define L_  BSLS_BSLTESTUTIL_L_  // current Line number

//=============================================================================
//                  COMPONENT SPECIFIC MACROS FOR TESTING
//-----------------------------------------------------------------------------

// Each of the macros below will test the 'bslmf::IsBitwiseMoveable' trait with
// a set of variations on a type.  There are several layers of macros, as
// object types support the full range of variation, but function types cannot
// form an array, nor be cv-qualified.  Similarly, 'void' may be cv-qualified
// but still cannot form an array.  As macros are strictly text-substitution we
// must use the appropriate 'add_decoration' traits to transform types in a
// manner that is guaranteed to be syntactically valid.  Note that these are
// not type-dependent contexts, so there is no need to use 'typename' when
// fetching the result from any of the queried traits.

#define ASSERT_IS_BITWISE_MOVEABLE_TYPE(TYPE, RESULT)                         \
    ASSERT( bslmf::IsBitwiseMoveable<TYPE>::value == RESULT);                 \
    ASSERT( bslmf::IsBitwiseMoveable<bsl::add_pointer<TYPE>::type>::value);   \
    ASSERT(!bslmf::IsBitwiseMoveable<                                         \
                                bsl::add_lvalue_reference<TYPE>::type>::value);

#define ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE, RESULT)                      \
    ASSERT_IS_BITWISE_MOVEABLE_TYPE(TYPE, RESULT);                            \
    ASSERT_IS_BITWISE_MOVEABLE_TYPE(bsl::add_const<TYPE>::type, RESULT);      \
    ASSERT_IS_BITWISE_MOVEABLE_TYPE(bsl::add_volatile<TYPE>::type, RESULT);   \
    ASSERT_IS_BITWISE_MOVEABLE_TYPE(bsl::add_cv<TYPE>::type, RESULT);

#define ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(TYPE, RESULT)                  \
    ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE, RESULT)                          \
    ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE[128], RESULT)                     \
    ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE[12][8], RESULT)                   \
    ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE[], RESULT)                        \
    ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(TYPE[][8], RESULT)

//=============================================================================
//                      GLOBAL HELPER FUNCTIONS FOR TESTING
//-----------------------------------------------------------------------------

//=============================================================================
//                  GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
//-----------------------------------------------------------------------------

enum { VERBOSE_ARG_NUM = 2, VERY_VERBOSE_ARG_NUM, VERY_VERY_VERBOSE_ARG_NUM };

namespace {

class MyTriviallyCopyableType {
};

struct MyNonTriviallyCopyableType {
    //...
};

}  // close unnamed namespace

namespace bsl {

template <>
struct is_trivially_copyable<MyTriviallyCopyableType> : bsl::true_type {
    // This template specialization for 'is_trivially_copyable' indicates that
    // 'MyTriviallyCopyableType' is a trivially copyable.
};

}  // close namespace bsl


namespace {

struct UserDefinedBwmTestType {
    // This user-defined type, which is marked to be bitwise movable using
    // template specialization (below), is used for testing.
};

struct UserDefinedBwmTestType2 {
    // This user-defined type, which is marked to be bitwise movable using the
    // 'BSLMF_NESTED_TRAIT_DECLARATION' macro, is used for testing.

    BSLMF_NESTED_TRAIT_DECLARATION(UserDefinedBwmTestType2,
                                   BloombergLP::bslmf::IsBitwiseMoveable);
};

struct UserDefinedTcTestType {
    // This user-defined type, which is marked to be trivially copyable using
    // template specialization (below), is used for testing.  Note that
    // trivially copyable types are bitwise movable by default.
};

struct UserDefinedTcTestType2 {
    // This user-defined type, which is marked to be trivially copyable using
    // the 'BSLMF_NESTED_TRAIT_DECLARATION' macro, is used for testing.  Note
    // that trivially copyable types are bitwise movable by default.

    BSLMF_NESTED_TRAIT_DECLARATION(UserDefinedTcTestType2,
                                   bsl::is_trivially_copyable);
};

struct UserDefinedNonTcTestType {
    // This user-defined type, which is not marked to be trivially copyable, is
    // used for testing.
};

enum EnumTestType {
    // This 'enum' type is used for testing.
};

typedef int (UserDefinedNonTcTestType::*MethodPtrTestType) ();
    // This pointer to non-static function member type is used for testing.

}  // close unnamed namespace


namespace bsl {

template <>
struct is_trivially_copyable<UserDefinedTcTestType> : bsl::true_type {
};

}  // close namespace bsl

namespace BloombergLP {
namespace bslmf {

template <>
struct IsBitwiseMoveable<UserDefinedBwmTestType> : bsl::true_type {
};

}  // close package namespace
}  // close enterprise namespace

//=============================================================================
//                  CLASSES FOR TESTING USAGE EXAMPLES
//-----------------------------------------------------------------------------

// BDE_VERIFY pragma : push
// BDE_VERIFY pragma : -AL01   // Strict aliasing (mostly) false-positives
// BDE_VERIFY pragma : -CC01   // C-style casts make this example readable
// BDE_VERIFY pragma : -FD01   // Function contracts are descriptive text
// BDE_VERIFY pragma : -IEC01  // Some example types have implicit ctors

// Used to test detection of legacy traits
#define BSLALG_DECLARE_NESTED_TRAITS(T, TRAIT)            \
    operator TRAIT::NestedTraitDeclaration<T>() const {   \
        return TRAIT::NestedTraitDeclaration<T>();        \
    }

//..
// Now we use this trait in a simple algorithm called 'destructiveMoveArray',
// which moves elements from one array to another.  The algorithm is
// implemented using two implementation functions, one for types that are known
// to be bit-wise moveable, and one for other types.  THe first takes an extra
// function argument of type 'true_type', the second takes and extra function
// argument of type 'false_type':
//..
    namespace BloombergLP {

    template <class TYPE>
    void destructiveMoveArrayImp(TYPE *to,
                                 TYPE *from,
                                 int   size,
                                 bsl::true_type)
    {
        // Bitwise moveable types can be moved using 'memcpy'.
        memcpy(to, from, size * sizeof(TYPE));
    }

    template <class TYPE>
    void destructiveMoveArrayImp(TYPE *to,
                                 TYPE *from,
                                 int   size,
                                 bsl::false_type)
    {
        for (int i = 0; i < size; ++i) {
            ::new(to + i) TYPE(from[i]);
            from[i].~TYPE();
        }
    }
//..
// Now we can dispatch between the two Imp functions, using the
// 'IsBitwiseMoveable' trait metafunction to determine at compile time which of
// the implementations should be used:
//..
    template <class TYPE>
    void destructiveMoveArray(TYPE *to, TYPE *from, int size)
    {
        destructiveMoveArrayImp(to, from, size,
                                bslmf::IsBitwiseMoveable<TYPE>());
    }
//..
// Next, to check our work, we create three classes that we will use to
// instantiate 'destructiveMoveArray'.  All of the classes will log the number
// of constructor and destructor calls.  The first class will not be decorated
// with the 'IsBitwiseMoveable' trait:
//..
    class NonMoveableClass
    {
    private:
        int d_value;

        static int d_ctorCount;
        static int d_dtorCount;

    public:
        static int ctorCount() { return d_ctorCount; }
        static int dtorCount() { return d_dtorCount; }

        NonMoveableClass(int val = 0) : d_value(val) { ++d_ctorCount; }
        NonMoveableClass(const NonMoveableClass& other)
            : d_value(other.d_value) { ++d_ctorCount; }
        ~NonMoveableClass() { d_dtorCount++; }

        int value() const { return d_value; }
    };

    int NonMoveableClass::d_ctorCount = 0;
    int NonMoveableClass::d_dtorCount = 0;
//..
// The second class is similar except that we declare it to be bit-wise
// moveable by specializing 'IsBitwiseMoveable':
//..
    class MoveableClass1
    {
    private:
        int d_value;

        static int d_ctorCount;
        static int d_dtorCount;

    public:
        static int ctorCount() { return d_ctorCount; }
        static int dtorCount() { return d_dtorCount; }

        MoveableClass1(int val = 0) : d_value(val) { ++d_ctorCount; }
        MoveableClass1(const MoveableClass1& other)
            : d_value(other.d_value) { ++d_ctorCount; }
        ~MoveableClass1() { d_dtorCount++; }

        int value() const { return d_value; }
    };

    int MoveableClass1::d_ctorCount = 0;
    int MoveableClass1::d_dtorCount = 0;

    namespace bslmf {
        template <> struct IsBitwiseMoveable<MoveableClass1> : bsl::true_type {
        };
    }  // close package namespace
//..
// The third class is also declared to be bitwise moveable, but this time we do
// it using the 'BSLMF_NESTED_TRAIT_DECLARATION' macro:
//..
    class MoveableClass2
    {
    private:
        int d_value;

        static int d_ctorCount;
        static int d_dtorCount;

    public:
        BSLMF_NESTED_TRAIT_DECLARATION(MoveableClass2,
                                       bslmf::IsBitwiseMoveable);

        static int ctorCount() { return d_ctorCount; }
        static int dtorCount() { return d_dtorCount; }

        MoveableClass2(int val = 0) : d_value(val) { ++d_ctorCount; }
        MoveableClass2(const MoveableClass2& other)
            : d_value(other.d_value) { ++d_ctorCount; }
        ~MoveableClass2() { d_dtorCount++; }

        int value() const { return d_value; }
    };

    int MoveableClass2::d_ctorCount = 0;
    int MoveableClass2::d_dtorCount = 0;
//..
// Finally, invoke 'destructiveMoveArray' on arrays of all three classes:
//..
    enum MoveableEnum { A_VALUE };

    int usageExample1()
    {
        using namespace bslmf;

        // First, check the basic operation of 'IsBitwiseMoveable':
        ASSERT(  IsBitwiseMoveable<int>::value);
        ASSERT(  IsBitwiseMoveable<int*>::value);
        ASSERT(  IsBitwiseMoveable<const int*>::value);
        ASSERT(  IsBitwiseMoveable<MoveableEnum>::value);
        ASSERT(! IsBitwiseMoveable<int&>::value);

        // For each of our test classes, allocate an array, construct three
        // objects into it, then move it into another array.
        const int nObj = 3;

        {
            NonMoveableClass* p1 = (NonMoveableClass*)
                ::operator new(nObj * sizeof(NonMoveableClass));
            NonMoveableClass* p2 =  (NonMoveableClass*)
                ::operator new(nObj * sizeof(NonMoveableClass));

            for (int i = 0; i < nObj; ++i) {
                new(p1 + i) NonMoveableClass(i);
            }

            ASSERT(nObj == NonMoveableClass::ctorCount());
            ASSERT(0    == NonMoveableClass::dtorCount());

            ASSERT(! IsBitwiseMoveable<NonMoveableClass>::value);
            destructiveMoveArray(p2, p1, nObj);

            // Verify that constructor and destructor were called on each move
            ASSERT(2 * nObj == NonMoveableClass::ctorCount());
            ASSERT(nObj     == NonMoveableClass::dtorCount());

            // Verify contents
            for (int i = 0; i < nObj; ++i) {
                ASSERT(i == p2[i].value());
            }

            // Destroy and deallocate
            for (int i = 0; i < nObj; ++i) {
                p2[i].~NonMoveableClass();
            }
            ::operator delete(p1);
            ::operator delete(p2);
        }

        {
            MoveableClass1* p1 = (MoveableClass1*)
                ::operator new(nObj * sizeof(MoveableClass1));
            MoveableClass1* p2 = (MoveableClass1*)
                ::operator new(nObj * sizeof(MoveableClass1));

            for (int i = 0; i < nObj; ++i) {
                ::new(p1 + i) MoveableClass1(i);
            }

            ASSERT(nObj == MoveableClass1::ctorCount());
            ASSERT(0    == MoveableClass1::dtorCount());

            ASSERT(IsBitwiseMoveable<MoveableClass1>::value);
            destructiveMoveArray(p2, p1, nObj);

            // Verify that constructor and destructor were NOT called on each
            // move
            ASSERT(nObj == MoveableClass1::ctorCount());
            ASSERT(0    == MoveableClass1::dtorCount());

            // Verify contents
            for (int i = 0; i < nObj; ++i) {
                ASSERT(i == p2[i].value());
            }

            // Destroy and deallocate
            for (int i = 0; i < nObj; ++i) {
                p2[i].~MoveableClass1();
            }
            ::operator delete(p1);
            ::operator delete(p2);
        }

        {
            MoveableClass2* p1 = (MoveableClass2*)
                ::operator new(nObj * sizeof(MoveableClass2));
            MoveableClass2* p2 = (MoveableClass2*)
                ::operator new(nObj * sizeof(MoveableClass2));

            for (int i = 0; i < nObj; ++i) {
                ::new(p1 + i) MoveableClass2(i);
            }

            ASSERT(nObj == MoveableClass2::ctorCount());
            ASSERT(0    == MoveableClass2::dtorCount());

            ASSERT(IsBitwiseMoveable<MoveableClass2>::value);
            destructiveMoveArray(p2, p1, nObj);

            // Verify that constructor and destructor were NOT called on each
            // move
            ASSERT(nObj == MoveableClass2::ctorCount());
            ASSERT(0    == MoveableClass2::dtorCount());

            // Verify contents
            for (int i = 0; i < nObj; ++i) {
                ASSERT(i == p2[i].value());
            }

            // Destroy and deallocate
            for (int i = 0; i < nObj; ++i) {
                p2[i].~MoveableClass2();
            }
            ::operator delete(p1);
            ::operator delete(p2);
        }

        return 0;
    }
//..
//
///Example 2: Associating a trait with a class template
/// - - - - - - - - - - - - - - - - - - - - - - - - - -
// In this example, we associate a trait not with a class, but with a class
// *template*.  We create three class templates, each of which uses a different
// mechanisms for being associated with the 'IsBitwiseMoveable' trait, plus a
// "control" template that is not bit-wise moveable.  First, we define the
// non-bit-wise-moveable template, 'NonMoveableTemplate':
//..
    template <class TYPE>
    struct NonMoveableTemplate
    {
    };
//..
// Second, we define a 'MoveableTemplate1', which uses partial template
// specialization to associate the 'IsBitwiseMoveable' trait with each
// instantiation:
//..
    template <class TYPE>
    struct MoveableTemplate1
    {
    };

    namespace bslmf {
        template <class TYPE>
        struct IsBitwiseMoveable<MoveableTemplate1<TYPE> > : bsl::true_type {
        };
    }  // close package namespace
//..
// Third, we define 'MoveableTemplate2', which uses the
// 'BSLMF_NESTED_TRAIT_DECLARATION' macro to associate the 'IsBitwiseMoveable'
// trait with each instantiation:
//..
    template <class TYPE>
    struct MoveableTemplate2
    {
        BSLMF_NESTED_TRAIT_DECLARATION(MoveableTemplate2,
                                       bslmf::IsBitwiseMoveable);
    };
//..
// Fourth, we define 'MoveableTemplate3', which is bit-wise moveable iff its
// 'TYPE' template parameter is bit-wise moveable.  There is no way to get this
// effect using 'BSLMF_NESTED_TRAIT_DECLARATION', so we use partial
// specialization combined with inheritance to "inherit" the trait from 'TYPE':
//..
    template <class TYPE>
    struct MoveableTemplate3
    {
    };

    namespace bslmf {
        template <class TYPE>
        struct IsBitwiseMoveable<MoveableTemplate3<TYPE> > :
            IsBitwiseMoveable<TYPE>::type { };
    }  // close package namespace
//..
// Now, we check that the traits are correctly associated by instantiating each
// class with both bit-wise moveable and non-moveable types and verifying the
// value of 'IsBitwiseMoveable<T>::value':
//..
    int usageExample2()
    {
        using namespace bslmf;

        ASSERT(! IsBitwiseMoveable<
               NonMoveableTemplate<NonMoveableClass> >::value);
        ASSERT(! IsBitwiseMoveable<
               NonMoveableTemplate<MoveableClass1> >::value);

        ASSERT(  IsBitwiseMoveable<
               MoveableTemplate1<NonMoveableClass> >::value);
        ASSERT(  IsBitwiseMoveable<
               MoveableTemplate1<MoveableClass1> >::value);

        ASSERT(  IsBitwiseMoveable<
               MoveableTemplate2<NonMoveableClass> >::value);
        ASSERT(  IsBitwiseMoveable<
               MoveableTemplate2<MoveableClass1> >::value);

        ASSERT(! IsBitwiseMoveable<
               MoveableTemplate3<NonMoveableClass> >::value);
        ASSERT(  IsBitwiseMoveable<
               MoveableTemplate3<MoveableClass1> >::value);

        return 0;
    }
//
///Example 3: Support for legacy traits
/// - - - - - - - - - - - - - - - - - -
// 'IsBitwiseMoveable' is not a new trait.  Within the legacy type traits
// system, 'bslalg::TypeTraitBitwiseMoveable' conveys the same information.
// The example shows how the legacy trait can be built on top of the new trait,
// how the legacy system for associating traits with types can be supported,
// and how the new system can be used to query for a trait that was associated
// using the legacy system.
//
// First, we define the 'bslalg::TypeTraitBitwiseMoveable' trait, using a
// special nested type to tie the legacy trait to 'IsBitwiseMoveable':
//..
    namespace bslalg {

    struct TypeTraitBitwiseMoveable {
        // Objects of a type with this trait can be "moved" from one memory
        // location to another using 'memmove' or 'memcpy'.  Although the
        // result of such a bitwise copy is two copies of the same object, this
        // trait guarantees only that one of the copies can be destroyed.  The
        // other copy must be considered invalid and its destructor must not be
        // called.  Most types, even those that contain pointers, are bitwise
        // moveable.  Undefined behavior may result if this trait is assigned
        // to a type that contains pointers to its own internals, uses virtual
        // inheritance, or places pointers to itself within other data
        // structures.

        template <class TYPE>
        struct NestedTraitDeclaration :
            bslmf::NestedTraitDeclaration<TYPE, bslmf::IsBitwiseMoveable>
        {
            // Ties this trait to the 'bslmf::IsBitwiseMoveable' trait.
        };
    };

    }  // close namespace bslalg
//..
// Second, we declare a class and a class template that use the legacy
// 'BSLALG_DECLARE_NESTED_TRAITS' macro to associate the above trait with
// themselves.  (Note: the legacy macro is in 'bslalg', not 'bslmf')
//..
    struct MoveableClass3
    {
        BSLALG_DECLARE_NESTED_TRAITS(MoveableClass3,
                                     bslalg::TypeTraitBitwiseMoveable);
    };

    template <class TYPE>
    struct MoveableTemplate4
    {
        BSLALG_DECLARE_NESTED_TRAITS(MoveableTemplate4,
                                     bslalg::TypeTraitBitwiseMoveable);
    };
//..
// Third, we use 'IsBitwiseMoveable' to check for the trait.  The
// 'BSLALG_DECLARE_NESTED_TRAITS' macro has been rewritten to allow for this
// detection of the 'bslalg::TypeTraitBitwiseMoveable' trait:
//..
    int usageExample3()
    {
        using namespace bslmf;

        ASSERT(  IsBitwiseMoveable<MoveableClass3>::value);
        ASSERT(  IsBitwiseMoveable<MoveableTemplate4<int> >::value);

        return 0;
    }

    }  // close enterprise namespace

// BDE_VERIFY pragma : pop

//=============================================================================
//                              MAIN PROGRAM
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int test = argc > 1 ? atoi(argv[1]) : 0;
    verbose = argc > 2;
    veryVerbose = argc > 3;
    veryVeryVerbose = argc > 4;

    printf("TEST " __FILE__ " CASE %d\n", test);

    switch (test) { case 0:  // Zero is always the leading case.
      case 3: {
        // --------------------------------------------------------------------
        // USAGE EXAMPLE
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Incorporate usage example from header into test driver, remove
        //:   leading comment characters, and replace 'assert' with 'ASSERT'.
        //:   (C-1)
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

        if (verbose) printf("\nUSAGE EXAMPLE"
                            "\n=============\n");

        usageExample1();
        usageExample2();
        usageExample3();

      } break;
      case 2: {
        // --------------------------------------------------------------------
        // EXTENDING 'bslmf::IsBitwiseMoveable'
        //   Ensure the 'bslmf::IsBitwiseMoveable' meta-function returns the
        //   correct value for types explicitly specialized to support this
        //   trait.
        //
        // Concerns:
        //: 1 The meta-function returns 'false' for normal user-defined types.
        //:
        //: 2 The meta-function returns 'true' for a user-defined type, if a
        //:   specialization for 'bslmf::IsBitwiseMoveable' on that type is
        //:   defined to inherit from 'bsl::true_type'.
        //:
        //: 3 The meta-function returns 'true' for a user-defined type that
        //:   specifies it has the trait using the
        //:   'BSLMF_NESTED_TRAIT_DECLARATION' macro.
        //:
        //: 4 The meta-function returns 'true' for a user-defined type, if a
        //:   specialization for 'bsl::is_trivially_copyable' on that type is
        //:   defined to inherit from 'bsl::true_type'.
        //:
        //: 5 For cv-qualified types, the meta-function returns 'true' if the
        //:   corresponding cv-unqualified type is trivially copyable, and
        //:   'false' otherwise.
        //:
        //: 6 For array types, the meta-function returns 'true' if the array
        //:   element is trivially copyable, and 'false' otherwise.
        //
        // Plan:
        //:  1 Create a set of macros that will generate an 'ASSERT' test for
        //:    all variants of a type:  (C5,6)
        //:    o  reference and pointer types
        //:    o  all cv-qualified combinations
        //:    o  arrays, of fixed and runtime bounds, and multiple dimensions
        //:
        //:  2 For each category of type in concerns 1-4, use the appropriate
        //:    test macro for confirm the correct result for a representative
        //:    sample of types.
        //
        // Testing:
        //   Extending bslmf::IsBitwiseMoveable
        // --------------------------------------------------------------------

        if (verbose)
            printf("\nEXTENDING 'bslmf::IsBitwiseMoveable'"
                   "\n======================================\n");

        // C-1
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(UserDefinedNonTcTestType,
                                               false);

        // C-2
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(UserDefinedBwmTestType, true);

        // C-3
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(UserDefinedBwmTestType2, true);

        // C-4
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(UserDefinedTcTestType, true);
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(UserDefinedTcTestType2, true);
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // 'bslmf::IsBitwiseMoveable::value'
        //   Ensure the 'bslmf::IsBitwiseMoveable' meta-function
        //   returns the correct value for intrinsically supported types.
        //
        // Concerns:
        //:  1 The meta-function returns 'true' for fundamental types.
        //:
        //:  2 The meta-function returns 'true' for enumerated types.
        //:
        //:  3 The meta-function returns 'true' for pointer to member types.
        //:
        //:  4 The meta-function returns 'false' for cv-qualified 'void' types.
        //:
        //:  5 The meta-function returns 'false' for function types.
        //
        //:  6 The meta-function returns 'true' for pointer types.
        //:
        //:  7 The meta-function returns 'false' for reference types.
        //:
        //:  8 The meta-function returns the same result for array types as it
        //:    would for the array's element type.
        //:
        //:  9 The meta-function returns the same result for cv-qualified
        //:    types that it would return 'true' for the corresponding
        //:    cv-unqualified type.
        //
        // Plan:
        //:  1 Create a set of macros that will generate an 'ASSERT' test for
        //:    all variants of a type:  (C6-9)
        //:    o  reference and pointer types
        //:    o  all cv-qualified combinations
        //:    o  arrays, of fixed and runtime bounds, and multiple dimensions
        //:
        //:  2 For each category of type in concerns 1-5, use the appropriate
        //:    test macro for confirm the correct result for a representative
        //:    sample of types.
        //
        // Testing:
        //   bslmf::IsBitwiseMoveable::value
        // --------------------------------------------------------------------

        if (verbose)
            printf("\n'bslmf::IsBitwiseMoveable::value'"
                   "\n===================================\n");

        // C-1
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(int, true);
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(char, true);
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(long double, true);

        // C-2
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(EnumTestType, true);

        // C-3
        ASSERT_IS_BITWISE_MOVEABLE_OBJECT_TYPE(MethodPtrTestType, true);

        // C-4 : 'void' is not an object type, but can be cv-qualified.
        ASSERT_IS_BITWISE_MOVEABLE_CV_TYPE(void, false);

        // C-5 : Function types are not object types, nor cv-qualifiable.
        ASSERT_IS_BITWISE_MOVEABLE_TYPE(void(), false);
        ASSERT_IS_BITWISE_MOVEABLE_TYPE(int(float, double), false);
      } break;
      default: {
        fprintf(stderr, "WARNING: CASE `%d' NOT FOUND.\n", test);
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        fprintf(stderr, "Error, non-zero test status = %d.\n", testStatus);
    }

    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2013 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
