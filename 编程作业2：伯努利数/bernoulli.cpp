#include <iostream>
#include <vector>
using namespace std;

int comb(int n, int k) {	// �����
    int i = 1, c = 1;
    while (i <= k) {
        c = c * n / i;
        i++, n--;
    }
    return c;
}

int gcd(int m, int n) {		// ���Լ����������������ͬ
    if (m > 0 && n < 0 || m < 0 && n>0)
        n = -n;			// ʹ��m��nͬ��
    while (true) {
        if (m == 0) return n;
        n %= m;
        if (n == 0) return m;
        m %= n;
    }
}

struct Rational {		// ������
    int n;			// ����
    int d;			// ��ĸ
    void print();		// ��ӡ
    // ���캯��
    Rational(int n_ = 0, int d_ = 1);
};

// �����Ա����
void Rational::print() {
    cout << n;
    if (d != 1)
        cout << '/' << d;
    cout << '\n';
}

// ���幹�캯��
Rational::Rational(int n_, int d_) {
    int g = gcd(d_, n_);
    n = n_ / g;
    d = d_ / g;
}

Rational add(Rational a, Rational b)	// �ӷ�
{
    return Rational{ a.n * b.d + a.d * b.n, a.d * b.d };
}

Rational mul(Rational a, Rational b)	// �˷�
{
    return Rational{ a.n * b.n, a.d * b.d };
}

int main() {
    int n;
    cout << "please enter the number: ";
    cin >> n;
    // ���ɲ�Ŭ����
    vector<Rational> bn;
    bn.push_back(Rational{ 1 });
    for (int m = 1; m <= n; ++m) {
        Rational s;
        for (int k = 0; k < m; ++k)
            s = add(s, mul(comb(m + 1, k), bn[k]));
        bn.push_back(mul(s, Rational{ -1, m + 1 }));
    }
    // ��ӡ��Ŭ����
    for (auto r : bn)
        r.print();
    return 0;
}
