#if !defined( SELECTORMAP_INCLUDED )  /* Include this file only once */
#define SELECTORMAP_INCLUDED
/*
*+
*  Name:
*     selectormap.h

*  Type:
*     C include file.

*  Purpose:
*     Define the interface to the SelectorMap class.

*  Invocation:
*     #include "selectormap.h"

*  Description:
*     This include file defines the interface to the SelectorMap class and
*     provides the type definitions, function prototypes and macros,
*     etc. needed to use this class.

*  Inheritance:
*     The SelectorMap class inherits from the Mapping class.

*  Attributes Over-Ridden:
*     None.

*  New Attributes Defined:
*     None.

*  Methods Over-Ridden:
*     Public:
*        None.
*
*     Protected:
*        astMapMerge
*           Merge a SelectorMap within a sequence of Mappings.
*        astTransform
*           Transform a set of points.

*  New Methods Defined:
*     Public:
*        None.
*
*     Protected:
*        None.

*  Other Class Functions:
*     Public:
*        astIsASelectorMap
*           Test class membership.
*        astSelectorMap
*           Create a SelectorMap.
*
*     Protected:
*        astCheckSelectorMap
*           Validate class membership.
*        astInitSelectorMap
*           Initialise a SelectorMap.
*        astInitSelectorMapVtab
*           Initialise the virtual function table for the SelectorMap class.
*        astLoadSelectorMap
*           Load a SelectorMap.

*  Macros:
*     None.

*  Type Definitions:
*     Public:
*        AstSelectorMap
*           SelectorMap object type.
*
*     Protected:
*        AstSelectorMapVtab
*           SelectorMap virtual function table type.

*  Feature Test Macros:
*     astCLASS
*        If the astCLASS macro is undefined, only public symbols are
*        made available, otherwise protected symbols (for use in other
*        class implementations) are defined. This macro also affects
*        the reporting of error context information, which is only
*        provided for external calls to the AST library.

*  Copyright:
*     <COPYRIGHT_STATEMENT>

*  Authors:
*     DSB: David S. Berry (Starlink)

*  History:
*     13-MAR-2006 (DSB):
*        Original version.
*-
*/

/* Include files. */
/* ============== */
/* Interface definitions. */
/* ---------------------- */
#include "mapping.h"             /* Coordinate Mappings (parent class) */
#include "region.h"              /* Coordinate Regions (parent class) */

#if defined(astCLASS)            /* Protected */
#include "pointset.h"            /* Sets of points/coordinates */
#include "channel.h"             /* I/O channels */
#endif

/* C header files. */
/* --------------- */
#if defined(astCLASS)            /* Protected */
#include <stddef.h>
#endif

/* Type Definitions. */
/* ================= */
/* SelectorMap structure. */
/* ----------------- */
/* This structure contains all information that is unique to each
   object in the class (e.g. its instance variables). */
typedef struct AstSelectorMap {

/* Attributes inherited from the parent class. */
   AstMapping mapping;       /* Parent class structure */

/* Attributes specific to objects in this class. */
   int nreg;                /* The number of Regions in the SelectorMap */
   AstRegion **reg;         /* Array of Region pointers */

} AstSelectorMap;

/* Virtual function table. */
/* ----------------------- */
/* This table contains all information that is the same for all
   objects in the class (e.g. pointers to its virtual functions). */
#if defined(astCLASS)            /* Protected */
typedef struct AstSelectorMapVtab {

/* Properties (e.g. methods) inherited from the parent class. */
   AstMappingVtab mapping_vtab;  /* Parent class virtual function table */

/* Unique flag value to determine class membership. */
   int *check;                   /* Check value */

/* Properties (e.g. methods) specific to this class. */
/* None. */
} AstSelectorMapVtab;
#endif

/* Function prototypes. */
/* ==================== */
/* Prototypes for standard class functions. */
/* ---------------------------------------- */
astPROTO_CHECK(SelectorMap)           /* Check class membership */
astPROTO_ISA(SelectorMap)             /* Test class membership */

/* Constructor. */
#if defined(astCLASS)            /* Protected. */
AstSelectorMap *astSelectorMap_( int, void **, const char *, ... );
#else
AstSelectorMap *astSelectorMapId_( int, void **, const char *, ... );
#endif

#if defined(astCLASS)            /* Protected */

/* Initialiser. */
AstSelectorMap *astInitSelectorMap_( void *, size_t, int, AstSelectorMapVtab *,
                                 const char *, int, AstRegion ** );

/* Vtab initialiser. */
void astInitSelectorMapVtab_( AstSelectorMapVtab *, const char * );

/* Loader. */
AstSelectorMap *astLoadSelectorMap_( void *, size_t, AstSelectorMapVtab *,
                                     const char *, AstChannel * );
#endif

/* Prototypes for member functions. */
/* -------------------------------- */
/* None. */

/* Function interfaces. */
/* ==================== */
/* These macros are wrap-ups for the functions defined by this class
   to make them easier to invoke (e.g. to avoid type mis-matches when
   passing pointers to objects from derived classes). */

/* Interfaces to standard class functions. */
/* --------------------------------------- */
/* Some of these functions provide validation, so we cannot use them
   to validate their own arguments. We must use a cast when passing
   object pointers (so that they can accept objects from derived
   classes). */

/* Check class membership. */
#define astCheckSelectorMap(this) astINVOKE_CHECK(SelectorMap,this)

/* Test class membership. */
#define astIsASelectorMap(this) astINVOKE_ISA(SelectorMap,this)

/* Constructor. */
#if defined(astCLASS)            /* Protected. */
#define astSelectorMap astINVOKE(F,astSelectorMap_)
#else
#define astSelectorMap astINVOKE(F,astSelectorMapId_)
#endif

#if defined(astCLASS)            /* Protected */

/* Initialiser. */
#define astInitSelectorMap(mem,size,init,vtab,name,nreg,regs) \
astINVOKE(O,astInitSelectorMap_(mem,size,init,vtab,name,nreg,regs))

/* Vtab Initialiser. */
#define astInitSelectorMapVtab(vtab,name) astINVOKE(V,astInitSelectorMapVtab_(vtab,name))
/* Loader. */
#define astLoadSelectorMap(mem,size,vtab,name,channel) \
astINVOKE(O,astLoadSelectorMap_(mem,size,vtab,name,astCheckChannel(channel)))
#endif

/* Interfaces to public member functions. */
/* -------------------------------------- */
/* Here we make use of astCheckSelectorMap to validate SelectorMap pointers
   before use.  This provides a contextual error report if a pointer
   to the wrong sort of Object is supplied. */
/* None. */
#endif
