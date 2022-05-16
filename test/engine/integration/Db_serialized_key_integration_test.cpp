#include <gtest/gtest.h>
#include <string>
#include "../DbFixture.h"
#include "../../../src/engine/db/Db.h"

using namespace pmem::storage;

struct EmployeeId {
    int id;
    std::string lastName;
};

struct Employee {
    int id;
    std::string firstName;
    std::string lastName;
};

class EmployeeIdComparator : public KeyComparator {
public:
    int compare(const Slice& a, const Slice& b) const override {
        const EmployeeId* first = reinterpret_cast<const EmployeeId*>(a.cdata());
        const EmployeeId* other = reinterpret_cast<const EmployeeId*>(b.cdata());

        if (first->id == other->id) {
            return 0;
        }
        if (first->id < other->id) {
            return -1;
        }
        return 1;
    }
};

const char* serializedKeyFilePath = "./serializedKey.log";

Db* open(int skipListTowerSize = 100) {
    Configuration configuration = Configuration(serializedKeyFilePath,
                                                8 * 1024 * 1024,
                                                skipListTowerSize,
                                                new EmployeeIdComparator());

    return Db::open(configuration);
}

void close(Db* db) {
    db -> close();
    remove(serializedKeyFilePath);
}


TEST(DbSerializedKeyIntegration, GetValueByKey) {
    std::vector<Employee*> employees = {new Employee{10, "Rahul", "Jain"},
                                       new Employee{5, "Kartik", "Rajan"},
                                       new Employee{-7, "John", ""},
                                       new Employee{11, "Mark", "Johnson"}};

    Db* db = open();
    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        char *value = (char*)malloc(sizeof(*employee));
        memcpy(value, (char*)employee, sizeof(*employee));

        db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    }

    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        std::pair<const char*, bool> existenceByValue = db->get(Slice(key, sizeof(*employeeId)));
        const Employee* saved = reinterpret_cast<const Employee*>(existenceByValue.first);

        ASSERT_EQ(employee->id, saved->id);
        ASSERT_EQ(employee->firstName, saved->firstName);
        ASSERT_EQ(employee->lastName, saved->lastName);
    }

    close(db);
}

TEST(DbSerializedKeyIntegration, GetByNonExistentKey) {
    Employee* employee = new Employee{10, "Rahul", "Jain"};

    Db* db = open();
    auto employeeId = new EmployeeId{employee->id, employee->lastName};

    char *key = (char*)malloc(sizeof(*employeeId));
    memcpy(key, (char*)employeeId, sizeof(*employeeId));

    char *value = (char*)malloc(sizeof(*employee));
    memcpy(value, (char*)employee, sizeof(*employee));

    db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));

    auto getEmployeeId = new EmployeeId{90, "Non-existent"};
    char *getKey = (char*)malloc(sizeof(*getEmployeeId));
    memcpy(getKey, (char*)getEmployeeId, sizeof(*getEmployeeId));

    std::pair<const char*, bool> existenceByValue = db->get(Slice(getKey, sizeof(*getEmployeeId)));
    ASSERT_FALSE(existenceByValue.second);

    close(db);
}

TEST(DbSerializedKeyIntegration, Scan) {
    std::vector<Employee*> employees = {new Employee{10, "Rahul", "Jain"},
                                       new Employee{5, "Kartik", "Rajan"},
                                       new Employee{-7, "John", ""},
                                       new Employee{11, "Mark", "Johnson"}};

    Db* db = open();

    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        char *value = (char*)malloc(sizeof(*employee));
        memcpy(value, (char*)employee, sizeof(*employee));

        db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    }

    std::vector<EmployeeId*> scan = {new EmployeeId{-7, "Random"}, new EmployeeId{10, "Random"}};

    auto beginEmployeeId = scan.at(0);
    char *beginKey = (char*)malloc(sizeof(*beginEmployeeId));
    memcpy(beginKey, (char*)beginEmployeeId, sizeof(*beginEmployeeId));

    auto endEmployeeId = scan.at(1);
    char *endKey = (char*)malloc(sizeof(*endEmployeeId));
    memcpy(endKey, (char*)endEmployeeId, sizeof(*endEmployeeId));

    std::vector<KeyValuePair> result = db->scan(Slice(beginKey, sizeof(*beginEmployeeId)), Slice(endKey, sizeof(*endEmployeeId)), 5);

    ASSERT_EQ(2, result.size());

    const Employee* first = reinterpret_cast<const Employee*>(result.at(0).getValue());
    ASSERT_EQ(-7, first->id);
    ASSERT_EQ("John", first->firstName);
    ASSERT_EQ("", first->lastName);

    const Employee* second = reinterpret_cast<const Employee*>(result.at(1).getValue());
    ASSERT_EQ(5, second->id);
    ASSERT_EQ("Kartik", second->firstName);
    ASSERT_EQ("Rajan", second->lastName);

    close(db);
}

TEST(DbSerializedKeyIntegration, MultiGet) {
    std::vector<Employee *> employees = {new Employee{10, "Rahul", "Jain"},
                                         new Employee{5, "Kartik", "Rajan"},
                                        new Employee{-7, "John", ""},
                                        new Employee{11, "Mark", "Johnson"}};

    Db* db = open();

    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        char *value = (char*)malloc(sizeof(*employee));
        memcpy(value, (char*)employee, sizeof(*employee));

        db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    }

    auto getId10 = new EmployeeId{10, "SomeRandomName"};
    char *getKey10 = (char*)malloc(sizeof(*getId10));
    memcpy(getKey10, (char*)getId10, sizeof(*getId10));

    auto getId5 = new EmployeeId{5, "OtherRandomName"};
    char *getKey5 = (char*)malloc(sizeof(*getId5));
    memcpy(getKey5, (char*)getId5, sizeof(*getId5));

    std::vector<Slice> multiGetKeys = {Slice(getKey10, sizeof(*getId10)), Slice(getKey5, sizeof(*getId5))};
    std::vector<std::pair<const char*, bool>> result = db->multiGet(multiGetKeys);

    ASSERT_EQ(2, result.size());

    const Employee* first = reinterpret_cast<const Employee*>(result.at(0).first);
    ASSERT_EQ(5, first->id);
    ASSERT_EQ("Kartik", first->firstName);
    ASSERT_EQ("Rajan", first->lastName);

    const Employee* second = reinterpret_cast<const Employee*>(result.at(1).first);
    ASSERT_EQ(10, second->id);
    ASSERT_EQ("Rahul", second->firstName);
    ASSERT_EQ("Jain", second->lastName);

    close(db);
}

