void test() {

    assert(std::to_string(-0114514.1919810) == "-114514.191981");
    assert(std::to_string(-0xff) == "-255");

    char buf[32];

    std::snprintf(buf, sizeof(buf), "%X %#x %#o", 255, 255, 255);
    assert((strncmp(buf, "FF 0xff 0377", sizeof(buf)) == 0));

    std::snprintf(buf, sizeof(buf), "%06zd %07.3f", 114LL, 5.14);
    assert((strncmp(buf, "000114 005.140", sizeof(buf)) == 0));

}