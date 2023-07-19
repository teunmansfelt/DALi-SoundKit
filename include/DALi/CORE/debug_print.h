#ifndef dali_DEBUG_PRINT_H_   
#define dali_DEBUG_PRINT_H_   

#ifndef DALI_LOG_LEVEL
    #ifdef DALI_DEBUG_MODE
        #define DALI_LOG_LEVEL 2
    #else
    #ifdef DALI_STRIPPED_MODE
        #define DALI_LOG_LEVEL 0
    #else
        #define DALI_LOG_LEVEL 1
    #endif // DALI_STRIPPED_MODE
    #endif // DALI_DEBUG_MODE
#endif // DALI_LOG_LEVEL

#if DALI_LOG_LEVEL > 0
    #include <stdarg.h>
    #include <stdio.h>
    #include <stdlib.h>

    // Terminal colour escape sequences.
    #define internal_dali_colour_red    "\033[91m"
    #define internal_dali_colour_orange "\033[93m"
    #define internal_dali_colour_green  "\033[92m"
    #define internal_dali_colour_blue   "\033[94m"
    #define internal_dali_colour_purple "\033[35m"
    #define internal_dali_colour_none   "\033[39m"

    __attribute__((format(printf, 5, 6), unused))
    static void internal_dali_debug_print(const char* label, const char* colour, const char* file, int line, 
                                          const char* fmt, ...) 
    {
        fprintf(stderr, "[%s%s"internal_dali_colour_none"] at %s:%d: \n   ", colour, label, file, line);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }

    // Log macros
    #define PRINT_NL   fputc('\n', stderr)
    #define ERROR(...) internal_dali_debug_print("ERROR", internal_dali_colour_red, __FILE__, __LINE__, __VA_ARGS__); PRINT_NL
    #define WARN(...)  internal_dali_debug_print("WARNING", internal_dali_colour_orange, __FILE__, __LINE__, __VA_ARGS__)

    // Assert macros
    #define ASSERT(check, ...)                                                                                                      \
        if (!(check)) {                                                                                                             \
            internal_dali_debug_print("ASSERTION ("#check") failed", internal_dali_colour_red, __FILE__, __LINE__, __VA_ARGS__);    \
            abort();                                                                                                                \
        }
    #define UNREACHABLE(desc)     ERROR("Unreachable code! : '"#desc"'")
    #define NOT_IMPLEMENTED(func) WARN("'"#func"' is not implemented yet!")
#else
    #define PRINT_NL
    #define ERROR(...) 
    #define WARN(...) 

    #define ASSERT(check, ...)
    #define UNREACHABLE(desc)
    #define NOT_IMPLEMENTED(func) 
#endif // DALI_LOG_LEVEL > 0

#if DALI_LOG_LEVEL > 1
    __attribute__((format(printf, 3, 4), unused))
    static void internal_dali_trace(const char* file, int line, const char* fmt, ...) 
    {
        fprintf(stderr, "[trace] (%s:%d): ", file, line);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }

    #define INFO(...)  internal_dali_debug_print("INFO", internal_dali_colour_green, __FILE__, __LINE__, __VA_ARGS__)
    #define DEBUG(...) internal_dali_debug_print("debug", internal_dali_colour_none, __FILE__, __LINE__, __VA_ARGS__)
    #define TRACE(...) internal_dali_trace(__FILE__, __LINE__, __VA_ARGS__)
    #define TODO(...)  internal_dali_debug_print("TODO", internal_dali_colour_blue, __FILE__, __LINE__, __VA_ARGS__)
    #define PRINT(...) fprintf(stderr, "   "__VA_ARGS__); PRINT_NL
#else
    #define INFO(...)  
    #define DEBUG(...) 
    #define TRACE(...)
    #define TODO(...) 
    #define PRINT(...) 
#endif //DALI_LOG_LEVEL > 1

#endif // dali_DEBUG_PRINT_

