#ifndef COUT_DBG_H
#define COUT_DBG_H

#include <typeinfo>
#include <stdio.h>


// Configurartion [
#define _DBG_VAL_WIDE 1
#define DEBUG_VALUE_MY_HANDLER(...) log(__VA_ARGS__)
//#define SUPPORT_VARIADIC
// ] Configuration


#ifndef _DBG_VAL_WIDE
#define _DBG_VAL_WIDE 0
#else
#if (_DBG_VAL_WIDE != 0)
#undef _DBG_VAL_WIDE
#define _DBG_VAL_WIDE 1
#endif
#endif

// The debugging [
#if _DBG_VAL_WIDE == 1
#define _DBG_VAL_DO_WIDE_(s) L ## s
#define _DBG_VAL_DO_WIDE(s) _DBG_VAL_DO_WIDE_(s)
#define _DBG_VAL_BUFF_TYPE wchar_t
#define _DBG_VAL_FORMAT_STRING(BUFF, LEN, ...) swprintf(BUFF, LEN, __VA_ARGS__)
#define _DBG_VAL_Fn(F) w ## F
#else
#define _DBG_VAL_DO_WIDE(str) str
#define _DBG_VAL_BUFF_TYPE char
#define _DBG_VAL_FORMAT_STRING(BUFF, LEN, ...) sprintf_s(BUFF, LEN, __VA_ARGS__)
#define _DBG_VAL_Fn(F) F
#endif // _DBG_VAL_WIDE
#define _DBG_VAL_MSG_TYPE const _DBG_VAL_BUFF_TYPE*

#define _DBG_VAL_IF_0(T, F) F
#define _DBG_VAL_IF_1(T, F) T
#define _DBG_VAL_IF_X(C) _DBG_VAL_IF_ ## C
#define _DBG_VAL_IF(T, F, C) _DBG_VAL_IF_X(C)(T, F)

#define _DBG_VAL_BUFF_NAME __dbg__val__auto_generated

#define _DBG_VAL_PRINTF_NV(...) do { \
                                    _DBG_VAL_BUFF_TYPE _DBG_VAL_BUFF_NAME[256]; \
                                    _DBG_VAL_FORMAT_STRING(_DBG_VAL_BUFF_NAME, 256, __VA_ARGS__); \
                                    _DBG_VAL_HANDLER(_DBG_VAL_BUFF_NAME); \
                                } while(0)

#define _DBG_VAL_PRINTF_V(...) _DBG_VAL_HANDLER(__VA_ARGS__)

#ifdef SUPPORT_VARIADIC
#define _DBG_VAL_PRINTF _DBG_VAL_PRINTF_V
#else
#define _DBG_VAL_PRINTF _DBG_VAL_PRINTF_NV
#endif


#define _DBG_VAL_QTT(X) _DBG_VAL_DO_WIDE(#X)

#define _DBG_VAL_FMT(Fmt, PrintThis) _DBG_VAL_IF( \
                                        _DBG_VAL_DO_WIDE("this = %#010X (" ## _DBG_VAL_IF("%S", "%s", _DBG_VAL_WIDE) ## " ) in %s, line %d value of %s\t= " ## Fmt ## "\n"), \
                                        _DBG_VAL_DO_WIDE("In %s, line %d value of %s\t= " ## Fmt ## "\n"), \
                                        PrintThis \
                                      )

#define _DBG_VAL_ARGS(...) __VA_ARGS__

#define _DBG_VAL_DEBUG_EX(Expr, Fmt, Val, PrintThis) _DBG_VAL_PRINTF(_DBG_VAL_FMT(Fmt, PrintThis), \
                        _DBG_VAL_IF( \
                           _DBG_VAL_ARGS( \
                                reinterpret_cast<unsigned int>(this), typeid(*this).name(), _DBG_VAL_DO_WIDE(__FUNCSIG__), __LINE__, _DBG_VAL_QTT(Expr), Val \
                           ), \
                           _DBG_VAL_ARGS( \
                                _DBG_VAL_DO_WIDE(__FUNCSIG__), __LINE__, _DBG_VAL_QTT(Expr), Val \
                           ), \
                           PrintThis \
                          ) \
                    )

#define PRINT_VALUE(Expr, Fmt, Val)           _DBG_VAL_DEBUG_EX(Expr, Fmt, Val, 1)
#define PRINT_VALUE_STATIC(Expr, Fmt, Val)    _DBG_VAL_DEBUG_EX(Expr, Fmt, Val, 0)

#define PRINT_VALUE_INT(Expr)             PRINT_VALUE(Expr, "%d", Expr)
#define PRINT_VALUE_STATIC_INT(Expr)      PRINT_VALUE_STATIC(Expr, "%d", Expr)
#define PRINT_VALUE_STRING(String)        PRINT_VALUE(String, "%S", String . c_str())
#define PRINT_VALUE_STATIC_STRING(String) PRINT_VALUE_STATIC(String, "%S", String . c_str())

// ] The debugging 
#ifdef DEBUG_VALUE_MY_HANDLER
#define _DBG_VAL_HANDLER(...) DEBUG_VALUE_MY_HANDLER(__VA_ARGS__)
#else
#define _DBG_VAL_HANDLER(...) _DBG_VAL_Fn(printf)(__VA_ARGS__)
#endif // DEBUG_VALUE_MY_HANDLER


void log(_DBG_VAL_MSG_TYPE msg)
{
    _DBG_VAL_Fn(printf)(msg);
}

#endif // COUT_DBG_H