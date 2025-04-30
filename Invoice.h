#pragma once
#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <iostream>
#include <limits>

// Enum cho các mã lỗi 
enum InvoiceError {
    SUCCESS = 0,
    LIST_NULL = -1,
    INVALID_ID = -2,
    ID_EXISTS = -3,
    INVALID_CUSTOMER = -4,
    INVALID_DATE = -5,
    INVALID_AMOUNT = -6,
    MEMORY_ERROR = -7,
    INVALID_USERNAME = -8,
    INVOICE_NOT_FOUND = -9
};

struct Date {
    int day;
    int month;
    int year;

    // Thêm phương thức kiểm tra tính hợp lệ của ngày
    bool isValid() const;
};

// Forward declaration
struct CustomerSpendingList;

struct Invoice {
    char id[20];
    char customerName[50];
    char username[50];
    Date date;
    double amount;

    // Thêm các phương thức kiểm tra tính hợp lệ
    bool isValidID() const;
    bool isValidCustomerName() const;
    bool isValidUsername() const;
    bool isValidAmount() const;
};

struct InvoiceList {
    Invoice** data;
    int count;
    int capacity;

    // Constructor và Destructor
    InvoiceList(int initialCapacity = 10);
    ~InvoiceList();

    // Các phương thức cơ bản
    void initialize(int initialCapacity = 10);
    void cleanup();
    void add(Invoice* invoice);
    Invoice* findById(const char* id);
    void printAll();

    // Các phương thức xử lý hóa đơn
    InvoiceError addInvoice(const Invoice& inv, CustomerSpendingList* spendingList);
    int findInvoiceIndexById(const char* targetId) const;
    InvoiceError updateInvoiceById(const char* targetId, const Invoice& newInvoice, CustomerSpendingList* spendingList);
    InvoiceError deleteInvoiceById(const char* targetId, CustomerSpendingList* spendingList);
    bool isIDUnique(const char* id) const;
    bool resize();

    // Phương thức sắp xếp 
    void sortInvoices(int (*compareFunc)(const Invoice*, const Invoice*));
};

// Template cho hàm nhập dữ liệu số
template<typename T>
bool getNumericInput(T& value) {
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// Chuyển các hàm kiểm tra tách rời thành các hàm ngoài class để có thể dùng chung
bool isValidID(const char* id);
bool isValidCustomerName(const char* name);
bool isValidDate(const Date& date);
bool isValidAmount(double amount);
bool getStringInput(char* buffer, size_t size);

#endif