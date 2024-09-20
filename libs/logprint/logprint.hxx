#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT extern "C" __declspec(dllexport)
#elif defined(__linux__)
#define LIB_EXPORT extern "C" __attribute__((visibility("default")))
#elif defined(__APPLE__)
#define LIB_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define LIB_EXPORT extern "C"
#endif

extern "C"
{
    LIB_EXPORT void LOGE(const char *tag, const char *message, ...);
    LIB_EXPORT void LOGI(const char *tag, const char *message, ...);
}
