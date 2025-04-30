#ifndef SPENDING_STATISTIC_H
#define SPENDING_STATISTIC_H

#include "Spending.h"

int compareSpendingDesc(const CustomerSpending* a, const CustomerSpending* b);
// Sửa định nghĩa hàm trong header
void quickSort_CustomerSpending(CustomerSpending arr[], int low, int high, int (*compareFunc)(const CustomerSpending*, const CustomerSpending*));
CustomerSpending* copyCustomerSpendingsToArray(const CustomerSpendingList* list, int& outCount);
CustomerSpending* getTopSpenders(const CustomerSpendingList* list, int topCount);
void printTop10Spending(const CustomerSpendingList* list);

#endif