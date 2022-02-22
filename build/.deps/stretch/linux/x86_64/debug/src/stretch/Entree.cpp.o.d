{
    files = {
        "src/stretch/Entree.cpp"
    },
    depfiles_gcc = "build/.objs/stretch/linux/x86_64/debug/src/stretch/Entree.cpp.o:  src/stretch/Entree.cpp\
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