/*
 *
 */
#ifndef ma_maat_h
#define ma_maat_h

/* $$Version and authors */

/* Maat version components */
#define MA_MAJOR_VERSION  1
#define MA_MINOR_VERSION  0
#define MA_PATCH_VERSION  0

/* String form of the version */
#define MA_VERSION  MA_MAJOR_VERSION "." MA_MINOR_VERSION

/* Some patches after releasing Maat version $MA_VERSION$ */
#define MA_PATCH      MA_VERSION "." MA_PATCH_VERSION
#define MA_PATCH_NUM  ((10 * MA_MAJOR_VERSION + MA_MINOR_VERSION) * 100 + MA_PATCH_VERSION)

#define MA_COPYRIGHT  "Maat " MA_PATCH  " Copyright (C) 2023 Maat.cm, PanLab.africa"
#define MA_AUTHORS    "Kueppo Tcheukam J. W."



#endif
