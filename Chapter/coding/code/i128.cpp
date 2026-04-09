using i128 = __int128;

std::istream &operator>>(std::istream &is, i128 &x) {
    std::string s; is >> s;
    auto it = s.begin();
    bool neg = false; x = 0;
    if (*it == '-') { neg = true; it++; }
    while (it != s.end()) x = x * 10 + (*it++ - '0');
    if (neg) x = -x;
    return is;
}

std::ostream &operator<<(std::ostream &os, i128 x) {
    if (x == 0) return os << 0;
    if (x < 0) { x = -x; os << '-'; }
    std::string s;
    while (x) { s += '0' + x % 10; x /= 10; }
    std::reverse(s.begin(), s.end());
    return os << s;
}