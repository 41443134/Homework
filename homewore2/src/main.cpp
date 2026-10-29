#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
private:
    float coef;
    int exp;
};

class Polynomial {
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;

    void newTerm(const float theCoef, const int theExp) {
        if (theCoef == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            copy(termArray, termArray + terms, temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = theCoef;
        termArray[terms++].exp = theExp;
    }

public:
    Polynomial() : capacity(2), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        copy(poly.termArray, poly.termArray + terms, termArray);
    }

    Polynomial& operator=(const Polynomial& poly) {
        if (this != &poly) {
            delete[] termArray;
            capacity = poly.capacity;
            terms = poly.terms;
            termArray = new Term[capacity];
            copy(poly.termArray, poly.termArray + terms, termArray);
        }
        return *this;
    }

    Polynomial Add(Polynomial poly) {
        Polynomial c;
        int aPos = 0, bPos = 0;

        while ((aPos < terms) && (bPos < poly.terms)) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float t = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (t != 0) {
                    c.newTerm(t, termArray[aPos].exp);
                }
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp < poly.termArray[bPos].exp) {
                c.newTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
            else {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
        }

        for (; aPos < terms; aPos++) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
        }
        for (; bPos < poly.terms; bPos++) {
            c.newTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        }
        return c;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial c;
        for (int i = 0; i < terms; i++) {
            Polynomial tempPoly;
            for (int j = 0; j < poly.terms; j++) {
                float rCoef = termArray[i].coef * poly.termArray[j].coef;
                int rExp = termArray[i].exp + poly.termArray[j].exp;
                tempPoly.newTerm(rCoef, rExp);
            }
            c = c.Add(tempPoly);
        }
        return c;
    }

    float Eval(float f) {
        float result = 0.0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * static_cast<float>(pow(f, termArray[i].exp));
        }
        return result;
    }
};

istream& operator>>(istream& is, Polynomial& poly) {
    int numTerms;
    if (!(is >> numTerms)) return is;
    poly.terms = 0;
    for (int i = 0; i < numTerms; i++) {
        float c;
        int e;
        is >> c >> e;
        poly.newTerm(c, e);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        if (i > 0 && c > 0) os << " + ";
        else if (c < 0) {
            if (i == 0) os << "-";
            else os << " - ";
            c = -c;
        }

        if (c != 1 || e == 0) {
            os << c;
        }

        if (e > 0) {
            os << "x";
            if (e > 1) {
                os << "^" << e;
            }
        }
    }
    return os;
}
int main() {
    Polynomial p1, p2;

    if (cin >> p1 >> p2) {
        cout << "p1: " << p1 << endl;
        cout << "p2: " << p2 << endl;
        cout << "Add: " << p1.Add(p2) << endl;
        cout << "Mult: " << p1.Mult(p2) << endl;
        cout << "Eval p1 at 2: " << p1.Eval(2) << endl;
    }
    return 0;
}


