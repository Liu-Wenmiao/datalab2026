/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

 /*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    /* 德摩根律：a & b == ~(~a | ~b)，把 & 翻译成白名单里的 ~ 和 |。 */
    return ~(~x | ~y);
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    /* 先造出 x|y：~(~x & ~y)。再 & ~(x & y) 把"两者同为 1"的位挖掉，剩下即异或。 */
    return ~(~x & ~y) & ~(x & y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    /* 0 既非正也非负，所以分三类（本题没有 ||，只能用 && 与 if）：
     *   都是 0       -> 视为同号，返回 1
     *   恰有一个是 0 -> 不同号，返回 0
     *   都不为 0     -> 取符号位比较。用算术右移 31 位把符号位复制满整个字
     *                   （即课件的"符号扩展"），得 0 或全 1，异或为 0 即同号 */
    if ((!x) && (!y)) {
        return 1;
    }
    if (!(x && y)) {
        return 0;
    }
    return !((x >> 31) ^ (y >> 31));
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    /* 求最高位 1 的下标，用的是课件例题"表示 X 最少要多少位"里的二分定位法：
     * 先问"高 16 位还有没有 1"，有就把范围折半到那一半、并记下权重 16；
     * 再问 8、4、2、1，每步把范围折半，五步锁定最高位。
     *
     * 与课件写法有两处必要的替换：
     *   1) 课件用 !!(x >> 16) 把"非零"压成 1，但 ! 不在本题白名单，改用 > 0；
     *   2) 课件用 + 累加权重，但 + 也不在白名单，改用 |。
     *      这一步成立是因为五段权重 16/8/4/2/1 两两不重叠——正是课件
     *      "提取和拼装"里说的"各段移到自己的位置再用 | 拼起来（前提是各段不重叠）"。
     * 首段写 r = b << 4 而不是 r | (b << 4)：r 初值为 0，那个 | 是多余的，
     * 省掉它正好把运算符计数压到上限 25。
     *
     * 题目保证 v > 0；v <= 1（含负数）时五步全落空，自然返回 0。 */
    int r = 0;
    int b;

    b = (v >> 16) > 0;
    r = b << 4;
    v >>= b << 4;

    b = (v >> 8) > 0;
    r = r | (b << 3);
    v >>= b << 3;

    b = (v >> 4) > 0;
    r = r | (b << 2);
    v >>= b << 2;

    b = (v >> 2) > 0;
    r = r | (b << 1);
    v >>= b << 1;

    r = r | (v > 1);
    return r;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    /* 课件"提取和拼装"的套路：
     *   提取第 p 字节 = (x >> (p<<3)) & 0xFF      —— 先右移到最低，再 & 掩码留宽度
     *   拼装 = 各字段移到自己的位置后用 | 合并，前提是互不重叠；
     *          所以先把两个原字节的位置清 0，交换后就不会互相覆盖。 */
    int nb = n << 3;
    int mb = m << 3;
    int a = (x >> nb) & 0xFF;
    int b = (x >> mb) & 0xFF;
    int cleared = x & ~(0xFF << nb) & ~(0xFF << mb);
    return cleared | (b << nb) | (a << mb);
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    /* 逐位处理：每轮把结果左移一位腾出最低位，再把 v 的最低位填进去。
     * (v & 1) 就是课件"取第 n 位"公式 (x >> n) & 1 在 n = 0 时的形式。 */
    unsigned r = 0;
    int i = 32;
    while (i) {
        r = (r << 1) | (v & 1);
        v = v >> 1;
        i = i - 1;
    }
    return r;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    /* x >> n 对负数是算术右移，高位补的是符号位，所以要再与一个
     * "低 32-n 位全 1、高 n 位全 0"的掩码相与。
     *
     * 难点：本题禁用了 unsigned / if / while / 减号，而 n = 0 时 1 << 32 是
     * 未定义行为。写法分两步绕开：
     *   (1) 把 32-n 写成 32 + ~n —— 因 ~n == -n-1，该式实为 31-n，
     *       移位量必定落在 [0,31]，不会越界；
     *   (2) 再把结果左移一位，补回丢掉的 2 倍。
     * 于是 n = 0 时先得 1<<31（移位量 31 合法，只是落进符号位），左移一位变 0，
     * 加上课件技巧表里的"全 1 掩码" ~0 恰好得到全 1；n >= 1 时 1<<(31-n) 是正数，
     * 左移一位恰为 1<<(32-n)。末尾的 + ~0 则是把那个孤立的 1 摊成低位全 1。 */
    int mask = ((1 << (32 + ~n)) << 1) + ~0;
    return (x >> n) & mask;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    /* 与课件 bitCount 的"折半并行求和"同源，只是方向相反：那边是自底向上把
     * 相邻位两两合并，这边是自顶向下分段确认，步长取 16、8、4、2、1。
     * 每步考察"紧接着已确认前缀的那 s 位"是否全为 1：右移 (32-c-s) 位把它移到
     * 最低端，用 s 位掩码取出（课件"取宽度 w 的字段：(x>>p) & ((1<<w)-1)"），
     * 再与"全 1"比较，成立就把 s 累加进 c。
     *
     * 三个容易踩的点：
     *   1) x 有符号，算术右移会复制符号位，取出该段后必须 & 掩码，
     *      否则 (x >> 16) 对负数得 0xFFFFFFFF 而非 0x0000FFFF，比较恒不成立；
     *   2) 本题没有 ==、*、-：判等用 !(a ^ b)（相等时异或为 0），乘步长用 <<，
     *      移位量 32-c-s 用 25+~c / 29+~c / 31+~c / 32+~c 表达（已逐项数值校验）；
     *   3) 五步合计只覆盖 31 位，最低位由最后一行按课件"五个必背公式"第 1 条
     *      (x >> n) & 1 单独取出。
     *
     * 最后两行形式相同（都是取当前考察位），但两行之间 c 会变，
     * 所以它们考察的不是同一位；第五行仍沿用前四步统一的"掩码后与全 1 比较"写法，
     * 因只有 1 位，它等价于直接取该位。
     * 各步移位量恒在 [0,31] 内，不会出现 32 位移位这一未定义行为。 */
    int c = 0;
    c = c + ((!(((x >> 16) & 0xFFFF) ^ 0xFFFF)) << 4);
    c = c + ((!(((x >> (25 + ~c)) & 0xFF) ^ 0xFF)) << 3);
    c = c + ((!(((x >> (29 + ~c)) & 0xF) ^ 0xF)) << 2);
    c = c + ((!(((x >> (31 + ~c)) & 0x3) ^ 0x3)) << 1);
    c = c + ((!(((x >> (32 + ~c)) & 0x1) ^ 0x1)));
    c = c + ((x >> (32 + ~c)) & 0x1);
    return c;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    /* 单精度 = 1 位符号 + 8 位阶码 + 23 位尾数。
     * 这里要绕开课件的"补码著名的坑"：-INT_MIN == INT_MIN，取负后没有对应的
     * 正数。把幅值交给 unsigned 变量承载，-x 得到的位模式正好是 0x80000000，
     * 也就是 INT_MIN 的幅值，所以不需要额外分支。 */
    if (x == 0) {
        return 0;
    }
    unsigned sign;
    unsigned mag;
    if (x < 0) {
        sign = 0x80000000;
        mag = -x;
    } else {
        sign = 0;
        mag = x;
    }
    /* 最高位 1 的下标 e 决定阶码：exp = e + 127（课件：阶码用移码存） */
    int e = 0;
    unsigned t = mag;
    while (t > 1) {
        t = t >> 1;
        e = e + 1;
    }
    unsigned exp = (e + 127) << 23;
    /* 丢掉隐含的最高位 1，其后 23 位为尾数，round 是全部被截掉的低位。
     * e == 23 时恰好整除，round 恒为 0，下面也就不用舍入。 */
    unsigned mant;
    unsigned round = 0;
    if (e >= 23) {
        round = mag & ((1 << (e - 23)) - 1);
        mant = (mag >> (e - 23)) & 0x7FFFFF;
    } else {
        mant = (mag << (23 - e)) & 0x7FFFFF;
    }
    /* 课件的"偶数舍入"（就近舍入、逢半取偶）：先与半个 ulp（round 的最高位）
     * 比较，大于就进位；正好等于一半时看末位奇偶，奇数才进位。
     * 本题没有 &&，所以"大于"与"等于"只能用嵌套 if 表达。 */
    unsigned result = sign | exp | mant;
    if (e >= 24) {
        unsigned half = 1 << (e - 24);
        if (round > half) {
            result = result + 1;
        } else {
            if (round == half) {
                if (result & 1) {
                    result = result + 1;
                }
            }
        }
    }
    return result;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    /* 按课件的"Float 的阶码分区"分四类：
     *   0   ：0 或非规格化数，乘 2 即整体左移一位（符号位单独保留），
     *         非规格化数可借此自然进位成规格化数
     *   255 ：无穷或 NaN，按题目要求原样返回
     *   254 ：已是最大阶，乘 2 必定溢出成无穷。只能返回"符号位 + 阶码全 1"，
     *         不能直接加 0x00800000——那会把阶码推到 255 而尾数非零，是非法位模式
     *   其余：规格化数，阶码加 1，即数值上加 0x00800000 */
    unsigned exp = (uf >> 23) & 0xFF;
    if (exp == 0) {
        return (uf << 1) | (uf & 0x80000000);
    }
    if (exp == 0xFF) {
        return uf;
    }
    if (exp == 0xFE) {
        return (uf & 0x80000000) | 0x7F800000;
    }
    return uf + 0x00800000;
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    /* 双精度 = 1 位符号 + 11 位阶码 + 52 位尾数，符号与阶码都在 uf2 的高 12 位。
     * 用"提取和拼装"取出隐含的 1 与其后的 31 位尾数，得到一个 32 位窗口 val：
     * 最高位（bit31）就是那个隐含的 1，其后是尾数的高 31 位。
     * 于是整体再右移 (1054-exp) 位，等价于把 53 位有效数字右移 (1075-exp) 位，
     * 移出去的部分正是小数部分——"向零取整"天然成立，无需额外判断。
     *
     * 边界：exp < 1023 表示 |v| < 1，返回 0；exp > 1054 表示 |v| >= 2^31，返回哨兵。
     * 移位之后还必须再判一次 val 是否越界——漏掉这步，负数的 -val 会符号回绕，
     * 例如 -2147483649.0 会错误地返回一个正数。 */
    int sign = uf2 >> 31;
    unsigned exp = (uf2 >> 20) & 0x7FF;
    unsigned val = 0x80000000 | ((uf2 & 0xFFFFF) << 11) | (uf1 >> 21);
    if (exp < 1023) {
        return 0;
    }
    if (exp > 1054) {
        return 0x80000000;
    }
    val = val >> (1054 - exp);
    if (sign) {
        if (val > 0x80000000) {
            return 0x80000000;
        }
        return -val;
    } else {
        if (val > 0x7FFFFFFF) {
            return 0x80000000;
        }
        return val;
    }
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    /* 按 x 分四段（阶码字段用移码，即真值 + 127）：
     *   x < -149            ：比最小非规格化数 2^-149 还小，返回 0
     *   -149 <= x <= -127   ：非规格化数，阶码字段为 0；
     *                         2^x = 2^-149 × 2^(x+149)，故尾数位为 1 << (x+149)，
     *                         写成 1 << (x + 126 + 23) 是为了避开被禁用的减法
     *   x > 127             ：超过最大阶 2^127，返回 +INF
     *   -126 <= x <= 127    ：规格化数，阶码字段为 x + 127，尾数为 0 */
    if (x < -126) {
        if (x < -126 - 23) {
            return 0;
        }
        return 1 << (x + 126 + 23);
    }
    if (x > 127) {
        return 0x7F800000;
    }
    return (x + 127) << 23;
}
