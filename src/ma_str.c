/*
 * ###
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_str_h
#define ma_str_h

#include "ma_val.h"

#define SHT_LEN_BITS  0x7F

/* Set length */
#define slgnlen(s, rl)  (s->u.len = rl)
#define sshtlen(s, rl)  (s->sl = (s->sl & ~SHT_LEN_BITS) | rl)

/* Get length */
#define lnglen(s)  (s->u.len)
#define shtlen(s)  (s->sl & SHT_LEN_BITS)

#define is_sht(s)    (co_type(s, O_VSHTSTR) || co_type(s, O_VUSHTSTR))
#define len(s)       (is_sht(s) ? shtlen(s) : lnglen(s))
#define slen(s, rl)  (is_sht(s) ? sshtlen(s, rl) : slnglen(s, rl))

#define SL_MSB  0x80

/* Is short string reserved? Set reserved bit. */
#define is_rev(s)      (is_sht(s) && (s->sl & SL_MSB))
#define set_revbit(s)  (ma_assert(is_sht(s)), s->sl |= ~SL_MSB)

/* Says 's' should be an U8Str, true? */
#define a_isus(s)  ma_assert(o_check)

/* Get and set visual length of 's'. */
#define svlen(s, vl)  (a_iss2(s), (s->vlen = vl))
#define vlen(s)       (a_iss2(s), s->vlen)

/* Is vlen cached? invalidate cache, validate cache. */
#define is_cached(s2)         (a_iss2(s) && s2->cache == 1)
#define cache_vl(s2, bool_v)  (a_iss2(s), s2->cache = bool_v)
#define in_cache(s2)          cache_vl(s, 0)
#define va_cache(s2)          cache_vl(s, 1)

/* Get next string for hash map of short strings (##Smap). */
#define next_str(s)  (s->u.snext)

#define is_lng(s)   (s->type == O_VLNGSTR || s->type == O_VLNG2STR)
#define a_islng(s)  ma_assert(is_lng(s))

#define hval(s)       (a_islng(s), s->hash)
#define mark_hash(s)  (a_islng(s), s->slen = 1)
#define is_hashed(s)  (is_lng(s) && s->slen == 1)

union Sunion {
   Str as;
   U8Str us;
} Sunion;

#endif
