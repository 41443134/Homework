# 41443134

作業一

## problem1

## 解題說明

本題目標為計算 Ackermann 函數 $A(m, n)$，並分別以遞迴（Recursive）與非遞迴（Non-recursive / Iterative）兩種方式進行實作。

Ackermann 函數的算術規則如下：

* 當 $m = 0$ 時：$A(m, n) = n + 1$

* 當 $m > 0$ 且 $n = 0$ 時：$A(m, n) = A(m - 1, 1)$

* 當 $m > 0$ 且 $n > 0$ 時：$A(m, n) = A(m - 1, A(m, n - 1))$

## 程式實作

以下為 C++ 實作程式碼，將邏輯封裝於類別（Class）中，非遞迴版本採用自訂結構與 `std::stack` 管理狀態：

```cpp
#include <iostream>
#include <stack>

class AckermannSolver {
public:
    static long long computeRecursive(long long m, long long n) {
        if (m == 0) return n + 1;
        if (n == 0) return computeRecursive(m - 1, 1);
        return computeRecursive(m - 1, computeRecursive(m, n - 1));
    }

    static long long computeIterative(long long m, long long n) {
        std::stack<long long> stk;
        stk.push(m);

        while (!stk.empty()) {
            long long top_m = stk.top();
            stk.pop();

            if (top_m == 0) {
                n = n + 1;
            } else if (n == 0) {
                n = 1;
                stk.push(top_m - 1);
            } else {
                stk.push(top_m - 1);
                stk.push(top_m);
                n = n - 1;
            }
        }
        return n;
    }
};

int main() {
    long long m = 3, n = 3;
    std::cout << "[Recursive Result] A(" << m << ", " << n << ") = " 
              << AckermannSolver::computeRecursive(m, n) << std::endl;
    std::cout << "[Iterative Result] A(" << m << ", " << n << ") = " 
              << AckermannSolver::computeIterative(m, n) << std::endl;
    return 0;
}
```

## 效能分析

* **時間複雜度**：Ackermann 函數運算結果呈現超指數級爆發，演算法執行步驟數直接受限於函數結果，總時間複雜度為 $\mathcal{O}(A(m, n))$。

* **空間複雜度**：

  * 遞迴模式：消耗系統隱式 Call Stack，最大堆疊層數界定為 $\mathcal{O}(A(m, n))$。

  * 非遞迴模式：維護顯式 `std::stack` 記憶體空間，其空間用量最高亦達到 $\mathcal{O}(A(m, n))$。

## 測試與驗證

| 測試編號 | 測試輸入 $(m, n)$ | 計算結果 |
| :---: | :---: | :---: |
| 1 | $A(0, 4)$ | `5` |
| 2 | $A(1, 3)$ | `5` |
| 3 | $A(2, 3)$ | `9` |
| 4 | $A(3, 2)$ | `29` |
| 5 | $A(3, 3)$ | `61` |

### 編譯結果

```bash
g++ -O3 src/problem1.cpp -o problem1.exe
./problem1.exe
[Recursive Result] A(3, 3) = 61
[Iterative Result] A(3, 3) = 61
```

## 申論及開發報告

遞迴版本是直接依照 Ackermann 函式的數學定義進行實作，透過函式自己呼叫自己的方式完成計算，遞迴程式的結構較為簡潔，也容易理解，但當遞迴層數過深時，會消耗較多的記憶體，且可能降低執行效率，甚至造成 Stack Overflow，非遞迴版本則不使用函式自行呼叫的方式，而是利用 Stack 來模擬遞迴的執行過程。由於 Stack 具有後進先出（LIFO）的特性，可以將尚未完成的計算工作暫時保存，並依照正確的順序取出處理，因此能夠模擬原本遞迴函式的執行方式。

## problem2

## 解題說明

本題目標為計算一個集合的 Powerset（冪集），亦即找出該集合所有可能的子集合，並以遞迴方式完成。

例如：
$S = \{1, 2, 3\}$

每一個元素都有「選擇」和「不選擇」兩種情況：

1. 不選擇目前的元素。

2. 選擇目前的元素。

透過遞迴將每個元素分成這兩種情況，就可以找出所有可能的子集合。當所有元素都處理完時，就將目前的結果輸出。

因為每個元素都有 $2$ 種選擇，所以如果集合有 $n$ 個元素，總共有 $2^n$ 個子集合。

## 程式實作

以下採用 C++ 實作，搭配 `std::vector` 與回溯法（Backtracking）印出子集：

```cpp
#include <iostream>
#include <vector>

void printPowerset(const std::vector<int>& elements, size_t idx, std::vector<int>& currentSubset) {
    if (idx == elements.size()) {
        std::cout << "{";
        for (size_t i = 0; i < currentSubset.size(); ++i) {
            std::cout << currentSubset[i] << (i + 1 < currentSubset.size() ? ", " : "");
        }
        std::cout << "}\n";
        return;
    }

    printPowerset(elements, idx + 1, currentSubset);

    currentSubset.push_back(elements[idx]);
    printPowerset(elements, idx + 1, currentSubset);
    currentSubset.pop_back();
}

int main() {
    std::vector<int> S = {1, 2, 3};
    std::vector<int> current;
    printPowerset(S, 0, current);
    return 0;
}
```

## 效能分析

* **時間複雜度**：大小為 $n$ 的集合共可生成 $2^n$ 個子集合，因此總執行時間複雜度為 $\mathcal{O}(2^n)$。

* **空間複雜度**：遞迴呼叫堆疊深度以及暫存陣列空間最大皆佔用 $n$ 個空間，故空間複雜度為 $\mathcal{O}(n)$。

## 測試與驗證

測試輸入：$S = \{1, 2, 3\}$

測試輸出：

```text
{}
{3}
{2}
{2, 3}
{1}
{1, 3}
{1, 2}
{1, 2, 3}
```

### 編譯結果

```bash
g++ -O3 src/problem2.cpp -o problem2.exe
./problem2.exe
{}
{3}
{2}
{2, 3}
{1}
{1, 3}
{1, 2}
{1, 2, 3}
```

## 申論及開發報告

這題是利用遞迴的方式來產生集合的所有子集合。每處理一個元素時，都會分成「選擇」和「不選擇」兩種情況，再繼續處理下一個元素。當所有元素都處理完成後，就會將目前產生的子集合輸出，因為每個元素都有兩種選擇，所以一個有 $n$ 個元素的集合，最後會產生 $2^n$ 個子集合，在開發過程中，我使用 vector 來存放原本的集合以及目前產生的子集合，並利用 index 判斷目前處理到哪一個元素，透過遞迴不斷處理「選擇」與「不選擇」的情況，最後成功產生所有可能的子集合，透過這次作業，我更加了解遞迴的使用方式，也了解到冪集的數量會隨著集合元素增加而快速增加。
