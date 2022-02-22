{
    files = {
        "src/tests/main.cpp"
    },
    depfiles_gcc = "build/.objs/test/linux/x86_64/debug/src/tests/main.cpp.o:  src/tests/main.cpp\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-std=c++17",
            "-Iinclude",
            "-isystem",
            "/root/.xmake/packages/p/pegtl/3.2.2/eb9f70b8886845ba88088412887ed882/include",
            "-isystem",
            "/root/.xmake/packages/c/catch2/2.13.7/fc9818b157154703aa80024c54a2b899/include"
        }
    }
}