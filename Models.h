#pragma once


#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <memory>
#include <chrono>

using namespace sqlite_orm;
#include "Timepointconversions.h"


struct Customer {
    int id;
    std::string name;
    std::string email;

    ~Customer()
    {}
};

struct Invoice {
    int id;
    int customerId;
    double totalAmount;
    int64_t date;

    // Getters to work with time_point
    std::chrono::system_clock::time_point getDate() const { return fromUnixTimestamp(date); }
    void setDate(const std::chrono::system_clock::time_point& tp) { date = toUnixTimestamp(tp); }
};

struct InvoiceLine {
    int id;
    int invoiceId;
    std::string description;
    double price;
    int quantity;

    double lineTotal() const {
        return price * quantity;
    }
};
using namespace sqlite_orm;

auto initStorage(const std::string& path) {
    return make_storage(
        path,
        make_table("customers",
            make_column("id", &Customer::id, primary_key().autoincrement()),
            make_column("name", &Customer::name),
            make_column("email", &Customer::email)),

        make_table("invoices",
            make_column("id", &Invoice::id, primary_key().autoincrement()),
            make_column("customer_id", &Invoice::customerId),
            make_column("total_amount", &Invoice::totalAmount),
            make_column("date", &Invoice::date)),

        make_table("invoice_lines",
            make_column("id", &InvoiceLine::id, primary_key().autoincrement()),
            make_column("invoice_id", &InvoiceLine::invoiceId),
            make_column("description", &InvoiceLine::description),
            make_column("price", &InvoiceLine::price),
            make_column("quantity", &InvoiceLine::quantity)));
}
