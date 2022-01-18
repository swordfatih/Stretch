#include <tao/pegtl.hpp>

using namespace tao::pegtl;

struct integer : seq<
        opt< one< '+', '-' > >,  // ('+'/'-')?
        plus< digit >            // digit+
     > {};


