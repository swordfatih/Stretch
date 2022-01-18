#include <tao/pegtl.hpp>

using namespace tao::pegtl;

struct variable : star<alpha> {};
struct repeter : istring<'r', 'e', 'p', 'e', 't', 'e', 'r'> {};

struct plus_minus : opt< one< '+', '-' > > {};  
struct addition : seq< digit, star<space>, plus_minus, star<space>, digit > {};
struct tant_que : string< 'T', 'a', 'n', 't', ' ', 'q', 'u','e' > {};
struct plus_grand : string< 'P', 'l', 'u', 's', ' ', 'g', 'r', 'a', 'n', 'd', ' ', 'q', 'u', 'e'> {};
struct plus_petit : string< 'P', 'l', 'u', 's', ' ', 'p', 'e', 't', 'i', 't', ' ', 'q', 'u', 'e'> {};
struct egal : string< 'e','g','a','l'> {};

struct dot : one< '.' > {};

// struct number
//    : tao::pegtl::plus< tao::pegtl::digit > {};

// struct addition;  // Forward declaration to break the cyclic dependency.

// struct bracket
//    : tao::pegtl::if_must< tao::pegtl::one< '(' >, addition, tao::pegtl::one< ')' > > {};

// struct atomic
//    : tao::pegtl::sor< number, bracket > {};

// struct addition
//    : tao::pegtl::list< atomic, tao::pegtl::one< '+' > > {};

// struct inf : seq< istring< 'i', 'n', 'f' >,
//                     opt< istring< 'i', 'n', 'i', 't', 'y' > > > {};

// struct nan : seq< istring< 'n', 'a', 'n' >,
//                     opt< one< '(' >,
//                         plus< alnum >,
//                         one< ')' > > > {};

// template< typename D >
// struct number : if_then_else< dot,
//                                 plus< D >,
//                                 seq< plus< D >, opt< dot, star< D > > > > {};

// struct e : one< 'e', 'E' > {};
// struct p : one< 'p', 'P' > {};
// struct exponent : seq< plus_minus, plus< digit > > {};

// struct decimal : seq< number< digit >, opt< e, exponent > > {};
// struct hexadecimal : seq< one< '0' >, one< 'x', 'X' >, number< xdigit >, opt< p, exponent > > {};

//struct grammar : star<seq< plus<dot>, eolf >> {};

struct grammar : star<
        seq<
            star<
                seq<
                    sor<repeter, egal>, 
                    space
                >
            >, 
            eolf
        >
    > {};

// star<plus<dot>, eolf>