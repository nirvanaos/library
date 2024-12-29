#ifdef _MSC_VER
#define NIRVANA_MOCK_EXPORT __declspec(dllexport)
#else
#define NIRVANA_MOCK_EXPORT __attribute__ ((visibility ("default")))
#endif
