#include "Spending.h"
#include <cstring>

// hash dựa trên ký tự dầu tiên
unsigned int hashString(const char* str, int capacity) {
    if (!str || str[0] == '\0') return 0; // Chuỗi rỗng trả về bucket 0
    return static_cast<unsigned int>(str[0]) % capacity; // Lấy ASCII ký tự đầu, modulo với capacity
}

CustomerSpendingList* createSpendingList(int initialCapacity) {
    if (initialCapacity <= 0) return nullptr;
    CustomerSpendingList* list = new CustomerSpendingList;
    list->buckets = new CustomerSpendingNode * [initialCapacity]();
    if (!list->buckets) {
        delete list;
        return nullptr;
    }
    list->size = 0;
    list->capacity = initialCapacity;
    return list;
}

int resizeCustomerSpendingList(CustomerSpendingList* list) {
    if (!list || !list->buckets) return 0;
    int newCapacity = list->capacity * 2;
    CustomerSpendingNode** newBuckets = new CustomerSpendingNode * [newCapacity]();
    if (!newBuckets) return 0;
    for (int i = 0; i < list->capacity; ++i) {
        CustomerSpendingNode* current = list->buckets[i];
        while (current) {
            CustomerSpendingNode* next = current->next;
            unsigned int newIndex = hashString(current->spending.username, newCapacity);
            current->next = newBuckets[newIndex];
            newBuckets[newIndex] = current;
            current = next;
        }
    }
    delete[] list->buckets;
    list->buckets = newBuckets;
    list->capacity = newCapacity;
    return 1;
}

int freeCustomerSpendingList(CustomerSpendingList* list) {
    if (!list) return 0;
    for (int i = 0; i < list->capacity; ++i) {
        CustomerSpendingNode* current = list->buckets[i];
        while (current) {
            CustomerSpendingNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] list->buckets;
    delete list;
    return 1;
}

// Alias cho freeCustomerSpendingList để phù hợp với lời gọi trong main.cpp
int deleteSpendingList(CustomerSpendingList* list) {
    return freeCustomerSpendingList(list);
}

CustomerSpending getCustomerSpending(const CustomerSpendingList* list, const char* username) {
    CustomerSpending notFound = { "", 0.0 };
    if (!list || !username || !list->buckets) return notFound;
    unsigned int index = hashString(username, list->capacity);
    CustomerSpendingNode* current = list->buckets[index];
    while (current) {
        if (strcmp(current->spending.username, username) == 0) return current->spending;
        current = current->next;
    }
    return notFound;
}

void addOrUpdateSpending(CustomerSpendingList* spendingList, const Invoice& invoice) {
    if (!spendingList || !spendingList->buckets || invoice.amount < 0) return;
    unsigned int index = hashString(invoice.username, spendingList->capacity);
    CustomerSpendingNode* current = spendingList->buckets[index];
    CustomerSpendingNode* prev = nullptr;
    // Tìm khách hàng
    while (current) {
        if (strcmp(current->spending.username, invoice.username) == 0) {
            // Cập nhật chi tiêu
            current->spending.totalAmount += invoice.amount;
            // Nếu chi tiêu về 0, xóa
            if (current->spending.totalAmount <= 0.0) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    spendingList->buckets[index] = current->next;
                }
                delete current;
                spendingList->size--;
            }
            return;
        }
        prev = current;
        current = current->next;
    }
    // Thêm khách hàng mới nếu amount > 0
    if (invoice.amount <= 0.0) return;
    // Kiểm tra, mở rộng nếu cần
    if (static_cast<double>(spendingList->size + 1) / spendingList->capacity > 0.75) {
        if (!resizeCustomerSpendingList(spendingList)) return;
        index = hashString(invoice.username, spendingList->capacity);
    }
    CustomerSpendingNode* newNode = new CustomerSpendingNode;
    strcpy_s(newNode->spending.username, invoice.username);
    newNode->spending.totalAmount = invoice.amount;
    newNode->next = spendingList->buckets[index];
    spendingList->buckets[index] = newNode;
    spendingList->size++;
}

int removeCustomerSpending(CustomerSpendingList* spendingList, const char* username) {
    if (!spendingList || !username || !spendingList->buckets) return 0;
    unsigned int index = hashString(username, spendingList->capacity);
    CustomerSpendingNode* current = spendingList->buckets[index];
    CustomerSpendingNode* prev = nullptr;
    while (current) {
        if (strcmp(current->spending.username, username) == 0) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                spendingList->buckets[index] = current->next;
            }
            delete current;
            spendingList->size--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}