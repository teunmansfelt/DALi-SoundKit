#ifndef DALI_CORE_DEBUG_PRINT_H_   
#define DALI_CORE_DEBUG_PRINT_H_   

#ifdef DALI_DEBUG_MODE
    #define DALI_LOG_LEVEL 2
#else
#ifdef DALI_OPTIMIZED_MODE
    #define DALI_LOG_LEVEL 0
#else
    #define DALI_LOG_LEVEL 1
#endif // DALI_OPTIMIZED_MODE
#endif // DALI_DEBUG_MODE

#if DALI_LOG_LEVEL > 0
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>

    // Terminal colour escape sequences.
    #define i_dali_core_colour_red    "\033[91m"
    #define i_dali_core_colour_orange "\033[93m"
    #define i_dali_core_colour_green  "\033[92m"
    #define i_dali_core_colour_blue   "\033[94m"
    #define i_dali_core_colour_purple "\033[35m"
    #define i_dali_core_colour_none   "\033[39m"

    __attribute__((format(printf, 5, 6)))
    static void i_dali_debug_print_internal(const char* label, const char* colour, const char* file, int line, 
                                        const char* fmt, ...) 
    {
        fprintf(stderr, "[%s%s"i_dali_core_colour_none"] at %s:%d: \n   ", colour, label, file, line);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }

    // Log macros
    #define ERROR(...)                                                                                                              \
        i_dali_debug_print_internal("ERROR",   i_dali_core_colour_red,    __FILE__, __LINE__, __VA_ARGS__); fputc('\n', stderr)
    #define WARN(...)  i_dali_debug_print_internal("WARNING", i_dali_core_colour_orange, __FILE__, __LINE__, __VA_ARGS__)
    #define PRINT(...) fprintf(stderr, "   "__VA_ARGS__); fputc('\n', stderr)
    #define PRINT_NL   fputc('\n', stderr)
    
    // Assert macros
    #define ASSERT(check, ...)                                                                                                      \
        if (!(check)) {                                                                                                             \
            i_dali_debug_print_internal("ASSERTION ("#check") failed", i_dali_core_colour_red, __FILE__, __LINE__, __VA_ARGS__);    \
            exit(EXIT_FAILURE);                                                                                                     \
        }
    #define UNREACHABLE(desc) ERROR("Unreachable! '"#desc"'"); exit(EXIT_FAILURE)
    #define NOT_IMPLEMENTED(func) WARN("'"#func"()' is not implemented yet")
    #define DEBUG_FUNCTION(func) WARN("Use of debug-function: '"#func"()'. Should be removed!")
#else
    #define ERROR(...)
    #define WARN(...)
    #define PRINT(...)
    #define PRINT_NL

    #define ASSERT(check, ...)
    #define UNREACHABLE(desc)
    #define NOT_IMPLEMENTED(func)
    #define TEST_FUNCTION(func)    
#endif // DALI_LOG_LEVEL > 0

#if DALI_LOG_LEVEL > 1
    __attribute__((format(printf, 3, 4)))
    static void i_dali_trace_print_internal(const char* file, int line, const char* fmt, ...) 
    {
        fprintf(stderr, "[Trace] (%s:%d): ", file, line);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }

    // Log macros
    #define INFO(...)  i_dali_debug_print_internal("INFO",  i_utils_colour_green, __FILE__, __LINE__, __VA_ARGS__)
    #define DEBUG(...) i_dali_debug_print_internal("Debug", i_utils_colour_none,  __FILE__, __LINE__, __VA_ARGS__)
    #define TRACE(...) i_dali_trace_print_internal(__FILE__, __LINE__, __VA_ARGS__);
    #define TODO(...)  i_dali_debug_print_internal("TODO",  i_utils_colour_blue,  __FILE__, __LINE__, __VA_ARGS__)
#else
    #define INFO(...)
    #define DEBUG(...)
    #define TRACE(...)
    #define TODO(...)
#endif // DALI_LOG_LEVEL > 1

#endif // DALI_CORE_DEBUG_PRINT_