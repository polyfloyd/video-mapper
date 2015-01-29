#ifndef _COMMON_HH
#define _COMMON_HH

#ifdef _RELEASE
	#define NDEBUG
#endif

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdint>

#include <exception>
#include <functional>
#include <memory>
#include <regex>
#include <string>

#include <array>
#include <list>
#include <map>
#include <queue>
#include <vector>

#ifdef _RELEASE
	#define logf(...)
	#define debugf(...)
	#define warnf(...)
#else
	#define logf(...) {\
		fprintf(stderr, "[log] ");\
		fprintf(stderr, __VA_ARGS__);\
		fprintf(stderr, "\n");\
	}

	#define debugf(...) {\
		fprintf(stderr, "\x1B[32m[debug]\x1B[37m %s:%d: ", __FILE__, __LINE__);\
		fprintf(stderr, __VA_ARGS__);\
		fprintf(stderr, "\x1B[0m\n");\
	}

	#define warnf(...) {\
		fprintf(stderr, "\x1B[33m[warn]\x1B[37m %s:%d: ", __FILE__, __LINE__);\
		fprintf(stderr, __VA_ARGS__);\
		fprintf(stderr, "\x1B[0m\n");\
	}
#endif

#define fatalf(...) {\
	fprintf(stderr, "\x1B[31m[fatal]\x1B[37m %s:%d: ", __FILE__, __LINE__);\
	fprintf(stderr, __VA_ARGS__);\
	fprintf(stderr, "\x1B[0m\n");\
	exit(1);\
}

#endif /* _COMMON_HH */
