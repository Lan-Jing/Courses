#ifndef STRUCT_H
#define STRUCT_H

#define USER_NAME_LEN 100
#define EMAIL_LEN     100

struct Person {
    char username[USER_NAME_LEN];
    int level;
    char email[EMAIL_LEN];
    time_t sendtime;
    time_t regtime;
};

#endif