#ifndef SPENDING_H
#define SPENDING_H

#include "Invoice.h"

struct CustomerSpending {
    char username[50];
    int totalAmount;
};

struct CustomerSpendingNode {
    CustomerSpending spending;
    CustomerSpendingNode* next;
};

struct CustomerSpendingList {
    CustomerSpendingNode** buckets;
    int size;
    int capacity;
};

CustomerSpendingList* createSpendingList(int initialCapacity);
int resizeCustomerSpendingList(CustomerSpendingList* list);
int freeCustomerSpendingList(CustomerSpendingList* list);
int deleteSpendingList(CustomerSpendingList* list); // Alias for freeCustomerSpendingList
CustomerSpending getCustomerSpending(const CustomerSpendingList* list, const char* username);
void addOrUpdateSpending(CustomerSpendingList* spendingList, const Invoice& invoice);
int removeCustomerSpending(CustomerSpendingList* spendingList, const char* username);

#endif