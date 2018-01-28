#if !defined(_XDS_)
#define _XDS_

#ifdef __cplusplus
extern "C" {
#endif

#define INT16   short int
#define UINT16  unsigned short int

void desinit( INT16 mode );
void desdone( void );
void setkey( char *key );

void endes( char *block );
void dedes( char *block );

#ifdef __cplusplus
 }
#endif

#endif


