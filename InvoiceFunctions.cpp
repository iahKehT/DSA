#include "Invoice.h"
#include "Spending.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

// Triển khai phương thức addInvoice dưới dạng method của InvoiceList
InvoiceError InvoiceList::addInvoice(const Invoice& inv, CustomerSpendingList* spendingList) {
    // Kiểm tra tính hợp lệ
    if (!isValidID(inv.id)) return INVALID_ID;
    if (!this->isIDUnique(inv.id)) return ID_EXISTS;
    if (!isValidCustomerName(inv.customerName)) return INVALID_CUSTOMER;
    if (!inv.date.isValid()) return INVALID_DATE;
    if (!inv.isValidAmount()) return INVALID_AMOUNT;
    if (inv.username[0] == '\0') return INVALID_USERNAME;

    // Nếu danh sách đầy, mở rộng
    if (count == capacity) {
        if (!this->resize()) {
            return MEMORY_ERROR;
        }
    }

    // Tạo bản sao của hóa đơn
    Invoice* newInvoice = nullptr;
    try {
        newInvoice = new Invoice;
    }
    catch (std::bad_alloc& e) {
        std::cerr << "Lỗi cấp phát bộ nhớ: " << e.what() << std::endl;
        return MEMORY_ERROR;
    }

    *newInvoice = inv;
    data[count] = newInvoice;
    count++;

    // Cập nhật chi tiêu
    addOrUpdateSpending(spendingList, inv);

    return SUCCESS;
}

// Triển khai phương thức updateInvoiceById dưới dạng method của InvoiceList
InvoiceError InvoiceList::updateInvoiceById(const char* targetId, const Invoice& newInvoice, CustomerSpendingList* spendingList) {
    if (targetId == nullptr) return LIST_NULL;

    // Kiểm tra tính hợp lệ của dữ liệu mới
    if (!isValidID(newInvoice.id)) return INVALID_ID;
    if (!isValidCustomerName(newInvoice.customerName)) return INVALID_CUSTOMER;
    if (!newInvoice.date.isValid()) return INVALID_DATE;
    if (!newInvoice.isValidAmount()) return INVALID_AMOUNT;
    if (newInvoice.username[0] == '\0') return INVALID_USERNAME;

    int index = findInvoiceIndexById(targetId);
    if (index == -1) return INVOICE_NOT_FOUND;

    // Kiểm tra nếu ID mới khác ID cũ và đã tồn tại
    if (strcmp(targetId, newInvoice.id) != 0) {
        if (!isIDUnique(newInvoice.id)) return ID_EXISTS;
    }

    // Xử lý cập nhật chi tiêu khách hàng
    Invoice* oldInvoice = data[index];
    if (strcmp(oldInvoice->username, newInvoice.username) == 0) {
        // Cùng username, cập nhật chênh lệch
        Invoice temp = *oldInvoice;
        temp.amount = newInvoice.amount - oldInvoice->amount;
        addOrUpdateSpending(spendingList, temp);
    }
    else {
        // Khác username, xóa chi tiêu cũ, thêm chi tiêu mới
        Invoice tempOld = *oldInvoice;
        tempOld.amount = -oldInvoice->amount;
        addOrUpdateSpending(spendingList, tempOld);
        addOrUpdateSpending(spendingList, newInvoice);
    }

    // Cập nhật thông tin hóa đơn
    strcpy_s(data[index]->id, newInvoice.id);
    strcpy_s(data[index]->customerName, newInvoice.customerName);
    strcpy_s(data[index]->username, newInvoice.username);
    data[index]->date = newInvoice.date;
    data[index]->amount = newInvoice.amount;

    return SUCCESS;
}

// Triển khai phương thức deleteInvoiceById dưới dạng method của InvoiceList
InvoiceError InvoiceList::deleteInvoiceById(const char* targetId, CustomerSpendingList* spendingList) {
    if (targetId == nullptr) return LIST_NULL;

    int index = findInvoiceIndexById(targetId);
    if (index == -1) return INVOICE_NOT_FOUND;

    // Giảm chi tiêu của khách hàng khi xóa hóa đơn
    Invoice* inv = data[index];
    Invoice temp = *inv;
    temp.amount = -inv->amount;
    addOrUpdateSpending(spendingList, temp);

    // Xóa hóa đơn và giảm kích thước danh sách
    delete data[index];
    for (int i = index; i < count - 1; i++) {
        data[i] = data[i + 1];
    }
    count--;

    return SUCCESS;
}

// Triển khai phương thức sắp xếp hóa đơn
void InvoiceList::sortInvoices(int (*compareFunc)(const Invoice*, const Invoice*)) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compareFunc(data[j], data[j + 1]) > 0) {
                // Hoán đổi vị trí
                Invoice* temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}