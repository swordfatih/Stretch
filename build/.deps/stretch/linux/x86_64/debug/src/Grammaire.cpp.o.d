{
    depfiles_gcc = "build/.objs/stretch/linux/x86_64/debug/src/Grammaire.cpp.o:  src/Grammaire.cpp\
",
    files = {
        "src/Grammaire.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++17",
            "-isystem",
            "/root/.xmake/packages/p/pegtl/3.2.2/eb9f70b8886845ba88088412887ed882/include"
        }
    }
}