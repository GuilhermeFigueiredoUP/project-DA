#pragma once

#include <string>

enum DataType {
    SUBMISSION,
    REVIEWER
};

struct DataNode {
    DataType type;
    int id;
    int primaryDomain;
    int secondaryDomain;
    std::string email;
    std::string nameTitle;
    std::string authors;
    bool operator==(const DataNode& other) const { return this->id == other.id; }
};