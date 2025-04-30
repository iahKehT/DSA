#include <iostream>
#include <cstring>
#include <limits>
#include "Invoice.h"
#include "Spending.h"
#include "Sort.h"
#include "SpendingStatistic.h"

#define _CRT_SECURE_NO_WARNINGS

// Hàm hiển thị menu chính
void displayMenu() {
    std::cout << "\n===== CHUONG TRINH QUAN LY HOA DON =====\n";
    std::cout << "1. Them hoa don moi\n";
    std::cout << "2. Hien thi tat ca hoa don\n";
    std::cout << "3. Tim hoa don theo ID\n";
    std::cout << "4. Cap nhat hoa don\n";
    std::cout << "5. Xoa hoa don\n";
    std::cout << "6. Hien thi chi tieu khach hang\n";
    std::cout << "7. Sap xep hoa don\n";
    std::cout << "8. Thong ke 10 khach hang chi tieu nhieu nhat\n";
    std::cout << "0. Thoat\n";
    std::cout << "Lua chon cua ban: ";
}

// Hàm nhập thông tin hóa đơn cho cập nhật
Invoice inputInvoiceForUpdate(const char* existingId, CustomerSpendingList* spendingList) {
    Invoice inv;
    bool isValid = false;

    strcpy_s(inv.id, existingId);

    // Nhập username
    do {
        std::cout << "Nhap username: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.getline(inv.username, 50);
        if (isValidCustomerName(inv.username)) {
            isValid = true;
        }
        else {
            std::cout << "Username khong hop le. Vui long nhap lai!\n";
        }
    } while (!isValid);
    isValid = false;

    // Nhập tên khách hàng
    do {
        std::cout << "Nhap ten khach hang: ";
        std::cin.getline(inv.customerName, 50);
        if (isValidCustomerName(inv.customerName)) {
            isValid = true;
        }
        else {
            std::cout << "Ten khach hang khong hop le. Vui long nhap lai!\n";
        }
    } while (!isValid);
    isValid = false;

    // Nhập ngày tháng
    do {
        std::cout << "Nhap ngay (dd mm yyyy): ";
        if (getNumericInput(inv.date.day) &&
            getNumericInput(inv.date.month) &&
            getNumericInput(inv.date.year)) {

            if (inv.date.isValid()) {
                isValid = true;
            }
            else {
                std::cout << "Ngay thang khong hop le. Vui long nhap lai!\n";
            }
        }
        else {
            std::cout << "Vui long nhap so cho ngay, thang, nam!\n";
        }
    } while (!isValid);

    // Nhập số tiền
    isValid = false;
    do {
        std::cout << "Nhap so tien: ";
        if (getNumericInput(inv.amount)) {
            if (inv.isValidAmount()) {
                isValid = true;
            }
            else {
                std::cout << "So tien phai lon hon 0. Vui long nhap lai!\n";
            }
        }
        else {
            std::cout << "Vui long nhap so cho so tien!\n";
        }
    } while (!isValid);

    return inv;
}

// Hàm nhập thông tin hóa đơn mới
Invoice inputInvoice(InvoiceList* list) {
    Invoice inv;
    bool isValid = false;

    // Nhập ID
    do {
        std::cout << "Nhap ID hoa don: ";
        std::cin >> inv.id;

        if (!isValidID(inv.id)) {
            std::cout << "ID khong hop le. Vui long nhap lai!\n";
            continue;
        }

        if (!list->isIDUnique(inv.id)) {
            std::cout << "ID da ton tai. Vui long nhap ID khac!\n";
            continue;
        }

        isValid = true;
    } while (!isValid);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Nhập username
    isValid = false;
    do {
        std::cout << "Nhap username: ";
        std::cin.getline(inv.username, 50);
        if (isValidCustomerName(inv.username)) {
            isValid = true;
        }
        else {
            std::cout << "Username khong hop le. Vui long nhap lai!\n";
        }
    } while (!isValid);

    // Nhập tên khách hàng
    isValid = false;
    do {
        std::cout << "Nhap ten khach hang: ";
        std::cin.getline(inv.customerName, 50);

        if (isValidCustomerName(inv.customerName)) {
            isValid = true;
        }
        else {
            std::cout << "Ten khach hang khong hop le. Vui long nhap lai!\n";
        }
    } while (!isValid);

    // Nhập ngày tháng
    isValid = false;
    do {
        std::cout << "Nhap ngay (dd mm yyyy): ";
        if (getNumericInput(inv.date.day) &&
            getNumericInput(inv.date.month) &&
            getNumericInput(inv.date.year)) {

            if (inv.date.isValid()) {
                isValid = true;
            }
            else {
                std::cout << "Ngay thang khong hop le. Vui long nhap lai!\n";
            }
        }
        else {
            std::cout << "Vui long nhap so cho ngay, thang, nam!\n";
        }
    } while (!isValid);

    // Nhập số tiền
    isValid = false;
    do {
        std::cout << "Nhap so tien: ";
        if (getNumericInput(inv.amount)) {
            if (inv.isValidAmount()) {
                isValid = true;
            }
            else {
                std::cout << "So tien phai lon hon 0. Vui long nhap lai!\n";
            }
        }
        else {
            std::cout << "Vui long nhap so cho so tien!\n";
        }
    } while (!isValid);

    return inv;
}

// Hàm hiển thị thông tin một hóa đơn
void displayInvoice(Invoice* inv) {
    std::cout << "ID: " << inv->id << std::endl;
    std::cout << "Username: " << inv->username << std::endl;
    std::cout << "Khach hang: " << inv->customerName << std::endl;
    std::cout << "Ngay: " << inv->date.day << "/" << inv->date.month << "/" << inv->date.year << std::endl;
    std::cout << "So tien: " << inv->amount << std::endl;
}

// Hàm hiển thị tất cả hóa đơn
void displayAllInvoices(InvoiceList* list) {
    if (list->count == 0) {
        std::cout << "Danh sach hoa don trong!\n";
        return;
    }

    std::cout << "\n===== DANH SACH HOA DON =====\n";
    for (int i = 0; i < list->count; i++) {
        std::cout << "\nHoa don #" << (i + 1) << ":\n";
        displayInvoice(list->data[i]);
        std::cout << "-------------------------\n";
    }
}

// Hàm hiển thị danh sách chi tiêu
void displayAllSpending(CustomerSpendingList* spendingList) {
    if (spendingList->size == 0) {
        std::cout << "Danh sach chi tieu trong!\n";
        return;
    }

    std::cout << "\n===== DANH SACH CHI TIEU KHACH HANG =====\n";
    for (int i = 0; i < spendingList->capacity; i++) {
        CustomerSpendingNode* current = spendingList->buckets[i];
        while (current) {
            std::cout << "Username: " << current->spending.username
                << ", Tong chi tieu: " << current->spending.totalAmount << std::endl;
            current = current->next;
        }
    }
}

// Hàm xử lý lỗi cho addInvoice
void handleAddInvoiceError(InvoiceError errorCode) {
    switch (errorCode) {
    case LIST_NULL:
        std::cout << "Loi: Danh sach hoa don khong ton tai!\n";
        break;
    case INVALID_ID:
        std::cout << "Loi: ID hoa don khong hop le!\n";
        break;
    case ID_EXISTS:
        std::cout << "Loi: ID hoa don da ton tai!\n";
        break;
    case INVALID_CUSTOMER:
        std::cout << "Loi: Ten khach hang khong hop le!\n";
        break;
    case INVALID_DATE:
        std::cout << "Loi: Ngay thang khong hop le!\n";
        break;
    case INVALID_AMOUNT:
        std::cout << "Loi: So tien khong hop le!\n";
        break;
    case MEMORY_ERROR:
        std::cout << "Loi: Khong the cap phat bo nho!\n";
        break;
    case INVALID_USERNAME:
        std::cout << "Loi: Username khong hop le!\n";
        break;
    default:
        std::cout << "Loi khong xac dinh!\n";
    }
}

