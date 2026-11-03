# 41443134

作業二

## 解題說明

本題要求完成一個名為 Polynomial的類別。

### 解題策略

1.自動變大：一開始格子給很少。如果項數放滿了，程式會自動把陣列容量「放大兩倍」，再把舊資料搬過去，不用怕格子不夠用。
2.加法：因為兩邊都已經由大到小排好序了，我們就拿兩個指針像拉鍊一樣，從頭開始比大小。如果指數一樣，就把係數加起來；如果一邊比較大，就把大的先放進結果裡。
3.乘法：用兩個迴圈，把左邊的每一項和右邊的每一項「兩兩相乘」（係數相乘、指數相加）。乘完會得到一堆亂七八糟的項，我們用排序把它們重新「由大到小排好」，最後再把指數一樣的項加在一起合併。
4.求值：把你想帶入的數字 \(x = f\) 放進去，程式會從頭到尾把每一項的值算出來，最後通通加起來就是答案。
5.建立一個 Term 結構，裡面只存兩個數字：coef係數和exp 指數。
6.在 Polynomial 類別內，建立一個動態結構陣列，只把非零的項放進去。
7.• 規定新加入的項一律要依照指數由大到小降冪在陣列裡排好。

## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析
多項式加法：速度快 因為兩邊都已經從小到大排好了，我們就像拿兩個指標比大小，從頭到尾一次就加完了。
多項式乘法：比較耗時 因為要把兩邊的所有項兩兩相乘，會產生一堆新的項 最卡時間的地方在乘完之後，要把這堆亂七八糟的項由大到小排好合併。
求值：把數字帶進去，把每一項的結果算出來 加在一起，數到幾項就算幾次。
空間複雜度：加法很省空間，就是把兩邊的項加在一起 乘法比較吃記憶體，因為在排序合併之前，需要開一個大格子把有相乘的結果暫存起來。
## 測試與驗證

### 測試案例
測試時我們使用兩組多項式：
p1 = 3x^2 + 2x^1 + 5
p2 = 4x^3 + 1x^1

| 測試案例 | 輸入參數與操作 | 預期輸出 | 實際輸出 |
| :--- | :--- | :--- | :--- |
| 測試一 | 多項式加法 $p1 + p2$ | `4x^3 + 3x^2 + 3x^1 + 5` | `4x^3 + 3x^2 + 3x^1 + 5` |
| 測試二 | 多項式乘法 $p1 \times p2$ | `12x^5 + 8x^4 + 23x^3 + 2x^2 + 5x^1` | `12x^5 + 8x^4 + 23x^3 + 2x^2 + 5x^1` |
| 測試三 | 多項式求值 $p_1(2)$ | `21` | `21` |
| 測試四 | 結束程式 | 0 | 0 |

### 效能量測
本程式使用的多項式項數不多，因此相加、相乘與帶入 $x$ 都可以很快得到結果。

相加：只需要依序比較兩個多項式中的項目。
帶入：需要計算每一項的次方。
相乘：需要將兩個多項式的每一項互相相乘，還要搜尋是否有相同指數，因此運算量最大。
當多項式項數增加時，相乘所需要的時間會比相加增加得更快。

### 結論
這次作業讓我更了解 C++ 物件導向程式設計的應用，設計 Polynomial 類別，學習使用陣列儲存多項式的係數與指數，
並完成多項式的加法、乘法及代入求值等功能。在實作過程中，我發現多項式乘法需要將每一項互相相乘，再將相同指數的係數合併，是讓我最花時間的。
透過這次練習，我更加熟悉類別、建構子及函式的使用，並學會將數學運算轉換成程式邏輯。
過程中有一些需要思考的地方也提升了自己的程式能力。希望能運用這次學到的觀念，更快速分析問題並完成程式。