TEST(DbSerializedKeyIntegration, Update) {
    std::vector<Employee *> employees = {new Employee{10, "Rahul", "Jain"},
                                         new Employee{5, "Kartik", "Rajan"},
                                         new Employee{-7, "John", ""},
                                         new Employee{11, "Mark", "Johnson"}};

    Db* db = open();

    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        char *value = (char*)malloc(sizeof(*employee));
        memcpy(value, (char*)employee, sizeof(*employee));

        db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    }

    auto updateEmployeeId10 = new EmployeeId{-7, "SomeRandomName"};
    char *updateKey10 = (char*)malloc(sizeof(*updateEmployeeId10));
    memcpy(updateKey10, (char*)updateEmployeeId10, sizeof(*updateEmployeeId10));

    auto updateEmployee = new Employee{-7, "John", "Mathews"};
    char *updateValue10 = (char*)malloc(sizeof(*updateEmployee));
    memcpy(updateValue10, (char*)updateEmployee, sizeof(*updateEmployee));

    db->update(Slice(updateKey10, sizeof(*updateEmployeeId10)), Slice(updateValue10, sizeof(*updateEmployee)));

    auto getId10 = new EmployeeId{-7, "OtherRandomName"};
    char *getKey10 = (char*)malloc(sizeof(*getId10));
    memcpy(getKey10, (char*)getId10, sizeof(*getId10));

    std::pair<const char*, bool> result = db->get(Slice(getKey10, sizeof(*getId10)));

    const Employee* first = reinterpret_cast<const Employee*>(result.first);
    ASSERT_EQ(-7, first->id);
    ASSERT_EQ("John", first->firstName);
    ASSERT_EQ("Mathews", first->lastName);

    close(db);
}


TEST(DbSerializedKeyIntegration, Delete) {
    std::vector<Employee *> employees = {new Employee{10, "Rahul", "Jain"},
                                         new Employee{5, "Kartik", "Rajan"},
                                         new Employee{-7, "John", ""},
                                         new Employee{11, "Mark", "Johnson"}};

    Db* db = open();

    for (auto employee : employees) {
        auto employeeId = new EmployeeId{employee->id, employee->lastName};

        char *key = (char*)malloc(sizeof(*employeeId));
        memcpy(key, (char*)employeeId, sizeof(*employeeId));

        char *value = (char*)malloc(sizeof(*employee));
        memcpy(value, (char*)employee, sizeof(*employee));

        db->put(Slice(key, sizeof(*employeeId)), Slice(value, sizeof(*employee)));
    }

    auto deleteEmployeeId = new EmployeeId{5, "SomeName"};
    char *deleteKey = (char*)malloc(sizeof(*deleteEmployeeId));
    memcpy(deleteKey, (char*)deleteEmployeeId, sizeof(*deleteEmployeeId));

    db->deleteBy(Slice(deleteKey, sizeof(*deleteEmployeeId)));

    auto getId5 = new EmployeeId{5, "SomeRandomName"};
    char *getKey5 = (char*)malloc(sizeof(*getId5));
    memcpy(getKey5, (char*)getId5, sizeof(*getId5));

    auto getId10 = new EmployeeId{10, "OtherRandomName"};
    char *getKey10 = (char*)malloc(sizeof(*getId10));
    memcpy(getKey10, (char*)getId10, sizeof(*getId10));

    auto getId11 = new EmployeeId{11, "OtherRandomName"};
    char *getKey11 = (char*)malloc(sizeof(*getId11));
    memcpy(getKey11, (char*)getId11, sizeof(*getId11));

    auto getId7 = new EmployeeId{-7, "OtherRandomName"};
    char *getKey7 = (char*)malloc(sizeof(*getId7));
    memcpy(getKey7, (char*)getId7, sizeof(*getId7));

    std::vector<Slice> multiGetKeys = {Slice(getKey5, sizeof(*getId5)), Slice(getKey7, sizeof(*getId7)), Slice(getKey10, sizeof(*getId10)), Slice(getKey11, sizeof(*getId11))};
    std::vector<std::pair<const char*, bool>> result = db->multiGet(multiGetKeys);

    ASSERT_EQ(4, result.size());

    const Employee* first = reinterpret_cast<const Employee*>(result.at(0).first);
    ASSERT_EQ(-7, first->id);
    ASSERT_EQ("John", first->firstName);
    ASSERT_EQ("", first->lastName);

    ASSERT_FALSE(result.at(1).second);

    const Employee* third = reinterpret_cast<const Employee*>(result.at(2).first);
    ASSERT_EQ(10, third->id);
    ASSERT_EQ("Rahul", third->firstName);
    ASSERT_EQ("Jain", third->lastName);

    const Employee* fourth = reinterpret_cast<const Employee*>(result.at(3).first);
    ASSERT_EQ(11, fourth->id);
    ASSERT_EQ("Mark", fourth->firstName);
    ASSERT_EQ("Johnson", fourth->lastName);

    close(db);
}
