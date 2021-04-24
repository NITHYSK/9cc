#ifndef __DBG_H__
#define __DBG_H__

//#define __DEBUG__

#ifdef __DEBUG__
#define dbg(...)                                                    \
	(printf("%s	%d	%s", __FILE__, __LINE__, __func__), \
	 printf("	"__VA_ARGS__), printf("\n"))
#define breakPoint() (printf("Break Point"), getchar(), printf("\e[1A\e[2K"))

#else
#define dbg(...)
#define breakPoint()

#endif

#endif
