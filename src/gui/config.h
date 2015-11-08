#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "input.h"

class Config {
public:
    Config();

    Input input() const;
    void saveInput(const Input &input);
};

#endif //_CONFIG_H_
