i64 floor_div(i64 x, i64 y) {
    if (y < 0) x = -x, y = -y;
    if (x < 0) return (x - y + 1) / y;
    return x / y;
}

i64 ceil_div(i64 x, i64 y) {
    if (y < 0) x = -x, y = -y;
    if (x < 0) return x / y;
    return (x + y - 1) / y;
}