#pragma once

#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#define ANONYMOUS_VAR(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VAR(str) CONCATENATE(str, __LINE__)
#endif

#define LOG_STATIC_CLASS(T)          \
   public:                           \
    T()                    = delete; \
    T(const T&)            = delete; \
    T(const T&&)           = delete; \
    T& operator=(const T&) = delete; \
    T& operator=(T&&)      = delete;

#define LOG_SINGLETON(T)             \
   public:                           \
    T(const T&)            = delete; \
    T(const T&&)           = delete; \
    T& operator=(const T&) = delete; \
    T& operator=(T&&)      = delete; \
                                     \
    static T& instance() {           \
        static T s_instance;         \
        return s_instance;           \
    }                                \
                                     \
   private:                          \
    T() = default;                   \
                                     \
   public:
