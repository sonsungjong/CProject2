#ifdef MAKEDLL_EXPORTS
#define MAKEDLL_API __declspec(dllexport)
#else
#define MAKEDLL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class MAKEDLL_API CMakeDll {
public:
	CMakeDll(void);
	// TODO: add your methods here.
	int TenTimeNum(int num);
};

extern MAKEDLL_API int nMakeDll;

MAKEDLL_API int fnMakeDll(void);

extern "C" MAKEDLL_API int sub(int x1, int y1);