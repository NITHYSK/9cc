#ifndef __DBG_H__
#define __DBG_H__

#ifdef __DEBUG__
#define dbg(...)                                                    \
	(printf("# %s	%d	%s", __FILE__, __LINE__, __func__), \
	 printf("	"__VA_ARGS__), printf("\n"))
#else
#define dbg(...)
#endif

#ifdef __BREAK__
#define breakPoint() (printf("Break Point"), getchar(), printf("\e[1A\e[2K"))
#else
#define breakPoint()
#endif

#endif
