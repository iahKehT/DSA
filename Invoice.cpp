#include "Invoice.h"
#include <iostream>
#include <cstring>
#include <cctype>

// Triển khai các phương thức kiểm tra tính hợp lệ
bool Date::isValid() const {
    if (month < 1 || month > 12) return false;
    if (day < 1 || year < 0) return false;

    // Số ngày trong tháng
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    // Kiểm tra năm nhuận
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        daysInMonth[2] = 29;
    }
    return day <= daysInMonth[month];
}

bool Invoice::isValidID() const {
    size_t len = strlen(id);
    if (len == 0 || len >= 20) return false;

    for (size_t i = 0; i < len; i++) {
        if (!isalnum(id[i])) {
            return false;
        }
    }
    return true;
}

bool Invoice::isValidCustomerName() const {
    size_t len = strlen(customerName);
    if (len == 0 || len >= 50) return false;

    for (size_t i = 0; i < len; i++) {
        if (!isalpha(customerName[i]) && customerName[i] != ' ' && !isalpha((unsigned char)customerName[i])) {
            return false;
        }
    }
    return true;
}

bool Invoice::isValidUsername() const {
    return strlen(username) > 0 && strlen(username) < 50;
}

bool Invoice::isValidAmount() const {
    return amount > 0;
}

// Constructor và Destructor cho InvoiceList
InvoiceList::InvoiceList(int initialCapacity) {
    initialize(initialCapacity);
}

InvoiceList::~InvoiceList() {
    cleanup();
}

// Các phương thức cơ bản của InvoiceList
void InvoiceList::initialize(int initialCapacity) {
    data = new Invoice * [initialCapacity];
    count = 0;
    capacity = initialCapacity;
}

void InvoiceList::cleanup() {
    for (int i = 0; i < count; i++) {
        delete data[i];
    }
    delete[] data;
    count = 0;
    capacity = 0;
}

void InvoiceList::add(Invoice* invoice) {
    if (count >= capacity) {
        resize();
    }

    if (count < capacity) {
        data[count] = invoice;
        count++;
    }
}

Invoice* InvoiceList::findById(const char* id) {
    for (int i = 0; i < count; i++) {
        if (strcmp(data[i]->id, id) == 0) {
            return data[i];
        }
    }
    return nullptr;
}

void InvoiceList::printAll() {
    for (int i = 0; i < count; i++) {
        Invoice* inv = data[i];
        std::cout << "ID: " << inv->id
            << ", Khach hang: " << inv->customerName
            << ", Ngay: " << inv->date.day << "/" << inv->date.month << "/" << inv->date.year
            << ", So tien: " << inv->amount << std::endl;
    }
}

// Phương thức resize - được gọi bên trong khi cần mở rộng danh sách
bool InvoiceList::resize() {
    int newCapacity = capacity * 2;
    Invoice** newData = nullptr;

    try {
        newData = new Invoice * [newCapacity];
    }
    catch (std::bad_alloc& e) {
        std::cerr << "Lỗi cấp phát bộ nhớ: " << e.what() << std::endl;
        return false;
    }

    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
    return true;
}

// Phương thức kiểm tra ID có duy nhất không
bool InvoiceList::isIDUnique(const char* id) const {
    for (int i = 0; i < count; i++) {
        if (strcmp(data[i]->id, id) == 0) {
            return false;
        }
    }
    return true;
}

// Tìm vị trí của hóa đơn theo ID
int InvoiceList::findInvoiceIndexById(const char* targetId) const {
    if (targetId == nullptr) return -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(data[i]->id, targetId) == 0) {
            return i;
        }
    }
    return -1;
}

// Các hàm kiểm tra độc lập để có thể sử dụng chung
bool isValidID(const char* id) {
    size_t len = strlen(id);
    if (len == 0 || len >= 20) return false;

    for (size_t i = 0; i < len; i++) {
        if (!isalnum(id[i])) {
            return false;
        }
    }
    return true;
}

bool isValidCustomerName(const char* name) {
    size_t len = strlen(name);
    if (len == 0 || len >= 50) return false;

    for (size_t i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != ' ' && !isalpha((unsigned char)name[i])) {
            return false;
        }
    }
    return true;
}

bool isValidDate(const Date& date) {
    return date.isValid();
}

bool isValidAmount(double amount) {
    return amount > 0;
}

bool getStringInput(char* buffer, size_t size) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.getline(buffer, size);
    return strlen(buffer) > 0;
}