// Hàm xử lý lỗi cho updateInvoiceById
void handleUpdateInvoiceError(InvoiceError errorCode) {
    switch (errorCode) {
    case INVOICE_NOT_FOUND:
        std::cout << "Loi: Khong tim thay hoa don!\n";
        break;
    case INVALID_ID:
        std::cout << "Loi: ID hoa don moi khong hop le!\n";
        break;
    case INVALID_CUSTOMER:
        std::cout << "Loi: Ten khach hang moi khong hop le!\n";
        break;
    case INVALID_DATE:
        std::cout << "Loi: Ngay thang moi khong hop le!\n";
        break;
    case INVALID_AMOUNT:
        std::cout << "Loi: So tien moi khong hop le!\n";
        break;
    case ID_EXISTS:
        std::cout << "Loi: ID moi da ton tai!\n";
        break;
    case INVALID_USERNAME:
        std::cout << "Loi: Username moi khong hop le!\n";
        break;
    default:
        std::cout << "Loi khong xac dinh!\n";
    }
}

// Hàm hiển thị menu sắp xếp
void displaySortMenu() {
    std::cout << "\n===== LUA CHON SAP XEP =====\n";
    std::cout << "1. Theo so tien (tang dan)\n";
    std::cout << "2. Theo so tien (giam dan)\n";
    std::cout << "3. Theo ngay giao dich (tang dan)\n";
    std::cout << "4. Theo ngay giao dich (giam dan)\n";
    std::cout << "5. Theo ten khach hang (A-Z)\n";
    std::cout << "6. Theo ten khach hang (Z-A)\n";
    std::cout << "7. Theo nhieu tieu chi (Ngay -> Tien -> Ten)\n";
    std::cout << "0. Quay lai\n";
    std::cout << "Lua chon cua ban: ";
}

