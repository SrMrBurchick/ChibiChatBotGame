#pragma once

enum class EOutputType {
    eBool = 0,
    eNumber,
    eString,
    eUnknown
};

enum class EConditionOperators {
    eEqual = 0,
    eGreater,
    eLower,
    eNotEqual,
    eUnknown
};

enum class EExecutionType {
    eRandom = 0,
    eSequence,
    eUnknown
};
