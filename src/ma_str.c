/*
 * $$$Maat string manipulation functions.
 * License: AGL, see LICENSE file for details.
 */

#ifndef ma_str_h
#define ma_str_h

#include "ma_val.h"

#define REV_BIT  (0b1 << 7)

/* Set length of the long/short string 's' */
#define slgnlen(s, l)  (s->u.len = l)
#define sshtlen(s, l)  (s->sl = (s->sl & REV_BIT) | l)

/* Get length of the long/short string 's' */
#define lnglen(s)  (s->u.len)
#define shtlen(s)  (s->sl & SHT_LEN_BITS)

#define is_sht(s)     (check_rtype(s, O_VSHTSTR) || check_rtype(s, O_VUSHTSTR))
#define len(s)        (is_sht(s) ? shtlen(s) : lnglen(s))
#define setlen(s, l)  (is_sht(s) ? sshtlen(s, l) : slnglen(s, l))

/* Is short string reserved? Set reserved bit. */
#define is_rev(s)      (is_sht(s) && (s->sl & REV_BIT))
#define set_revbit(s)  (ma_assert(is_sht(s)), s->sl |= REV_BIT)

/* Get next string for hash map of short strings ($$Smap). */
#define next_str(s)  (s->u.snext)

#define is_lng(s)  (check_rtype(s, O_VLNGSTR) || check_rtype(s, O_VLNG2STR))

/* Hash of short strings */
#define hashval(s)    (s->hash)
#define is_hashed(s)  (is_lng(s) && s->sl == 1)
#define markhash(s)   (ma_assert(is_lng(s)), s->sl = 1)

/* Says 's' should be an U8Str, true? */
#define is_u8s(s)  ma_assert(check_type(s, O_U8STR))

/* Get visual length of 's'. */
#define vlen(s)  (is_u8s(s) ? (len(s) - a2u8(s)->ngraph) : len(s))

#endif
