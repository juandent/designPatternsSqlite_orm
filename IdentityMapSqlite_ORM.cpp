#include <sqlite_orm/sqlite_orm.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>


#include "Models.h"
#include "UnitOfWork.h"

using namespace sqlite_orm;


int main() {

    auto storage = initStorage("example.db");
    storage.sync_schema();

    IdentityMap identityMap(storage);

    UnitOfWork uow(storage, identityMap);

    try {
        uow.begin();

        // Create a customer
        Customer customer = { 0, "John Doe", "johndoe@example.com" };
        // auto cc = std::make_shared<Customer>(customer);
        // cc = nullptr;

        uow.add(customer);

        // identityMap.unloadCustomers();

        // Create an invoice for the customer
        Invoice invoice = { 0, customer.id, 300.0 }; // , std::chrono::system_clock::now()
        invoice.setDate(std::chrono::system_clock::now());
    
        uow.add(invoice);

        // Add invoice lines
        InvoiceLine line1 = { 0, invoice.id, "Widget A", 15.0, 2 };
        InvoiceLine line2 = { 0, invoice.id, "Widget B", 25.0, 1 };
        uow.add(line1);
        uow.add(line2);

        // Commit the transaction if everything is successful
        uow.commit();
        std::cout << "Transaction committed successfully.\n";

        auto retrievedInvoice = identityMap.getInvoice(invoice.id);
        // //std::cout << "Invoice Date: " << toUnixTimestamp(retrievedInvoice.getDate()) << "\n";
        std::cout << "Invoice Date: " << retrievedInvoice->getDate() << "\n";

        // auto p = uow.get<Invoice>(invoice.id);

    }
    catch (const std::exception& e) {
        // Rollback the transaction in case of error
        uow.rollback();
        std::cerr << "Transaction rolled back due to an error: " << e.what() << std::endl;
    }

    return 0;
}
