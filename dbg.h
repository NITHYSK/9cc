#ifndef __DBG_H__
#define __DBG_H__

#ifdef __DEBUG__
#define dbg(...)                                                    \
	(printf("%s	%d	%s", __FILE__, __LINE__, __func__), \
	 printf("	"__VA_ARGS__), printf("\n"))
#else
#define dbg(...)
#endif

#endif
