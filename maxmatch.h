/*
  Last changed Time-stamp: <2006-05-29 23:22:43 xtof>
  $Id: maxmatch.h,v 1.2 2006/08/04 23:49:23 Kinwalker Exp $
*/
#ifndef _MAXMATCH_H_
#define _MAXMATCH_H_

#if __cplusplus
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS   }
#else
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif

BEGIN_C_DECLS

float max_matching(char *string, char *structure);
void init_maxmat(int length, int minls, int minss);
void update_mm_params(void);
void free_mm_arrays(int length);
short*** get_mu_array(void);
void mm_fill_arrays(char *string, char *structure);
double mm_get_ratio(int i, int j);
double mm_get_energy(int i, int j);
char* mm_backtrack_structure(int i, int j);
void mm_print_matrix(short ***m, char *string);

END_C_DECLS

#endif /* _MAXMATCH_H_ */