// Hàm main
int main() {
    // Khởi tạo danh sách hóa đơn với OOP
    InvoiceList invoiceList(5);
    CustomerSpendingList* spendingList = createSpendingList(5);
    int choice;

    do {
        displayMenu();
        if (!getNumericInput(choice)) {
            std::cout << "Vui long nhap so!\n";
            continue;
        }

        switch (choice) {
        case 1: {
            std::cout << "\n===== THEM HOA DON MOI =====\n";
            Invoice newInv = inputInvoice(&invoiceList);
            InvoiceError result = invoiceList.addInvoice(newInv, spendingList);

            if (result == SUCCESS) {
                std::cout << "Them hoa don thanh cong!\n";
            }
            else {
                handleAddInvoiceError(result);
            }
            break;
        }
        case 2: {
            displayAllInvoices(&invoiceList);
            break;
        }
        case 3: {
            std::cout << "\n===== TIM HOA DON THEO ID =====\n";
            char searchId[20];
            std::cout << "Nhap ID can tim: ";
            std::cin >> searchId;

            Invoice* foundInvoice = invoiceList.findById(searchId);
            if (foundInvoice) {
                std::cout << "\nTim thay hoa don:\n";
                displayInvoice(foundInvoice);
            }
            else {
                std::cout << "Khong tim thay hoa don voi ID: " << searchId << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\n===== CAP NHAT HOA DON =====\n";
            char updateId[20];
            std::cout << "Nhap ID hoa don can cap nhat: ";
            std::cin >> updateId;

            Invoice* foundInvoice = invoiceList.findById(updateId);
            if (foundInvoice) {
                std::cout << "\nThong tin hoa don hien tai:\n";
                displayInvoice(foundInvoice);
                std::cout << "\nNhap thong tin moi:\n";

                Invoice updatedInv = inputInvoiceForUpdate(updateId, spendingList);

                InvoiceError result = invoiceList.updateInvoiceById(updateId, updatedInv, spendingList);
                if (result == SUCCESS) {
                    std::cout << "Cap nhat hoa don thanh cong!\n";
                }
                else {
                    handleUpdateInvoiceError(result);
                }
            }
            else {
                std::cout << "Khong tim thay hoa don voi ID: " << updateId << std::endl;
            }
            break;
        }
        case 5: {
            std::cout << "\n===== XOA HOA DON =====\n";
            char deleteId[20];
            std::cout << "Nhap ID hoa don can xoa: ";
            std::cin >> deleteId;

            Invoice* foundInvoice = invoiceList.findById(deleteId);
            if (foundInvoice) {
                std::cout << "\nThong tin hoa don se bi xoa:\n";
                displayInvoice(foundInvoice);

                char confirm;
                std::cout << "Ban co chac chan muon xoa hoa don nay? (y/n): ";
                std::cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    InvoiceError result = invoiceList.deleteInvoiceById(deleteId, spendingList);
                    if (result == SUCCESS) {
                        std::cout << "Xoa hoa don thanh cong!\n";
                    }
                    else {
                        std::cout << "Xoa hoa don that bai!\n";
                    }
                }
                else {
                    std::cout << "Da huy thao tac xoa hoa don!\n";
                }
            }
            else {
                std::cout << "Khong tim thay hoa don voi ID: " << deleteId << std::endl;
            }
            break;
        }
        case 6: {
            displayAllSpending(spendingList);
            break;
        }
        case 7: {
            if (invoiceList.count == 0) {
                std::cout << "Danh sach hoa don trong, khong the sap xep!\n";
                break;
            }

            int sortChoice;
            displaySortMenu();
            if (!getNumericInput(sortChoice)) {
                std::cout << "Vui long nhap so!\n";
                break;
            }

            switch (sortChoice) {
            case 1:
                invoiceList.sortInvoices(compareInvoiceByAmount);
                std::cout << "Da sap xep hoa don theo so tien tang dan!\n";
                break;
            case 2:
                invoiceList.sortInvoices(compareInvoiceByAmountDesc);
                std::cout << "Da sap xep hoa don theo so tien giam dan!\n";
                break;
            case 3:
                invoiceList.sortInvoices(compareInvoiceByDate);
                std::cout << "Da sap xep hoa don theo ngay tang dan!\n";
                break;
            case 4:
                invoiceList.sortInvoices(compareInvoiceByDateDesc);
                std::cout << "Da sap xep hoa don theo ngay giam dan!\n";
                break;
            case 5:
                invoiceList.sortInvoices(compareInvoiceByName);
                std::cout << "Da sap xep hoa don theo ten khach hang (A-Z)!\n";
                break;
            case 6:
                invoiceList.sortInvoices(compareInvoiceByNameDesc);
                std::cout << "Da sap xep hoa don theo ten khach hang (Z-A)!\n";
                break;
            case 7:
                invoiceList.sortInvoices(compareMultiCriteria);
                std::cout << "Da sap xep hoa don theo nhieu tieu chi!\n";
                break;
            case 0:
                break;
            default:
                std::cout << "Lua chon khong hop le!\n";
            }

            if (sortChoice >= 1 && sortChoice <= 7) {
                // Hiển thị danh sách sau khi sắp xếp
                displayAllInvoices(&invoiceList);
            }
            break;
        }
        case 8: {
            std::cout << "\n===== TOP 10 KHACH HANG CHI TIEU NHIEU NHAT =====\n";
            if (spendingList->size == 0) {
                std::cout << "Danh sach chi tieu trong!\n";
                break;
            }

            CustomerSpending* topSpenders = getTopSpenders(spendingList, 10);
            if (topSpenders) {
                for (int i = 0; i < 10 && i < spendingList->size; i++) {
                    std::cout << "#" << (i + 1) << ": Username: " << topSpenders[i].username
                        << ", Tong chi tieu: " << topSpenders[i].totalAmount << std::endl;
                }
                delete[] topSpenders;
            }
            else {
                std::cout << "Khong the lay danh sach top chi tieu!\n";
            }
            break;
        }
        case 0:
            std::cout << "Cam on ban da su dung chuong trinh!\n";
            break;
        default:
            std::cout << "Lua chon khong hop le. Vui long chon lai!\n";
        }
    } while (choice != 0);

    // Giải phóng bộ nhớ trước khi kết thúc chương trình
    invoiceList.cleanup();
    deleteSpendingList(spendingList);

    return 0;
}