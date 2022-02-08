struct Variable
{
    std::variant<long, double, std::string, std::bool> types;
};


//variable en elle-même + le reste