{
    files = {
        "src/main.cpp"
    },
    depfiles_gcc = "build/.objs/stretch/linux/x86_64/release/src/main.cpp.o: src/main.cpp  src/Grammaire.cpp src/Actions.cpp\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-std=c++17",
            "-isystem",
            "/root/.xmake/packages/p/pegtl/3.2.2/eb9f70b8886845ba88088412887ed882/include"
        }
    }
}