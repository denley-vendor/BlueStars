#ifndef _OAUTH_XMALLOC_H
#define _OAUTH_XMALLOC_H      1 

/* Prototypes for functions defined in xmalloc.c  */
extern void *xmalloc (size_t size);
extern void *xcalloc (size_t nmemb, size_t size);
extern void *xrealloc (void *ptr, size_t size);
extern char *xstrdup (const char *s);

#endif
