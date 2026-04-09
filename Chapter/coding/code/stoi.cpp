void test() {

    std::string s = "\n\t -0xff";
    
    size_t pos;
    int x = std::stoi(s, &pos, 0);
    assert(x == -255 && pos == 8);

    std::stoi(s); // ERROR, base == 10

}