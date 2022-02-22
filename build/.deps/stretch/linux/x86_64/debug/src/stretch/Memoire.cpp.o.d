{
    files = {
        "src/stretch/Memoire.cpp"
    },
    depfiles_gcc = "build/.objs/stretch/linux/x86_64/debug/src/stretch/Memoire.cpp.o:  src/stretch/Memoire.cpp include/stretch/Memoire.hpp  include/stretch/Variable.hpp\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++17",
            "-Iinclude",
            "-isystem",
            "/root/.xmake/packages/p/pegtl/3.2.2/eb9f70b8886845ba88088412887ed882/include"
        }
    }
}