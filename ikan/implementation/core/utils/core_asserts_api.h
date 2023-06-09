//
//  core_assets_api.h
//  ikan
//
//  Created by Ashish . on 05/04/23.
//

#pragma once

#include "core/utils/asserts.h"

#define IK_CORE_ASSERT_NO_MESSAGE(condition) { \
if(!(condition)) { \
IK_CORE_ERROR("Assert", "Assertion Failed"); \
ASSERT(condition); \
} \
}

#define IK_CORE_ASSERT_MESSAGE(condition, ...) { \
if(!(condition)) { \
IK_CORE_ERROR("Assert", "Assertion Failed: {0}", __VA_ARGS__); \
ASSERT(condition); \
} \
}

#define IK_GET_CORE_ASSERT_MACRO(...) \
IK_EXPAND_VARGS( \
IK_ASSERT_RESOLVE(\
__VA_ARGS__, \
IK_CORE_ASSERT_MESSAGE, \
IK_CORE_ASSERT_NO_MESSAGE \
)\
)

#define IK_CORE_ASSERT(...)\
IK_EXPAND_VARGS( \
IK_GET_CORE_ASSERT_MACRO(__VA_ARGS__)(__VA_ARGS__) \
)

