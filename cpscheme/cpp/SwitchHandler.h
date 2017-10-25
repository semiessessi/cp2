// Copyright (c) 2014 Cranium Software

#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H

struct SwitchHandlerInitialiser
{
    const char* const szSwitch;
    bool bParameter;
    int ( *pfnHandlerFunction )( const char* const );
};

#endif
