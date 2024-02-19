/* bb_prototypes.h */

#define BB_SIZE	300

/* in file bb_init.c */

extern int32 bb_init();

/* in file bb_start.c */

extern int32 bb_start();

/* in file bb_insert.c */

extern int32 bb_insert(byte b);

/* in file bb_extract.c */

extern int32 bb_extract();

/* in file bb_endinsert.c */

extern int32 bb_endinsert();

/* in file bb_endextract.c */

extern int32 bb_endextract();

/* in file bb_abort.c */

extern int32 bb_abort();

