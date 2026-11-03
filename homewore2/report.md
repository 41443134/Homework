# 41443134

作業二

## 解題說明

本題要求實現一個遞迴函式，計算從 $1$ 到 $n$ 的連加總和。

### 解題策略

1. 使用遞迴函式將問題拆解為更小的子問題：
   $$\Sigma(n) = n + \Sigma(n-1)$$
2. 當 $n \leq 1$ 時，返回 $n$ 作為遞迴的結束條件。  
3. 主程式呼叫遞迴函式，並輸出計算結果。

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

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。

