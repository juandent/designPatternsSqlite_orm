#pragma once

#include <thread>
#include <mutex>


template<typename Db, typename T>
class EntityCache
{
    std::unordered_map<int, std::shared_ptr<T>> cache;
    Db& storage;
    std::mutex cacheMutex;  // Protect cache for thread safety
public:
    EntityCache( Db& storage ) : storage{storage}  {}
    std::shared_ptr<T> get(int id)  // retrieve or load
    {
        std::lock_guard<std::mutex> lock(cacheMutex);

        if (cache.find(id) == cache.end()) {
            auto ptr = storage.get_pointer<T>(id);
            if (ptr) {
                cache[id] = std::shared_ptr<T>(std::move(ptr));
                // cache[id] = nullptr; // std::make_shared<T>(ptr);
            }
            else {
                return nullptr;
            }
        }
        return cache[id];
    }
    std::shared_ptr<T> insert(T& object) {
        std::lock_guard<std::mutex> lock(cacheMutex);

        // Insert into the database and get the assigned ID
        object.id = storage.insert(object);

        // Cache the newly inserted customer
        auto objPtr = std::make_shared<T>(object);
        cache[object.id] = objPtr;
        return objPtr;
    }
	void clear()
    {
        cache.clear();
    }

};



template<typename Db>
class IdentityMap {
    Db& storage;

    // Generic cache for each entity type, stored by type and ID
    // template <typename T>
    // using EntityCache = std::unordered_map<int, std::shared_ptr<T>>;

    // template<typename T>
    // EntityCache<T> getCache() { return cache<T> }

    // Maps to hold caches by type
    EntityCache<Db, Customer> customerCache;
    EntityCache<Db, Invoice> invoiceCache;
    EntityCache<Db, InvoiceLine> invoiceLineCache;

public:
    explicit IdentityMap(Db& storage) : storage(storage), customerCache(storage), invoiceCache(storage), invoiceLineCache(storage) {}

    void unloadCustomers()
    {
        customerCache.clear();
    }
    void unloadInvoices()
    {
        invoiceCache.clear();
    }
    void unloadInvoiceLines()
    {
        invoiceLineCache.clear();
    }
    // Retrieve or load a Customer by ID
    std::shared_ptr<Customer> getCustomer(int id) {
        return customerCache.get(id);
    }
    // insert Customer object
    std::shared_ptr<Customer> insert(Customer& object) {
        return customerCache.insert(object);
    }
    // Retrieve or load an Invoice by ID
    std::shared_ptr<Invoice> getInvoice(int id) {
        return invoiceCache.get(id);
    }
    // insert Invoice object
    std::shared_ptr<Invoice> insert(Invoice& invoice) {
        return invoiceCache.insert(invoice);
    }
    // Retrieve or load an InvoiceLine by ID
    std::shared_ptr<InvoiceLine> getInvoiceLine(int id) {
        return invoiceLineCache.get(id);
    }
    // insert InvoiceLine object
    std::shared_ptr<InvoiceLine> insert(InvoiceLine& invoiceLine) {
        return invoiceLineCache.insert(invoiceLine);
    }
};
