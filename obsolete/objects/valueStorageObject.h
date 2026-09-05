//-----------------------------------------------------------------------------
// Copyright (c) 2026 Thomas Hühn (XXTH)
// SPDX-License-Identifier: MIT
//-----------------------------------------------------------------------------
// ElfScript ValueStorageObject
// like PointStorageObject but a list of ConsoleValues
//-----------------------------------------------------------------------------
#pragma once
#include "console/engineAPI.h"


class ValueStorageObject: public SimObject
{
    typedef SimObject Parent;
public:
    DECLARE_CONOBJECT(ValueStorageObject);
    Vector<ConsoleValue> mValues;

    static void initPersistFields();

    // ------------------------------------------------------------------------
    inline bool isIndexValid(S32 index) {
        return index >= 0 && index < mValues.size();
    }
    // ------------------------------------------------------------------------
    inline void setStorageSize(U32 size) {
        if ( size <= 1000000) {
            U32 oldSize = this->mValues.size();
            this->mValues.setSize(size);
            if (size > oldSize) {
                U32 newElements = size - oldSize;
                ConsoleValue* startOfNewData = this->mValues.address() + oldSize;
                dMemset(startOfNewData, 0, newElements * sizeof(ConsoleValue));
            }
        } else {
            Con::errorf("setStorageSize value too high! max: 1000000, send:%u", size);
        }
    }
};
