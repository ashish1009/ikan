//
//  ikan_common.h
//  ikan
//
//  Created by Ashish . on 27/03/23.
//

#pragma once

namespace ikan {
  
  /// This Emum defines the supported Operating system for ikan Engine
  enum class OperatingSystem : uint8_t {
    None = 0, Mac = 1
    // Add More supported OS
  };
  
  /// This enum defines the axis names
  enum Axis : uint8_t { X, Y, Z };
  
}

// ----------------------------------
// Multi thread Dispatcher queue
// ----------------------------------
static dispatch_queue_t loop_dispactch_queue_ = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

// ----------------------------------
// Common Macros
// ----------------------------------
/// This MACRO Shifts the LSB bit of number 1 at position x
#define BIT(x)          (1 << x)
/// This MACRO Cast uint32_t into void pointer
#define INT2VOIDP(i)    (void*)(uintptr_t)(i)

/// These MACRO defines the min and max value of float
#define MIN_FLT         (float)INT_MIN
#define MAX_FLT         (float)INT_MAX

/// This MACRO return the function if condition x is true
#define RETURN_IF(condition) \
  if(condition) \
    return;

/// This MACRO check the widget flag
#define CHECK_WIDGET_FLAG(x) RETURN_IF(x and *x == false);

// --------------------------------------------------------
// Macro Wrapper to define Enums and its String to print
// --------------------------------------------------------
#define VAL(name) name ,
#define STRING(name) #name,

#define CreateEnum(Name) \
enum class Name { Name(VAL) }; \
static const char* Name##String[] = { Name(STRING) }; \

// -------------------------------------------
// Copy and Move Constructors and Operators
// -------------------------------------------
/// This MACRO Deletes the Constructors (Default, Copy and Move ) and Operator = (Copy and Move) for any class to make pure static class
#define MAKE_PURE_STATIC(x) \
x() = delete; \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator =(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO deletes the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x to make a class singleton.
/// NOTE: default constructor still need to be private
#define DELETE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = delete; \
x(x&&) = delete; \
x& operator=(const x&) = delete; \
x& operator =(x&&) = delete; \

/// This MACRO Defines the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x
#define DEFINE_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&); \
x(x&&); \
x& operator=(const x&); \
x& operator =(x&&); \

/// This MACRO Defines the Constructors (Copy and Move ) and Operator = (Copy and Move) for any class x
#define DEFAULT_COPY_MOVE_CONSTRUCTORS(x) \
x(const x&) = default; \
x(x&&) = default; \
x& operator=(const x&) = default; \
x& operator =(x&&) = default; \
