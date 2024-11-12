#pragma once

#include "IdentityMap.h"


template<typename Db, typename IdentityMap>
class UnitOfWork {
    Db& storage;
    IdentityMap& map;
    bool transactionActive = false;

public:
    explicit UnitOfWork(Db& storage, IdentityMap& map) : storage(storage), map{ map } {}

    // Begin a transaction
    void begin() {
        if (!transactionActive) {
            storage.begin_transaction();
            transactionActive = true;
        }
    }

    // Commit the transaction
    void commit() {
        if (transactionActive) {
            storage.commit();
            transactionActive = false;
        }
    }

    // Rollback the transaction
    void rollback() {
        if (transactionActive) {
            storage.rollback();
            transactionActive = false;
        }
    }

    // Add a new entity to the database and track it
    template <typename T>
    void add(T& entity) {
        begin();
        map.insert(entity);
    }

    // Update an existing entity in the database and track it
    template <typename T>
    void update(const T& entity) {
        begin();
        storage.update(entity);
    }

    // Remove an entity from the database and track it
    template <typename T>
    void remove(int id) {
        begin();
        storage.remove<T>(id);
    }


    // Destructor to ensure the transaction is not left open
    ~UnitOfWork() {
        if (transactionActive) {
            storage.rollback();
        }
    }
};

