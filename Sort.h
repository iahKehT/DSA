#ifndef SORT_H
#define SORT_H

#include "Invoice.h"

// Hoán đổi 2 hóa đơn
void swapInvoices(Invoice* a, Invoice* b);

// Các hàm so sánh tăng dần
int compareInvoiceByAmount(const Invoice* a, const Invoice* b);
int compareInvoiceByDate(const Invoice* a, const Invoice* b);
int compareInvoiceByName(const Invoice* a, const Invoice* b);

// Các hàm so sánh giảm dần
int compareInvoiceByAmountDesc(const Invoice* a, const Invoice* b);
int compareInvoiceByDateDesc(const Invoice* a, const Invoice* b);
int compareInvoiceByNameDesc(const Invoice* a, const Invoice* b);

// Thêm các hàm đa tiêu chí kết hợp
int compareByAmountAndDate(const Invoice* a, const Invoice* b);
int compareByNameAndDate(const Invoice* a, const Invoice* b);
int compareMultiCriteria(const Invoice* a, const Invoice* b); // Ngày -> Tiền -> Tên

// Thuật toán HeapSort
void heapify(Invoice** arr, int n, int i, int (*compareFunc)(const Invoice*, const Invoice*));
void sortInvoiceList(InvoiceList* list, int (*compareFunc)(const Invoice*, const Invoice*));

#endif