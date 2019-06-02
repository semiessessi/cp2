// Copyright (c) 2019 Cranium Software

#ifndef PASS_H
#define PASS_H

#include <string>

namespace CP2
{

namespace Compiler
{

class Pass
{

public:

    Pass()
    : mbIsSwitch( false )
    {

    }

    Pass( const Pass& xPass )
    : mbIsSwitch( xPass.mbIsSwitch )
    {

    }

    Pass& operator =( const Pass& xPass )
    {
        mbIsSwitch = xPass.mbIsSwitch;
        return *this;
    }

    void SetSwitch( const bool bSwitchValue )
    {
        mbIsSwitch = bSwitchValue;
    }

private:

    bool mbIsSwitch;

};

}
}

#endif
