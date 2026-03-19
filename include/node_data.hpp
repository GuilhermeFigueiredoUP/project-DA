#pragma once

#include <string>

enum DataType {
    SUBMISSION,
    REVIEWER,
    SOURCE,
    SINK
};

struct DataNode {
    DataType type;
    int id;
    int primaryDomain;
    int secondaryDomain;
    std::string email;
    std::string nameTitle;
    std::string authors;

    bool operator==(DataNode other) {
        if(id != other.id) return false;
        return true;
    }
};