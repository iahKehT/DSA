#include "Sort.h"
#include <cstring> // cho strcmp

// Hàm hoán đổi
void swapInvoices(Invoice* a, Invoice* b) {
    Invoice temp = *a;
    *a = *b;
    *b = temp;
}

// So sánh theo số tiền
int compareInvoiceByAmount(const Invoice* a, const Invoice* b) {
    if (a->amount < b->amount) return -1; // a nhỏ hơn -> đứng trước
    if (a->amount > b->amount) return 1;  // a lớn hơn -> đứng sau
    return 0;
}

// So sánh theo ngày (năm -> tháng -> ngày)
int compareInvoiceByDate(const Invoice* a, const Invoice* b) {
    if (a->date.year != b->date.year)
        return a->date.year - b->date.year; // Năm nhỏ hơn -> đứng trước
    if (a->date.month != b->date.month)
        return a->date.month - b->date.month;
    return a->date.day - b->date.day;
}

// So sánh theo tên khách hàng
int compareInvoiceByName(const Invoice* a, const Invoice* b) {
    return strcmp(a->customerName, b->customerName);
}

// Các hàm giảm dần
int compareInvoiceByAmountDesc(const Invoice* a, const Invoice* b) {
    return -compareInvoiceByAmount(a, b); // Đảo ngược kết quả
}

int compareInvoiceByDateDesc(const Invoice* a, const Invoice* b) {
    return -compareInvoiceByDate(a, b);
}

int compareInvoiceByNameDesc(const Invoice* a, const Invoice* b) {
    return -compareInvoiceByName(a, b);
}

// Các hàm đa tiêu chí
int compareByAmountAndDate(const Invoice* a, const Invoice* b) {
    int amountCompare = compareInvoiceByAmount(a, b);
    if (amountCompare != 0) return amountCompare;
    return compareInvoiceByDate(a, b);
}

int compareByNameAndDate(const Invoice* a, const Invoice* b) {
    int nameCompare = compareInvoiceByName(a, b);
    if (nameCompare != 0) return nameCompare;
    return compareInvoiceByDate(a, b);
}

// Hàm đa tiêu chí chính (Ngày -> Tiền -> Tên)
int compareMultiCriteria(const Invoice* a, const Invoice* b) {
    int dateCompare = compareInvoiceByDate(a, b);
    if (dateCompare != 0) return dateCompare;

    int amountCompare = compareInvoiceByAmount(a, b);
    if (amountCompare != 0) return amountCompare;

    return compareInvoiceByName(a, b);
}

// Hàm heapify để tạo và duy trì cấu trúc heap
void heapify(Invoice** arr, int n, int i, int (*compareFunc)(const Invoice*, const Invoice*)) {
    int largest = i;      // Khởi tạo largest là nút gốc
    int left = 2 * i + 1; // Con bên trái = 2*i + 1
    int right = 2 * i + 2; // Con bên phải = 2*i + 2

    // Nếu con trái lớn hơn nút gốc
    if (left < n && compareFunc(arr[left], arr[largest]) > 0)
        largest = left;

    // Nếu con phải lớn hơn nút gốc hoặc con trái
    if (right < n && compareFunc(arr[right], arr[largest]) > 0)
        largest = right;

    // Nếu largest không phải là nút gốc
    if (largest != i) {
        // Hoán đổi nút gốc với largest
        swapInvoices(arr[i], arr[largest]);

        // Đệ quy heapify cây con bị ảnh hưởng
        heapify(arr, n, largest, compareFunc);
    }
}

// Hàm sắp xếp danh sách hóa đơn sử dụng Heap Sort
void sortInvoiceList(InvoiceList* list, int (*compareFunc)(const Invoice*, const Invoice*)) {
    if (list == nullptr || list->count <= 1) {
        return; // Không cần sắp xếp nếu danh sách trống hoặc chỉ có 1 phần tử
    }

    int n = list->count;
    Invoice** arr = list->data;

    // Xây dựng heap (sắp xếp lại mảng)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, compareFunc);

    // Trích xuất từng phần tử từ heap
    for (int i = n - 1; i > 0; i--) {
        // Di chuyển nút gốc hiện tại đến cuối
        swapInvoices(arr[0], arr[i]);

        // Gọi max heapify trên heap đã giảm kích thước
        heapify(arr, i, 0, compareFunc);
    }
}