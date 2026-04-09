void test() {

    // index:  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
    int a[] = {1, 2, 4, 4, 4, 4, 4, 9, 9, 9, 11, 13, 14};
    int *L = std::begin(a), *R = std::end(a);

    int *l, *r;
    l = std::lower_bound(L, R, 9);
    r = std::upper_bound(L, R, 9); // [7, 10)
    l = std::lower_bound(L, R, 8);
    r = std::upper_bound(L, R, 8); // [7, 7)

    int x = 4, *p1, *p2, *p3, *p4;
    p1 = std::lower_bound(L, R, x);     // first of >= x
    p2 = std::upper_bound(L, R, x);     // first of >  x
    p3 = std::lower_bound(L, R, x) - 1; //  last of <  x
    p4 = std::upper_bound(L, R, x) - 1; //  last of <= x

    assert(p1 - a == 2 && p2 - a == 7);
    assert(p3 - a == 1 && p4 - a == 6);

}