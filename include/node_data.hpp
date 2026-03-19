#pragma once

#include <string>

enum DataType {
    SUBMISSION,
    REVIEWER,
    SOURCE,
    SINK
};

typedef struct DataNode {
    DataType type;
    int id;
    int primaryDomain;
    int secondaryDomain;
    std::string email;
    std::string nameTitle;
    std::string authors;
};