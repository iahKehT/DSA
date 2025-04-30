#include "SpendingStatistic.h"
#include <iostream>
#include <cstring>

int compareSpendingDesc(const CustomerSpending* a, const CustomerSpending* b) {
    if (a->totalAmount > b->totalAmount) return -1;
    if (a->totalAmount < b->totalAmount) return 1;
    return 0;
}

// Sửa định nghĩa hàm để đúng với tham số truyền vào
void quickSort_CustomerSpending(CustomerSpending arr[], int low, int high, int (*compareFunc)(const CustomerSpending*, const CustomerSpending*)) {
    if (low < high) {
        CustomerSpending pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (compareFunc(&arr[j], &pivot) < 0) { // compareFunc dùng để so sánh theo tổng chi tiêu giảm dần
                ++i;
                CustomerSpending temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        CustomerSpending temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pivotIndex = i + 1;

        quickSort_CustomerSpending(arr, low, pivotIndex - 1, compareFunc);
        quickSort_CustomerSpending(arr, pivotIndex + 1, high, compareFunc);
    }
}

// Duyệt hết toàn bộ CustomerSpendingList
// và copy từng khách (CustomerSpending) vào một mảng liên tiếp trong bộ nhớ (CustomerSpending[])
CustomerSpending* copyCustomerSpendingsToArray(const CustomerSpendingList* list, int& outCount) {
    outCount = 0;

    // Đếm tổng số khách hàng
    for (int i = 0; i < list->capacity; ++i) {
        CustomerSpendingNode* node = list->buckets[i];
        while (node != nullptr) {
            ++outCount;
            node = node->next;
        }
    }

    if (outCount == 0) return nullptr;

    // Cấp phát mảng
    CustomerSpending* arr = new CustomerSpending[outCount];
    int idx = 0;

    // Copy dữ liệu
    for (int i = 0; i < list->capacity; ++i) {
        CustomerSpendingNode* node = list->buckets[i];
        while (node != nullptr) {
            arr[idx++] = node->spending;
            node = node->next;
        }
    }

    return arr;
}

// Thêm hàm getTopSpenders thiếu
CustomerSpending* getTopSpenders(const CustomerSpendingList* list, int topCount) {
    if (!list || topCount <= 0) return nullptr;

    int count = 0;
    CustomerSpending* allSpenders = copyCustomerSpendingsToArray(list, count);

    if (!allSpenders || count == 0) return nullptr;

    // Sắp xếp giảm dần theo chi tiêu
    quickSort_CustomerSpending(allSpenders, 0, count - 1, compareSpendingDesc);

    // Tạo mảng kết quả với kích thước là topCount hoặc nhỏ hơn nếu không đủ dữ liệu
    int resultCount = (count < topCount) ? count : topCount;
    CustomerSpending* result = new CustomerSpending[resultCount];

    // Copy các phần tử hàng đầu vào mảng kết quả
    for (int i = 0; i < resultCount; i++) {
        strcpy_s(result[i].username, allSpenders[i].username);
        result[i].totalAmount = allSpenders[i].totalAmount;
    }

    delete[] allSpenders;
    return result;
}

void printTop10Spending(const CustomerSpendingList* list) {
    int count = 0;
    CustomerSpending* arr = copyCustomerSpendingsToArray(list, count);

    if (arr == nullptr || count == 0) {
        std::cout << "No customers found.\n";
        return;
    }

    // Sửa cách gọi hàm không cần ép kiểu
    quickSort_CustomerSpending(arr, 0, count - 1, compareSpendingDesc);

    int top = (count < 10) ? count : 10;

    std::cout << "Top " << top << " customers with highest spending:\n";
    std::cout << "---------------------------------------------\n";
    for (int i = 0; i < top; ++i) {
        std::cout << i + 1 << ". Username: " << arr[i].username << " | Total Spending: " << arr[i].totalAmount << "\n";
    }
    std::cout << "---------------------------------------------\n";

    delete[] arr;
}