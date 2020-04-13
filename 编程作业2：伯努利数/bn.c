int gcd(int m, int n)		/* 最大公约数 */
{
	int s = 1;
	if (m == 0) return n;
	if (n == 0) return m;
	if (m < 0) m = -m, s = -1;
	if (n < 0) n = -n;
	while (m != n) {
		if (m > n) m -= n;
		if (n > m) n -= m;
	}
	return s * m;
}

typedef struct rational_t {
	int n;		/* 分子 */
	int d;		/* 分母 */
} rational;		/* 有理数 */


rational reduction(int n, int d)	/* 约分 */
{
	rational r;
	int g = gcd(d, n);
	r.n = n / g;
	r.d = d / g;
	return r;
}

rational add(rational a, rational b) /* 加法 */
{
	return reduction(a.n * b.d + a.d * b.n, a.d * b.d);
}

#define N 30
int pt[N+1][N+2] = {0};
#define comb(m, n) pt[m][n+1]

void gen_comb()		/* 生成组合数 */
{
	int i, j;
	comb(0, 0) = 1;
	for (i = 1; i <= N; i++)
		for (j = 0; j <= i; j++)
			comb(i, j) = comb(i-1, j-1) + comb(i-1, j);
}
