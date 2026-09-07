//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ElfScript Array
//-----------------------------------------------------------------------------
#pragma once

#include "console/engineAPI.h"
#include "console/console.h"
#include "console/enginePrimitives.h"
#include "console/dynamicTypes.h"



class Array: public SimObject
{
    typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(Array);
    Vector<ConsoleValue> mValues;

    bool onAdd() override;
    void write(Stream &stream, U32 tabStop, U32 flags) override;
    void toFields(); // save array to _p fields
    void fromFields(bool removeFields = true); // load array from _p fields
    // ------------------------------------------------------------------------
    S32 fromString(const char* text, bool doAdd = true);
    // ------------------------------------------------------------------------
    inline bool isIndexValid(S32 index) {
        return index >= 0 && index < mValues.size();
    }

};
