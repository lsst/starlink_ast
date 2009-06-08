/*
*class++
*  Name:
*     StcsChan

*  Purpose:
*     I/O Channel using STC-S to represent Objects.

*  Constructor Function:
c     astStcsChan
f     AST_STCSCHAN

*  Description:
*     A StcsChan is a specialised form of Channel which supports STC-S
*     I/O operations. Writing an Object to an StcsChan (using
c     astWrite) will, if the Object is suitable, generate an
f     AST_WRITE) will, if the Object is suitable, generate an
*     STC-S description of that Object, and reading from an StcsChan will 
*     create a new Object from its STC-S description.
*
*     When an STC-S description is read using
c     astRead,
f     AST_READ,
*     the returned AST Object may be 1) a PointList describing the STC
*     AstroCoords (i.e. a single point of interest within the coordinate frame
*     described by the STC-S description), or 2) a Region describing the STC
*     AstrCoordsArea (i.e. an area or volume of interest within the coordinate 
*     frame described by the STC-S description), or 3) a KeyMap
*     containing the uninterpreted property values read form the STC-S
*     description, or 4) a KeyMap containing any combination of the first
*     3 options. The attributes StcsArea, StcsCoords and StcsProps
*     control which of the above is returned by 
c     astRead.
f     AST_READ.
*
*     When an STC-S description is created from an AST Object using 
c     astWrite,
f     AST_WRITE,
*     the AST Object must be either a Region or a KeyMap. If it is a
*     Region, it is assumed to define the AstroCoordsArea or (if the
*     Region is a single point) the AstroCoords to write to the STC-S
*     description. If the Object is a KeyMap, it may contain an entry 
*     with the key "AREA", holding a Region to be used to define the
*     AstroCoordsArea. It may also contain an entry with the key "COORDS",
*     holding a Region (a PointList) to be used to create the
*     AstroCoords. It may also contain an entry with key "PROPS", holding
*     a KeyMap that contains uninterpreted property values to be used as 
*     defaults for any STC-S properties that are not determined by the
*     other supplied Regions. In addition, a KeyMap supplied to
c     astWrite
f     AST_WRITE
*     may itself hold the default STC-S properties (rather than defaults
*     being held in a secondary KeyMap, stored as the "PROPS" entry in the 
*     supplied KeyMap).
*
*     The 
c     astRead and astWrite
f     AST_READ and AST_WRITE
*     functions work together so that any Object returned by 
c     astRead can immediately be re-written using astWrite.
f     AST_READ can immediately be re-written using AST_WRITE.
*
*     Normally, when you use an StcsChan, you should provide "source"
c     and "sink" functions which connect it to an external data store
c     by reading and writing the resulting text. These functions
f     and "sink" routines which connect it to an external data store
f     by reading and writing the resulting text. These routines
*     should perform any conversions needed between external character 
c     encodings and the internal ASCII encoding. If no such functions 
f     encodings and the internal ASCII encoding. If no such routines
*     are supplied, a Channel will read from standard input and write 
*     to standard output.

*     Support for STC-S is currently based on the IVOA document "STC-S: 
*     Space-Time Coordinate (STC) Metadata Linear String Implementation", 
*     version 1.30 (dated 5th December 2007), available at
*     http://www.ivoa.net/Documents/latest/STC-S.html. Note, this
*     document is a recommednation only and does not constitute an accepted 
*     IVOA standard. 
*
*     The full text of version 1.30 is supported by the StcsChan class, 
*     with the following exceptions and provisos:
*
*     - There is no support for multiple intervals specified within a 
*     TimeInterval, PositionInterval, SpectralInterval or RedshiftInterval.
*     - If the ET timescale is specified, TT is used instead.
*     - If the TEB timescale is specified, TDB is used instead.
*     - The LOCAL timescale is not supported.
*     - The AST TimeFrame and SkyFrame classes do not currently allow a 
*     reference position to be specified. Consequently, any <refpos> 
*     specified within the Time or Space sub-phrase of an STC-S document 
*     is ignored.
*     - The Convex identifier for the space sub-phrase is not supported.
*     - The GEO_C and GEO_D space frames are not supported.
*     - The UNITSPHERE and SPHER3 space flavours are not supported.
*     - If any Error values are supplied in a space sub-phrase, then the
*     number of values supplied should equal the number of spatial axes, 
*     and the values are assumed to specify an error box (i.e. error
*     circles, ellipses, etc, are not supported).
*     - The spectral and redshift sub-phrases do not support the
*     following <refpos> values: LOCAL_GROUP_CENTER, UNKNOWNRefPos,
*     EMBARYCENTER, MOON, MERCURY, VENUS, MARS, JUPITER, SATURN, URANUS,
*     NEPTUNE, PLUTO.
*     - Error values are supported but error ranges are not.
*     - Resolution, PixSize and Size values are ignored.
*     - Space velocity sub-phrases are ignored.

*  Inheritance:
*     The StcsChan class inherits from the Channel class.

*  Attributes:
*     In addition to those attributes common to all Channels, every
*     StcsChan also has the following attributes:
*
*     - StcsArea: Return the CoordinateArea component after reading an STC-S?
*     - StcsCoords: Return the Coordinates component after reading an STC-S?
*     - StcsProps: Return the STC-S properties after reading an STC-S?

*  Functions:
c     The StcsChan class does not define any new functions beyond those
f     The StcsChan class does not define any new routines beyond those
*     which are applicable to all Channels.

*  Copyright:
*     Copyright (C) 2009 Science & Technology Facilities Council.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public Licence as
*     published by the Free Software Foundation; either version 2 of
*     the Licence, or (at your option) any later version.
*     
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public Licence for more details.
*     
*     You should have received a copy of the GNU General Public Licence
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 59 Temple Place,Suite 330, Boston, MA
*     02111-1307, USA

*  Authors:
*     DSB: David Berry (Starlink)

*  History:
*     18-DEC-2008 (DSB):
*        Original version.
*     22-MAY-2008 (DSB):
*        Retain default Equinox values in SkyFrame when reading an STC-S.
*class--
*/

/* Module Macros. */
/* ============== */
/* Set the name of the class we are implementing. This indicates to
   the header files that define class interfaces that they should make
   "protected" symbols available. */
#define astCLASS StcsChan

/* Values identifying particular forms of CoordArea */
#define NULL_ID                1
#define TIME_INTERVAL_ID       2
#define START_TIME_ID          3
#define STOP_TIME_ID           4
#define POSITION_INTERVAL_ID   5
#define ALL_SKY_ID             6
#define CIRCLE_ID              7
#define ELLIPSE_ID             8
#define BOX_ID                 9
#define POLYGON_ID            10
#define CONVEX_ID             11
#define POSITION_ID           12
#define TIME_ID               13
#define SPECTRAL_INTERVAL_ID  14
#define SPECTRAL_ID           15
#define REDSHIFT_INTERVAL_ID  16
#define REDSHIFT_ID           17
#define VELOCITY_INTERVAL_ID  18

/* The number of words used to form an extract from an STC-S description
   for use in an error message. */
#define NEWORD 10
 
/* Max length of string returned by GetAttrib */
#define GETATTRIB_BUFF_LEN 50    

/* Include files. */
/* ============== */
/* Interface definitions. */
/* ---------------------- */

#include "frame.h"               /* Generic cartesian coordinate systems */
#include "globals.h"             /* Thread-safe global data access */
#include "error.h"               /* Error reporting facilities */
#include "memory.h"              /* Memory allocation facilities */
#include "object.h"              /* Base Object class */
#include "channel.h"             /* Interface for parent class */
#include "stcschan.h"            /* Interface definition for this class */
#include "loader.h"              /* Interface to the global loader */
#include "skyframe.h"            /* Celestial coordinate systems */
#include "timeframe.h"           /* Time coordinate systems */
#include "specframe.h"           /* Spectral coordinate systems */
#include "wcsmap.h"              /* PI-related constants */
#include "region.h"              /* Abstract regions */
#include "interval.h"            /* Axis intervals */
#include "unitmap.h"             /* Unit mappings */
#include "nullregion.h"          /* Boundless regions */
#include "box.h"                 /* Box regions */
#include "prism.h"               /* Prism regions */
#include "circle.h"              /* Circle regions */
#include "ellipse.h"             /* Ellipse regions */
#include "polygon.h"             /* Polygon regions */
#include "pointlist.h"           /* Lists of points */
#include "keymap.h"              /* KeyMap interface */


/* Error code definitions. */
/* ----------------------- */
#include "ast_err.h"             /* AST error codes */

/* C header files. */
/* --------------- */
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Module Types. */
/* ============= */
typedef struct WordContext {
   char *line;
   char *e;
   int done;
   char *words[ NEWORD ];
   int next;
} WordContext;

/* Module Variables. */
/* ================= */

/* Pointers to parent class methods which are extended by this class. */
static const char *(* parent_getattrib)( AstObject *, const char *, int * );
static int (* parent_testattrib)( AstObject *, const char *, int * );
static void (* parent_clearattrib)( AstObject *, const char *, int * );
static void (* parent_setattrib)( AstObject *, const char *, int * );

/* Address of this static variable is used as a unique identifier for
   member of this class. */
static int class_check;

/* Define macros for accessing each item of thread specific global data. */
#ifdef THREAD_SAFE

/* Define how to initialise thread-specific globals. */ 
#define GLOBAL_inits \
   globals->GetAttrib_Buff[ 0 ] = 0; \
   globals->Class_Init = 0;

/* Create the function that initialises global data for this module. */
astMAKE_INITGLOBALS(StcsChan)

/* Define macros for accessing each item of thread specific global data. */
#define getattrib_buff astGLOBAL(StcsChan,GetAttrib_Buff)
#define class_init astGLOBAL(StcsChan,Class_Init)
#define class_vtab astGLOBAL(StcsChan,Class_Vtab)


/* If thread safety is not needed, declare and initialise globals at static 
   variables. */ 
#else

/* Define the class virtual function table and its initialisation flag
   as static variables. */
static AstStcsChanVtab class_vtab;   /* Virtual function table */
static int class_init = 0;       /* Virtual function table initialised? */

/* Buffer returned by GetAttrib. */
static char getattrib_buff[ GETATTRIB_BUFF_LEN + 1 ];

#endif


/* External Interface Function Prototypes. */
/* ======================================= */
/* The following functions have public prototypes only (i.e. no
   protected prototypes), so we must provide local prototypes for use
   within this module. */
AstStcsChan *astStcsChanForId_( const char *(*)( void ), 
                                char *(*)( const char *(*)( void ), int * ), 
                                void (*)( const char * ), 
                                void (*)( void (*)( const char * ), const char *, int * ),
                                const char *, ... );
AstStcsChan *astStcsChanId_( const char *(* source)( void ),
                             void (* sink)( const char * ),
                             const char *options, ... );

/* Prototypes for Private Member Functions. */
/* ======================================== */
static AstObject *Read( AstChannel *, int * );
static AstPointList *SinglePointList( AstFrame *, double *, AstRegion *, int *);
static char *AddItem( AstKeyMap *, const char *, const char *, char *, int *, int * );
static char *ContextFragment( WordContext *, char **, int * );
static char *SourceWrap( const char *(*)( void ), int * );
static const char *GetNextWord( AstStcsChan *, WordContext *, int * );
static double *BoxCorners( AstFrame *, const double[2], const double[2], int * );
static int Write( AstChannel *, AstObject *, int * );
static int WriteRegion( AstStcsChan *, AstRegion *, AstKeyMap *, int * );
static void Dump( AstObject *, AstChannel *, int * );
static void FreeContext( WordContext *, int * );
static void GetFmt( const char *, AstKeyMap *, int, int, char *, int * );
static void MapPut0C( AstKeyMap *, const char *, const char *, const char *, int, int * );
static void MapPut0D( AstKeyMap *, const char *, double, double, int, int * );
static void SetUnc( AstRegion *, AstRegion *, AstFrame *, int, double, double *, int, int * );
static void SinkWrap( void (*)( const char * ), const char *, int * );
static void WriteProps( AstStcsChan *, AstKeyMap *, int * );

static int GetStcsArea( AstStcsChan *, int * );
static int TestStcsArea( AstStcsChan *, int * );
static void ClearStcsArea( AstStcsChan *, int * );
static void SetStcsArea( AstStcsChan *, int, int * );

static int GetStcsCoords( AstStcsChan *, int * );
static int TestStcsCoords( AstStcsChan *, int * );
static void ClearStcsCoords( AstStcsChan *, int * );
static void SetStcsCoords( AstStcsChan *, int, int * );

static int GetStcsProps( AstStcsChan *, int * );
static int TestStcsProps( AstStcsChan *, int * );
static void ClearStcsProps( AstStcsChan *, int * );
static void SetStcsProps( AstStcsChan *, int, int * );

static void ClearAttrib( AstObject *, const char *, int * );
static const char *GetAttrib( AstObject *, const char *, int * );
static void SetAttrib( AstObject *, const char *, int * );
static int TestAttrib( AstObject *, const char *, int * );

/* Member functions. */
/* ================= */

static char *AddItem( AstKeyMap *km, const char *key, const char *prefix, 
                      char *line, int *nc, int *status ){
/*
*  Name:
*     AddItem

*  Purpose:
*     Add an STC-S property item to a buffer.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     char *AddItem( AstKeyMap *km, const char *key, const char *prefix, 
*                    char *line, int *nc, int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function creates a new Box with dimensions specified by the
*     values in the "error" array, centred on a representative position
*     within one of the supplied Regions, and then stores the Box as the
*     uncertainty Region within both the supplied Regions. 

*  Parameters:
*     km
*        Pointer to a KeyMap containing the STC-S properties.
*     key
*        The key name associated with the property to be checked.
*     prefix
*        if not NULL, this is a string that is to be written out before
*        the property value. It should usually include a trailing space.
*     line
*        Pointer to the buffer to recieve the prefix and property value.
*     nc
*        Pointer to an int in which to store the number of characaters in
*        the buffer. Updated on exit.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to the buffer. This will usually be "line", but may be
*     different to "line" if it was necessary to expand the memory to make 
*     room for the new property. 

*/

/* Local Variables: */
   char *result;          /* Returned pointer */
   const char *word;      /* Property value */

/* Initialise */
   result = line;

/* Check the global error status. */
   if ( !astOK ) return result;

/* If the KeyMap contains the required property... */
   if( astMapGet0C( km, key, &word ) ) {

/* Add any supplied prefix to the returned buffer. */
      if( prefix ) result = astAppendString( result, nc, prefix );

/* Add the property value to the returned buffer. */
      result = astAppendString( result, nc, word );

/* Add a traling space to the returned buffer. */
      result = astAppendString( result, nc, " " );
   }

/* Return the buffer pointer. */
   return result;
}

static double *BoxCorners( AstFrame *frm, const double centre[2],
                           const double bsize[2], int *status ) {
/*
*  Name:
*     BoxCorners

*  Purpose:
*     Determine the positions of the corners of an STC Box.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     double *BoxCorners( AstFrame *frm, const double centre[2],
*                         const double bsize[2], int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function returns a pointer to a dynamically allocated array
*     holding the positions of the corners of the STC Box defined by the
*     supplied "centre" and "bsize" arrays.

*  Parameters:
*     frm
*        Pointer to the Frame in which the Box is defined. Must be 2-D.
*     centre
*        Two element array holding the Frame co-ordinates at the centre
*        of the Box.
*     bsize
*        Two element array holding the full width and height of the Box.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to a dynamically allocated array holding the axis values
*     at the four corners, in a form suitable for passing to the
*     astPolygon constructor function. NULL is returned if an error has 
*     already occurred, of if this function fails for any reason.
*/

/* Local Variables: */
   double *result;         /* Returned pointer. */
   double bh1[ 2 ];        /* A first point on the bottom horizontal edge */
   double bh2[ 2 ];        /* A second point on the bottom horizontal edge */
   double blc[ 2 ];        /* Position of bottom left corner */
   double brc[ 2 ];        /* Position of bottom right corner */
   double lv1[ 2 ];        /* A first point on the left vertical edge */
   double lv2[ 2 ];        /* A second point on the left vertical edge */
   double pa;              /* Position angle of great circle/straight line */
   double rv1[ 2 ];        /* A first point on the right vertical edge */
   double rv2[ 2 ];        /* A second point on the right vertical edge */
   double th1[ 2 ];        /* A first point on the top horizontal edge */
   double th2[ 2 ];        /* A second point on the top horizontal edge */
   double tlc[ 2 ];        /* Position of top left corner */
   double trc[ 2 ];        /* Position of top right corner */

/* Initialise. */
   result = NULL;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Check the Frame is 2-dimensional. */
   if( astGetNaxes( frm ) != 2 ) {
      astError( AST__BADIN, "astRead(StcsChan): Supplied space frame has "
                "%d axes.", status, astGetNaxes( frm ) );
      astError( AST__BADIN, "astRead(StcsChan): Can only use STC Box regions "
                "with 2-dimensional space frames.", status );
   }

/* Offset away from the centre by half the Box width along a great circle
   initially parallel to the positive first frame axis (i.e. position 
   angle +pi/2). The end position goes in "rv1" and the position angle of
   the great circle (or straight line) at that point is returned as the 
   function value. NOTE, the use of the words "left" and "right" below is 
   vague because it depends on whether we are using a SkyFrame (which has 
   a reversed first axis) or a basic Frame. In general, the choice of "left"
   and "right" below is appropriate for a basic Frame. */
   pa = astOffset2( frm, centre, AST__DPIBY2, bsize[ 0 ]/2, rv1 );  

/* Turn by 90 degrees and offset away by half the box height. This is done 
   so that we have a second point (rv2) to define the great circle (or 
   straight line) that forms the first vertical edge of the Box (i.e. the 
   great circle or straight line through rv1 and rv2). Note, for spherical 
   Frames (i.e. SkyFrames) "rv2" is not necessarily a corner of the box. */
   (void) astOffset2( frm, rv1, pa + AST__DPIBY2, bsize[ 1 ]/2, rv2 );  

/* In the same way, get two points on the second vertical Box edge. */
   pa = astOffset2( frm, centre, -AST__DPIBY2, bsize[ 0 ]/2, lv1 );  
   (void) astOffset2( frm, lv1, pa + AST__DPIBY2, bsize[ 1 ]/2, lv2 );  

/* In the same way, get two points on the top horizontal Box edge. */
   pa = astOffset2( frm, centre, 0.0, bsize[ 1 ]/2, th1 );  
   (void) astOffset2( frm, th1, pa + AST__DPIBY2, bsize[ 0 ]/2, th2 );  

/* In the same way, get two points on the bottom horizontal Box edge. */
   pa = astOffset2( frm, centre, AST__DPI, bsize[ 1 ]/2, bh1 );  
   (void) astOffset2( frm, bh1, pa + AST__DPIBY2, bsize[ 0 ]/2, bh2 );  

/* The first corner of the Box is at the intersection of the first
   vertical and top horizontal edges. */
   astIntersect( frm, lv1, lv2, th1, th2, tlc );

/* The top right corner of the Box is at the intersection of the right
   vertical and top horizontal edges. */
   astIntersect( frm, rv1, rv2, th1, th2, trc );

/* The bottom left corner of the Box is at the intersection of the left
   vertical and bottom horizontal edges. */
   astIntersect( frm, lv1, lv2, bh1, bh2, blc );

/* The bottom right corner of the Box is at the intersection of the right
   vertical and bottom horizontal edges. */
   astIntersect( frm, rv1, rv2, bh1, bh2, brc );

/* Gather the corners together into an array suitable for use with
   astPolygon. Make sure the vertices are traversed in an ant-clockwise
   sense whether in a SkyFrame or a basic Frame. */
   result = astMalloc( 8*sizeof( *result ) );
   if( result ) {
      if( astIsASkyFrame( frm ) ) {        
         result[ 0 ] = tlc[ 0 ];
         result[ 1 ] = trc[ 0 ];
         result[ 2 ] = brc[ 0 ];
         result[ 3 ] = blc[ 0 ];
         result[ 4 ] = tlc[ 1 ];
         result[ 5 ] = trc[ 1 ];
         result[ 6 ] = brc[ 1 ];
         result[ 7 ] = blc[ 1 ];
      } else {
         result[ 3 ] = tlc[ 0 ];
         result[ 2 ] = trc[ 0 ];
         result[ 1 ] = brc[ 0 ];
         result[ 0 ] = blc[ 0 ];
         result[ 7 ] = tlc[ 1 ];
         result[ 6 ] = trc[ 1 ];
         result[ 5 ] = brc[ 1 ];
         result[ 4 ] = blc[ 1 ];
      }

   }

/* Return the pointer. */
   return result;
}

static void ClearAttrib( AstObject *this_object, const char *attrib, int *status ) {
/*
*  Name:
*     ClearAttrib

*  Purpose:
*     Clear an attribute value for a StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void ClearAttrib( AstObject *this, const char *attrib, int *status )

*  Class Membership:
*     StcsChan member function (over-rides the astClearAttrib protected
*     method inherited from the Channel class).

*  Description:
*     This function clears the value of a specified attribute for a
*     StcsChan, so that the default value will subsequently be used.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     attrib
*        Pointer to a null terminated string specifying the attribute
*        name.  This should be in lower case with no surrounding white
*        space.
*     status
*        Pointer to the inherited status variable.
*/

/* Local Variables: */
   AstStcsChan *this;              /* Pointer to the StcsChan structure */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_object;

/* Check the attribute name and clear the appropriate attribute. */

   if ( !strcmp( attrib, "stcsarea" ) ) {
      astClearStcsArea( this );

   } else if ( !strcmp( attrib, "stcscoords" ) ) {
      astClearStcsCoords( this );

   } else if ( !strcmp( attrib, "stcsprop" ) ) {
      astClearStcsProps( this );

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      (*parent_clearattrib)( this_object, attrib, status );
   }
}

static char *ContextFragment( WordContext *con, char **buf, int *status ){
/*
*  Name:
*     ContextFragment

*  Purpose:
*     Returns a string holding a fragment of the document being read.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     char *ContextFragment( WordContext *con, char **buf, int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function returns a pointer to a string that holds a fragment
*     of the STC-S document currently being read. The fragment ends at
*     the last word read by function GetNextWord, and starts a certain
*     number of words earlier in the document, as specified by the NEWORD 
*     macro.

*  Parameters:
*     con
*        Pointer to the context structure, managed by GetNextWord.
*     buf
*        Address of a pointer to a dynamically allocated buffer. This 
*        pointer should be NULL on the first call to this function, and
*        will be updated by this function. The pointer should be freed
*        using astFree when no longer needed.
*     status
*        Address of the inherited status value.

*  Returned Value:
*     A pointer to the buffer.
*/

/* Local Variables: */
   int i;                     /* Word count */
   int j;                     /* Word index */
   int nc;                    /* Text length */

/* Initialise the number of characters written to the buffer. */
   nc = 0;

/* Get the index of the first word to add to the buffer. The "next"
   component of the context structure holds the index at which the word 
   returned by the next call to GetNextWord will be stored. So at the
   moment, this is the index of the oldest word in the cyclic list. */
   j = con->next;

/* Loop round all non-NULL words in the cyclic list. */
   for( i = 0; i < NEWORD; i++ ) {
      if( con->words[ j ] ) {

/* Append this word to the buffer, extending the buffer size as
   necessary. */
         *buf = astAppendString( *buf, &nc, con->words[ j ] );

/* Append a trailingh space. */
         *buf = astAppendString( *buf, &nc, " " );
      }

/* Increment the index of the next word to use in the cyclic list. Wrap
   back to zerp when the end of the list is reached. */
      if( ++j == NEWORD ) j = 0;
   }

/* Remove the final trailing space. */
   (*buf)[ nc - 1 ] = 0;

/* Return a pointer to the supplied buffer. */
   return *buf;
}

static void FreeContext( WordContext *con, int *status ){
/*
*  Name:
*     FreeContext

*  Purpose:
*     Free the resources used by a word-reading context structure.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     voidFreeContext( WordContext *con, int *status );

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function frees the resources used by the supplied WordContext
*     structure. This structure is used by GetNextWord to keep track of
*     which word to return next.
*
*     This function frees the dynamic memory pointers stored within the 
*     WordContext structure, but does not free the memory holding the
*     WordContext structure itself.

*  Parameters:
*     con
*        Pointer to a structure holding the context. 
*     status
*        Pointer to the inherited status variable.

*/

/* Local Variables: */
   int i;                         /* Word index */

/* Check the supplied pointer. */
   if ( !con ) return;

/* Free the resources. */
   con->line = astFree( con->line );

   for( i = 0; i < NEWORD; i++ ) {
      con->words[ i ] = astFree( con->words[ i ] );
   }

}

static const char *GetAttrib( AstObject *this_object, const char *attrib, int *status ) {
/*
*  Name:
*     GetAttrib

*  Purpose:
*     Get the value of a specified attribute for a StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     const char *GetAttrib( AstObject *this, const char *attrib, int *status )

*  Class Membership:
*     StcsChan member function (over-rides the protected astGetAttrib
*     method inherited from the Channel class).

*  Description:
*     This function returns a pointer to the value of a specified
*     attribute for a StcsChan, formatted as a character string.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     attrib
*        Pointer to a null terminated string containing the name of
*        the attribute whose value is required. This name should be in
*        lower case, with all white space removed.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     - Pointer to a null terminated string containing the attribute
*     value.

*  Notes:
*     - The returned string pointer may point at memory allocated
*     within the StcsChan, or at static memory. The contents of the
*     string may be over-written or the pointer may become invalid
*     following a further invocation of the same function or any
*     modification of the StcsChan. A copy of the string should
*     therefore be made if necessary.
*     - A NULL pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*/

/* Local Variables: */
   astDECLARE_GLOBALS;           /* Declare the thread specific global data */
   AstStcsChan *this;            /* Pointer to the StcsChan structure */
   const char *result;           /* Pointer value to return */
   int ival;                     /* Integer attribute value */

/* Initialise. */
   result = NULL;

/* Check the global error status. */   
   if ( !astOK ) return result;

/* Get a pointer to the structure holding thread-specific global data. */   
   astGET_GLOBALS(this_object);

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_object;

/* Compare "attrib" with each recognised attribute name in turn,
   obtaining the value of the required attribute. If necessary, write
   the value into "getattrib_buff" as a null terminated string in an appropriate
   format.  Set "result" to point at the result string. */

/* StcsArea. */
/* --------- */
   if ( !strcmp( attrib, "stcsarea" ) ) {
      ival = astGetStcsArea( this );
      if ( astOK ) {
         (void) sprintf( getattrib_buff, "%d", ival );
         result = getattrib_buff;
      }

/* StcsCoords. */
/* ----------- */
   } else if ( !strcmp( attrib, "stcscoords" ) ) {
      ival = astGetStcsCoords( this );
      if ( astOK ) {
         (void) sprintf( getattrib_buff, "%d", ival );
         result = getattrib_buff;
      }


/* StcsProps. */
/* ---------- */
   } else if ( !strcmp( attrib, "stcsprops" ) ) {
      ival = astGetStcsProps( this );
      if ( astOK ) {
         (void) sprintf( getattrib_buff, "%d", ival );
         result = getattrib_buff;
      }

/* If the attribute name was not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      result = (*parent_getattrib)( this_object, attrib, status );
   }

/* Return the result. */
   return result;

}

static void GetFmt( const char *key, AstKeyMap *props, int i, int defdigs, 
                    char *fmt, int *status ){
/*
*  Name:
*     GetFmt

*  Purpose:
*     Decide how many digits to use when formatting a numerical STC-S
*     property value.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void GetFmt( const char *key, AstKeyMap *props, int i, 
*                  int defdigs, char *fmt, int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function locates the named property in the supplied KeyMap. If
*     it is found, a printf format specifier is generated that matches
*     the value is determined and returned. Otherwise, a default format
*     specified based on the supplied default number of digits is returned.

*  Parameters:
*     key
*        The key name associated with the property.
*     km
*        Pointer to a KeyMap containing the STC-S properties.
*     i
*        For vector values, this is the index of the vector element to be
*        checked. Should be zero for scalar values. If "i" is greater
*        than the number of values in the vector, then the number of digits 
*        in the first element is found and returned.
*     defdigs
*        The value to return if the KeyMap does not contain an entry with
*        the supplied key.
*     fmt
*        Pointer to a string in which to return the format specifier.
*     status
*        Pointer to the inherited status variable.

*/

/* Local Variables: */
   const char *dot;       /* Pointer to decimal point */
   const char *p;         /* Pointer to next character */
   const char *word;      /* Property value */
   int after0;            /* Digits after the decimal point in first word */
   int after;             /* Digits after the decimal point in current word */
   int before0;           /* Digits before the decimal point in first word */
   int before;            /* Digits before the decimal point in current word */
   int exp0;              /* Was an exponent found in first word? */
   int exp;               /* Was an exponent found in current word? */
   int j;                 /* Index of current word */

/* Check the global error status. */
   if ( !astOK ) return;

/* Initialise. */
   exp = 1;
   before = defdigs;
   after = 0;
   exp0 = 0;
   before0 = 0;
   after0 = 0;

/* If the KeyMap contains the required property... */
   if( astMapGet0C( props, key, &word ) ) {

/* Skip over the words in the string. */
      p = word;
      for( j = 0; j <= i; j++ ) {

/* Find the next space or terminating null at the end of the current word. 
   Also count the number of digits before and after the decimal point and
   see if the word includes an exponent. */
         exp = 0;
         before = 0;
         after = 0;
         dot = NULL;

         while( *p != 0 && *p != ' ' ) {
            if( ! exp ) {
               if( isdigit( *p ) ) {
                  if( dot ) {
                     after++;
                  } else {
                     before++;
                  }

               } else if( *p == '.' ) {
                  dot = p;
                  
               } else if( *p == 'e' || *p == 'E' ) {
                  exp = 1;
               }
            }
            p++;
         }

/* Note the values for the first word. */
         if( j == 0 ) {
            exp0 = exp;
            before0 = before;
            after0 = after;
         }

/* Find the following non-space marking the start of the next word, 
   or the terminating null. */
         while( *p != 0 && *p == ' ' ) p++;

/* If we find the terminating null before we have found the i'th word,
   break out of the loop using the first word instead of the i'th word. */
         if( *p == 0 ) {
            exp = exp0;
            before = before0;
            after = after0;
            break;
         }
      }
   }

   if( exp ) {
      sprintf( fmt, "%%.%dg", before + after );
   } else {
      sprintf( fmt, "%%.%df", after );
   }
}

static const char *GetNextWord( AstStcsChan *this, WordContext *con, 
                                int *status ){
/*
*  Name:
*     GetNextWord

*  Purpose:
*     Get a pointer to the next input word read from an STC-S source.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     const char *GetNextWord( AstStcsChan *this, WordContext *con, 
*                              int *status ) 

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function returns a pointer to the next word of an STC-S
*     description.

*  Parameters:
*     this
*        Pointer to the StcsChan, or NULL (to initialise "con"). 
*     con
*        Pointer to a structure holding context. The structure should be
*        initialised by calling this function with a NULL "this" pointer
*        before making further use of this function. When finished, it
*        should be released using FreeContext.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to the new word. NULL is returned if an error has already
*     occurred, of if "this" is NULL.
*/

/* Local Variables: */
   const char *result;            /* Returned pointer. */
   int i;                         /* Word index */
   size_t len;                    /* Word length */

/* Initialise. */
   result = NULL;

/* Check the global error status. */
   if ( !astOK ) return result;

/* If no StcChan was supplied, initialise the supplied WordContext. */
   if( !this ) {
      con->e = NULL;
      con->line = NULL;
      con->done = 0;
      con->next = 0;
      for( i = 0; i < NEWORD; i++ ) con->words[ i ] = NULL;

/* Otherwise... */   
   } else {

/* If the previous invocation of this function converted a space
   character into a null character, change it back again. */
      if( con->e ) *(con->e) = ' ';

/* Get a pointer to the next non-white character in the current line of
   input text. */
      result = con->e;
      if( result ) {
         while( *result && isspace( *result ) ) result++;
      }

/* If we have exhausted the current line, get the next line by invoking
   the source function. First free the memory holding the previous line. */
      if( !result || ! *result ) {
         if( con->line ) con->line = astFree( con->line );
         con->line = astGetNextText( this );

/* Get a pointer to the first non-white character in the new line. */
         result = con->line;
         if( result ) {
            while( *result && isspace( *result ) ) result++;
         }
      }

/* Find the end of the word. */
      if( result ) {
         con->e = (char *) result + 1;
         while( *(con->e) && !isspace( *(con->e) ) ) (con->e)++;

/* If the word is already null-terminated, nullify the "e" pointer to
   indicate this. Otherwise, change the white-space character into a
   null. */
         if( *(con->e) ) {
            *(con->e) = 0;
            len = con->e - result;
         } else {
            con->e = NULL;
            len = strlen( result );
         }

/* Add the word into the cyclic list of words used to form a document
   fragment to include in error and warning messages. */
         con->words[ con->next ] = astStore( con->words[ con->next ],
                                             result, len + 1 );
         if( ++(con->next) == NEWORD ) con->next = 0;

/* If we have run out of input words, but we have not yet finished
   interpreting the previous word returned, return a null string, rather
   than a null pointer in order to allow further interpretation of the
   previous word. */
      } else if( ! con->done ) {
         result = "";
      }
   }

/* Return the pointer to the next word. */
   return result;
}

void astInitStcsChanVtab_(  AstStcsChanVtab *vtab, const char *name, int *status ) {
/*
*+
*  Name:
*     astInitStcsChanVtab

*  Purpose:
*     Initialise a virtual function table for an StcsChan.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "stcschan.h"
*     void astInitStcsChanVtab( AstStcsChanVtab *vtab, const char *name )

*  Class Membership:
*     StcsChan vtab initialiser.

*  Description:
*     This function initialises the component of a virtual function
*     table which is used by the StcsChan class.

*  Parameters:
*     vtab
*        Pointer to the virtual function table. The components used by
*        all ancestral classes will be initialised if they have not already
*        been initialised.
*     name
*        Pointer to a constant null-terminated character string which contains
*        the name of the class to which the virtual function table belongs (it 
*        is this pointer value that will subsequently be returned by the Object
*        astClass function).
*-
*/

/* Local Variables: */
   astDECLARE_GLOBALS;           /* Pointer to thread-specific global data */
   AstObjectVtab *object;        /* Pointer to Object component of Vtab */
   AstChannelVtab *channel;      /* Pointer to Channel component of Vtab */

/* Check the local error status. */
   if ( !astOK ) return;

/* Get a pointer to the thread specific global data structure. */
   astGET_GLOBALS(NULL);

/* Initialize the component of the virtual function table used by the
   parent class. */
   astInitChannelVtab( (AstChannelVtab *) vtab, name );

/* Store a unique "magic" value in the virtual function table. This
   will be used (by astIsAStcsChan) to determine if an object belongs
   to this class.  We can conveniently use the address of the (static)
   class_check variable to generate this unique value. */
   vtab->check = &class_check;

/* Initialise member function pointers. */
/* ------------------------------------ */

   vtab->ClearStcsArea = ClearStcsArea;
   vtab->GetStcsArea = GetStcsArea;
   vtab->SetStcsArea = SetStcsArea;
   vtab->TestStcsArea = TestStcsArea;

   vtab->ClearStcsCoords = ClearStcsCoords;
   vtab->GetStcsCoords = GetStcsCoords;
   vtab->SetStcsCoords = SetStcsCoords;
   vtab->TestStcsCoords = TestStcsCoords;

   vtab->ClearStcsProps = ClearStcsProps;
   vtab->GetStcsProps = GetStcsProps;
   vtab->SetStcsProps = SetStcsProps;
   vtab->TestStcsProps = TestStcsProps;

/* Save the inherited pointers to methods that will be extended, and
   replace them with pointers to the new member functions. */
   object = (AstObjectVtab *) vtab;
   channel = (AstChannelVtab *) vtab;

   parent_clearattrib = object->ClearAttrib;
   object->ClearAttrib = ClearAttrib;
   parent_getattrib = object->GetAttrib;
   object->GetAttrib = GetAttrib;
   parent_setattrib = object->SetAttrib;
   object->SetAttrib = SetAttrib;
   parent_testattrib = object->TestAttrib;
   object->TestAttrib = TestAttrib;

   channel->Write = Write;
   channel->Read = Read;

/* Declare the Dump function for this class. There is no destructor or
   copy constructor. */
   astSetDump( vtab, Dump, "StcsChan", "STC-S I/O Channel" );

/* If we have just initialised the vtab for the current class, indicate
   that the vtab is now initialised. */
   if( vtab == &class_vtab ) class_init = 1;

}

static void MapPut0C( AstKeyMap *km, const char *key, const char *value, 
                      const char *def, int defs, int *status ){
/*
*  Name:
*     MapPut0C

*  Purpose:
*     Store a text STC-S property in the supplied keymap.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void MapPut0C( AstKeyMap *km, const char *key, const char *value, 
*                    const char *def, int defs, int *status )

*  Class Membership:
*     StcsChan member function.

*  Description:
*     This function stors the supplied value in the given KeyMap,
*     handling default values.

*  Parameters:
*     km 
*        Pointer to the KeyMap in which to store the value.
*     key
*        Pointer to a string holding the property name associated with
*        the value.
*     value
*        The property value. If this is NULL then the function
*        returns without action.
*     def
*        The default property value. 
*     defs
*        If zero, then the value is not stored in the KeyMap if the value
*        is equal to the default value.
*     status
*        Pointer to the inherited status variable.

*/

/* Check the inherited status */
   if( !astOK ) return;

/* If the value is NULL, ignore the entry. */
   if( value ) {

/* If the value is equal to the default value, and we are NOT storing
   default values, ensure the KeyMap has no entry for the given key. */
      if( !strcmp( value, def ) && !defs ) {
         astMapRemove( km, key );

/* Otherwise, store the value. */
      } else {
         astMapPut0C( km, key, value, NULL );
      }
   }
}

static void MapPut0D( AstKeyMap *km, const char *key, double value, double def,
                      int defs, int *status ){
/*
*  Name:
*     MapPut0D

*  Purpose:
*     Store a floating point STC-S property in the supplied keymap.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void MapPut0D( AstKeyMap *km, const char *key, double value, double def,
*                    int defs, int *status )

*  Class Membership:
*     StcsChan member function.

*  Description:
*     This function stors the supplied value in the given KeyMap,
*     handling default values.

*  Parameters:
*     km 
*        Pointer to the KeyMap in which to store the value.
*     key
*        Pointer to a string holding the property name associated with
*        the value.
*     value
*        The property value. If this is AST__BAD then the function
*        returns without action.
*     def
*        The default property value. 
*     defs
*        If zero, then the value is not stored in the KeyMap if the value
*        is equal to the default value.
*     status
*        Pointer to the inherited status variable.

*/

/* Check the inherited status */
   if( !astOK ) return;

/* If the value is bad, ignore the entry. */
   if( value != AST__BAD ) {

/* If the value is equal to the default value, and we are NOT storing
   default values, ensure the KeyMap has no entry for the given key. */
      if( value == def && !defs ) {
         astMapRemove( km, key );

/* Otherwise, store the value. */
      } else {
         astMapPut0D( km, key, value, NULL );
      }
   }
}

static AstObject *Read( AstChannel *this_channel, int *status ) {
/*
*  Name:
*     Read

*  Purpose:
*     Read an Object from a Channel.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     AstObject *Read( AstChannel *this_channel, int *status ) 

*  Class Membership:
*     StcsChan member function (over-rides the astRead method
*     inherited from the Channel class).

*  Description:
*     This function reads an Object from an StcsChan.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to the new Object.
*/

/* Local Variables: */
   AstFrame *spacefrm;       /* Pointer to SpaceFrame for space sub-phrase */
   AstFrameSet *fs;          /* Temporary FrameSet */
   AstKeyMap *full_props;    /* KeyMap holding all sub-phrase properties */
   AstKeyMap *props;         /* KeyMap holding current sub-phrase properties */
   AstKeyMap *red_props;     /* KeyMap holding redshift sub-phrase properties */
   AstKeyMap *space_props;   /* KeyMap holding space sub-phrase properties */
   AstKeyMap *spec_props;    /* KeyMap holding spectral sub-phrase properties */
   AstKeyMap *time_props;    /* KeyMap holding time sub-phrase properties */
   AstObject *new;           /* Pointer to returned Object */
   AstPrism *tr;             /* Temporary Region pointer */
   AstRegion *full_co;       /* Region describing full coord position */
   AstRegion *full_enc;      /* Region describing full enclosure */
   AstRegion *red_co;        /* Region describing red-shift coord */
   AstRegion *red_enc;       /* Region describing red-shift enclosure */
   AstRegion *space_co;      /* Region describing space coord */
   AstRegion *space_enc;     /* Region describing space enclosure */
   AstRegion *spec_co;       /* Region describing spectral coord */
   AstRegion *spec_enc;      /* Region describing spectral enclosure */
   AstRegion *time_co;       /* Region describing time coord */
   AstRegion *time_enc;      /* Region describing time enclosure */
   AstSpecFrame *redfrm;     /* Pointer to SpecFrame for redshift sub-phrase */
   AstSpecFrame *specfrm;    /* Pointer to SpecFrame for spectral sub-phrase */
   AstStcsChan *this;        /* Pointer to the StcsChan structure */
   AstStdOfRestType sor;     /* Standard of rest */
   AstSystemType sys;        /* Frame System attribute value */
   AstTimeFrame *tf1;        /* Temporary TimeFrame */
   AstTimeFrame *timefrm;    /* Pointer to TimeFrame for time sub-phrase */
   AstTimeScaleType ts;      /* TimeFrame TimeScale attribute value */
   WordContext con;          /* Context for finding next source word */
   char *fbuf;               /* Pointer to buffer holding document fragment */
   char *frame;              /* Space frame name */
   char *prop;               /* String holding complete property value */
   char *unit;               /* Pointer to unit string */
   const char *new_ts;       /* Time scale string */
   const char *subphrase;    /* Name of current sub phrase */
   const char *t;            /* Temporary character string pointer */
   const char *word;         /* Pointer to next source word */
   double *p;                /* Pointer to next element of temp */
   double *temp;             /* Temporary array pointer */
   double *vertices;         /* Pointer to array holding vertex axis values */
   double axval;             /* Axis value */
   double bsize[ 3 ];        /* Box dimensions */
   double centre[ 3 ];       /* Circle or ellipse centre */
   double epoch;             /* Value to use for the Epoch attribue */
   double err[ 3 ];          /* Sub-phrase error values */
   double fill;              /* Filling factor */
   double hilim;             /* Axis upper limit */
   double lolim;             /* Axis lower limit */
   double pa;                /* Ellipse positoon angle */
   double pos[ 3 ];          /* Spatial position */
   double radius2;           /* Ellipse second radius */
   double radius;            /* Circle or ellipse radius */
   double scale;             /* Units scaling factor */
   double shilim[ 3 ];       /* Space frame upper axis limits */
   double slolim[ 3 ];       /* Space frame lower axis limits */
   double start;             /* Start time */
   double stop;              /* Stop time */
   double time;              /* Time value */
   double time_origin;       /* Value to use as TimeFrame TimeOrigin*/
   double value;             /* Axis value */
   int i;                    /* Loop index */
   int iaxis;                /* Axis index */
   int is_skyframe;          /* Is the space frame a SkyFrame? */
   int isvel;                /* Does the redshift represent velocity? */
   int ivert;                /* Vertex index */
   int level;                /* Warning reporting level */
   int naxes;                /* No. of space Frame axes */
   int nc;                   /* Number of characters written to string */
   int nerror;               /* No. of error values to read */
   int new_word;             /* Get a new word at the end of the pass? */
   int nig;                  /* No. of ignored numerical values */
   int nvert;                /* No. of polygon vertices */
   int nwant;                /* Number of objects to return */
   int redid;                /* Redshift sub-phrase component identifier */
   int spaceid;              /* Space sub-phrase component identifier */
   int specid;               /* Spectral sub-phrase component identifier */
   int timeid;               /* Time sub-phrase component identifier */
   int use_co;               /* Do we have a full coordinate position? */
   int use_enc;              /* Do we have a full enclosure? */
   int velid;                /* Velocity sub-phrase component identifier */
   int want_co;              /* Is the Coordinates component wanted? */
   int want_enc;             /* Is the enclosure region wanted? */
   int want_props;           /* Are the STC-S properties wanted? */

/* The stage reached in the parsing of the STC-S description is indicated
   by the "look_for" variable. This variable is allowed the following
   values, indicating the item that is to be checked for next. */
   enum look_for_type {
      ALLSKY,
      BOX,
      CIRCLE,
      CONVEX,
      ELLIPSE,
      ERROR,
      FILL_FACTOR,
      FLAVOUR,
      FRAME,
      LIMITS,
      PIX_SIZE,
      POLYGON,
      POSITION,
      POSITION_INTERVAL,
      POSITION_LABEL,
      REDSHIFT_IDENTIFIER,
      RED_SPEC_LABEL,
      RED_SPEC_VALUE,
      REFPOS,
      RESOLUTION,
      SIZE,
      SPACE_IDENTIFIER,
      SPECTRAL_IDENTIFIER,
      START,
      STOP,
      TIME,
      TIME_IDENTIFIER,
      TIME_LABEL,
      TIME_SCALE,
      TYPE_DOPPLER,
      UNIT,
      VELOCITY_IDENTIFIER,
      VELOCITY_VALUE
   } look_for;

/* Initialise. */
   new = NULL;

/* Check the global error status. */
   if ( !astOK ) return new;

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_channel;

/* Initialise the word search context. */
   (void) GetNextWord( NULL, &con, status );

/* Get a pointer to the first word in the STC-S description. */
   word = GetNextWord( this, &con, status );

/* Indicate we are currently looking for the time sub-phrase (the first
   item in an STC-S description). */
   look_for = TIME_IDENTIFIER;

/* Create KeyMaps to store the properties of each sub-phrase for future
   use. */
   time_props = astKeyMap( " ", status );
   space_props = astKeyMap( " ", status );
   spec_props = astKeyMap( " ", status );
   red_props = astKeyMap( " ", status );

/* Initialise everything else. */
   epoch = AST__BAD;
   err[ 0 ] = AST__BAD;
   fbuf = NULL;
   fill = AST__BAD;
   frame = NULL;
   is_skyframe = 0;
   isvel = 0;
   naxes = 0;
   nvert = 0;
   pos[ 0 ] = AST__BAD;
   prop = NULL;
   props = NULL;
   radius2 = 0.0;
   red_co = NULL;
   red_enc = NULL;
   redfrm = NULL;
   redid = NULL_ID;
   scale = 1.0;
   sor = AST__BADSOR;
   space_co = NULL;
   space_enc = NULL;
   spacefrm = NULL;
   spaceid = NULL_ID;
   spec_co = NULL;
   spec_enc = NULL;
   specfrm = NULL;
   specid = NULL_ID;
   start = AST__BAD;
   stop = AST__BAD;
   subphrase = NULL;
   sys = 0;
   t = NULL;
   time = AST__BAD;
   time_co = NULL;
   time_enc = NULL;
   timefrm = NULL;
   timeid = NULL_ID;
   time_origin = AST__BAD;
   ts = 0;
   unit = NULL;
   use_co = 0;
   use_enc = 0;
   velid = NULL_ID;
   vertices = NULL;

/* Loop until all words in the STC-S description have been interpreted or 
   an error has occurred. */
   while( word && astOK ) {

/* Initialise a flag to indicate that we have interpreted the current word 
   sucesfully and so will need to get a new word for the next pass through 
   this loop. If it turns out that we cannot interpret the current word
   in this pass, then this flag will be set to zero at some point, thus 
   preventing a new word from being acquired and causing another attempt to 
   re-interpret the current word in a different context. */
      new_word = 1;

/* If we are currently looking for the time sub-phrase, see if the current
   word is any of the known time sub-phrase identifiers. Is so, move on
   to read the associated sub-phrase component. */
      if( look_for == TIME_IDENTIFIER ) {
/* ------------------------------------------------------------------ */

/* Assume that we will be moving on to read the fill factor (most time
   sub-phrases start with the fill factor ). */
         look_for = FILL_FACTOR;

/* Now check the word to see if it a known time sub-phrase identifier. */
         if( !strcmp( word, "TimeInterval" ) ) {
            timeid = TIME_INTERVAL_ID;

         } else if( !strcmp( word, "StartTime" ) ) {
            timeid = START_TIME_ID;

         } else if( !strcmp( word, "StopTime" ) ) {
            timeid = STOP_TIME_ID;

         } else if( !strcmp( word, "Time" ) ) {
            look_for = TIME_SCALE;  /* After "Time", we move on to find the 
            timeid = TIME_ID;          time-scale, not the fill factor */

/* If the word is not a known time sub-phrase identifier, indicate that we
   should attempt to re-interpret the current word as a space sub-phrase 
   identifier, rather than getting a new word. */
         } else {
            look_for = SPACE_IDENTIFIER;        
            new_word = 0;
         }

/* If required, construct a default TimeFrame to represent this sub-phrase. */
         if( timeid != NULL_ID ) {
            timefrm = astTimeFrame( "", status );        
            subphrase = "time";

/* Record the sub-phrase identifier property in the relevant keymap. */
            props = time_props;
            astMapPut0C( props, "ID", word, NULL );
         }

/* If we are currently looking for the space sub-phrase, see if the current
   word is any of the known space sub-phrase identifiers. Is so, move on
   to read the associated sub-phrase component. */
      } else if( look_for == SPACE_IDENTIFIER ) {
/* ------------------------------------------------------------------ */

/* We have now finished reading the time sub-phrase, so we can now
   finalise any remaining details of the TimeFrame and Region. If 
   necessary, convert the epoch value to TDB as required by AST. First
   save the current epoch value for use as the time origin. */
         time_origin = epoch;
         if( epoch != AST__BAD && ts != AST__TDB ) {
            tf1 = astCopy( timefrm );
            astSetTimeScale( tf1, AST__TDB );
            fs = astConvert( timefrm, tf1, "" );
            astTran1( fs, 1, &epoch, 1, &epoch );
            fs = astAnnul( fs );
            tf1 = astAnnul( tf1 );
         }

/* Store the epoch value in any TimeFrame. */
         if( timefrm ) {
            if( epoch != AST__BAD ) astSetEpoch( timefrm, epoch );

/* Store the TimeOrigin value in any TimeFrame, modifying the time values 
   accordingly. */
            if( time_origin != AST__BAD ) { 
               astSetTimeOrigin( timefrm, time_origin );
               if( start != AST__BAD ) start -= time_origin;
               if( stop != AST__BAD ) stop -= time_origin;
               if( time != AST__BAD ) time -= time_origin;
            }               

/* Create a suitable Region to describe the enclosure for the time coords */
            if( start != AST__BAD || stop != AST__BAD ) {
               time_enc = (AstRegion *) astInterval( timefrm, &start, &stop, 
                                                     NULL, "", status );
               use_enc = 1;

            } else if( time != AST__BAD ) {
               time_enc = (AstRegion *) SinglePointList( (AstFrame *) timefrm, 
                                                         &time, NULL, status);
            }

/* Create a suitable Region to describe the time coords contained within
   the above enclosure. If any sub-phrase has no coordinate value, then
   we cannot produce a PointList describing the complete coordinate set. */
            if( time != AST__BAD ) {
               time_co = (AstRegion *) SinglePointList( (AstFrame *) timefrm, 
                                                         &time, NULL, status);
               use_co = 1;
            }

/* Set the filling factor. */
            if( time_enc && fill != AST__BAD ) {
               astSetFillFactor( time_enc, fill );
               fill = AST__BAD;
            }

/* Associate an uncertainty with the two Regions. */
            SetUnc( time_enc, time_co, (AstFrame *) timefrm, 0, scale, err, 1, 
                    status );

/* Free the TimeFrame pointer. */
            timefrm = astAnnul( timefrm );
         }

/* Reset the err array to incicate no error values have been read for the
   new sub-phrase. */
         err[ 0 ] = AST__BAD;

/* Indicate we have finished any preceeding time sub-phrase. */
         timeid = NULL_ID;         

/* Assume that we will be moving on to read the fill factor (most space
   sub-phrases start with the fill factor ). */
         look_for = FILL_FACTOR;

/* Now check the word to see if it a known space sub-phrase identifier. */
         if( !strcmp( word, "PositionInterval" ) ) {
            spaceid = POSITION_INTERVAL_ID;

         } else if( !strcmp( word, "AllSky" ) ) {
            spaceid = ALL_SKY_ID;

         } else if( !strcmp( word, "Circle" ) ) {
            spaceid = CIRCLE_ID;

         } else if( !strcmp( word, "Ellipse" ) ) {
            spaceid = ELLIPSE_ID;

         } else if( !strcmp( word, "Box" ) ) {
            spaceid = BOX_ID;

         } else if( !strcmp( word, "Polygon" ) ) {
            spaceid = POLYGON_ID;

         } else if( !strcmp( word, "Convex" ) ) {
            spaceid = CONVEX_ID;

         } else if( !strcmp( word, "Position" ) ) {
            look_for = FRAME;              /* Move on to find the frame next */
            spaceid = POSITION_ID;

/* If the word is not a known space sub-phrase identifier, move on to 
   re-interpret it as a Spectral sub-phrase identifier. */
         } else {
            look_for = SPECTRAL_IDENTIFIER;
            new_word = 0;
         }

/* We cannot create a Frame yet, since we do not yet known whether to
   create a SkyFrame or a Frame. This will become apparent when we have read
   the "flavor" term. */

/* Indicate we have started reading information related to the space 
   sub-phrase. */
         if( spaceid != NULL_ID ) {
            subphrase = "space";

/* Record the sub-phrase identifier property in the relevant keymap. */
            props = space_props;
            astMapPut0C( props, "ID", word, NULL );
         }

/* If we are currently looking for the velocity sub-phrase, see if the current
   word is any of the known velocity sub-phrase identifiers. Is so, move on
   to read the associated sub-phrase component.  We read the velocity
   sub-phrase in order to reach the following sub-phrases, but otherwise
   ignore it since AST does not support velocity space. */
      } else if( look_for == VELOCITY_IDENTIFIER ) {
/* ------------------------------------------------------------------ */

/* We have now finished reading the space sub-phrase, so we can now
   finalise any remaining details of the space Frame and Region. Store 
   any epoch value in any space Frame. */
         if( spacefrm ) {
            if( epoch != AST__BAD ) astSetEpoch( spacefrm, epoch );

/* Create a suitable Region to enclose the space positions. This
   includes scaling the supplied axis values to the units used by 
   the Frame. */
            if( spaceid == POSITION_INTERVAL_ID ) {
               use_enc = 1;
               for( iaxis = 0; iaxis < naxes; iaxis++ ) {
                  slolim[ iaxis ] *= scale;
                  shilim[ iaxis ] *= scale;
               }   
               space_enc = (AstRegion *) astBox( spacefrm, 1, slolim, 
                                                shilim, NULL, "", status );

            } else if( spaceid == ALL_SKY_ID ) {
               use_enc = 1;
               space_enc = (AstRegion *) astNullRegion( spacefrm, NULL, 
                                                       "Negated=1", status );
   
            } else if( spaceid == CIRCLE_ID ) {
               use_enc = 1;
               for( iaxis = 0; iaxis < naxes; iaxis++ ) {
                  centre[ iaxis ] *= scale;
               }   
               radius *= scale;
               space_enc = (AstRegion *) astCircle( spacefrm, 1, centre, 
                                                   &radius, NULL, "", status );

            } else if( spaceid == ELLIPSE_ID ) {
               use_enc = 1;
               for( iaxis = 0; iaxis < naxes; iaxis++ ) {
                  centre[ iaxis ] *= scale;
               }   
               shilim[ 0 ] = radius*scale;
               shilim[ 1 ] = radius2*scale;
               if( !is_skyframe ) pa = 90.0 - pa;
               pa *= AST__DD2R;
               space_enc = (AstRegion *) astEllipse( spacefrm, 1, centre, 
                                                    shilim, &pa, NULL, "", status );

            } else if( spaceid == BOX_ID ) {
               use_enc = 1;
               for( iaxis = 0; iaxis < naxes; iaxis++ ) {
                  centre[ iaxis ] *= scale;
                  bsize[ iaxis ] *= scale;
               }   

               vertices = BoxCorners( spacefrm, centre, bsize, status );
               space_enc = (AstRegion *) astPolygon( spacefrm, 4, 4, vertices,
                                                    NULL, "", status );
               vertices = astFree( vertices );

            } else if( spaceid == POLYGON_ID ) {
               use_enc = 1;
               p = vertices;
               for( i = 0; i < nvert*naxes; i++ ) *(p++) *= scale;
               space_enc = (AstRegion *) astPolygon( spacefrm, nvert, nvert, 
                                                     vertices, NULL, " ", 
                                                     status );
               vertices = astFree( vertices );

            } else if( spaceid == POSITION_ID ) {
               for( iaxis = 0; iaxis < naxes; iaxis++ ) pos[ iaxis ] *= scale;
               space_enc = (AstRegion *) SinglePointList( spacefrm, pos, NULL, 
                                                          status );
            }

/* Create a suitable Region to describe the space coords contained within
   the above enclosure. If any sub-phrase has no coordinate value, then
   we cannot produce a PointList describing the complete coordinate set. */
            if( pos[ 0 ] != AST__BAD ) {
               for( iaxis = 0; iaxis < naxes; iaxis++ ) pos[ iaxis ] *= scale;
               space_co = (AstRegion *) SinglePointList( spacefrm, pos, 
                                                         NULL, status);
            } else {
               use_co = 0;
            }

/* Set the filling factor. */
            if( space_enc && fill != AST__BAD ) {
               astSetFillFactor( space_enc, fill );
               fill = AST__BAD;
            }

/* Associate an uncertainty with the two Regions. */
            SetUnc( space_enc, space_co, (AstFrame *) spacefrm, is_skyframe, 
                    scale, err, naxes, status );

/* Free the Frame pointer. */
            spacefrm = astAnnul( spacefrm );
         }

/* Indicate we have finished any preceededing space sub-phrase. */
         spaceid = NULL_ID;         

/* Reset the err array to incicate no error values have been read for the
   new sub-phrase. */
         err[ 0 ] = AST__BAD;

/* We read the velocity sub-phrase in order to reach the following 
   sub-phrases, but otherwise ignore it since AST does not support 
   velocity space. */
         if( !strcmp( word, "VelocityInterval" ) ) {
            velid = VELOCITY_INTERVAL_ID;
            look_for = FILL_FACTOR;
            subphrase = "velocity";
            astAddWarning( this, 1, "Ignoring unsupported VelocityInterval"
                           " sub-phrase found in an STC-S description: '%s'.", 
                           "astRead", status, ContextFragment( &con, &fbuf, 
                           status ) );

         } else {
            look_for = SPECTRAL_IDENTIFIER;
            new_word = 0;
         }


/* If we are currently looking for the spectral sub-phrase, see if the
   word is any of the known spectral sub-phrase identifiers. Is so, move 
   on to read the associated sub-phrase component. */
      } else if( look_for == SPECTRAL_IDENTIFIER ) {
/* ------------------------------------------------------------------ */

/* Finished skipping over the velocity sub-phrase. */
         velid = NULL_ID;

/* Now check the word to see if it a known spectral sub-phrase identifier. */
         if( !strcmp( word, "SpectralInterval" ) ) {
            look_for = FILL_FACTOR;         /* Move on to find the fill factor */
            specid = SPECTRAL_INTERVAL_ID;

         } else if( !strcmp( word, "Spectral" ) ) {
            look_for = REFPOS;              /* Move on to find the refpos */
            specid = SPECTRAL_ID;

/* If the word is not a known spectral sub-phrase identifier, move on to 
   look for the Redshift sub-phrase. */
         } else {
            look_for = REDSHIFT_IDENTIFIER;
            new_word = 0;
         }

/* If required, construct a default SpecFrame to represent this sub-phrase. */
         if( specid != NULL_ID ) {
            specfrm = astSpecFrame( "", status );        
            subphrase = "spectral";

/* Record the sub-phrase identifier property in the relevant keymap. */
            props = spec_props;
            astMapPut0C( props, "ID", word, NULL );
         }

/* We have not yet found an axis value for this sub-phrase. */
         value = AST__BAD;
         lolim = AST__BAD;
         hilim = AST__BAD;



/* If we are currently looking for the redshift sub-phrase, see if the
   word is any of the known redshift sub-phrase identifiers. Is so, move 
   on to read the associated sub-phrase component. */
      } else if( look_for == REDSHIFT_IDENTIFIER ) {
/* ------------------------------------------------------------------ */

/* We have now finished reading the spectral sub-phrase, so we can now
   finalise any remaining details of the spectral Frame and Region. Store 
   any epoch and standard of rest value in any spectral Frame. */
         if( specfrm ) {
            if( epoch != AST__BAD ) astSetEpoch( specfrm, epoch );
            if( sor != AST__BADSOR ) astSetStdOfRest( specfrm, sor );

/* Create a suitable Region to describe the enclosure for the spectral 
   coords */
            if( specid == SPECTRAL_INTERVAL_ID ) {
               spec_enc = (AstRegion *) astInterval( specfrm, &lolim, &hilim, 
                                                     NULL, "", status );
               use_enc = 1;
            } else if( specid == SPECTRAL_ID ) {
               spec_enc = (AstRegion *) SinglePointList( (AstFrame *) specfrm, 
                                                         &value, NULL, status);
            }

/* Create a suitable Region to describe the spectral coords contained within
   the above enclosure. If any sub-phrase has no coordinate value, then
   we cannot produce a PointList describing the complete coordinate set. */
            if( value != AST__BAD ) {
               spec_co = (AstRegion *) SinglePointList( (AstFrame *) specfrm, 
                                                         &value, NULL, status);
            } else {
               use_co = 0;
            }

/* Set the filling factor. */
            if( spec_enc && fill != AST__BAD ) {
               astSetFillFactor( spec_enc, fill );
               fill = AST__BAD;
            }

/* Associate an uncertainty with the two Regions. */
            SetUnc( spec_enc, spec_co, (AstFrame *) specfrm, 0, scale, err, 1, 
                    status );

/* Free the SpecFrame pointer. */
            specfrm = astAnnul( specfrm );
         }

/* Reset the err array to incicate no error values have been read for the
   new sub-phrase. */
         err[ 0 ] = AST__BAD;

/* Indicate we have finished any preceeding spectral sub-phrase. */
         specid = NULL_ID;         

/* Now check the word to see if it a known spectral sub-phrase identifier. */
         if( !strcmp( word, "RedshiftInterval" ) ) {
            look_for = FILL_FACTOR;       /* Move on to find the fill factor */
            redid = REDSHIFT_INTERVAL_ID;

         } else if( !strcmp( word, "Redshift" ) ) {
            look_for = REFPOS;            /* Move on to find the refpos */
            redid = REDSHIFT_ID;

/* If the word is not a known redshift sub-phrase identifier, report a
   warning. */
         } else if( word[ 0 ] && astOK ) {
            astError( AST__BADIN, "astRead(%s): Unsupported or irrelevant "
                      "word '%s' found in STC-S %s sub-phrase: '%s'.", status,
                      astGetClass( this ), word, subphrase, 
                      ContextFragment( &con, &fbuf, status ) );
         }

/* If required, construct a SpecFrame with Domain=REDSHIFT to represent 
   this sub-phrase. */
         if( redid != NULL_ID ) {
            redfrm = astSpecFrame( "Domain=REDSHIFT", status );
            subphrase = "redshift";

/* Record the sub-phrase identifier property in the relevant keymap. */
            props = red_props;
            astMapPut0C( props, "ID", word, NULL );
         }

/* We have not yet found an axis value for this sub-phrase. */
         value = AST__BAD;
         lolim = AST__BAD;
         hilim = AST__BAD;

/* Indicate we can now end when we run out of input words. */
         con.done = 1;



/* If we are currently looking for a fill factor... */
      } else if( look_for == FILL_FACTOR ) {
/* ------------------------------------------------------------------ */

/* If the current word is "fillfactor" attempt to read the numerical filling 
   factor from the next word. If this fails, or if the current word is
   not "fillfactor", indicate that we will be re-interpreting the current 
   word in a new context and so do not need a new word. */
         if( !strcmp( word, "fillfactor" ) ) {
            word = GetNextWord( this, &con, status );
            fill = astChr2Double( word );
            if( fill == AST__BAD ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected numerical "
                         "filling factor, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );
               new_word = 0;
            } 
         } else {
            new_word = 0;
         }

/* If we are reading a time sub-phrase, move on to read the timescale. */
         if( timeid != NULL_ID ) {
            look_for = TIME_SCALE;            

/* If we are reading a space sub-phrase, move on to read the frame. */
         } else if( spaceid != NULL_ID ) {
            look_for = FRAME;            

/* If we are reading a velocity sub-phrase, move on to read the limits. */
         } else if( velid != NULL_ID ) {
            look_for = LIMITS;

/* Otherwise (i.e. for spectral and redshift sub-phrases) move on to read 
   the refpos. */
         } else {
            look_for = REFPOS;            
         }

/* If the word was usable, record it as the fillfactor property. */
         if( new_word ) astMapPut0C( props, "FILLFACTOR", word, NULL );



/* If we are currently looking for a time scale... */
      } else if( look_for == TIME_SCALE ) {
/* ------------------------------------------------------------------ */

/* Consider each supported Time scale. */
         ts = AST__BADTS;
         new_ts = NULL;
         level = 3;

         if( !strcmp( word, "TT" ) ) {
            ts = AST__TT;

         } else if( !strcmp( word, "TDT" ) ) {
            ts = AST__TT;
            new_ts = "TT";

         } else if( !strcmp( word, "ET" ) ) {
            ts = AST__TT;
            new_ts = "TT";

         } else if( !strcmp( word, "TAI" ) ) {
            ts = AST__TAI;

         } else if( !strcmp( word, "IAT" ) ) {
            ts = AST__TAI;
            new_ts = "TAI";

         } else if( !strcmp( word, "UTC" ) ) {
            ts = AST__UTC;

         } else if( !strcmp( word, "TEB" ) ) {
            ts = AST__TDB;
            new_ts = "TDB";
            level = 1;

         } else if( !strcmp( word, "TDB" ) ) {
            ts = AST__TDB;

         } else if( !strcmp( word, "TCG" ) ) {
            ts = AST__TCG;

         } else if( !strcmp( word, "TCB" ) ) {
            ts = AST__TCB;

         } else if( !strcmp( word, "LST" ) ) {
            ts = AST__LMST;

         } else if( !strcmp( word, "nil" ) ) {
            astAddWarning( this, 2, "Time scale defaulting to 'TAI' in an "
                           "STC-S description: '%s'.", "astRead", status, 
                           ContextFragment( &con, &fbuf, status ) );

/* If the current word did not look like a timescale, indicate that it
   should be re-considered as the next allowed element. */
         } else {
            astAddWarning( this, 2, "Time scale defaulting to 'TAI' in an "
                           "STC-S description: '%s'.", "astRead", status, 
                           ContextFragment( &con, &fbuf, status ) );
            new_word = 0;
         }

/* Issue a warning if a different time-scale was substituted for the supplied 
   time-scale. */
         if( new_ts ) {
            astAddWarning( this, level, "'%s' being used in place of "
                           "unsupported time scale '%s' found in STC-S "
                           "description: '%s'.", "astRead", status, new_ts, 
                           word, ContextFragment( &con, &fbuf, status ) );
         }

/* If we got a time scale, set the TimeScale attribute in the TimeFrame
   to the same value. */
         if( ts != AST__BADTS ) astSetTimeScale( timefrm, ts );

/* Move on to look for a refpos */
         look_for = REFPOS;

/* If the word was usable, record it as the timescale property. */
         if( new_word ) astMapPut0C( props, "TIMESCALE", word, NULL );



/* If we are currently looking for a space frame... */
      } else if( look_for == FRAME ) {
/* ------------------------------------------------------------------ */

/* Consider each supported space frame. Report an error for frames
   not supported by AST. */
         sys = AST__UNKNOWN;
         
         if( !strcmp( word, "ICRS" ) ) {
            sys = AST__ICRS;

         } else if( !strcmp( word, "FK5" ) ) {
            sys = AST__FK5;

         } else if( !strcmp( word, "FK4" ) ) {
            sys = AST__FK4;

         } else if( !strcmp( word, "J2000" ) ) {
            sys = AST__FK5;

         } else if( !strcmp( word, "B1950" ) ) {
            sys = AST__FK4;

         } else if( !strcmp( word, "ECLIPTIC" ) ) {
            sys = AST__ECLIPTIC;

         } else if( !strcmp( word, "GALACTIC" ) ) {
            sys = AST__GALACTIC;

         } else if( !strcmp( word, "GALACTIC_II" ) ) {
            sys = AST__GALACTIC;

         } else if( !strcmp( word, "SUPER_GALACTIC" ) ) {
            sys = AST__SUPERGALACTIC;

         } else if( !strcmp( word, "UNKNOWNFrame" ) ) {
            sys = AST__UNKNOWN;

         } else if( !strcmp( word, "GEO_C" ) ||
                    !strcmp( word, "GEO_D" ) ){
            astAddWarning( this, 1, "'UNKNOWNFrame' being used in place of "
                           "unsupported frame '%s' in an STC-S description: '%s'.", 
                           "astRead", status, word, 
                           ContextFragment( &con, &fbuf, status ) );

/* If the current word did not look like a frame, indicate that it
   should be re-considered as the next allowed element. */
         } else {
            new_word = 0;
         }

/* Move on to look for a refpos */
         look_for = REFPOS;

/* Save the frame name for later use. */
         frame = astStore( frame, word, strlen( word ) + 1 );

/* If the word was usable, record it as the FRAME property. */
         if( new_word ) astMapPut0C( props, "FRAME", word, NULL );



/* If we are currently looking for a refpos... */
      } else if( look_for == REFPOS ) {
/* ------------------------------------------------------------------ */

/* First consider refpos for time frames. The AST TimeFrame class has no
   reference position, so allow any reference position but issue a warning 
   for anything other than "TOPOCENTER" and "UNKNOWNRefPos". */
         if( timeid != NULL_ID ) {
            if( !strcmp( word, "TOPOCENTER" ) ) {

            } else if( !strcmp( word, "UNKNOWNRefPos" ) ) {
               astAddWarning( this, 1, "'TOPOCENTER' being used in place of "
                              "time 'UNKNOWNRefPos' in an STC-S description: '%s'.", 
                              "astRead", status, ContextFragment( &con, &fbuf, status ) );

            } else if( !strcmp( word, "HELIOCENTER" ) ||
                       !strcmp( word, "BARYCENTER" ) ||
                       !strcmp( word, "GEOCENTER" ) ||
                       !strcmp( word, "GALACTIC_CENTER" ) ||
                       !strcmp( word, "EMBARYCENTER" ) ||
                       !strcmp( word, "MOON" ) ||
                       !strcmp( word, "MERCURY" ) ||
                       !strcmp( word, "VENUS" ) ||
                       !strcmp( word, "MARS" ) ||
                       !strcmp( word, "JUPITER" ) ||
                       !strcmp( word, "SATURN" ) ||
                       !strcmp( word, "URANUS" ) ||
                       !strcmp( word, "NEPTUNE" ) ||
                       !strcmp( word, "PLUTO" ) ){
               astAddWarning( this, 1, "Unsupported time reference position '%s' "
                              "found in STC-S description. Using 'TOPOCENTER' "
                              "instead: '%s'.", "astRead", status, word, 
                              ContextFragment( &con, &fbuf, status ) );

/* If the current word did not look like a refpos, indicate that it
   should be re-considered as the next allowed element. */
            } else {
               astAddWarning( this, 2, "Time reference position defaulting to "
                        "'TOPOCENTER' in an STC-S description: '%s'.", "astRead", 
                        status, ContextFragment( &con, &fbuf, status ) );
               new_word = 0;
            } 

/* Choose what to look for next on the basis of the type of sub-phrase
   currently being interpreted. */
            if( timeid == TIME_INTERVAL_ID ){
               look_for = START;   /* Move on to find the start time */

            } else if( timeid == START_TIME_ID ){
               look_for = START;   /* Move on to find the start time */

            } else if( timeid == STOP_TIME_ID ){
               look_for = STOP;    /* Move on to find the stop time */

            } else {
               look_for = TIME;    /* Move on to find the time */
            }

/* Next consider refpos for space frames. The AST Frame and SkyFrame
   classes have no reference position, so for SkyFrames we consider 
   "TOPOCENTER" and "UNKNOWN" acceptable and all other unsupported. 
   For other Frames we allow any reference position. */
         } else if( spaceid != NULL_ID ) {

            if( sys != AST__UNKNOWN ) {
               if( !strcmp( word, "TOPOCENTER" ) ) {

               } else if( !strcmp( word, "UNKNOWNRefPos" ) ) {
                  astAddWarning( this, 1, "'TOPOCENTER' being used in place "
                                 "of space 'UNKNOWNRefPos' in an STC-S "
                                 "description: '%s'.", "astRead", status, 
                                 ContextFragment( &con, &fbuf, status ) );

               } else if( !strcmp( word, "HELIOCENTER" ) ||
                          !strcmp( word, "BARYCENTER" ) ||
                          !strcmp( word, "GEOCENTER" ) ||
                          !strcmp( word, "GALACTIC_CENTER" ) ||
                          !strcmp( word, "EMBARYCENTER" ) ||
                          !strcmp( word, "MOON" ) ||
                          !strcmp( word, "MERCURY" ) ||
                          !strcmp( word, "VENUS" ) ||
                          !strcmp( word, "MARS" ) ||
                          !strcmp( word, "JUPITER" ) ||
                          !strcmp( word, "SATURN" ) ||
                          !strcmp( word, "URANUS" ) ||
                          !strcmp( word, "NEPTUNE" ) ||
                          !strcmp( word, "PLUTO" ) ){
                  astAddWarning( this, 1, "'TOPOCENTER' being used in place of "  
                                 "unsupported space reference 'position '%s' "
                                 "in an STC-S description: '%s'.", "astRead", status,
                                 word, ContextFragment( &con, &fbuf, status ) );
               } else {
                  astAddWarning( this, 2, "Space reference position defaulting"
                                 " to 'TOPOCENTER' in an STC-S description: '%s'.", 
                                 "astRead", status, ContextFragment( &con, &fbuf, status ) );
                  new_word = 0;
               }

            } else {
               if( !strcmp( word, "TOPOCENTER" ) ||
                   !strcmp( word, "UNKNOWNRefPos" ) ||
                   !strcmp( word, "HELIOCENTER" ) ||
                   !strcmp( word, "BARYCENTER" ) ||
                   !strcmp( word, "GEOCENTER" ) ||
                   !strcmp( word, "GALACTIC_CENTER" ) ||
                   !strcmp( word, "EMBARYCENTER" ) ||
                   !strcmp( word, "MOON" ) ||
                   !strcmp( word, "MERCURY" ) ||
                   !strcmp( word, "VENUS" ) ||
                   !strcmp( word, "MARS" ) ||
                   !strcmp( word, "JUPITER" ) ||
                   !strcmp( word, "SATURN" ) ||
                   !strcmp( word, "URANUS" ) ||
                   !strcmp( word, "NEPTUNE" ) ||
                   !strcmp( word, "PLUTO" ) ){
                  astAddWarning( this, 1, "Ignoring space reference position '%s'"
                                " found in an STC-S description: '%s'.", "astRead", 
                                status, word, ContextFragment( &con, &fbuf, status ) );
               } else {
                  new_word = 0;
               }
            }

/* Move on to look for the cooridnate system flavour. */
            look_for = FLAVOUR;       

/* Next consider refpos for spectral and redshift frames. The AST SpecFrame 
   uses the "StdOfRest" (standard of rest) attribute to store the
   equivalent information. */
         } else{
            t = ( specid != NULL_ID ) ? "Spectral" : "Redshift";
            sor = AST__BADSOR;

            if( !strcmp( word, "GEOCENTER" ) ) {
               sor = AST__GESOR;

            } else if( !strcmp( word, "BARYCENTER" ) ) {
               sor = AST__BYSOR;

            } else if( !strcmp( word, "HELIOCENTER" ) ) {
               sor = AST__HLSOR;

            } else if( !strcmp( word, "TOPOCENTER" ) ) {
               sor = AST__TPSOR;

            } else if( !strcmp( word, "LSR" ) ||
                       !strcmp( word, "LSRK" ) ) {
               sor = AST__LKSOR;

            } else if( !strcmp( word, "LSRD" ) ) {
               sor = AST__LDSOR;

            } else if( !strcmp( word, "GALACTIC_CENTER" ) ) {
               sor = AST__GLSOR;

            } else if( !strcmp( word, "UNKNOWNRefPos" ) ) {
               sor = AST__BADSOR;
               astAddWarning( this, 1, "'HELIOCENTER' being used in place "
                              "of %s 'UNKNOWNRefPos' in an STC-S description: '%s'.",
                              "astRead", status, t, ContextFragment( &con, &fbuf, status ) );

            } else if( !strcmp( word, "LOCAL_GROUP_CENTER" ) ||
                       !strcmp( word, "EMBARYCENTER" ) ||
                       !strcmp( word, "MOON" ) ||
                       !strcmp( word, "MERCURY" ) ||
                       !strcmp( word, "VENUS" ) ||
                       !strcmp( word, "MARS" ) ||
                       !strcmp( word, "JUPITER" ) ||
                       !strcmp( word, "SATURN" ) ||
                       !strcmp( word, "URANUS" ) ||
                       !strcmp( word, "NEPTUNE" ) ||
                       !strcmp( word, "PLUTO" ) ) {
               astAddWarning( this, 1, "Using 'HELIOCENTER' in place of "
                              "unsupported %s reference position '%s' "
                              "found in an STC-S description: '%s'.", "astRead", 
                              status, t, word, ContextFragment( &con, &fbuf, status ) );

/* If the current word did not look like a refpos, indicate that it
   should be re-considered as the next allowed element. */
            } else {
               astAddWarning( this, 2, "%s reference position defaulting to "
                              "'HELIOCENTER' in an STC-S description: '%s'.", 
                              "astRead", status, t, ContextFragment( &con, &fbuf, status ) );
               new_word = 0;
            } 

/* Choose what to look for next on the basis of the type of sub-phrase
   currently being interpreted. */
            if( specid == SPECTRAL_INTERVAL_ID ) {
               look_for = LIMITS;       /* Move on to find the spectral limits */

            } else if( specid == SPECTRAL_ID ) {
               look_for = RED_SPEC_VALUE; /* Move on to find the spectral value */

            } else if( redid == REDSHIFT_INTERVAL_ID ) {
               look_for = TYPE_DOPPLER;   /* Move on to find the redshift type */

            } else {  /* REDSHIFT */
               look_for = RED_SPEC_VALUE; /* Move on to find the redshift value */
            }
         }

/* If the word was usable, record it as the REFPOS property. */
         if( new_word ) astMapPut0C( props, "REFPOS", word, NULL );




/* If we are currently looking for a start time... */
      } else if( look_for == START ) {
/* ------------------------------------------------------------------ */

/* Save the current word as the start of the START value. */
         nc = 0;
         prop = astAppendString( prop, &nc, word );

/* If the current word is "JD", the following word should be a numerical
   Julian date. */
         if( !strcmp( word, "JD" ) ) {
            word = GetNextWord( this, &con, status );
            start = astChr2Double( word );
            if( start == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected numerical "
                         "JD Start time, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );

/* Convert JD time to MJD. */
            } else {
               start -= 2400000.5;

/* Append the second word to the first word. */
               prop = astAppendString( prop, &nc, " " );
               prop = astAppendString( prop, &nc, word );
            }

/* If the current word is "MJD", the following word should be a numerical
   MJD. */
         } else if( !strcmp( word, "MJD" ) ) {
            word = GetNextWord( this, &con, status );
            start = astChr2Double( word );
            if( start == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected numerical "
                         "MJD Start time, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );

/* Append the second word to the first word. */
            } else {
               prop = astAppendString( prop, &nc, " " );
               prop = astAppendString( prop, &nc, word );
            }

/* Otherwise, the current word should be an ISO date. Use the TimeFrame
   to parse the string, producing an MJD (the default System for a
   TimeFrame is MJD). */
         } else if( !astUnformat( timefrm, 0, word, &start ) && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected ISO date "
                      "string Start time, but found '%s' in an STC-S "
                      "description: '%s'.", status, word, 
                      ContextFragment( &con, &fbuf, status ) );
         }

/* We will use the start time as the epoch (if no other value has already
   been set). */
         if( epoch == AST__BAD ) epoch = start;

/* Decide what to do next. */
         if( timeid == TIME_INTERVAL_ID ){
            look_for = STOP;        /* Move on to find the stop time */

         } else if( timeid == START_TIME_ID ){
            look_for = TIME_LABEL;  /* Move on to find the "coord" time */

         } 

/* Record the START property. */
         astMapPut0C( props, "START", prop, NULL );



/* If we are currently looking for a stop time... */
      } else if( look_for == STOP ) {
/* ------------------------------------------------------------------ */

/* Save the current word as the start of the STOP value. */
         nc = 0;
         prop = astAppendString( prop, &nc, word );

/* If the current word is "JD", the following word should be a numerical
   Julian date. */
         if( !strcmp( word, "JD" ) ) {
            word = GetNextWord( this, &con, status );
            stop = astChr2Double( word );
            if( stop == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected numerical "
                         "JD Stop time, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );

/* Convert JD time to MJD. */
            } else {
               stop -= 2400000.5;

/* Append the second word to the first word. */
               prop = astAppendString( prop, &nc, " " );
               prop = astAppendString( prop, &nc, word );
            }

/* If the current word is "MJD", the following word should be a numerical
   MJD. */
         } else if( !strcmp( word, "MJD" ) ) {
            word = GetNextWord( this, &con, status );
            stop = astChr2Double( word );
            if( stop == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected numerical "
                         "MJD Stop time, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );

/* Append the second word to the first word. */
            } else {
               prop = astAppendString( prop, &nc, " " );
               prop = astAppendString( prop, &nc, word );
            }

/* Otherwise, the current word should be an ISO date. Use the TimeFrame
   to parse the string, producing an MJD (the default System for a
   TimeFrame is MJD). */
         } else if( !astUnformat( timefrm, 0, word, &stop ) && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected ISO date "
                      "string Stop time, but found '%s' in an STC-S "
                      "description: '%s'.", status, word, 
                      ContextFragment( &con, &fbuf, status ) );
         }

/* We will use the stop time as the epoch (if no other value has already
   been set). */
         if( epoch == AST__BAD ) epoch = stop;

/* Move on to find the "coord" time. */
         look_for = TIME_LABEL; 

/* Record the STOP property. */
         astMapPut0C( props, "STOP", prop, NULL );



/* If we are currently looking for the label before a time coord value... */
      } else if( look_for == TIME_LABEL ) {
/* ------------------------------------------------------------------ */
         if( !strcmp( word, "Time" ) ) {
            look_for = TIME;
         } else {
            new_word = 0;
            look_for = UNIT;
         }



/* If we are currently looking for a time... */
      } else if( look_for == TIME ) {
/* ------------------------------------------------------------------ */

/* Save the current word as the start of the TIME value. */
         nc = 0;
         prop = astAppendString( prop, &nc, word );

/* If the current word is "JD", the following word should be a numerical
   Julian date. */
         if( !strcmp( word, "JD" ) ) {
            word = GetNextWord( this, &con, status );
            time = astChr2Double( word );
            if( time == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected a numerical "
                         "JD time value, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );

/* Convert JD time to MJD. */
            } else {
               time -= 2400000.5;
            }

/* If the current word is "MJD", the following word should be a numerical
   MJD. */
         } else if( !strcmp( word, "MJD" ) ) {
            word = GetNextWord( this, &con, status );
            time = astChr2Double( word );
            if( time == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected a numerical "
                         "MJD time value, but found '%s' in an STC-S "
                         "description: '%s'.", status, word, 
                         ContextFragment( &con, &fbuf, status ) );
            }

/* Otherwise, the current word should be an ISO date. Use the TimeFrame
   to parse the string, producing an MJD (the default System for a
   TimeFrame is MJD). */
         } else if( !astUnformat( timefrm, 0, word, &time ) && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected an ISO "
                      "date string, but found '%s' in an STC-S "
                      "description: '%s'.", status, word, 
                      ContextFragment( &con, &fbuf, status ) );
         }

/* We will use the time as the epoch (if no other value has already
   been set). */
         if( epoch == AST__BAD ) epoch = time;

/* Move on to look for the units. */
         look_for = UNIT;

/* Append the second word to the first word, and record it as the TIME
   property. */
         prop = astAppendString( prop, &nc, " " );
         prop = astAppendString( prop, &nc, word );
         astMapPut0C( props, "TIME", prop, NULL );


/* If we are currently looking for a space "flavor"... */
      } else if( look_for == FLAVOUR ) {
/* ------------------------------------------------------------------ */
         is_skyframe = 0;

         if( !strcmp( word, "SPHER2" ) ) {
            spacefrm = (AstFrame *) astSkyFrame( "", status );
            is_skyframe = 1;

         } else if( !strcmp( word, "CART1" ) ) {
            spacefrm = astFrame( 1, "", status );

         } else if( !strcmp( word, "CART2" ) ) {
            spacefrm = astFrame( 2, "", status );

         } else if( !strcmp( word, "CART3" ) ) {
            spacefrm = astFrame( 3, "", status );

         } else if( !strcmp( word, "UNITSPHERE" ) ||
                    !strcmp( word, "SPHER3" ) ){
            astError( AST__BADIN, "astRead(StcsChan): Unsupported "
                      "space 'Flavor' (%s) found in STC-S description: '%s'.", 
                      status, word, ContextFragment( &con, &fbuf, status ) );
         } else {
            spacefrm = (AstFrame *) astSkyFrame( "", status );
            is_skyframe = 1;
            new_word = 0;
         }

/* Basic Frames can only have the System "UNKNOWNFrame". */
         if( new_word && !is_skyframe && sys != AST__UNKNOWN && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Unsupported "
                      "combination of space 'Flavor' (%s) and 'Frame' (%s)"
                      "found in STC-S description: '%s'.", status, word,
                      frame, ContextFragment( &con, &fbuf, status ) );
         }

/* Now we have a space frame, we can set the System. */
         if( is_skyframe ) astSetSystem( spacefrm, sys );

/* If we have a basic Frame, or a SkyFrame with an unknonw System, set
   the Domain equal to the STC-S frame value. */
         if( !is_skyframe || sys == AST__UNKNOWN ) {
            astSetDomain( spacefrm, frame );
         }

/* Move on to the nest look_for. */
         naxes = astGetNaxes( spacefrm );

         if( spaceid == POSITION_INTERVAL_ID ) {
            look_for = POSITION_INTERVAL;

         } else if( spaceid == ALL_SKY_ID ) {
            look_for = ALLSKY;

         } else if( spaceid == CIRCLE_ID ) {
            look_for = CIRCLE;

         } else if( spaceid == ELLIPSE_ID ) {
            look_for = ELLIPSE;

         } else if( spaceid == BOX_ID ) {
            look_for = BOX;

         } else if( spaceid == POLYGON_ID ) {
            look_for = POLYGON;

         } else if( spaceid == CONVEX_ID ) {
            look_for = CONVEX;

         } else {   /* POSITION */
            look_for = POSITION;
         }         

/* If the word was usable, record it as the FLAVOR and FLAVOUR property. */
         if( new_word ) {
            astMapPut0C( props, "FLAVOR", word, NULL );
            astMapPut0C( props, "FLAVOUR", word, NULL );
         }


/* If we are currently looking for interval "lolimit"and "hilimit" ... */
      } else if( look_for == LIMITS ) {
/* ------------------------------------------------------------------ */
         if( velid != NULL_ID ) {
            t = "velocity";
            look_for = VELOCITY_VALUE;

         } else if( specid != NULL_ID ) {
            t = "spectral";
            look_for = RED_SPEC_LABEL;

         } else {
            t = "redshift";
            look_for = RED_SPEC_LABEL;
         }

/* The current word should be a numerical value (the low limit ). */
         lolim = astChr2Double( word );
         if( lolim == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected a numerical "
                      "value for a %s lolimit, but found '%s' in an STC-S "
                      "description: '%s'.", status, t, word, 
                      ContextFragment( &con, &fbuf, status ) );
         } else {
            astMapPut0C( props, "LOLIMIT", word, NULL );
         }

/* The next word should be a numerical value (the high limit ). */
         word = GetNextWord( this, &con, status );
         hilim = astChr2Double( word );
         if( hilim == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected a numerical "
                      "value for a %s hilimit, but found '%s' in an STC-S "
                      "description: '%s'.", status, t, word, 
                      ContextFragment( &con, &fbuf, status ) );
         } else {
            astMapPut0C( props, "HILIMIT", word, NULL );
         }



/* If we are currently looking for the label before a spectral or redshift 
   value... */
      } else if( look_for == RED_SPEC_LABEL ) {
/* ------------------------------------------------------------------ */
         if( specid != NULL_ID && !strcmp( word, "Spectral" ) ) {
            look_for = RED_SPEC_VALUE;

         } else if( redid != NULL_ID && !strcmp( word, "Redshift" ) ) {
            look_for = RED_SPEC_VALUE;

         } else {
            new_word = 0;
            look_for = UNIT;
         }



/* If we are currently looking for an spectral or redshift value. */
      } else if( look_for == RED_SPEC_VALUE ) {
/* ------------------------------------------------------------------ */

         t = ( specid != NULL_ID ) ? "spectral" : "redshift";
         value = astChr2Double( word );
         if( value == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected a numerical "
                      "%s value, but found '%s' in an STC-S "
                      "description: '%s'.", status, t, word, 
                      ContextFragment( &con, &fbuf, status ) );
         } else {
            astMapPut0C( props, ( specid != NULL_ID ) ? "SPECTRAL" : "REDSHIFT", 
                         word, NULL );
         }

/* Decide what to do next. */
         if( specid != NULL_ID ) {
            look_for = UNIT;
         } else if( redid == REDSHIFT_INTERVAL_ID ) {
            look_for = UNIT;
         } else {
            look_for = TYPE_DOPPLER;
         }



/* If we are currently looking for information needed to create a spatial
   Interval... */
      } else if( look_for == POSITION_INTERVAL ) {
/* ------------------------------------------------------------------ */

/* Get a lolimit value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            slolim[ iaxis ] = astChr2Double( word );
            if( slolim[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                      "'lolimit' value for a PositionInterval, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "LOLIMIT", prop, NULL );
         
/* Get a hilimit value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            shilim[ iaxis ] = astChr2Double( word );
            if( shilim[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                      "'hilimit' value for a PositionInterval, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "HILIMIT", prop, NULL );
         
/* Move on to read the "Position" item. */
         new_word = 0;
         look_for = POSITION_LABEL;




/* If we are currently looking for information needed to create a spatial
   AllSky ... */
      } else if( look_for == ALLSKY ) {
/* ------------------------------------------------------------------ */
         new_word = 0;
         look_for = POSITION_LABEL;



/* If we are currently looking for information needed to create a spatial
   Circle ... */
      } else if( look_for == CIRCLE ) {
/* ------------------------------------------------------------------ */

/* Get a centre value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            centre[ iaxis ] = astChr2Double( word );
            if( centre[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                      "'centre' value for a Circle, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "CENTRE", prop, NULL );

/* Get the radius value . */
         radius = astChr2Double( word );
         if( radius == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected a radius "
                      "value for a Circle, but found '%s' in an STC-S "
                      "description: '%s'.", status, word, 
                      ContextFragment( &con, &fbuf, status ) );
         }

/* Store the property value in the KeyMap. */
         astMapPut0C( props, "RADIUS", word, NULL );

/* Move on to read the "Position" item. */
         look_for = POSITION_LABEL;



/* If we are currently looking for information needed to create a spatial
   Ellipse ... */
      } else if( look_for == ELLIPSE ) {
/* ------------------------------------------------------------------ */

/* Get a centre value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            centre[ iaxis ] = astChr2Double( word );
            if( centre[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                      "centre value for an Ellipse, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "CENTRE", prop, NULL );

/* Get the first radius value . */
         radius = astChr2Double( word );
         if( radius == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected the first "
                      "radius value for an Ellipse, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
         }
         word = GetNextWord( this, &con, status );

/* Store the property value in the KeyMap. */
         astMapPut0C( props, "RADIUS1", word, NULL );

/* Get the second radius value . */
         radius2 = astChr2Double( word );
         if( radius2 == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected the second "
                      "radius value for an Ellipse, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
         }
         word = GetNextWord( this, &con, status );

/* Store the property value in the KeyMap. */
         astMapPut0C( props, "RADIUS2", word, NULL );

/* Get the position angle value. */
         pa = astChr2Double( word );
         if( pa == AST__BAD && astOK ) {
            astError( AST__BADIN, "astRead(StcsChan): Expected the position "
                      "angle value for an Ellipse, but found "
                      "'%s' in an STC-S description: '%s'.", status, word,
                      ContextFragment( &con, &fbuf, status ) );
         }

/* Store the property value in the KeyMap. */
         astMapPut0C( props, "POSANGLE", word, NULL );

/* Move on to read the "Position" item. */
         look_for = POSITION_LABEL;



/* If we are currently looking for information needed to create a spatial
   Box ... */
      } else if( look_for == BOX ) {
/* ------------------------------------------------------------------ */

/* Get a centre value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            centre[ iaxis ] = astChr2Double( word );
            if( centre[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                         "centre value for a Box, but found "
                         "'%s' in an STC-S description: '%s'.", status,
                         word, ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "CENTRE", prop, NULL );

/* Get bsize value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            bsize[ iaxis ] = astChr2Double( word );
            if( bsize[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                         "'bsize' value for a Box, but found "
                         "'%s' in an STC-S description: '%s'.", status,
                         word, ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "BSIZE", prop, NULL );

/* Move on to read the "Position" item. */
         new_word = 0;
         look_for = POSITION_LABEL;



/* If we are currently looking for information needed to create a spatial
   Polygon ... */
      } else if( look_for == POLYGON ) {
/* ------------------------------------------------------------------ */

/* Read the first vertex into a dynamically allocated array. */
         temp = astMalloc( sizeof( *temp )*naxes );
         if( temp ) {
            nc = 0;
            p = temp;
            for( iaxis = 0; iaxis < naxes; iaxis++,p++ ) {
               axval = astChr2Double( word );
               if( axval == AST__BAD && astOK ) {
                  astError( AST__BADIN, "astRead(StcsChan): Expected another "
                         "vertex value for a Polygon, but found "
                         "'%s' in an STC-S description: '%s'.", status,
                         word, ContextFragment( &con, &fbuf, status ) );
               } else {
                  *p = axval;
               }
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
               word = GetNextWord( this, &con, status );
            }

/* Loop round reading remaining vertices, expanding the array as needed. */
            nvert = 1;
            axval = astChr2Double( word );
            while( axval != AST__BAD && astOK ) {

               temp = astGrow( temp, naxes*( nvert + 1 ), sizeof( *temp ) );
               if( astOK ) {
                  p = temp + naxes*nvert;

                  for( iaxis = 0; iaxis < naxes; iaxis++, p++ ) {
                     if( axval == AST__BAD && astOK ) {
                        astError( AST__BADIN, "astRead(StcsChan): Expected "
                                  "another vertex value for a Polygon, but "
                                  "found '%s' in an STC-S description: '%s'.", 
                                  status, word, ContextFragment( &con, &fbuf, 
                                                                 status ) );
                     } else {
                        *p = axval;
                     }
                     prop = astAppendString( prop, &nc, word );
                     prop = astAppendString( prop, &nc, " " );
                     word = GetNextWord( this, &con, status );
                     axval = astChr2Double( word );
                  }
                  nvert++;
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "VERTICES", prop, NULL );

/* The "temp" array holds the vertex axis values in the wrong order
   for the AstPolygon constructor. Therefore, transpose the array. */
            vertices = astMalloc( sizeof( *vertices )*naxes*nvert );
            if( astOK ) {
               p = temp;               
               for( ivert = 0; ivert < nvert; ivert++ ) {
                  for( iaxis = 0; iaxis < naxes; iaxis++,p++ ) {
                     vertices[ iaxis*nvert + ivert ] = *p;
                  }
               }
            }
            temp = astFree( temp );
         }

/* Move on to read the "Position" item. */
         new_word = 0;
         look_for = POSITION_LABEL;



/* If we are currently looking for information needed to create a spatial
   Convex ... */
      } else if( look_for == CONVEX ) {
/* ------------------------------------------------------------------ */
         astError( AST__BADIN, "astRead(StcsChan): A Convex was found "
                   "within an STC-S description ('Convex' regions "
                   "are not yet supported by AST): %s", status,
                   ContextFragment( &con, &fbuf, status ) );



/* If we are currently looking for information needed to create a spatial
   Position ... */
      } else if( look_for == POSITION ) {
/* ------------------------------------------------------------------ */

/* Get a value for every space axis. */
         nc = 0;
         for( iaxis = 0; iaxis < naxes; iaxis++ ) {
            pos[ iaxis ] = astChr2Double( word );
            if( pos[ iaxis ] == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected another "
                         "axis value for a space Position, but found "
                         "'%s' in an STC-S description: '%s'.", status,
                         word, ContextFragment( &con, &fbuf, status ) );
            }
            prop = astAppendString( prop, &nc, word );
            prop = astAppendString( prop, &nc, " " );
            word = GetNextWord( this, &con, status );
         }

/* Remove the trailing space, and store the property value in the KeyMap. */
         prop[ nc - 1 ] = 0;
         astMapPut0C( props, "POSITION", prop, NULL );

/* Move on to read the "unit" item. */
         new_word = 0;
         look_for = UNIT;



/* If we are currently looking for the label before a space position ... */
      } else if( look_for == POSITION_LABEL ) {
/* ------------------------------------------------------------------ */
         if( !strcmp( word, "Position" ) ) {
            look_for = POSITION;
         } else {
            new_word = 0;
            look_for = UNIT;
         }


/* If we are currently looking for the redshift type and doppler
   definition ... */
      } else if( look_for == TYPE_DOPPLER ) {
/* ------------------------------------------------------------------ */

/* First determine the redshift type (redshift or velocity). */
         isvel = 1;
         if( !strcmp( word, "REDSHIFT" ) ) {
            isvel = 0;
         } else if( strcmp( word, "VELOCITY" ) ) {
            new_word = 0;
         }

/* If the current word was a valid TYPE value, store it as a property in
   the relevant KeyMap and get the next word. */
         if( new_word ) {
            astMapPut0C( props, "TYPE", word, NULL );
            word = GetNextWord( this, &con, status );
         }

/* Now get the velocity definition, and decide on the equivalent SpecFrame
   System value. AST only supports optical redshift, so report an error
   or a warning for unsupported combinations. */
         if( !strcmp( word, "OPTICAL" ) ) {
            if( isvel ) {
               astSetSystem( redfrm, AST__VOPTICAL );

            } else {
               astSetSystem( redfrm, AST__REDSHIFT );
            }

         } else if( !strcmp( word, "RADIO" ) ) {
            if( isvel ) {
               astSetSystem( redfrm, AST__VRADIO );
               astAddWarning( this, 1, "STC-S RADIO redshift not supported. "
                              "Assuming OPTICAL redshift instead: '%s'.",
                              "astRead", status, 
                              ContextFragment( &con, &fbuf, status ) );
            }

         } else if( !strcmp( word, "RELATIVISTIC" ) ) {
            if( isvel ) {
               astSetSystem( redfrm, AST__VREL );

            } else {
               astAddWarning( this, 1, "STC-S RELATIVISTIC redshift not "
                              "supported. Assuming OPTICAL redshift instead: '%s'.", 
                              "astRead", status, ContextFragment( &con, &fbuf, status ) );
            }

         } else {
            if( isvel ) {
               astSetSystem( redfrm, AST__VOPTICAL );

            } else {
               astSetSystem( redfrm, AST__REDSHIFT );
            }
            new_word = 0;
         }

/* If the word was usable, record it as the DOPPLERDEF property. */
         if( new_word ) astMapPut0C( props, "DOPPLERDEF", word, NULL );

/* Decide what to do next. */
         look_for = ( redid == REDSHIFT_INTERVAL_ID ) ? LIMITS : UNIT;



/* If we are currently looking for a "Velocity" value... */
      } else if( look_for == VELOCITY_VALUE ) {
/* ------------------------------------------------------------------ */

/* We do not use the velocity info for anything, but we still need to read 
   it in order to reach the following spectral sub-phrase. */
         if( !strcmp( word, "Velocity" ) ) {
            word = GetNextWord( this, &con, status );
            if( astChr2Double( word ) == AST__BAD && astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected a "
                         "numerical value but found '%s' after 'Velocity' "
                         "in an STC-S description: '%s'.", status, word,
                         ContextFragment( &con, &fbuf, status ) );
            }


         } else {
            new_word = 0;
         }
         look_for = UNIT;



/* If we are currently looking for an "Error" string... */
      } else if( look_for == ERROR ) {
/* ------------------------------------------------------------------ */

/* If the current word is "Error" we use the following numerical error
   values. */
         if( !strcmp( word, "Error" ) ) {

/* Determine how many numerical values should be read. Note, AST currently
   does not support error ranges (only error values), so we only read one
   numerical value per axis. */
            if( spaceid != NULL_ID ) {
               nerror = naxes;
            } else {
               nerror = 1;
            }

/* Read the subsequent numerical values and store in the "err" array. */
            nc = 0;
            for( i = 0; i < nerror; i++ ) {
               word = GetNextWord( this, &con, status );
               err[ i ] = astChr2Double( word );
               if( err[ i ] == AST__BAD ) break;
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
            }

/* If an error value was found, duplicate the last value as required. */
            if( i > 0 ) {
               if( i < nerror ) {
                  for( ; i < nerror; i++ ) err[ i ] = err[ i - 1 ];
               } else {
                  word = GetNextWord( this, &con, status );
               }

            } else if( astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Expected a "
                         "numerical %s error value but found '%s' within "
                         "an STC-S description: '%s'.", status, subphrase,
                         word, ContextFragment( &con, &fbuf, status ) );
            }

/* Read and count any following numerical values. If any are found,
   we are reading an error range rather than an error value. Issue a 
   warning that the upper error bounds will be ignored. */
            nig = 0;
            while( astChr2Double( word ) != AST__BAD ) {
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
               nig++;
               word = GetNextWord( this, &con, status );
            }

            if( nig ) astAddWarning( this, 1, "Ignoring extra 'Error' parameters "
                                     "found in an STC-S %s sub-phrase: '%s'.", 
                                     "astRead", status, subphrase, 
                                     ContextFragment( &con, &fbuf, status ) );

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "ERROR", prop, NULL );

         }

/* Indicate that we do not need to get a new word (we can re-use the last
   one that turned out not to be a numerical valeu above). */
         new_word = 0;

/* Next look for Resolution (also ignored). */
         look_for = RESOLUTION;



/* If we are currently looking for a "Resolution" string... */
      } else if( look_for == RESOLUTION ) {
/* ------------------------------------------------------------------ */

/* AST does not support resolution, so if this word is the start of a
   resolution section, read it and discard it, together with any floating 
   point values that follow, then issue a warning. */
         if( !strcmp( word, "Resolution" ) ) {
            nc = 0;
            word = GetNextWord( this, &con, status );
            while( astChr2Double( word ) != AST__BAD ) {
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
               word = GetNextWord( this, &con, status );
            }
            new_word = 0;
            astAddWarning( this, 1, "Ignoring 'Resolution' values found in an "
                           "STC-S %s sub-phrase: '%s'.", "astRead", status, 
                           subphrase, ContextFragment( &con, &fbuf, status ) );

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "RESOLUTION", prop, NULL );

         } else {
            new_word = 0;
         }

/* Next look for Size or PixSize (also ignored). */
         if( spaceid != NULL_ID ) {
            look_for = SIZE;
         } else {
            look_for = PIX_SIZE;
         }



/* If we are currently looking for a spatial "Size" string... */
      } else if( look_for == SIZE ) {
/* ------------------------------------------------------------------ */

/* AST does not support size, so if this word is the start of a
   size section, read it and discard it, together with any floating 
   point values that follow, then issue a warning. */
         if( !strcmp( word, "Size" ) ) {
            nc = 0;
            word = GetNextWord( this, &con, status );
            while( astChr2Double( word ) != AST__BAD ) {
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
               word = GetNextWord( this, &con, status );
            }
            new_word = 0;
            astAddWarning( this, 1, "Ignoring 'Size' values found in an "
                           "STC-S %s sub-phrase: '%s'.", "astRead", status, 
                           subphrase, ContextFragment( &con, &fbuf, status ) );

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "SIZE", prop, NULL );

         } else {
            new_word = 0;
         }

/* Next look for PixSize (also ignored). */
         look_for = PIX_SIZE;


/* If we are currently looking for a "PixSize" string... */
      } else if( look_for == PIX_SIZE ) {
/* ------------------------------------------------------------------ */

/* AST does not support PixSize, so if this word is the start of a
   PixSize section, read it and discard it, together with any floating 
   point values that follow, then issue a warning. */
         if( !strcmp( word, "PixSize" ) ) {
            nc = 0;
            word = GetNextWord( this, &con, status );
            while( astChr2Double( word ) != AST__BAD ) {
               prop = astAppendString( prop, &nc, word );
               prop = astAppendString( prop, &nc, " " );
               word = GetNextWord( this, &con, status );
            }
            new_word = 0;
            astAddWarning( this, 1, "Ignoring 'PixSize' values in an STC-S "
                           "%s sub-phrase: '%s'.", "astRead", status, 
                           subphrase, ContextFragment( &con, &fbuf, status ) );

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "PIXSIZE", prop, NULL );

         } else {
            new_word = 0;
         }

/* Next look for the next section. */
         if( timeid != NULL_ID ) {
            look_for = SPACE_IDENTIFIER;

         } else if( spaceid != NULL_ID ) {
            look_for = VELOCITY_IDENTIFIER;

         } else if( velid != NULL_ID ) {
            look_for = SPECTRAL_IDENTIFIER;

         } else if( specid != NULL_ID ) {
            look_for = REDSHIFT_IDENTIFIER;

         } else {
            break;
         }



/* If we are currently looking for a "unit" string... */
      } else if( look_for == UNIT ) {
/* ------------------------------------------------------------------ */

/* See if the current word is "unit". If so, read the next word and use
   it as the unit string. Otherwise, use a null pointer to indicate that the
   default units should be used, and set new_word to indicate that the
   next pass will re-interpret the current word instead of a new word. */
         if( !strcmp( word, "unit" ) ) {
            word = GetNextWord( this, &con, status );
            unit = astStore( unit, word, strlen( word ) + 1 );
            astMapPut0C( props, "UNIT", word, NULL );

         } else {
            unit = astFree( unit );
            new_word = 0;
         }

/* If we are currently reading the time sub-phrase... */
         if( timeid != NULL_ID ) {

/* The TimeFrame represents default units (e.g. days for MJD) but
   the errors may be given in different units, so not a scaling factor
   that must be applied to the errors. */
            if( !unit || !strcmp( unit, "s" ) ) {  
               scale = 1.0/86400.0;

            } else if( !strcmp( unit, "d" ) ) {  
               scale = 1.0;

            } else if( !strcmp( unit, "a" ) ) {  
               scale = 365.25;

            } else if( !strcmp( unit, "yr" ) ) {  
               scale = 365.25;

            } else if( !strcmp( unit, "cy" ) ) {  
               scale = 36525.0;

            } else { 
               astError( AST__BADIN, "astRead(StcsChan): Unsupported "
                         "units (%s) for the time axis within an "
                         "STC-S description: '%s'.", status, unit, 
                         ContextFragment( &con, &fbuf, status ) );
            }

/* If we are currently reading the space sub-phrase... */
         } else if( spaceid != NULL_ID ) {

/* In AST, SkyFrames always use radians, so set up a scaling factor to
   convert supplied axis values into radians. */
            if( is_skyframe ) {
   
               if( !unit || !strcmp( unit, "deg" ) ) {
                  scale = AST__DD2R;
   
               } else if( !strcmp( unit, "arcmin" ) ) {
                  scale = AST__DD2R/60.0;
   
               } else if( !strcmp( unit, "arcsec" ) ) {
                  scale = AST__DD2R/3600.0;
   
               } else if( astOK ) {
                  astError( AST__BADIN, "astRead(StcsChan): Unsupported "
                            "units (%s) for a spherical co-ordinate system "
                            "within an STC-S description: '%s'.", status, 
                            unit, ContextFragment( &con, &fbuf, status ) );
               }


/* Basic Frames can use any of the allowed units, so set the Unit
   attribute and use a scale factor of 1.0. Also set the active unit 
   flag in the space frame to enable intelligent units conversion by
   astConvert etc. */
            } else {
               scale = 1.0;
               for( iaxis = 0; iaxis < naxes; iaxis++ ) {
                  astSetUnit( spacefrm, iaxis, unit ? unit : "m" );
               }
               astSetActiveUnit( spacefrm, 1 );
            }

/* If we are currently reading the velocity sub-phrase... */
         } else if( velid != NULL_ID ) {

/* If we are currently reading the spectral sub-phrase... */
         } else if( specid != NULL_ID ) {

/* Set the spectral system implied by the unit string. */
            if( !unit || !strcmp( unit, "Hz" ) || !strcmp( unit, "MHz" ) || 
                !strcmp( unit, "GHz" ) ) {
               astSetSystem( specfrm, AST__FREQ );
   
            } else if( !strcmp( unit, "m" ) || !strcmp( unit, "mm" ) || 
                       !strcmp( unit, "um" ) || !strcmp( unit, "nm" ) ||
                       !strcmp( unit, "A" ) ) {
               astSetSystem( specfrm, AST__WAVELEN );
   
            } else if( !strcmp( unit, "eV" ) || !strcmp( unit, "keV" ) || 
                       !strcmp( unit, "MeV" ) ) {
               astSetSystem( specfrm, AST__ENERGY );
   
            } else if( astOK ) {
               astError( AST__BADIN, "astRead(StcsChan): Unsupported spectral "
                         "units (%s) found within an STC-S description: '%s'.",
                          status, unit, ContextFragment( &con, &fbuf, status ) );
            }

/* SpecFrames can use any of the allowed units, so set the Unit
   attribute and use a scale factor of 1.0. We do not set the active 
   unit flag since SpecFrames always have active units. */
            scale = 1.0;
            astSetUnit( specfrm, 0, unit ? unit : "Hz" );

/* If we are currently reading the redshift sub-phrase... */
         } else if( redid != NULL_ID ) {

/* SpecFrames can use any of the allowed units, so set the Unit
   attribute and use a scale factor of 1.0. We do not set the active 
   unit flag since SpecFrames always have active units. */
            scale = 1.0;
            if( isvel ) {
               astSetUnit( redfrm, 0, unit ? unit : "km/s" );
            } else {
               if( !unit || !strcmp( unit, "nill" ) ) {
                  astClearUnit( redfrm, 0 );
               } else {
                  astSetUnit( redfrm, 0, unit );
               }
            }
         }

/* Move on to find the errors. */
         look_for = ERROR;



/* Report an error for any unknown look_for. */
/* ------------------------------------------------------------------ */
      } else if( astOK ) { 
         astError( AST__INTER, "astRead(StcsChan): Illegal look_for value "
                   "(%d) encountered (internal AST programming error).",
                   status, look_for );
      }

/* If required, get the next word in the STC-S description. */
      if( new_word ) word = GetNextWord( this, &con, status );
   }

/* Free resources stored in the GetNextWord context structure. */
   con.done = 1;
   (void) GetNextWord( this, &con, status );

/* We have now finished reading the redshift sub-phrase, so we can now
   finalise any remaining details of the redshift Frame and Region. Store 
   any epoch and standard of rest value in any redshift Frame. */
   if( redfrm ) {
      if( epoch != AST__BAD ) astSetEpoch( redfrm, epoch );
      if( sor != AST__BADSOR ) astSetStdOfRest( redfrm, sor );

/* Create a suitable Region to describe the enclosure for the redshift
   coords */
      if( redid == REDSHIFT_INTERVAL_ID ) {
         red_enc = (AstRegion *) astInterval( redfrm, &lolim, &hilim, NULL, 
                                              "", status );
         use_enc = 1;

      } else if( redid == REDSHIFT_ID ) {
         red_enc = (AstRegion *) SinglePointList( (AstFrame *) redfrm, 
                                                  &value, NULL, status);
      }

/* Create a suitable Region to describe the redshift coords contained within
   the above enclosure. If any sub-phrase has no coordinate value, then
   we cannot produce a PointList describing the complete coordinate set. */
      if( value != AST__BAD ) {
         red_co = (AstRegion *) SinglePointList( (AstFrame *) redfrm, 
                                                  &value, NULL, status);
      } else {
         use_co = 0;
      }

/* Set the filling factor. */
      if( red_enc && fill != AST__BAD ) {
         astSetFillFactor( red_enc, fill );
         fill = AST__BAD;
      }

/* Associate an uncertainty with the two Regions. */
      SetUnc( red_enc, red_co, (AstFrame *) redfrm, 0, scale, err, 1, status );

/* Free the redshift SpecFrame pointer. */
      redfrm = astAnnul( redfrm );
   }


/* If a particular position was specified by the STC_S document, create the
   full position from the individual sub-phrase position */
   if( use_co ) {
      new = time_co ? astClone( time_co ) : NULL;

      if( space_co ) {
         if( new ) {
            tr = astPrism( new, space_co, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( space_co );
         }
      }

      if( spec_co ) {
         if( new ) {
            tr = astPrism( new, spec_co, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( spec_co );
         }
      }

      if( red_co ) {
         if( new ) {
            tr = astPrism( new, red_co, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( red_co );
         }
      }

      full_co = astSimplify( new );

      new = astAnnul( new );

   } else {
      full_co = NULL;
   }

/* If an enclosing volume was specified by the STC_S document, create the
   full enclosure Region from the individual sub-phrase enclosure Regions. */
   if( use_enc ) {
      new = time_enc ? astClone( time_enc ) : NULL;

      if( space_enc ) {
         if( new ) {
            tr = astPrism( new, space_enc, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( space_enc );
         }
      }

      if( spec_enc ) {
         if( new ) {
            tr = astPrism( new, spec_enc, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( spec_enc );
         }
      }

      if( red_enc ) {
         if( new ) {
            tr = astPrism( new, red_enc, "", status );
            (void) astAnnul( new );
            new = (AstObject *) tr;
         } else {
            new = astClone( red_enc );
         }
      }
      full_enc = astSimplify( new );
      new = astAnnul( new );

   } else {
      full_enc = NULL;
   }

/* Combine the property KeyMaps into a single KeyMap. */
   full_props = astKeyMap( " ", status );
   if( astMapSize( time_props ) > 0 ) astMapPut0A( full_props, "TIME_PROPS", 
                                                   time_props, NULL );
   if( astMapSize( space_props ) > 0 ) astMapPut0A( full_props, "SPACE_PROPS", 
                                                    space_props, NULL );
   if( astMapSize( spec_props ) > 0 ) astMapPut0A( full_props, "SPECTRAL_PROPS", 
                                                   spec_props, NULL );
   if( astMapSize( red_props ) > 0 ) astMapPut0A( full_props, "REDSHIFT_PROPS", 
                                                  red_props, NULL );

/* See which, and how many, items are to be returned. */
   nwant = 0;
   if( ( want_enc = astGetStcsArea( this ) ) ) nwant++;
   if( ( want_co = astGetStcsCoords( this ) ) ) nwant++;
   if( ( want_props = astGetStcsProps( this ) ) ) nwant++;

/* If one, and only one, of the three items is to be returned, return it. */   
   if( nwant == 1 ) {
      if( want_enc && full_enc ) {
         new = astClone( full_enc );
      } else if( want_co && full_co ) {
         new = astClone( full_co );
      } else if( want_props && full_props ){
         new = astClone( full_props );
      }

/* If more than one item is to be returned, put them into a KeyMap and
   return the KeyMap. */         
   } else if( nwant > 1 ) {
      new = (AstObject *) astKeyMap( " ", status );
      if( want_enc && full_enc ) astMapPut0A( new, "AREA", full_enc, NULL );
      if( want_co && full_co ) astMapPut0A( new, "COORDS", full_co, NULL );
      if( want_props && full_props ) astMapPut0A( new, "PROPS", full_props, NULL );

/* Report an error if nothing is to be returned. */
   } else if( astOK ){
      astError( AST__ATTIN, "astRead(StcsChan): The StcsArea, StcsCoords "
                "and StcsProps attributes indicate that nothing is to be "
                "returned (possible programming error).", status );
   }

/* Free resources */
   FreeContext( &con, status );
   unit = astFree( unit );
   prop = astFree( prop );
   frame = astFree( frame );
   fbuf = astFree( fbuf );
   time_props = astAnnul( time_props );
   space_props = astAnnul( space_props );
   spec_props = astAnnul( spec_props );
   red_props = astAnnul( red_props );
   if( spacefrm ) spacefrm = astAnnul( spacefrm );
   if( specfrm ) specfrm = astAnnul( specfrm );
   if( timefrm ) timefrm = astAnnul( timefrm );
   if( redfrm ) redfrm = astAnnul( redfrm );
   if( space_enc ) space_enc = astAnnul( space_enc );
   if( spec_enc ) spec_enc = astAnnul( spec_enc );
   if( time_enc ) time_enc = astAnnul( time_enc );
   if( red_enc ) red_enc = astAnnul( red_enc );
   if( space_co ) space_co = astAnnul( space_co );
   if( spec_co ) spec_co = astAnnul( spec_co );
   if( time_co ) time_co = astAnnul( time_co );
   if( red_co ) red_co = astAnnul( red_co );
   if( full_enc ) full_enc = astAnnul( full_enc );
   if( full_co ) full_co = astAnnul( full_co );
   if( full_props ) full_props = astAnnul( full_props );

/* If an error occurred, clean up by deleting the new Object and
   return a NULL pointer. */
   if ( !astOK ) new = astDelete( new );

/* Return the pointer to the new Object. */
   return new;
}

static void SetAttrib( AstObject *this_object, const char *setting, int *status ) {
/*
*  Name:
*     SetAttrib

*  Purpose:
*     Set an attribute value for a StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void SetAttrib( AstObject *this, const char *setting )

*  Class Membership:
*     StcsChan member function (over-rides the astSetAttrib protected
*     method inherited from the Channel class).

*  Description:
*     This function assigns an attribute value for a StcsChan, the
*     attribute and its value being specified by means of a string of
*     the form:
*
*        "attribute= value "
*
*     Here, "attribute" specifies the attribute name and should be in
*     lower case with no white space present. The value to the right
*     of the "=" should be a suitable textual representation of the
*     value to be assigned and this will be interpreted according to
*     the attribute's data type.  White space surrounding the value is
*     only significant for string attributes.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     setting
*        Pointer to a null terminated string specifying the new attribute
*        value.
*/

/* Local Variables: */
   AstStcsChan *this;          /* Pointer to the StcsChan structure */
   int ival;                   /* Integer attribute value */
   int len;                    /* Length of setting string */
   int nc;                     /* Number of characters read by "astSscanf" */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_object;

/* Obtain the length of the setting string. */
   len = (int) strlen( setting );

/* Test for each recognised attribute in turn, using "astSscanf" to parse
   the setting string and extract the attribute value (or an offset to
   it in the case of string values). In each case, use the value set
   in "nc" to check that the entire string was matched. Once a value
   has been obtained, use the appropriate method to set it. */

/* StcsArea. */
/* --------- */
   if ( nc = 0,
               ( 1 == astSscanf( setting, "stcsarea= %d %n", &ival, &nc ) )
               && ( nc >= len ) ) {
      astSetStcsArea( this, ival );

/* StcsCoords. */
/* ----------- */
   } else if ( nc = 0,
               ( 1 == astSscanf( setting, "stcscoords= %d %n", &ival, &nc ) )
               && ( nc >= len ) ) {
      astSetStcsCoords( this, ival );

/* StcsProps. */
/* ----------- */
   } else if ( nc = 0,
               ( 1 == astSscanf( setting, "stcsprops= %d %n", &ival, &nc ) )
               && ( nc >= len ) ) {
      astSetStcsProps( this, ival );

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      (*parent_setattrib)( this_object, setting, status );
   }
}

static void SetUnc( AstRegion *reg1, AstRegion *reg2, AstFrame *frm, 
                    int is_skyframe, double scale, double *error, int nax, 
                    int *status ){
/*
*  Name:
*     SetUnc

*  Purpose:
*     Store an uncertainty Box with a supplied Region.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void SetUnc( AstRegion *reg1, AstRegion *reg2, AstFrame *frm, 
*                  int is_skyframe, double scale, double *error, int nax, 
*                  int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function creates a new Box with dimensions specified by the
*     values in the "error" array, centred on a representative position
*     within one of the supplied Regions, and then stores the Box as the
*     uncertainty Region within both the supplied Regions. 

*  Parameters:
*     reg1
*        Pointer to a Region to which the error values relate.
*     reg2
*        Pointer to another Region to which the error values relate.
*     frm
*        Pointer to the Frame encapsulated by both Regions.
*     is_skyframe
*        Should be non-zero if "frm" is a SkyFrame.
*     scale
*        A scale factor to apply to the error values before using them.
*     error
*        Pointer to an array of RMS error values, one for each axis in 
*        "frm". These are modified on exit. For a SkyFrame, both values
*        (including the longitude axis value) should be given as an 
*        arc-distance. This function will convert the arc-distance to
*        a longitude increment using a representative latitude for the 
*        region.
*     nax
*        The numner of axes in "frm". 
*     status
*        Pointer to the inherited status variable.

*/

/* Local Variables: */
   AstBox *unc;           /* Uncertainty box */
   double dist;           /* Diagonal length of Region bounding box */
   double lbnd[ 6 ];      /* Lower bounds of Region bounding box */
   double spos1[ 6 ];     /* A representative position in the Region */
   double spos2[ 6 ];     /* A second position in the Region */
   double ubnd[ 6 ];      /* Upper bounds of Region bounding box */
   int i;                 /* Axis index */

/* Check the global error status. Also check an error value was supplied,
   and at least one of the Region pointers is not NULL. */
   if ( !astOK || error[ 0 ] == AST__BAD || ( !reg1 && !reg2 ) ) return;

/* We need a representative position within the region. First get the
   coordinates at opposite corners of the region bounding box. */
   astRegBaseBox( reg1 ? reg1 : reg2, lbnd, ubnd );

/* Find the diagonal length of the bounding box. */
   dist = astDistance( frm, lbnd, ubnd );

/* Offset away from one corner towards the other by half the diagonal
   length. The resulting position returned in spos1 is our representative
   position for the region. */
   astOffset( frm, lbnd, ubnd, dist/2, spos1 );

/* Scale the error values */
   for( i = 0; i < nax; i++ ) error[ i ] *= scale;

/* If the region is defined within a SkyFrame, the supplied longitude 
   error value will be an arc-distance value. But we need a longitude
   increment to create an uncertainty Region, so do the conversion. */
   if( is_skyframe ) {

/* Offset away from the representative position found above along the 
   first (i.e. longitude) axis by an arc-distance given by the Error 
   value. */
      (void) astOffset2( frm, spos1, AST__DPIBY2, error[ 0 ], spos2 );   

/* Find the positive axis increment along the first axis. */
      error[ 0 ] = astAxDistance( frm, 1, spos1[ 0 ], spos2[ 0 ] );
      if( error[ 0 ] != AST__BAD ) error[ 0 ] = fabs( error[ 0 ] );
   }

/* The uncertainty Region will be a Box centred at the representative 
   position found above. Modify the "error" array to hold the corner  
   axis values. */
   for( i = 0; i < nax; i++ ) error[ i ] += spos1[ i ];

/* Create the box, and store it as the uncertainty Region in the supplied
   Region. */
   unc = astBox( frm, 0, spos1, error, NULL, " ", status );
   if( reg1 ) astSetUnc( reg1, unc );
   if( reg2 ) astSetUnc( reg2, unc );

/* Free resources. */
   unc = astAnnul( unc );
}

static AstPointList *SinglePointList( AstFrame *frm, double *pos, 
                                      AstRegion *unc, int *status){
/*
*  Name:
*     SinglePointList

*  Purpose:
*     Create a PointList holding a single point.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     AstPointList *SinglePointList( AstFrame *frm, double *pos, 
*                                    AstRegion *unc, int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function creates a new PointList holding a single supplied
*     position.

*  Parameters:
*     frm
*        Pointer to the Frame in which the PointList is defined. 
*     pos 
*        Array holding the position. The length of this array must equal
*        the number of axes in "frm".
*     unc
*        Pointer to an uncertainty Region to associate with the new
*        PointList, or NULL.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to the new PointList. NULL is returned if an error has 
*     already occurred, of if this function fails for any reason.
*/

/* Local Variables: */
   AstPointList *result;   /* Returned pointer. */
   AstPointSet *pset;      /* PointSet holding axis values */
   double **ptr;           /* Pointer to PointSet data arrays */
   int i;                  /* Axis index */
   int nax;                /* Number of axes */

/* Initialise. */
   result = NULL;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Get he number of axes. */
   nax = astGetNaxes( frm );

/* Create a PointSet to hold the supplied point, and get a pointer to its
   data arrays. */
   pset = astPointSet( 1, nax, "", status );
   ptr = astGetPoints( pset );
   if( astOK ) {

/* Copy the supplied axis values into the PointSet data arrays. */
      for( i = 0; i < nax; i++ ) ptr[ i ][ 0 ] = pos[ i ];

/* Create the PointList. */
      result = astPointList( frm, pset, unc, "", status );
   }

/* Free resources */
   pset = astAnnul( pset );

/* Return the result. */
   return result;
}

static void SinkWrap( void (* sink)( const char * ), const char *line, int *status ) {
/*
*  Name:
*     SinkWrap

*  Purpose:
*     Wrapper function to invoke a C StcsChan sink function.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void SinkWrap( void (* sink)( const char * ), const char *line, int *status )

*  Class Membership:
*     StcsChan member function.

*  Description:
*     This function invokes the sink function whose pointer is
*     supplied in order to write an output line to an external data
*     store.

*  Parameters:
*     sink
*        Pointer to a sink function, whose single parameter is a
*        pointer to a const, null-terminated string containing the
*        text to be written, and which returns void. This is the form
*        of StcsChan sink function employed by the C language interface
*        to the AST library.
*     status
*        Pointer to the inherited status variable.
*/

/* Check the global error status. */
   if ( !astOK ) return;

/* Invoke the sink function. */
   ( *sink )( line );
}

static char *SourceWrap( const char *(* source)( void ), int *status ) {
/*
*  Name:
*     SourceWrap

*  Purpose:
*     Wrapper function to invoke a C StcsChan source function.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     char *SourceWrap( const char *(* source)( void ), int *status )

*  Class Membership:
*     StcsChan member function.

*  Description:
*     This function invokes the source function whose pointer is
*     supplied in order to read the next input line from an external
*     data store. It then returns a pointer to a dynamic string
*     containing a copy of the text that was read.

*  Parameters:
*     source
*        Pointer to a source function, with no parameters, that
*        returns a pointer to a const, null-terminated string
*        containing the text that it read. This is the form of StcsChan
*        source function employed by the C language interface to the
*        AST library.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A pointer to a dynamically allocated, null terminated string
*     containing a copy of the text that was read. This string must be
*     freed by the caller (using astFree) when no longer required.
*
*     A NULL pointer will be returned if there is no more input text
*     to read.

*  Notes:
*     - A NULL pointer value will be returned if this function is
*     invoked with the global error status set or if it should fail
*     for any reason.
*/

/* Local Variables: */
   char *result;                 /* Pointer value to return */
   const char *line;             /* Pointer to input line */

/* Initialise. */
   result = NULL;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Invoke the source function to read the next input line and return a
   pointer to the resulting string. */
   line = ( *source )();

/* If a string was obtained, make a dynamic copy of it and save the
   resulting pointer. */
   if ( line ) result = astString( line, (int) strlen( line ) );

/* Return the result. */
   return result;
}

static void StoreTimeProp( AstKeyMap *props, AstTimeFrame *frm, 
                           const char *key, double value, int *status ){
/*
*  Name:
*     StoreTimeProp

*  Purpose:
*     Store a time value as an STC-S property, using the existing format.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void StoreTimeProp( AstKeyMap *props, AstTimeFrame *frm, 
*                         const char *key, double value, int *status )

*  Class Membership:
*     StcsChan member function.

*  Description:
*     This function formats the supplied time value and stores it in 
*     the "props" KeyMap, using the supplied key name. If the KeyMap
*     already contains an entry for the given key, the new value is
*     written using the same format. Otherwise, the new value is written
*     as an ISO date and time string.

*  Parameters:
*     props
*        Pointer to the KeyMap in which to store the time value.
*     frm
*        Pointer to a TimeFrame that can be used to format the time value.
*     key
*        Pointer to a string holding the property name associated with
*        the time value.
*     value
*        The time value, in the system described by "frm".
*     status
*        Pointer to the inherited status variable.

*/

/* Local Variables: */
   AstFrame *fmtfrm;    /* Frame defining Format/System for formatted value */
   AstFrame *fs;        /* FrameSet connecting Frames */
   const char *fmttxt;  /* Formatted text */
   const char *oldval;  /* Pointer to old formatted time value */
   const char *p;       /* Pointer to next character in formatted value */
   double fmtval;       /* The time value in the formatting system */
   int ndp;             /* Number of decimal places in formatted value */

/* Check the global error status. */
   if ( !astOK ) return;

/* We want a TimeFrame (fmtfrm) that describes how to format the time 
   value. If the Format attribute of the supplied TimeFrame has been 
   set, use it (and the current System). So just take a clone of the 
   supplied frame pointer. */
   if( astTestFormat( frm, 0 ) ) {
      fmtfrm = astClone( frm );

/* If the Format attribute has not been set, we create a copy of the
   supplied TimeFrame, and set its System and Format attributes to 
   produce the required format. */
   } else {
      fmtfrm = astCopy( frm );

/* If the KeyMap contains an entry for the specified key, determine the 
   format of the time string it contains. */
      if( astMapGet0C( props, key, &oldval ) && oldval ) {

/* See how many digits there are after the decimal place */
         p = strchr( oldval, '.' );
         ndp = 0;
         if( p ) {
            while( *(++p) ) {
               if( isdigit( *p ) ) {
                  ndp++;
               } else {
                  break;
               }
            }
         }

/* If the string starts with "JD", the time is formatted as a numerical
   Julian date. */
         if( !strncmp( oldval, "JD", 2 ) ) {
            astSetSystem( fmtfrm, AST__JD );
            if( ndp > 0 ) {
               astSet( fmtfrm, "Format=JD %%.%df", status, ndp );
            } else {
               astSetFormat( fmtfrm, 0, "JD %d" );
            }

/* If the string starts with "MJD", the time is formatted as a numerical
   Modified Julian date. */
         } else if( !strncmp( oldval, "MJD", 3 ) ) {
            astSetSystem( fmtfrm, AST__MJD );
            if( ndp > 0 ) {
               astSet( fmtfrm, "Format=MJD %%.%df", status, ndp );
            } else {
               astSetFormat( fmtfrm, 0, "MJD %d" );
            }

/* Otherwise, the current word should be an ISO date. See how many
   decimal paces in the seconds field there are (if any). */
         } else {
            astSet( fmtfrm, "Format=iso.%dT", status, ndp );
         }

/* If the KeyMap does not contain an entry for the specified key, an
   ISO date/time string with 1 decimal place in the seconds field 
   is used. */
      } else {
         astSetFormat( fmtfrm, 0, "iso.1T" );
      }
   }

/* Ensure the displayed value is an abolute value. */
   astClearTimeOrigin( fmtfrm );

/* Convert the supplied time value into the required system. */
   fs = astConvert( frm, fmtfrm, "" );   
   astTran1( fs, 1, &value, 1, &fmtval );

/* Format the value. */
   fmttxt = astFormat( fmtfrm, 0, fmtval );

/* Store it in the KeyMap. */
   astMapPut0C( props, key, fmttxt, NULL );

/* Free resources. */
   fs = astAnnul( fs );
   fmtfrm = astAnnul( fmtfrm );
}

static int TestAttrib( AstObject *this_object, const char *attrib, int *status ) {
/*
*  Name:
*     TestAttrib

*  Purpose:
*     Test if a specified attribute value is set for a StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     int TestAttrib( AstObject *this, const char *attrib, int *status )

*  Class Membership:
*     StcsChan member function (over-rides the astTestAttrib protected
*     method inherited from the Object class).

*  Description:
*     This function returns a boolean result (0 or 1) to indicate whether
*     a value has been set for one of a StcsChan's attributes.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     attrib
*        Pointer to a null terminated string specifying the attribute
*        name.  This should be in lower case with no surrounding white
*        space.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     One if a value has been set, otherwise zero.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the global status set, or if it should fail for any reason.
*/

/* Local Variables: */
   AstStcsChan *this;            /* Pointer to the StcsChan structure */
   int result;                   /* Result value to return */

/* Initialise. */
   result = 0;

/* Check the global error status. */
   if ( !astOK ) return result;

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_object;

/* Check the attribute name and test the appropriate attribute. */

   if ( !strcmp( attrib, "stcsarea" ) ) {
      result = astTestStcsArea( this );

   } else if ( !strcmp( attrib, "stcscoords" ) ) {
      result = astTestStcsCoords( this );

   } else if ( !strcmp( attrib, "stcsprops" ) ) {
      result = astTestStcsProps( this );

/* If the attribute is still not recognised, pass it on to the parent
   method for further interpretation. */
   } else {
      result = (*parent_testattrib)( this_object, attrib, status );
   }

/* Return the result, */
   return result;
}

static int Write( AstChannel *this_channel, AstObject *object, int *status ) {
/*
*  Name:
*     Write

*  Purpose:
*     Write an Object to a StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     int Write( AstChannel *this, AstObject *object, int *status )

*  Class Membership:
*     StcsChan member function (over-rides the astWrite method
*     inherited from the Channel class).

*  Description:
*     This function writes an Object to a StcsChan.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     object
*        Pointer to the Object which is to be written.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     The number of Objects written to the StcsChan by this invocation of 
*     astWrite.

*  Notes:
*     - A value of zero will be returned if this function is invoked
*     with the AST error status set, or if it should fail for any
*     reason.
*/

/* Local Variables: */
   AstFrame *frm;            /* AREA Frame */
   AstFrameSet *fs;          /* FrameSet connecting AREA and COORDS */
   AstKeyMap *props;         /* A KeyMap holding the STC-S properties list */
   AstMapping *map;          /* Mapping connecting AREA and COORDS */
   AstObject *obj;           /* A temporary Object pointer */
   AstRegion *area;          /* The Region representing the STC CoordArea */
   AstRegion *coords;        /* The Region representing the STC Coords */
   AstRegion *new_coords;    /* COORDS Region mapped into frame of AREA */
   AstStcsChan *this;        /* Pointer to the StcsChan structure */
   astDECLARE_GLOBALS;       /* Declare the thread specific global data */
   const char *class;        /* Pointer to string holding object class */
   const char *errclass;     /* Type of the failed entry */
   const char *errname;      /* Name of the failed entry */
   const char *method;       /* Pointer to string holding calling method */
   const char *wantclass;    /* The expected type */
   int ret;                  /* Number of objects read */

/* Initialise. */
   ret = 0;

/* Check the global error status. */
   if ( !astOK ) return ret;

/* Get a pointer to the structure holding thread-specific global data. */   
   astGET_GLOBALS(this_channel);

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_channel;

/* Store the calling method, and object class. */
   method = "astWrite";
   class = astGetClass( this );

/* Initialise */
   area = NULL;
   coords = NULL;
   props = NULL;

/* If the supplied Object is a Region, we will use it to define the AREA
   properties. */
   if( astIsARegion( object ) ) {
      area = (AstRegion *) astClone( object );

/* If the supplied Object is a KeyMap... */
   } else if( astIsAKeyMap( object ) ) {
      errname = NULL;
      wantclass = NULL;
      errclass = NULL;

/* If the supplied KeyMap contains an entry with key "AREA", and if it is 
   a Region, use it to define the AREA properties. */
      if( astMapGet0A( (AstKeyMap *) object, "AREA", &obj ) ) {
         if( astIsARegion( obj ) ) {         
            area = (AstRegion *) obj;
         } else {
            wantclass = "Region";
            errclass = astGetClass( obj );
            errname = "AREA";
            obj = astAnnul( obj );
         }
      }

/* If the supplied KeyMap contains an entry with key "COORDS", and if it is 
   a Region, use it to define the COORDS properties. */
      if( astMapGet0A( (AstKeyMap *) object, "COORDS", &obj ) ) {
         if( astIsARegion( obj ) ) {         
            coords = (AstRegion *) obj;
         } else {
            wantclass = "Region";
            errclass = astGetClass( obj );
            errname = "COORDS";
            obj = astAnnul( obj );
         }
      }

/* If the supplied KeyMap contains an entry with key "PROPS", and if it is 
   a KeyMap, use it to define values for the properties that cannot be
   determined from the supplied Regions (Resolution, PixSize, etc). */
      if( astMapGet0A( (AstKeyMap *) object, "PROPS", &obj ) ) {
         if( astIsAKeyMap( obj ) ) {         
            props = (AstKeyMap *) obj;
         } else {
            wantclass = "KeyMap";
            errclass = astGetClass( obj );
            errname = "PROPS";
            obj = astAnnul( obj );
         }
      }

/* If the supplied KeyMap contains an entry with any of the keys
   "TIME_PROPS", "SPACE_PROPS", "SPECTRAL_PROPS" or "REDSHIFT_PROPS",
   use the supplied KeyMap to define values for all properties. */
      if( astMapGet0A( (AstKeyMap *) object, "TIME_PROPS", &obj ) ||
          astMapGet0A( (AstKeyMap *) object, "SPACE_PROPS", &obj ) ||
          astMapGet0A( (AstKeyMap *) object, "SPECTRAL_PROPS", &obj ) ||
          astMapGet0A( (AstKeyMap *) object, "REDSHIFT_PROPS", &obj ) ) {
         props = astClone( object );
      }

/* Report an error if the Object in the keymap has the wrong type. */
      if( errname && astOK ) {
         astAddWarning( this, 1, "The supplied KeyMap contains a %s "
                        "called '%s'. But '%s' should be a %s "
                        "(programming error).", method, status, 
                        errclass, errname, errname, wantclass );
      }

/* Report an error if the keymap contains none of the above. */
      if( !area && !coords && !props && astOK ) {
         astAddWarning( this, 1, "The supplied KeyMap does not "
                        "contains anything that can be written out "
                        "through a %s.", method, status, class );
      }

/* If both COORDS and AREA were supplied, ensure they are in the same
   Frame by mapping the COORDS Region into the Frame of the AREA Region. */
      if( area && coords ) {
         fs = astConvert( coords, area, " " );
         if( fs ) {
            map = astGetMapping( fs, AST__BASE, AST__CURRENT );
            frm = astGetFrame( fs, AST__CURRENT );

            new_coords = astMapRegion( coords, map, frm );

            map = astAnnul( map );
            frm = astAnnul( frm );
            coords = astAnnul( coords );
            fs = astAnnul( fs );

            coords = new_coords;

         } else if( astOK ){
            astAddWarning( this, 1, "Cannot convert between the co-ordinate "
                           "frame of the COORDS Region and the co-ordinate "
                           "frame of the AREA Region.", method, status );
         }
      }

/* Report an error if the supplied object is neither a KeyMap nor a
   Region. */
   } else if( astOK ) {
      astAddWarning( this, 1, "Failed to write out a %s through a %s. "
                     "The %s class cannot be used to write out a %s.", 
                     method, status, astGetClass( object ), class, class,
                     astGetClass( object ) );
   }

   
/* If we do not have a KeyMap in which to store the STC-S properties,
   create one now. */
   if( astOK ) {
      if( ! props ) props = astKeyMap ( " ", status );

/* Determine the set of STC-S properties that describe the COORDS Region,
   and add them into the properties keymap, over-writing any values for the
   same properties that are already in the props keymap. */
      ret = coords ? WriteRegion( this, coords, props, status ) : 1;

/* Determine the set of STC-S properties that describe the AREA Region,
   and add them into the properties keymap, over-writing any values for the
   same properties that are already in the props keymap. NB, we need to
   do AREA after COORDS so that the sub-phrase identifier implied by the
   AREA is used in preference to that implied by the COORDS. */
      if( area && ret ) ret = WriteRegion( this, area, props, status );

/* Convert the properties list into text and write it out through the
   parent Channel's sink function. */
      if( ret ) WriteProps( this, props, status );
   }

/* Free resources. */
   if( area ) area = astAnnul( area );
   if( coords ) coords = astAnnul( coords );
   if( props ) props = astAnnul( props );

/* If an error has occurred, return zero. */
   if( !astOK ) ret = 0;

/* Return the answer. */
   return ret;
}

static void WriteProps( AstStcsChan *this, AstKeyMap *props, int *status ){
/*
*  Name:
*     WriteProps

*  Purpose:
*     Write out a set of STC-S properties to the sink function.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     void WriteProps( AstStcsChan *this, AstKeyMap *props, int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function converts the STC-S properties supplied in a KeyMap
*     into text, and writes the text out through the sink function associated
*     with the parent Channel.

*  Parameters:
*     this
*        Pointer to the StcsChan.
*     props
*        Pointer to the KeyMap holding the STC-S properties.
*     status
*        Pointer to the inherited status variable.

*/

/* Local Variables: */
   AstKeyMap *spprops;     /* Sub-phrase properties */
   AstObject *obj;         /* Generic Object pointer */
   char *line;             /* Dynamically allocated buffer for output text */
   char id[ 30 ];          /* Sub-phrase identifier */
   const char *prefix;     /* Prefix for property value */
   int first;              /* Is this the first sub-phrase to be output? */
   int nc;                 /* Number of characters in "line" */

/* Check the global error status. */
   if ( !astOK ) return;

/* Initialise the line length. */
   first = 1;
   nc = 0;
   line = NULL;

/* Add each word in the time sub-phrase into the output buffer, in the
   order defined by the STC-S standard. */
   if( astMapGet0A( props, "TIME_PROPS", &obj ) ) {
      spprops = (AstKeyMap *) obj;
      first = 0;

      line = AddItem( spprops, "ID", NULL, line, &nc, status );
      strcpy( id, line );
      
      line = AddItem( spprops, "FILLFACTOR", "fillfactor ", line, &nc, status );
      line = AddItem( spprops, "TIMESCALE", NULL, line, &nc, status );
      line = AddItem( spprops, "REFPOS", NULL, line, &nc, status );
      line = AddItem( spprops, "START", NULL, line, &nc, status );
      line = AddItem( spprops, "STOP", NULL, line, &nc, status );

      prefix = strcmp( id, "Time " ) ? "Time " : NULL;
      line = AddItem( spprops, "TIME", prefix, line, &nc, status );

      line = AddItem( spprops, "UNIT", "unit ", line, &nc, status );
      line = AddItem( spprops, "ERROR", "Error ", line, &nc, status );
      line = AddItem( spprops, "RESOLUTION", "Resolution ", line, &nc, status );
      line = AddItem( spprops, "PIXSIZE", "PixSize ", line, &nc, status );

      spprops = astAnnul( spprops );

/* Write out the time sub-phrase text through the Channel sink function. */
      astPutNextText( this, line );
   }

/* Add each word in the space sub-phrase into the output buffer, in the
   order defined by the STC-S standard. */
   if( astMapGet0A( props, "SPACE_PROPS", &obj ) ) {
      spprops = (AstKeyMap *) obj;

      if( !first ) line = astAppendString( line, &nc, "\n" );
      first = 0;

      nc = 0;
      line = AddItem( spprops, "ID", NULL, line, &nc, status );
      strcpy( id, line  );
      
      line = AddItem( spprops, "FILLFACTOR", "fillfactor ", line, &nc, status );
      line = AddItem( spprops, "FRAME", NULL, line, &nc, status );
      line = AddItem( spprops, "REFPOS", NULL, line, &nc, status );
      line = AddItem( spprops, "FLAVOUR", NULL, line, &nc, status );
      line = AddItem( spprops, "LOLIMIT", NULL, line, &nc, status );
      line = AddItem( spprops, "HILIMIT", NULL, line, &nc, status );
      line = AddItem( spprops, "CENTRE", NULL, line, &nc, status );
      line = AddItem( spprops, "RADIUS", NULL, line, &nc, status );
      line = AddItem( spprops, "RADIUS1", NULL, line, &nc, status );
      line = AddItem( spprops, "RADIUS2", NULL, line, &nc, status );
      line = AddItem( spprops, "POSANGLE", NULL, line, &nc, status );
      line = AddItem( spprops, "VERTICES", NULL, line, &nc, status );

      prefix = strcmp( id, "Position " ) ? "Position " : NULL;
      line = AddItem( spprops, "POSITION", prefix, line, &nc, status );

      line = AddItem( spprops, "UNIT", "unit ", line, &nc, status );
      line = AddItem( spprops, "ERROR", "Error ", line, &nc, status );
      line = AddItem( spprops, "RESOLUTION", "Resolution ", line, &nc, status );
      line = AddItem( spprops, "SIZE", "Size ", line, &nc, status );
      line = AddItem( spprops, "PIXSIZE", "PixSize ", line, &nc, status );

      spprops = astAnnul( spprops );

/* Write out the spatial sub-phrase text through the Channel sink function. */
      astPutNextText( this, line );
   }

/* Add each word in the spectral sub-phrase into the output buffer, in the
   order defined by the STC-S standard. */
   if( astMapGet0A( props, "SPECTRAL_PROPS", &obj ) ) {
      spprops = (AstKeyMap *) obj;

      if( !first ) line = astAppendString( line, &nc, "\n" );
      first = 0;

      nc = 0;
      line = AddItem( spprops, "ID", NULL, line, &nc, status );
      strcpy( id, line );
      
      line = AddItem( spprops, "FILLFACTOR", "fillfactor ", line, &nc, status );
      line = AddItem( spprops, "REFPOS", NULL, line, &nc, status );
      line = AddItem( spprops, "LOLIMIT", NULL, line, &nc, status );
      line = AddItem( spprops, "HILIMIT", NULL, line, &nc, status );

      prefix = strcmp( id, "Spectral " ) ? "Spectral " : NULL;
      line = AddItem( spprops, "SPECTRAL", prefix, line, &nc, status );

      line = AddItem( spprops, "UNIT", "unit ", line, &nc, status );
      line = AddItem( spprops, "ERROR", "Error ", line, &nc, status );
      line = AddItem( spprops, "RESOLUTION", "Resolution ", line, &nc, status );
      line = AddItem( spprops, "PIXSIZE", "PixSize ", line, &nc, status );

      spprops = astAnnul( spprops );

/* Write out the spectral sub-phrase text through the Channel sink function. */
      astPutNextText( this, line );

   }

/* Add each word in the redshift sub-phrase into the output buffer, in the
   order defined by the STC-S standard. */
   if( astMapGet0A( props, "REDSHIFT_PROPS", &obj ) ) {
      spprops = (AstKeyMap *) obj;

      if( !first ) line = astAppendString( line, &nc, "\n" );
      first = 0;

      nc = 0;
      line = AddItem( spprops, "ID", NULL, line, &nc, status );
      strcpy( id, line );
      
      line = AddItem( spprops, "FILLFACTOR", "fillfactor ", line, &nc, status );
      line = AddItem( spprops, "REFPOS", NULL, line, &nc, status );
      line = AddItem( spprops, "TYPE", NULL, line, &nc, status );
      line = AddItem( spprops, "DOPPLERDEF", NULL, line, &nc, status );
      line = AddItem( spprops, "LOLIMIT", NULL, line, &nc, status );
      line = AddItem( spprops, "HILIMIT", NULL, line, &nc, status );

      prefix = strcmp( id, "Redshift " ) ? "Redshift " : NULL;
      line = AddItem( spprops, "REDSHIFT", prefix, line, &nc, status );

      line = AddItem( spprops, "UNIT", "unit ", line, &nc, status );
      line = AddItem( spprops, "ERROR", "Error ", line, &nc, status );
      line = AddItem( spprops, "RESOLUTION", "Resolution ", line, &nc, status );
      line = AddItem( spprops, "PIXSIZE", "PixSize ", line, &nc, status );

      spprops = astAnnul( spprops );

/* Write out the redshift sub-phrase text through the Channel sink function. */
      astPutNextText( this, line );

   }

/* Free resources. */
   line = astFree( line );

}

static int WriteRegion( AstStcsChan *this, AstRegion *reg, AstKeyMap *props, 
                        int *status ){
/*
*  Name:
*     WriteRegion

*  Purpose:
*     Convert a Region into a set of STC-S properties and store them in a
*     KeyMap.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     int WriteRegion( AstStcsChan *this, AstRegion *reg, AstKeyMap *props, 
*                      int *status )

*  Class Membership:
*     StcsChan member function 

*  Description:
*     This function attempts to convert the supplied Region nto a set of
*     STC-S properties, and stores them in the supplied KeyMap.

*  Parameters:
*     this
*        Pointer to the StcsChan being used.
*     reg
*        Pointer to the region to be converted.
*     props
*        Pointer to the KeyMap in which to store the STC-S properties.
*        On exit, each STC-S sub-phrase has an entry in this KeyMap, 
*        and each of these entries has a value that is another KeyMap
*        holding the properties for the sub-phrase.
*     status
*        Pointer to the inherited status variable.

*  Returned Value:
*     A non-zero value is returned if the conversion was succesful, and
*     zero is returned otherwise.
*/

/* Local Variables: */
   AstFrame *efrm;         /* Pointer to encapsulated Frame */
   AstFrame *pfrm;         /* Pointer to primary Frame cntaining an axis */
   AstFrame *spfrm;        /* The sub-phrase Frame */
   AstKeyMap *spprops;     /* Sub-phrase properties */
   AstMapping *map;        /* Base->current Region Mapping */
   AstMapping *sreg;       /* Simplified Region */
   AstObject *obj;         /* Generic object pointer */
   AstRegion *spreg;       /* The sub-phrase Region */
   AstRegion *treg;        /* Temporary Region pointer */
   AstRegion *unc;         /* Uncertainty region */
   AstRegion *unca;        /* Adaptive uncertainty region */
   AstStdOfRestType sor;   /* StdOfRest attribute value */
   AstSystemType sys;      /* System attribute value */
   char *prop;             /* Formatted property string */
   char buf[ 100 ];        /* Buffer for formatted values */
   char fmt[ 10 ];         /* Buffer for format specifier */
   const char *class;      /* Class name */
   const char *dom;        /* Domain name */
   const char *dopdef;     /* DopplerDef value */
   const char *flavour;    /* The STC-S flavour for the space frame */
   const char *runit;      /* Region units string */
   const char *tfrm;       /* STC-S string for Frame */
   const char *tsor;       /* STC-S string for RefPos */
   const char *tts;        /* Time scale label */
   const char *type;       /* Redshift Type value */
   const char *unit;       /* Unit string */
   double *p;              /* Pointer to next axis value */
   double *points;         /* Pointer to array of Region axis values */
   double a;               /* Circle or ellipse radius */
   double angle;           /* Ellipse position angle */
   double b;               /* Ellipse radius */
   double centre[ 3 ];     /* Circle or ellipse centre */
   double *pcen;           /* Pointer to Circle or ellipse centre */
   double equinox;         /* The required equinox value */
   double error;           /* Axis error value */  
   double fill;            /* Fill factor */
   double lbnd[ 3 ];       /* Region lower bounds */
   double lim;             /* Unlimited bounds value */
   double p1[ 2 ];         /* End point of error line */
   double scale;           /* Factor for scaling Region values into required units */
   double ubnd[ 3 ];       /* Region upper bounds */
   int defdigs;            /* Default number of digits */
   int defs;               /* Include default values in outptu STC-S? */
   int i;                  /* Loop index */
   int issky;              /* Do the space axes form a SkyFrame? */
   int j;                  /* Loop index */
   int nax;                /* The number of axes */
   int nc;                 /* Number of characters in "prop" string */
   int np;                 /* Number of points defining the Region */
   int nspace;             /* Number of space axes */
   int ok;                 /* Can the Region be written out? */
   int pax;                /* Index of axis in primary Frame */
   int redax;              /* The index of the redshift axis */
   int retain_units;       /* Retain the units/system in properties KeyMap? */
   int spaceax[ 3 ];       /* Indicies of the space axes */
   int specax;             /* The index of the spectral axis */
   int timeax;             /* Index of time axis */
   int ts;                 /* Time scale identifier */

/* Check the global error status. */
   if ( !astOK ) return 0;

/* Initialise things to avoid comiler warnings. */
   sys = AST__BADSYSTEM;

/* Assume we can do the conversion. */
   ok = 1;

/* See if default values are to be included in the output. */
   defs = ( astGetFull( this ) > 0 );

/* STC-S requires that the spatial shape (circle, box. etc) refers to 
   the coordinate system described by the STC-S. This is not quite like 
   AST, in that the AST class type (Circle, Box, etc) defines the
   shape of the region in the base Frame, rather than the current Frame.
   So we can only write the Region out using STC-S if the shape in the
   current Frame is the same as the shape in the base Frame. This is the
   case if the simplified Mapping connecting base and current Frames is 
   a UnitMap. Get the base->current Mapping from the Region. */
   map = astRegMapping( reg );

/* If it is not UnitMap, see if simplifying the whole Region results in
   the base->current Mapping in the simplified Region being a UnitMap. */
   if( !astIsAUnitMap( map ) ) {
      map = astAnnul( map );
      sreg = astSimplify( reg );
      map = astRegMapping( sreg );

/* If it is still not UnitMap, we cannot write out the region. */
      if( !astIsAUnitMap( map ) ) {
         astAddWarning( this, 1, "The supplied Region does not have a "
                        "supported shape within its current coordinate "
                        "system.", "astWrite", status );
         ok = 0;
      } 

   } else {
      sreg = astClone( reg );
   }
   map = astAnnul( map );

/* Store a safe value that can be used to test unbounded axes. */
   lim = sqrt( DBL_MAX );

/* First job is to identify the Time, Space, Spectral and Redshift axes
   in the supplied Region. 
   ------------------------------------------------------------------- */

/* Initialise things. */
   timeax = -1;
   nspace = 0;
   issky = 0;
   specax = -1;
   redax = -1;
   prop = NULL;

/* Get a pointer to the Frame encapsulated by the Region. */
   efrm = astRegFrame( sreg );

/* Loop round all axes. */
   nax = astGetNaxes( sreg );
   for( i = 0; i < nax; i++ ) {

/* Get the primary Frame that defines the current axis of the Region. */
      astPrimaryFrame( efrm, i, &pfrm, &pax );

/* Get its class and domain. */
      class = astGetClass( pfrm );
      dom = astGetDomain( pfrm );
      if( astOK ) {

/* The time axis is described by a TimeFrame with any domain. */
         if( !strcmp( class, "TimeFrame" ) ) {
            if( timeax == -1 ) {
               timeax = i;
            } else {
               astAddWarning( this, 1, "More than one time axis found. "
                              "Extra axis (axis %d) will be ignored.",
                              "astWrite", status, i + 1 );
            }

/* The space axes are described by a SkyFrame or a basic Frame. If a
   mixture of both types are found, report a warning and ignore the later
   axes. */
         } else if( !strcmp( class, "SkyFrame" ) ) {
            if( issky || nspace == 0 ) {
               if( nspace < 2 ) {
                  spaceax[ nspace++ ] = i;
                  issky = 1;
               } else {
                  astAddWarning( this, 1, "More than two sky frame axes "
                                 "found. Extra axis (axis %d) will be ignored.",
                                 "astWrite", status, i + 1 );
               }

            } else {
               astAddWarning( this, 1, "Mixture of basic and sky frame "
                              "axes found. Sky frame axis %d will be "
                              "ignored.", "astWrite", status, i + 1 );
            }

         } else if( !strcmp( class, "Frame" ) ) {
            if( !issky ) {
               if( nspace < 3 ) {
                  spaceax[ nspace++ ] = i;
               } else {
                  astAddWarning( this, 1, "More than three basic space frame axes "
                                 "found. Extra axis (axis %d) will be ignored.",
                                 "astWrite", status, i + 1 );
               }

            } else {
               astAddWarning( this, 1, "Mixture of basic and sky frame "
                              "axes found. Basic frame axis %d will be "
                              "ignored.", "astWrite", status, i + 1 );
            }

/* The spectral axis is described by a SpecFrame with domain SPECTRUM. */
         } else if( !strcmp( class, "SpecFrame" ) &&
                    !strcmp( dom, "SPECTRUM" ) ) {
            if( specax == -1 ) {
               specax = i;
            } else {
               astAddWarning( this, 1, "More than one spectral axis found. "
                              "Extra axis (axis %d) will be ignored.",
                              "astWrite", status, i + 1 );
            }

/* The redshift axis is described by a SpecFrame with domain REDSHIFT. */
         } else if( !strcmp( class, "SpecFrame" ) &&
                    !strcmp( dom, "REDSHIFT" ) ) {
            if( redax == -1 ) {
               redax = i;
            } else {
               astAddWarning( this, 1, "More than one redshift axis found. "
                              "Extra axis (axis %d) will be ignored.",
                              "astWrite", status, i + 1 );
            }

/* Warn about unused axes. */
         } else {
            astAddWarning( this, 1, "Could not classify axis %d (class=%s "
                           "domain=%s). It will be ignored.", "astWrite", status, 
                           i + 1, class, dom );
         }
      }

/* Free resources. */
      pfrm = astAnnul( pfrm );
   }
   efrm = astAnnul( efrm );

/* Set a flag indicating if there is anything to convert. */
   ok = ok && ( timeax != -1 || nspace > 0 || specax != -1 || redax != -1 );


/* Now we have identified the axes, we convert each available STC-S
   sub-phrase, starting with the time sub-phrase. 
   ---------------------------------------------------------------- */
   if( timeax != -1 ) {

/* Create a Region by picking the time axis from the supplied Region. */
      spreg = astPickAxes( sreg, 1, &timeax, NULL );

/* Check it is a Region. If not, we cannot convert anything. */
      if( !astIsARegion( spreg ) ) {
         astAddWarning( this, 1, "Cannot determine the region covered by "
                        "the time axis.", "astWrite", status );
         ok = 0;

/* Otherwise we add a description of the time sub-phrase to the
   properties keymap. */
      } else {

/* Get a pointer to the Region's time phrase property KeyMap, creating
   one if necessary. */
         if( astMapGet0A( props, "TIME_PROPS", &obj ) ) {
            spprops = (AstKeyMap *) obj;
         } else {
            spprops = astKeyMap( " ", status );
            astMapPut0A( props, "TIME_PROPS", spprops, NULL );
         }

/* Get the Region's fill factor. */
         fill = astGetFillFactor( spreg );

/* Ensure the TimeFrame represents MJD. If not, take a deep copy (to
   avoid changing the supplied Region), and set its system to MJD. */
         if( astGetSystem( spreg ) != AST__MJD ) {
            treg = astCopy( spreg );
            (void) astAnnul( spreg );
            spreg = treg;
            astSetAdaptive( spreg, 1 );
            astSetSystem( spreg, AST__MJD );
         }

/* Get the bounds of the Region (i.e. the time axis coverage). */
         astGetRegionBounds( spreg, lbnd, ubnd );

/* Get a pointer to the time Region's encapsulated Frame. */
         spfrm = astRegFrame( spreg );

/* Report a warning if the sub-phrase Frame is not a TimeFrame */
         if( !astIsATimeFrame( spfrm ) ) {
            ok = 0;
            astAddWarning( this, 1, "The time sub-phrase in the supplied "
                           "KeyMap is not described using an AST TimeFrame.",
                           "astWrite", status );

/* Store properties that are specific to Time moments... */
         } else if( lbnd[ 0 ] == ubnd[ 0 ] ) {
            astMapPut0C( spprops, "ID", "Time", NULL );
            StoreTimeProp( spprops,  (AstTimeFrame *) spfrm, "TIME", lbnd[ 0 ], status );
            fill = AST__BAD;

/* Store properties that are specific to Time intervals... */
         } else if( lbnd[ 0 ] > -lim && ubnd[ 0 ] < lim ) {
            astMapPut0C( spprops, "ID", "TimeInterval", NULL );
            StoreTimeProp( spprops,  (AstTimeFrame *) spfrm, "START", lbnd[ 0 ], status );
            StoreTimeProp( spprops,  (AstTimeFrame *) spfrm, "STOP", ubnd[ 0 ], status );

/* Store properties that are specific to Start times... */
         } else if( lbnd[ 0 ] > -lim ) {
            astMapPut0C( spprops, "ID", "StartTime", NULL );
            StoreTimeProp( spprops,  (AstTimeFrame *) spfrm, "START", lbnd[ 0 ], status );

/* Store properties that are specific to Stop times... */
         } else {
            astMapPut0C( spprops, "ID", "StopTime", NULL );
            StoreTimeProp( spprops,  (AstTimeFrame *) spfrm, "STOP", ubnd[ 0 ], status );
       
         }

/* Store properties that are common to all time sub-phrase types. First the
   fill factor. */
         MapPut0D( spprops, "FILLFACTOR", fill, 1.0, defs, status );

/* Now the time scale. */
         ts = astGetTimeScale( spfrm );
         if( ts == AST__TT ) {
            tts = "TT";

         } else if( ts == AST__TAI ) {
            tts = "TAI";

         } else if( ts == AST__UTC ) {
            tts = "UTC";

         } else if( ts == AST__TDB ) {
            tts = "TDB";

         } else if( ts == AST__TCG ) {
            tts = "TCG";

         } else if( ts == AST__TCB ) {
            tts = "TCB";

         } else if( ts == AST__LMST ) {
            tts = "LST";

         } else {
            tts = "nil";
            astAddWarning( this, 1, "Timescale '%s' is unsupported by "
                           "STC-S.", "astWrite", status, 
                           astGetC( spfrm, "TimeScale" ) );
            ok = 0;
         }

         MapPut0C( spprops, "TIMESCALE", tts, "nil", defs, status );

/* RefPos. The AST TimeFrame class has no reference position, we leave 
   unchanged any refpos already in the keymap. If there is no refpos in the
   keymap, we use "TOPOCENTER". */ 
         if( !astMapHasKey( spprops, "REFPOS" ) ) {
	    astMapPut0C( spprops, "REFPOS", "TOPOCENTER", NULL ); 
         }

/* That's it for the time sub-phrase, unless the supplied Region has an
   explicit (non-default) uncertainty. */
         unc = astGetUnc( spreg, 0 );
         if( unc ) {

/* See if the supplied properties KeyMap contains any item that refers to
   the Unit included in the STC-S description, but which is not updated by
   this function. If it does, we need to retain any units specified
   within the KeyMap. */
            retain_units = ( astMapHasKey( spprops, "RESOLUTION" ) ||
                             astMapHasKey( spprops, "PIXSIZE" ) ||
                             astMapHasKey( spprops, "SIZE" ) );

            if( retain_units ) {
               if( !astMapGet0C( spprops, "UNIT", &unit ) ) unit = "s";
            } else {
               unit = "s";
            }

/* If necessary, map the uncertainty region into the requied units. Take
   a deep copy to avoid changing the supplied Region. */
            if( strcmp( unit, astGetUnit( unc, 0 ) ) ) {
               unca = astCopy( unc );
               astSetAdaptive( unca, 0 );
               astSetUnit( unca, 0, unit );
            } else {
               unca = astClone( unc );
            }            

/* Get the bounds of the uncertainty. */
            astGetRegionBounds( unca, lbnd, ubnd );

/* The error is half the width of the bounding box. */
            astMapPut0D( spprops, "ERROR", 0.5*( ubnd[ 0 ] - lbnd[ 0 ] ), NULL );

/* Free resources. */
            unca = astAnnul( unca );
            unc = astAnnul( unc );
         }

/* Free resources. */
         spfrm = astAnnul( spfrm );
         spprops = astAnnul( spprops );
      }

/* Free resources. */
      spreg = astAnnul( spreg );

   }


/* Now convert the space sub-phrase.
   ---------------------------------------------------------------- */
   if( nspace > 0 && ok ) {

/* Create a Region by picking the space axes from the supplied Region. */
      spreg = astPickAxes( sreg, nspace, spaceax, NULL );

/* Check it is a Region. If not, we cannot convert anything. */
      if( ! astIsARegion( spreg ) ) {
         astAddWarning( this, 1, "Cannot determine the region covered by "
                        "the space axes.", "astWrite", status );
         ok = 0;

/* Otherwise we add a description of the space sub-phrase to the
   properties keymap. */
      } else {

/* Get a pointer to the Region's space phrase property KeyMap, creating
   one if necessary. */
         if( astMapGet0A( props, "SPACE_PROPS", &obj ) ) {
            spprops = (AstKeyMap *) obj;
         } else {
            spprops = astKeyMap( " ", status );
            astMapPut0A( props, "SPACE_PROPS", spprops, NULL );
         }

/* If the space frame is a SkyFrame, ensure it refers to a coodinate
   system that is supported by STC-S. Take a deep copy before changing
   anything. */
         if( issky ) {
            sys = astGetSystem( spreg );
            if( sys != AST__FK4 &&
                sys != AST__FK5 &&
                sys != AST__ICRS &&
                sys != AST__ECLIPTIC &&
                sys != AST__GALACTIC &&
                sys != AST__SUPERGALACTIC &&
                sys != AST__UNKNOWN ) {
               treg = astCopy( spreg );
               (void) astAnnul( spreg );
               spreg = treg;
               astSetAdaptive( spreg, 1 );
               astSetSystem( spreg, AST__ICRS );
            }
         }               

/* Get a pointer to the Region's encapsulated Frame. */
         spfrm = astRegFrame( spreg );

/* If the supplied Region is defined in a SkyFrame, choose the units to
   use when storing radius, error, etc in the KeyMap. If the props KeyMap
   already contains a unit specification, we use it. Otherwise we use the
   default (degrees). AST uses radians internally, so find the scaling 
   factor. */
         if( issky ) {
            if( astMapGet0C( spprops, "UNIT", &unit ) ) {
               if( !strcmp( unit, "arcmin" ) ) {
                  scale = AST__DR2D*60.0;
               } else if( !strcmp( unit, "arcsec" ) ) {
                  scale = AST__DR2D*3600.0;
               } else {
                  unit = "deg";
                  scale = AST__DR2D;
               }
            } else {
               unit = "deg";
               scale = AST__DR2D;
            }

/* If the supplied Region is not defined in a SkyFrame, we will arrange
   that the Region and the KeyMap use the same units, so set a scale 
   factor of 1.0. */
         } else {
            scale = 1.0;

/* Check that the supplied Region uses the same units on every spatial
   axis. */
            runit = astGetUnit( spfrm, 0 );
            for( i = 1; i < nspace; i++ ) {
               if( strcmp( runit, astGetUnit( spfrm, i ) ) ) {
                  astAddWarning( this, 1, "Spatial axis 1 has units "
                                 "'%s' but spatial axis %d has units "
                                 "'%s' - units must be the same on "
                                 "all axes.", "astWrite", status, runit, 
                                 i + 1, astGetUnit( spfrm, i ) );
                  ok = 0; 
                  break;
               }
            }

/* See if the supplied properties KeyMap contains any item that refers to
   the Unit included in the STC-S description, but which is not updated by
   this function. If it does, we need to retain any units specified
   within the KeyMap. */
            retain_units = ( astMapHasKey( spprops, "RESOLUTION" ) ||
                             astMapHasKey( spprops, "PIXSIZE" ) ||
                             astMapHasKey( spprops, "SIZE" ) );

/* If so, and if the properties KeyMap already contains a Unit 
   specification, we convert the Region to the same units. Take a deep 
   copy of the Region first to avoid modifying the supplied Region. */
            if( retain_units ) {
               if( !astMapGet0C( spprops, "UNIT", &unit ) ) unit = "deg";

               treg = astCopy( spreg );
               (void) astAnnul( spreg );
               spreg = treg;

               for( i = 0; i < nspace; i++ ) {
                  astSetUnit( spreg, i, unit );
               }

/* If we are not retaining the units specified in the properties KeyMap, we
   retain the existing Region units instead, and store these units in the
   properties KeyMap. We also check that these units are supported by
   STC-S. */
            } else {
               unit = runit;
               astMapPut0C( spprops, "UNIT", runit, NULL );
               if( strcmp( runit, "deg" ) &&
                   strcmp( runit, "arcmin" ) &&
                   strcmp( runit, "arcsec" ) &&
                   strcmp( runit, "m" ) &&
                   strcmp( runit, "mm" ) &&
                   strcmp( runit, "km" ) &&
                   strcmp( runit, "AU" ) &&
                   strcmp( runit, "pc" ) &&
                   strcmp( runit, "kpc" ) &&
                   strcmp( runit, "Mpc" ) ) {
                  astAddWarning( this, 1, "Cannot use spatial units '%s'.", 
                                 "astWrite", status, runit );
                  ok = 0; 
               }                  
            }
         }

/* Get the fill factor. */
         fill = astGetFillFactor( spreg );

/* Get the default number of digits. This is only used if the supplied
   properties KeyMap does not have a value for the item being stored. If
   it does, the number of digits is inherited form the value int he KeyMap. */
         defdigs = astGetDigits( spfrm );

/* Store properties that are specific to AllSky sub-phrases (i.e. none)... */
         if( astIsANullRegion( spreg ) && astGetNegated( spreg ) ) {
            astMapPut0C( spprops, "ID", "AllSky", NULL );

/* Store properties that are specific to Circle sub-phrases... */
         } else if( astIsACircle( spreg ) ) {
            astMapPut0C( spprops, "ID", "Circle", NULL );

/* Get the geometric parameters of the Circle. */
            astCirclePars( spreg, centre, &a, NULL );

/* Create a string holding the formatted centre axis values, scaling
   to the required units. Use the Frame's Digits attribute to specify 
   how many digits to use when formatting the axis values. */
            nc = 0;
            for( i = 0; i < nspace; i++ ) {
               if( centre[ i ] != AST__BAD ) {
                  GetFmt( "CENTRE", spprops, i, defdigs, fmt, status );
                  (void) sprintf( buf, fmt, scale*centre[ i ] );
                  prop = astAppendString( prop, &nc, buf );
                  prop = astAppendString( prop, &nc, " " );

               } else {
                  ok = 0;
                  astAddWarning( this, 1, "The supplied Circle contains "
                                 "one or more bad centre axis values.", 
                                 "astWrite", status );
                  break;
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( spprops, "CENTRE", prop, NULL );

/* Scale, format and store the radius. */
            if( a != AST__BAD ) {
               GetFmt( "RADIUS", spprops, 0, defdigs, fmt, status );
               (void) sprintf( buf, fmt, scale*a );
               astMapPut0C( spprops, "RADIUS", buf, NULL );
            } else {
               ok = 0;
               astAddWarning( this, 1, "The supplied Circle has an "
                              "undefined radius.", "astWrite", status );
            }

/* Store properties that are specific to PositionInterval sub-phrases... */
         } else if( astIsAInterval( spreg ) || astIsABox( spreg ) ) {
            astMapPut0C( spprops, "ID", "PositionInterval", NULL );

/* Get the bounds of the Region. */
            astGetRegionBounds( spreg, lbnd, ubnd );

/* Create a string holding the formatted low limits, scaling to the 
   required units. Use the Frame's Digits attribute to specify how 
   many digits to use when formatting the axis values. */
            nc = 0;
            for( i = 0; i < nspace; i++ ) {
               if( lbnd[ i ] == AST__BAD || lbnd[ i ] == DBL_MAX || 
                   lbnd[ i ] == -DBL_MAX ) {
                  astAddWarning( this, 1, "Spatial axis %d has an undefined "
                                 "lower limit.", "astWrite", status, i + 1 );
                  ok = 0; 
                  break;
               } else {
                  GetFmt( "LOLIMIT", spprops, i, defdigs, fmt, status );
                  (void) sprintf( buf, fmt, scale*lbnd[ i ] );
                  prop = astAppendString( prop, &nc, buf );
                  prop = astAppendString( prop, &nc, " " );
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "LOLIMIT", prop, NULL );
         
/* Do the same for the upper limits. */
            nc = 0;
            for( i = 0; i < nspace; i++ ) {
               if( ubnd[ i ] == AST__BAD || ubnd[ i ] == DBL_MAX || 
                   ubnd[ i ] == -DBL_MAX ) {
                  astAddWarning( this, 1, "Spatial axis %d has an undefined "
                                 "upper limit.", "astWrite", status, i + 1 );
                  ok = 0; 
                  break;
               } else {
                  GetFmt( "HILIMIT", spprops, i, defdigs, fmt, status );
                  (void) sprintf( buf, fmt, scale*ubnd[ i ] );
                  prop = astAppendString( prop, &nc, buf );
                  prop = astAppendString( prop, &nc, " " );
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( props, "HILIMIT", prop, NULL );
         
/* Store properties that are specific to Ellipse sub-phrases... */
         } else if( astIsAEllipse( spreg ) ) {
            astMapPut0C( spprops, "ID", "Ellipse", NULL );

/* Get the geometric parameters of the Ellipse. */
            astEllipsePars( spreg, centre, &a, &b, &angle, NULL, NULL );

/* Create a string holding the formatted centre axis values, scaling
   to the required units. Use the Frame's Digits attribute to specify 
   how many digits to use when formatting the axis values. */
            nc = 0;
            for( i = 0; i < nspace; i++ ) {
               if( centre[ i ] != AST__BAD ) {
                  GetFmt( "CENTRE", spprops, i, defdigs, fmt, status );
                  (void) sprintf( buf, fmt, scale*centre[ i ] );
                  prop = astAppendString( prop, &nc, buf );
                  prop = astAppendString( prop, &nc, " " );

               } else {
                  ok = 0;
                  astAddWarning( this, 1, "The supplied Ellipse contains "
                                 "one or more bad centre axis values.", 
                                 "astWrite", status );
                  break;
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( spprops, "CENTRE", prop, NULL );

/* Scale, format and store the two radii. */
            if( a != AST__BAD && b != AST__BAD && angle != AST__BAD ) {
               GetFmt( "RADIUS1", spprops, 0, defdigs, fmt, status );
               (void) sprintf( buf, fmt, scale*a );
               astMapPut0C( spprops, "RADIUS1", buf, NULL );
  
               GetFmt( "RADIUS2", spprops, 0, defdigs, fmt, status );
               (void) sprintf( buf, fmt, scale*b );
               astMapPut0C( spprops, "RADIUS2", buf, NULL );

/* Convert the angle to degrees in the direction required by STC-S, 
   format and store. */
               angle *= AST__DR2D;
               if( !issky )  angle = 90 - angle;
               while( angle < 0.0 ) angle += 360.0;
               while( angle >= 360.0 ) angle -= 360.0;

               GetFmt( "POSANGLE", spprops, 0, defdigs, fmt, status );
               (void) sprintf( buf, fmt, angle );
               astMapPut0C( spprops, "POSANGLE", buf, NULL );

            } else {
               astAddWarning( this, 1, "The gemeotric parameters of the "
                              "supplied Ellipse are undefined.",
                              "astWrite", status );
               ok = 0;
            }

/* Store properties that are specific to Polygon sub-phrases... */
         } else if( astIsAPolygon( spreg ) ) {
            astMapPut0C( spprops, "ID", "Polygon", NULL );

/* Get an array holding the axis values at the polygon vertices. */
            astGetRegionPoints( spreg, 0, 0, &np, NULL );
            points = astMalloc( sizeof( double )*np*nspace );
            astGetRegionPoints( spreg, np, nspace, &np, points );

/* Create a string holding the formatted vertex axis values, scaling
   to the required units. Use the Frame's Digits attribute to specify 
   how many digits to use when formatting the axis values. */
            GetFmt( "VERTICES", spprops, 0, defdigs, fmt, status );
            nc = 0;
            for( j = 0; j < np; j++ ) {
               p = points + j;
               for( i = 0; i < nspace; i++ ) {
                  if( *p != AST__BAD ) {
                     (void) sprintf( buf, fmt, scale*(*p) );
                     prop = astAppendString( prop, &nc, buf );
                     prop = astAppendString( prop, &nc, " " );
                     p += np;
                  } else {
                     astAddWarning( this, 1, "The supplied Polygon contains "
                                    "one or more bad axis values.", "astWrite", 
                                    status );
                     ok = 0;
                     break;
                  }
               }
            }

/* Remove the trailing space, and store the property value in the KeyMap. */
            prop[ nc - 1 ] = 0;
            astMapPut0C( spprops, "VERTICES", prop, NULL );

/* Free resources. */
            points = astFree( points );

/* Store properties that are specific to Position sub-phrases... */
         } else if( astIsAPointList( spreg ) ) {
            astMapPut0C( spprops, "ID", "Position", NULL );

/* Check the PointList contains only a single point. */
            astGetRegionPoints( spreg, 0, 0, &np, NULL );
            if( np > 1 ) {
               astAddWarning( this, 1, "The supplied PointList contains "
                              "more than one position.", "astWrite", status );
               ok = 0; 

/* If so, get the axis values at the point. */
            } else {
               astGetRegionPoints( spreg, 1, nspace, &np, centre );

/* Create a string holding the formatted axis values, scaling to the 
   required units. Use the Frame's Digits attribute to specify how many 
   digits to use when formatting the axis values. */
               nc = 0;
               for( i = 0; i < nspace; i++ ) {
                  if( centre[ i ] != AST__BAD ) {
                     GetFmt( "POSITION", spprops, i, defdigs, fmt, status );
                     (void) sprintf( buf, fmt, scale*centre[ i ] );
                     prop = astAppendString( prop, &nc, buf );
                     prop = astAppendString( prop, &nc, " " );
                     p += nspace;

                  } else {
                     astAddWarning( this, 1, "The supplied PointList contains "
                                    "one or more bad axis values.", "astWrite", 
                                    status );
                     ok = 0;
                     break;
                  }
               }

/* Remove the trailing space, and store the property value in the KeyMap. */
               prop[ nc - 1 ] = 0;
               astMapPut0C( spprops, "POSITION", prop, NULL );

/* A Position has no fill factor. */
               fill = AST__BAD;
            }

/* All other classes of Region are unsupported. */
         } else {
            astAddWarning( this, 1, "The supplied %s cannot be written "
                           "out since STC-S does not support %s regions.",
                           "astWrite", status, astGetClass( spreg ),
                           astGetClass( spreg ) ); 
            ok = 0;
         }

/* If the above went OK, store values for the properties that are common
   to all types of space sub-phrase. */
         if( ok ) {

/* First the fill factor. */
            MapPut0D( spprops, "FILLFACTOR", fill, 1.0, defs, status );

/* Now the coordinate frame. */
            tfrm = NULL;
            sys = astGetSystem( spfrm );
            if( issky ) {
               if( sys == AST__FK4 ){
                  tfrm = "B1950";
                  equinox = 1950.0;

               } else if( sys == AST__FK5 ){
                  tfrm = "J2000";
                  equinox = 2000.0;

               } else if( sys == AST__ICRS ){
                  tfrm = "ICRS";
                  equinox = AST__BAD;

               } else if( sys == AST__ECLIPTIC ){
                  tfrm = "ECLIPTIC";
                  equinox = 2000.0;

               } else if( sys == AST__GALACTIC ){
                  tfrm = "GALACTIC";
                  equinox = AST__BAD;

               } else if( sys == AST__SUPERGALACTIC ){
                  tfrm = "SUPER_GALACTIC";
                  equinox = AST__BAD;

               } else if( sys == AST__UNKNOWN ){
                  tfrm = NULL;
                  equinox = AST__BAD;

               } else {
                  tfrm = NULL;
                  astAddWarning( this, 1, "Sky system '%s' is "
                                 "unsupported by STC-S.", "astWrite", 
                                 status, astGetC( spfrm, "System" ) );
                  ok = 0;
               }

               if( tfrm && equinox != AST__BAD ) {
                  if( astGetD( spfrm, "Equinox" ) != equinox ) {
                     astAddWarning( this, 1, "STC-S requires an equinox "
                                    "of %g for the %s frame, but the "
                                    "supplied %s equinox is %g.", "astWrite", 
                                    status, equinox, tfrm, 
                                    astGetClass( spfrm ), 
                                    astGetD( spfrm, "Equinox" ) );
                     ok = 0;
                     tfrm = NULL;
                  }
               }
            }

/* If we do not yet have a Frame, use the Domain value if it is set (and
   is a legal STC-S Frame). */
            if( ! tfrm ) {
               if( astTestDomain( spfrm ) ) {
                  tfrm = astGetDomain( spfrm );
                  if( strcmp( tfrm, "ICRS" ) &&
                      strcmp( tfrm, "FK5" ) &&
                      strcmp( tfrm, "FK4" ) &&
                      strcmp( tfrm, "J2000" ) &&
                      strcmp( tfrm, "B1950" ) &&
                      strcmp( tfrm, "ECLIPTIC" ) &&
                      strcmp( tfrm, "GALACTIC" ) &&
                      strcmp( tfrm, "GALACTIC_II" ) &&
                      strcmp( tfrm, "SUPER_GALACTIC" ) &&
                      strcmp( tfrm, "GEO_C" ) &&
                      strcmp( tfrm, "GEO_D" ) ){
                     astAddWarning( this, 1, "'UNKNOWNFrame' being used in "
                                    "place of unsupported frame '%s'.",
                                     "astWrite", status, tfrm );
                     tfrm = NULL;
                  }
               }
            }

/* Store the Frame name in the props keymap. */
            if( !tfrm ) tfrm = "UNKNOWNFrame";
            MapPut0C( spprops, "FRAME", tfrm, "UNKNOWNFrame", defs, status );

/* RefPos. The AST SkyFrame and Frame classes have no reference position, so 
   we leave unchanged any refpos already in the props keymap. If there is
   no refpos in the keymap, we use "TOPOCENTER". */
            if( !astMapHasKey( spprops, "REFPOS" ) ) {
               astMapPut0C( spprops, "REFPOS", "TOPOCENTER", NULL );
            }

/* Flavour. */
            if( issky ) {
               flavour = "SPHER2";
            } else if( nspace == 1 ){
               flavour = "CART1";
            } else if( nspace == 2 ){
               flavour = "CART2";
            } else {
               flavour = "CART3";
            }
            MapPut0C( spprops, "FLAVOUR", flavour, "SPHER2", defs, status );

/* That's it for the space sub-phrase, unless the supplied Region has an
   explicit (non-default) uncertainty. */
            unc = astGetUnc( spreg, 0 );
            if( unc ) {

/* Get the bounds of the uncertainty. */
               astGetRegionBounds( unc, lbnd, ubnd );

/* If its a sky frame, find the position of the centre of the uncertainty
   region. */
               pcen = issky ? astRegCentre( unc, NULL, NULL, 0, 
                                            AST__CURRENT ) : NULL;

/* Find the half-width of the bounding box for each space axis, and
   concatenate their formatted values into a string. If any bound is
   undefined, quit the axis loop with nc=0. We need to convert longitude
   axis values from lingitude increments to arc-distance.  */
               nc = 0;
               defdigs = astGetDigits( unc );

               for( i = 0; i < nspace; i++ ) {
                  if( ubnd[ i ] != AST__BAD && lbnd[ i ] != AST__BAD ){

                     if( ! issky ) {
                        error = 0.5*( ubnd[ i ] - lbnd[ i ] );
                     } else {
                        if( i == 0 ) {
                           p1[ 0 ] = ubnd[ 0 ];
                           p1[ 1 ] = pcen[ 1 ];
                        } else {
                           p1[ 0 ] = pcen[ 0 ];
                           p1[ 1 ] = ubnd[ 1 ];
                        }
                        error = astDistance( spfrm, pcen, p1 );
                     }

                     GetFmt( "ERROR", spprops, i, defdigs, fmt, status );
                     (void) sprintf( buf, fmt, scale*error );
                     prop = astAppendString( prop, &nc, buf );
                     prop = astAppendString( prop, &nc, " " );

                  } else {
                     nc = 0;
                     break;
                  }
               }

/* If the bounds were all good, store the string holding the formatted
   error values in the properties KeyMap. */
               if( nc > 0 ) {
                  prop[ nc - 1 ] = 0;
                  astMapPut0C( spprops, "ERROR", prop, NULL );
               }

/* Free resources. */
               pcen = astFree( pcen );
               unc = astAnnul( unc );
            }
         }

/* Free resources. */
         spfrm = astAnnul( spfrm );
         spprops = astAnnul( spprops );
      }

/* Free resources. */
      spreg = astAnnul( spreg );

   }



/* Convert the spectral sub-phrase. 
   ---------------------------------------------------------------- */
   if( specax != -1 ) {

/* Create a Region by picking the spectral axis from the supplied Region. */
      spreg = astPickAxes( sreg, 1, &specax, NULL );

/* Check it is a Region. If not, we cannot convert anything. */
      if( !astIsARegion( spreg ) ) {
         astAddWarning( this, 1, "Cannot determine the region covered by "
                        "the spectral axis.", "astWrite", status );
         ok = 0;

/* Otherwise we add a description of the spectral sub-phrase to the
   properties keymap. */
      } else {

/* Get a pointer to the Region's spectral phrase property KeyMap, creating
   one if necessary. */
         if( astMapGet0A( props, "SPECTRAL_PROPS", &obj ) ) {
            spprops = (AstKeyMap *) obj;
         } else {
            spprops = astKeyMap( " ", status );
            astMapPut0A( props, "SPECTRAL_PROPS", spprops, NULL );
         }

/* See if the supplied properties KeyMap contains any item that refers to
   the Unit included in the STC-S description, but which is not updated by
   this function. If it does, we need to retain any units specified
   within the KeyMap. */
         retain_units = ( astMapHasKey( spprops, "RESOLUTION" ) ||
                          astMapHasKey( spprops, "PIXSIZE" ) ||
                          astMapHasKey( spprops, "SIZE" ) );

/* If so, and if the properties KeyMap already contains a Unit specification, 
   we convert the Region to the same units and system. Determine the
   required system and units. */
         if( retain_units ) {
            if( !astMapGet0C( spprops, "UNIT", &unit ) ) unit = "Hz";

            if( !strcmp( unit, "Hz" ) ||
                !strcmp( unit, "MHz" ) ||
                !strcmp( unit, "GHz" ) ) {
               sys = AST__FREQ;

            } else if( !strcmp( unit, "m" ) ||
                       !strcmp( unit, "mm" ) ||
                       !strcmp( unit, "um" ) ||
                       !strcmp( unit, "nm" ) ||
                       !strcmp( unit, "Angstrom" ) ) {
               sys = AST__WAVELEN;

            } else if( !strcmp( unit, "eV" ) ||
                       !strcmp( unit, "keV" ) ||
                       !strcmp( unit, "MeV" ) ) {
               sys = AST__ENERGY;

            } else {
               astAddWarning( this, 1, "Illegal STC-S units '%s' found in "
                              "supplied KeyMap", "astWrite", status, unit );
               ok = 0;
            }
         
/* If we do not need to retain the units implied by the supplied KeyMap,
   use the Units and system in the supplied Region so long as they are  
   supported by STC-S. If not, use a related supported system instead. */
         } else {
            sys = astGetSystem( spreg );
            unit = astGetUnit( spreg, 0 );

            if( sys == AST__ENERGY ) {
               sys = AST__ENERGY;
               if( strcmp( unit, "eV" ) &&
                   strcmp( unit, "keV" ) &&
                   strcmp( unit, "MeV" ) ) unit = "eV";

            } else if( sys == AST__WAVELEN || sys == AST__AIRWAVE ||
                       sys == AST__VOPTICAL || sys == AST__REDSHIFT ){
               sys = AST__WAVELEN;
               if( strcmp( unit, "m" ) &&
                   strcmp( unit, "mm" ) &&
                   strcmp( unit, "um" ) &&
                   strcmp( unit, "nm" ) &&
                   strcmp( unit, "Angstrom" ) ) unit = "m";

            } else {
               sys = AST__FREQ;
               if( strcmp( unit, "Hz" ) &&
                   strcmp( unit, "MHz" ) &&
                   strcmp( unit, "GHz" ) ) unit = "Hz";

            }
         }

/* If either the System or Unit needs to be changed in the Region, take a
   deep copy first in order to avoid changing the supplied Region. */
         if( sys != astGetSystem( spreg ) ||
             ( unit && strcmp( unit, astGetUnit( spreg, 0 ) ) ) ) {
            treg = astCopy( spreg );
            (void) astAnnul( spreg );
            spreg = treg;
            astSetAdaptive( spreg, 1 );
            astSetSystem( spreg, sys );
            astSetUnit( spreg, 0, unit );
         }

/* Get the Region's fill factor. */
         fill = astGetFillFactor( spreg );

/* Get the bounds of the Region (i.e. the spectral axis coverage). */
         astGetRegionBounds( spreg, lbnd, ubnd );

/* Get a pointer to the spectral Region's encapsulated Frame. */
         spfrm = astRegFrame( spreg );

/* Report a warning if the sub-phrase Frame is not a SpecFrame */
         if( !astIsASpecFrame( spfrm ) ) {
            ok = 0;
            astAddWarning( this, 1, "The spectral sub-phrase in the supplied "
                           "KeyMap is not described using an AST SpecFrame.",
                           "astWrite", status );

/* Store properties that are specific to spectral positions... */
         } else if( lbnd[ 0 ] == ubnd[ 0 ] ) {
            astMapPut0C( spprops, "ID", "Spectral", NULL );
            astMapPut0D( spprops, "SPECTRAL", lbnd[ 0 ], NULL );
            fill = AST__BAD;

/* Store properties that are specific to Spectral intervals... */
         } else if( lbnd[ 0 ] > -lim && ubnd[ 0 ] < lim ) {
            astMapPut0C( spprops, "ID", "SpectralInterval", NULL );
            astMapPut0D( spprops, "LOLIMIT", lbnd[ 0 ], NULL );
            astMapPut0D( spprops, "HILIMIT", ubnd[ 0 ], NULL );

         } else {
            ok = 0;
            astAddWarning( this, 1, "Cannot write out an unbounded "
                           "spectral interval.", "astWrite", status );
         }

/* Store properties that are common to all spectral sub-phrase types. First the
   fill factor. */
         MapPut0D( spprops, "FILLFACTOR", fill, 1.0, defs, status );

/* Now the reference position. */
         sor = astGetStdOfRest( spfrm );
         if( sor == AST__GESOR ) {
            tsor = "GEOCENTER";

         } else if( sor == AST__BYSOR ) {
            tsor = "BARYCENTER";

         } else if( sor == AST__HLSOR ) {
            tsor = "HELIOCENTER";

         } else if( sor == AST__TPSOR ) {
            tsor = "TOPOCENTER";

         } else if( sor == AST__LKSOR ) {
            tsor = "LSRK";

         } else if( sor == AST__LDSOR ) {
            tsor = "LSRD";

         } else if( sor == AST__GLSOR ) {
            tsor = "GALACTIC_CENTER";

         } else {
            tsor = NULL;
         }

         if( !tsor ) tsor = "UNKNOWNRefPos";
         MapPut0C( spprops, "REFPOS", tsor, "UNKNOWNRefPos", defs,
                   status );

/* Now the unit string. */
         MapPut0C( spprops, "UNIT", unit, "Hz", defs, status );

/* That's it for the spectral sub-phrase, unless the supplied Region has an
   explicit (non-default) uncertainty. */
         unc = astGetUnc( spreg, 0 );
         if( unc ) {

/* Get the bounds of the uncertainty. */
            astGetRegionBounds( unc, lbnd, ubnd );

/* The error is half the width of the bounding box. */
            astMapPut0D( spprops, "ERROR", 0.5*( ubnd[ 0 ] - lbnd[ 0 ] ), NULL );

/* Free resources. */
            unc = astAnnul( unc );
         }

/* Free resources. */
         spfrm = astAnnul( spfrm );
         spprops = astAnnul( spprops );
      }

/* Free resources. */
      spreg = astAnnul( spreg );

   }



/* Convert the redshift sub-phrase. 
   ---------------------------------------------------------------- */
   if( redax != -1 ) {

/* Create a Region by picking the redshift axis from the supplied Region. */
      spreg = astPickAxes( sreg, 1, &redax, NULL );

/* Check it is a Region. If not, we cannot convert anything. */
      if( !astIsARegion( spreg ) ) {
         astAddWarning( this, 1, "Cannot determine the region covered by "
                        "the redshift axis.", "astWrite", status );
         ok = 0;

/* Otherwise we add a description of the redshift sub-phrase to the
   properties keymap. */
      } else {

/* Get a pointer to the Region's redshift phrase property KeyMap, creating
   one if necessary. */
         if( astMapGet0A( props, "REDSHIFT_PROPS", &obj ) ) {
            spprops = (AstKeyMap *) obj;
         } else {
            spprops = astKeyMap( " ", status );
            astMapPut0A( props, "REDSHIFT_PROPS", spprops, NULL );
         }

/* See if the supplied properties KeyMap contains any item that refers to
   the system included in the STC-S description, but which is not updated by
   this function. If it does, we need to retain any system specified
   within the KeyMap. */
         retain_units = ( astMapHasKey( spprops, "RESOLUTION" ) ||
                          astMapHasKey( spprops, "PIXSIZE" ) ||
                          astMapHasKey( spprops, "SIZE" ) );

/* If so, and if the properties KeyMap already contains a DopplerDef or
   Type specification, we convert the Region to the same system. */
         if( retain_units ){
            if( !astMapGet0C( spprops, "DOPPLERDEF", &dopdef ) ) dopdef = "OPTICAL";
            if( !astMapGet0C( spprops, "TYPE", &type ) ) type = "VELOCITY";

            if( !strcmp( type, "VELOCITY" ) ) {
               if( !strcmp( dopdef, "OPTICAL" ) ) {
                  sys = AST__VOPTICAL;
               } else if( !strcmp( dopdef, "RADIO" ) ) {
                  sys = AST__VRADIO;
               } else if( !strcmp( dopdef, "RELATIVISTIC" ) ) {
                  sys = AST__VREL;
               } else {
                  astAddWarning( this, 1, "Illegal STC-S DopplerDef '%s' "
                                 "found in supplied KeyMap", "astWrite", status, 
                                 dopdef );
                  ok = 0;
               }

            } else if( !strcmp( type, "REDSHIFT" ) ) {
               if( !strcmp( dopdef, "OPTICAL" ) ) {
                  sys = AST__REDSHIFT;
               } else {
                  astAddWarning( this, 1, "Unsupported combination of "
                                 "DopplerDef='%s' and Type='%s' found in "
                                 "supplied KeyMap", "astWrite", status, dopdef,
                                 type );
                  ok = 0;
               }

            } else {
               astAddWarning( this, 1, "Illegal STC-S Redshift Type '%s' "
                              "found in supplied KeyMap", "astWrite", status, 
                              type );
               ok = 0;
            } 

/* If the supplied KeyMap does not imply the required system, use the 
   system in the supplied Region. */
         } else {
            sys = astGetSystem( spreg );
         }

/* Choose the requied units. */
         unit = ( sys == AST__REDSHIFT ) ? "": "km/s";

/* If either the System or Unit needs to be changed in the Region, take a
   deep copy first in order to avoid changing the supplied Region. */
         if( sys != astGetSystem( spreg ) ||
             ( unit && strcmp( unit, astGetUnit( spreg, 0 ) ) ) ) {
            treg = astCopy( spreg );
            (void) astAnnul( spreg );
            spreg = treg;
            astSetAdaptive( spreg, 1 );
            astSetSystem( spreg, sys );
            astSetUnit( spreg, 0, unit );
         }

/* Get the Region's fill factor. */
         fill = astGetFillFactor( spreg );

/* Get the bounds of the Region (i.e. the redshift axis coverage). */
         astGetRegionBounds( spreg, lbnd, ubnd );

/* Get a pointer to the spectral Region's encapsulated Frame. */
         spfrm = astRegFrame( spreg );

/* Report a warning if the sub-phrase Frame is not a SpecFrame */
         if( !astIsASpecFrame( spfrm ) ) {
            ok = 0;
            astAddWarning( this, 1, "The redshift sub-phrase in the supplied "
                           "KeyMap is not described using an AST SpecFrame.",
                           "astWrite", status );

/* Store properties that are specific to redshift positions... */
         } else if( lbnd[ 0 ] == ubnd[ 0 ] ) {
            astMapPut0C( spprops, "ID", "Redshift", NULL );
            astMapPut0D( spprops, "REDSHIFT", lbnd[ 0 ], NULL );
            fill = AST__BAD;

/* Store properties that are specific to Redshift intervals... */
         } else if( lbnd[ 0 ] > -lim && ubnd[ 0 ] < lim ) {
            astMapPut0C( spprops, "ID", "RedshiftInterval", NULL );
            astMapPut0D( spprops, "LOLIMIT", lbnd[ 0 ], NULL );
            astMapPut0D( spprops, "HILIMIT", ubnd[ 0 ], NULL );

         } else {
            ok = 0;
            astAddWarning( this, 1, "Cannot write out an unbounded "
                           "redshift interval.", "astWrite", status );
         }

/* Store properties that are common to all redshift sub-phrase types. First the
   fill factor. */
         MapPut0D( spprops, "FILLFACTOR", fill, 1.0, defs, status );

/* Now the reference position. */
         sor = astGetStdOfRest( spfrm );

         if( sor == AST__GESOR ) {
            tsor = "GEOCENTER";

         } else if( sor == AST__BYSOR ) {
            tsor = "BARYCENTER";

         } else if( sor == AST__HLSOR ) {
            tsor = "HELIOCENTER";

         } else if( sor == AST__TPSOR ) {
            tsor = "TOPOCENTER";

         } else if( sor == AST__LKSOR ) {
            tsor = "LSRK";

         } else if( sor == AST__LDSOR ) {
            tsor = "LSRD";

         } else if( sor == AST__GLSOR ) {
            tsor = "GALACTIC_CENTER";

         } else {
            tsor = NULL;
         }

         if( !tsor ) tsor = "UNKNOWNRefPos";
         MapPut0C( spprops, "REFPOS", tsor, "UNKNOWNRefPos", defs,
                   status );

/* Type and DopplerDef. */
         if( sys == AST__VOPTICAL ) {
            type = "VELOCITY";
            dopdef = "OPTICAL";

         } else if( sys == AST__VRADIO ) {
            type = "VELOCITY";
            dopdef = "RADIO";

         } else if( sys == AST__VREL ) {
            type = "VELOCITY";
            dopdef = "RELATIVISTIC";

         } else {
            type = "REDSHIFT";
            dopdef = "OPTICAL";
         }
         MapPut0C( spprops, "DOPPLERDEF", dopdef, "OPTICAL", defs,
                   status );
         MapPut0C( spprops, "TYPE", type, "VELOCITY", defs, status );

/* Now the unit string. */
         MapPut0C( spprops, "UNIT", unit, unit, defs, status );

/* That's it for the redshift sub-phrase, unless the supplied Region has an
   explicit (non-default) uncertainty. */
         unc = astGetUnc( spreg, 0 );
         if( unc ) {

/* Get the bounds of the uncertainty. */
            astGetRegionBounds( unc, lbnd, ubnd );

/* The error is half the width of the bounding box. */
            astMapPut0D( spprops, "ERROR", 0.5*( ubnd[ 0 ] - lbnd[ 0 ] ), NULL );

/* Free resources. */
            unc = astAnnul( unc );
         }

/* Free resources. */
         spfrm = astAnnul( spfrm );
         spprops = astAnnul( spprops );
      }

/* Free resources. */
      spreg = astAnnul( spreg );

   }

/* Free resources */
   if( sreg ) sreg = astAnnul( sreg );
   if( prop ) prop = astFree( prop );

/* Return the result. */
   return ok;
}

/* Functions which access class attributes. */
/* ---------------------------------------- */
/* Implement member functions to access the attributes associated with
   this class using the macros defined for this purpose in the
   "object.h" file. For a description of each attribute, see the class
   interface (in the associated .h file). */

/*
*att++
*  Name:
*     StcsArea

*  Purpose:
*     Return the CoordinateArea component when reading an STC-S document?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean).

*  Description:
*     This is a boolean attribute which controls what is returned
*     by the
c     astRead
f     AST_READ
*     function when it is used to read from an StcsChan.
*     If StcsArea is set non-zero (the default), then a Region
*     representing the STC CoordinateArea will be returned by 
c     astRead.
f     AST_READ.
*     If StcsArea is set to zero, then the STC CoordinateArea
*     will not be returned.

*  Notes:
*     - Other attributes such as StcsCoords and StcsProps can be used to
*     specify other Objects to be returned by 
c     astRead.
f     AST_READ.
*     If more than one of these attributes is set non-zero, then the
*     actual Object returned by 
c     astRead
f     AST_READ
*     will be a KeyMap, containing the requested Objects. In this
*     case, the Region representing the STC CoordinateArea will be
*     stored in the returned KeyMap using the key "AREA". If StcsArea
*     is the only attribute to be set non-zero, then the Object returned by
c     astRead
f     AST_READ
*     will be the CoordinateArea Region itself.
*     - The class of Region used to represent the CoordinateArea for each
*     STC-S sub-phrase is determined by the first word in the
*     sub-phrase (the "sub-phrase identifier"). The individual sub-phrase
*     Regions are combined into a single Prism, which is then simplified
c     using astSimplify
f     using AST_SIMPLIFY
*     to form the returned region.
*     - Sub-phrases that represent a single value ( that is, have
*     identifiers "Time", "Position", "Spectral" or "Redshift" ) are
*     considered to be be part of the STC CoordinateArea component.
*     - The TimeFrame used to represent a time STC-S sub-phrase will have
*     its TimeOrigin attribute set to the sub-phrase start time. If no
*     start time is specified by the sub-phrase, then the stop time will be
*     used instead. If no stop time is specified by the sub-phrase, then 
*     the single time value specified in the sub-phrase will be used
*     instead. Subsequently clearing the TimeOrigin attribute (or setting
*     its value to zero) will cause the TimeFrame to reprsent absolute times.
*     - The Epoch attribute for the returned Region is set in the same
*     way as the TimeOrigin attribute (see above).

*  Applicability:
*     StcsChan
*        All StcsChans have this attribute.
*att--
*/

/* This ia a boolean value (0 or 1) with a value of -INT_MAX when
   undefined but yielding a default of 1. */
astMAKE_CLEAR(StcsChan,StcsArea,stcsarea,-INT_MAX)
astMAKE_GET(StcsChan,StcsArea,int,1,( this->stcsarea != -INT_MAX ? this->stcsarea : 1 ))
astMAKE_SET(StcsChan,StcsArea,int,stcsarea,( value != 0 ))
astMAKE_TEST(StcsChan,StcsArea,( this->stcsarea != -INT_MAX ))

/*
*att++
*  Name:
*     StcsCoords

*  Purpose:
*     Return the Coordinates component when reading an STC-S document?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean).

*  Description:
*     This is a boolean attribute which controls what is returned
*     by the
c     astRead
f     AST_READ
*     function when it is used to read from an StcsChan.
*     If StcsCoords is set non-zero, then a PointList
*     representing the STC Coordinates will be returned by 
c     astRead.
f     AST_READ.
*     If StcsCoords is set to zero (the default), then the STC 
*     Coordinates will not be returned.

*  Notes:
*     - Other attributes such as StcsArea and StcsProps can be used to
*     specify other Objects to be returned by 
c     astRead.
f     AST_READ.
*     If more than one of these attributes is set non-zero, then the
*     actual Object returned by 
c     astRead
f     AST_READ
*     will be a KeyMap, containing the requested Objects. In this
*     case, the PointList representing the STC Coordinates will be
*     stored in the returned KeyMap using the key "COORDS". If StcsCoords
*     is the only attribute to be set non-zero, then the Object returned by
c     astRead
f     AST_READ
*     will be the Coordinates PointList itself.
*     - The Coordinates component is specified by the additional axis
*     values embedded within the body of each STC-S sub-phrase that
*     represents an extended area. Sub-phrases that represent a single 
*     value ( that is, have identifiers "Time", "Position", "Spectral" 
*     or "Redshift" ) are not considered to be be part of the STC
*     Coordinates component.
*     - If the STC-S documents does not contain a Coordinates component,
*     then a NULL object pointer
f     (AST__NULL)
*     will be returned by 
c     astRead
f     AST_READ
*     if the Coordinates component is the only object being returned. If
*     other objects are also being returned (see attributes StcsProps and 
*     StcsArea), then the returned KeyMap will contain a "COORDS" key
*     only if the Coordinates component is read succesfully.
*     - The TimeFrame used to represent a time STC-S sub-phrase will have
*     its TimeOrigin attribute set to the sub-phrase start time. If no
*     start time is specified by the sub-phrase, then the stop time will be
*     used instead. If no stop time is specified by the sub-phrase, then 
*     the single time value specified in the sub-phrase will be used
*     instead. Subsequently clearing the TimeOrigin attribute (or setting
*     its value to zero) will cause the TimeFrame to reprsent absolute times.
*     - The Epoch attribute for the returned Region is set in the same
*     way as the TimeOrigin attribute (see above).

*  Applicability:
*     StcsChan
*        All StcsChans have this attribute.
*att--
*/

/* This ia a boolean value (0 or 1) with a value of -INT_MAX when
   undefined but yielding a default of zero. */
astMAKE_CLEAR(StcsChan,StcsCoords,stcscoords,-INT_MAX)
astMAKE_GET(StcsChan,StcsCoords,int,0,( this->stcscoords != -INT_MAX ? this->stcscoords : 0 ))
astMAKE_SET(StcsChan,StcsCoords,int,stcscoords,( value != 0 ))
astMAKE_TEST(StcsChan,StcsCoords,( this->stcscoords != -INT_MAX ))

/*
*att++
*  Name:
*     StcsProps

*  Purpose:
*     Return all properties when reading an STC-S document?

*  Type:
*     Public attribute.

*  Synopsis:
*     Integer (boolean).

*  Description:
*     This is a boolean attribute which controls what is returned
*     by the
c     astRead
f     AST_READ
*     function when it is used to read from an StcsChan.
*     If StcsProps is set non-zero, then a KeyMap containing all the
*     properties read from the STC-S document will be returned by 
c     astRead.
f     AST_READ.
*     If StcsProps is set to zero (the default), then the properties
*     will not be returned.

*  Notes:
*     - Other attributes such as StcsCoords and StcsArea can be used to
*     specify other Objects to be returned by 
c     astRead.
f     AST_READ.
*     If more than one of these attributes is set non-zero, then the
*     actual Object returned by 
c     astRead
f     AST_READ
*     will be a KeyMap containing the requested Objects. In this
*     case, the properties KeyMap will be stored in the returned KeyMap 
*     using the key "PROPS". If StcsProps is the only attribute to be 
*     set non-zero, then the Object returned by 
c     astRead
f     AST_READ
*     will be the properties KeyMap itself.
*     - The KeyMap containing the properties will have entries for one or
*     more of the following keys: "TIME_PROPS", "SPACE_PROPS", "SPECTRAL_PROPS" 
*     and "REDSHIFT_PROPS". Each of these entries will be another KeyMap
*     containing the properties of the corresponding STC-S sub-phrase.

*  Applicability:
*     StcsChan
*        All StcsChans have this attribute.
*att--
*/

/* This ia a boolean value (0 or 1) with a value of -INT_MAX when
   undefined but yielding a default of zero. */
astMAKE_CLEAR(StcsChan,StcsProps,stcsprops,-INT_MAX)
astMAKE_GET(StcsChan,StcsProps,int,0,( this->stcsprops != -INT_MAX ? this->stcsprops : 0 ))
astMAKE_SET(StcsChan,StcsProps,int,stcsprops,( value != 0 ))
astMAKE_TEST(StcsChan,StcsProps,( this->stcsprops != -INT_MAX ))

/* Copy constructor. */
/* ----------------- */

/* Destructor. */
/* ----------- */

/* Dump function. */
/* -------------- */

static void Dump( AstObject *this_object, AstChannel *channel, int *status ) {
/*
*  Name:
*     Dump

*  Purpose:
*     Dump function for StcsChan objects.

*  Type:
*     Private function.

*  Synopsis:
*     void Dump( AstObject *this, AstChannel *channel, int *status )

*  Description:
*     This function implements the Dump function which writes out data
*     for the StcsChan class to an output Channel.

*  Parameters:
*     this
*        Pointer to the Object (an StcsChan) whose data are being written.
*     channel
*        Pointer to the Channel to which the data are being written.
*     status
*        Pointer to the inherited status variable.
*/

/* Local Variables: */
   AstStcsChan *this;            /* Pointer to the StcsChan structure */
   int ival;                     /* Integer value */
   int set;                      /* Attribute value set? */

/* Check the global error status. */
   if ( !astOK ) return;

/* Obtain a pointer to the StcsChan structure. */
   this = (AstStcsChan *) this_object;

/* Write out values representing the instance variables for the
   StcsChan class.  Accompany these with appropriate comment strings,
   possibly depending on the values being written.*/

/* In the case of attributes, we first use the appropriate (private)
   Test...  member function to see if they are set. If so, we then use
   the (private) Get... function to obtain the value to be written
   out.

   For attributes which are not set, we use the astGet... method to
   obtain the value instead. This will supply a default value
   (possibly provided by a derived class which over-rides this method)
   which is more useful to a human reader as it corresponds to the
   actual default attribute value.  Since "set" will be zero, these
   values are for information only and will not be read back. */

/* StcsArea. */
/* --------- */
   set = TestStcsArea( this, status );
   ival = set ? GetStcsArea( this, status ) : astGetStcsArea( this );
   astWriteInt( channel, "StcsArea", set, 0, ival,
                ival ? "Read the STC CoordinatesArea component" :
                       "Do not read the STC CoordinatesArea component" );

/* StcsCoords. */
/* ----------- */
   set = TestStcsCoords( this, status );
   ival = set ? GetStcsCoords( this, status ) : astGetStcsCoords( this );
   astWriteInt( channel, "StcsCoords", set, 0, ival,
                ival ? "Read the STC Coordinates component" :
                       "Do not read the STC Coordinates component" );

/* StcsProps. */
/* ---------- */
   set = TestStcsProps( this, status );
   ival = set ? GetStcsProps( this, status ) : astGetStcsProps( this );
   astWriteInt( channel, "StcsProps", set, 0, ival,
                ival ? "Read the STC-S properties" :
                       "Do not read the STC-S properties" );

}

/* Standard class functions. */
/* ========================= */
/* Implement the astIsAStcsChan and astCheckStcsChan functions using the macros
   defined for this purpose in the "object.h" header file. */
astMAKE_ISA(StcsChan,Channel,check,&class_check)
astMAKE_CHECK(StcsChan)

AstStcsChan *astStcsChan_( const char *(* source)( void ),
                           void (* sink)( const char * ),
                           const char *options, int *status, ...) {
/*
*++
*  Name:
c     astStcsChan
f     AST_STCSCHAN

*  Purpose:
*     Create an StcsChan.

*  Type:
*     Public function.

*  Synopsis:
c     #include "stcschan.h"
c     AstStcsChan *astStcsChan( const char *(* source)( void ),
c                               void (* sink)( const char * ),
c                               const char *options, ... )
f     RESULT = AST_STCSCHAN( SOURCE, SINK, OPTIONS, STATUS )

*  Class Membership:
*     StcsChan constructor.

*  Description:
*     This function creates a new StcsChan and optionally initialises
*     its attributes.
*
*     A StcsChan is a specialised form of Channel which supports STC-S
*     I/O operations. Writing an Object to an StcsChan (using
c     astWrite) will, if the Object is suitable, generate an
f     AST_WRITE) will, if the Object is suitable, generate an
*     STC-S description of that Object, and reading from an StcsChan will 
*     create a new Object from its STC-S description.
*
*     Normally, when you use an StcsChan, you should provide "source"
c     and "sink" functions which connect it to an external data store
c     by reading and writing the resulting text. These functions
f     and "sink" routines which connect it to an external data store
f     by reading and writing the resulting text. These routines
*     should perform any conversions needed between external character 
c     encodings and the internal ASCII encoding. If no such functions 
f     encodings and the internal ASCII encoding. If no such routines
*     are supplied, a Channel will read from standard input and write 
*     to standard output.

*  Parameters:
c     source
f     SOURCE = SUBROUTINE (Given)
c        Pointer to a source function that takes no arguments and
c        returns a pointer to a null-terminated string.  This function
c        will be used by the StcsChan to obtain lines of input text. On
c        each invocation, it should return a pointer to the next input
c        line read from some external data store, and a NULL pointer
c        when there are no more lines to read.
c
c        If "source" is NULL, the Channel will read from standard
c        input instead.
f        A source routine, which is a subroutine which takes a single
f        integer error status argument.  This routine will be used by
f        the StcsChan to obtain lines of input text. On each
f        invocation, it should read the next input line from some
f        external data store, and then return the resulting text to
f        the AST library by calling AST_PUTLINE. It should supply a
f        negative line length when there are no more lines to read.
f        If an error occurs, it should set its own error status
f        argument to an error value before returning.
f
f        If the null routine AST_NULL is suppied as the SOURCE value,
f        the Channel will read from standard input instead.
c     sink
f     SINK = SUBROUTINE (Given)
c        Pointer to a sink function that takes a pointer to a
c        null-terminated string as an argument and returns void.  This
c        function will be used by the StcsChan to deliver lines of
c        output text. On each invocation, it should deliver the
c        contents of the string supplied to some external data store.
c
c        If "sink" is NULL, the StcsChan will write to standard output
c        instead.
f        A sink routine, which is a subroutine which takes a single
f        integer error status argument.  This routine will be used by
f        the StcsChan to deliver lines of output text. On each
f        invocation, it should obtain the next output line from the
f        AST library by calling AST_GETLINE, and then deliver the
f        resulting text to some external data store.  If an error
f        occurs, it should set its own error status argument to an
f        error value before returning.
f
f        If the null routine AST_NULL is suppied as the SINK value,
f        the Channel will write to standard output instead.
c     options
f     OPTIONS = CHARACTER * ( * ) (Given)
c        Pointer to a null-terminated string containing an optional
c        comma-separated list of attribute assignments to be used for
c        initialising the new StcsChan. The syntax used is identical to
c        that for the astSet function and may include "printf" format
c        specifiers identified by "%" symbols in the normal way.
f        A character string containing an optional comma-separated
f        list of attribute assignments to be used for initialising the
f        new StcsChan. The syntax used is identical to that for the
f        AST_SET routine.
c     ...
c        If the "options" string contains "%" format specifiers, then
c        an optional list of additional arguments may follow it in
c        order to supply values to be substituted for these
c        specifiers. The rules for supplying these are identical to
c        those for the astSet function (and for the C "printf"
c        function).
f     STATUS = INTEGER (Given and Returned)
f        The global status.

*  Returned Value:
c     astStcsChan()
f     AST_STCSCHAN = INTEGER
*        A pointer to the new StcsChan.

*  Notes:
f     - The names of the routines supplied for the SOURCE and SINK
f     arguments should appear in EXTERNAL statements in the Fortran
f     routine which invokes AST_STCSCHAN. However, this is not generally
f     necessary for the null routine AST_NULL (so long as the AST_PAR
f     include file has been used).
*     - If the external data source or sink uses a character encoding
*     other than ASCII, the supplied source and sink functions should
*     translate between the external character encoding and the internal 
*     ASCII encoding used by AST.
*     - A null Object pointer (AST__NULL) will be returned if this
*     function is invoked with the AST error status set, or if it
*     should fail for any reason.
f     - Note that the null routine AST_NULL (one underscore) is
f     different to AST__NULL (two underscores), which is the null Object
f     pointer.
*--
*/

/* Local Variables: */
   astDECLARE_GLOBALS;           /* Pointer to thread-specific global data */
   AstStcsChan *new;             /* Pointer to new StcsChan */
   va_list args;                 /* Variable argument list */

/* Get a pointer to the thread specific global data structure. */
   astGET_GLOBALS(NULL);

/* Check the global status. */
   if ( !astOK ) return NULL;

/* Initialise the StcsChan, allocating memory and initialising the
   virtual function table as well if necessary. This interface is for
   use by other C functions within AST, and uses the standard "wrapper"
   functions included in this class. */
   new = astInitStcsChan( NULL, sizeof( AstStcsChan ), !class_init, 
                          &class_vtab, "StcsChan", source, SourceWrap,
                          sink, SinkWrap );

/* If successful, note that the virtual function table has been
   initialised. */
   if ( astOK ) {
      class_init = 1;

/* Obtain the variable argument list and pass it along with the
   options string to the astVSet method to initialise the new
   StcsChan's attributes. */
      va_start( args, status );
      astVSet( new, options, NULL, args );
      va_end( args );

/* If an error occurred, clean up by deleting the new object. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return a pointer to the new StcsChan. */
   return new;
}

AstStcsChan *astStcsChanId_( const char *(* source)( void ),
                             void (* sink)( const char * ),
                             const char *options, ... ) {
/*
*  Name:
*     astStcsChanId_

*  Purpose:
*     Create an StcsChan.

*  Type:
*     Private function.

*  Synopsis:
*     #include "stcschan.h"
*     AstStcsChan *astStcsChanId_( const char *(* source)( void ),
*                                  void (* sink)( const char * ),
*                                  const char *options, ... )

*  Class Membership:
*     StcsChan constructor.

*  Description:
*     This function implements the external (public) C interface to the
*     astStcsChan constructor function. Another function (astStcsChanForId)
*     should be called to create an StcsChan for use within other languages.
*     Both functions return an ID value (instead of a true C pointer) to 
*     external users, and must be provided because astStcsChan_ has a variable 
*     argument list which cannot be encapsulated in a macro (where this conversion would otherwise
*     occur).
*
*     The variable argument list also prevents this function from
*     invoking astStcsChan_ directly, so it must be a re-implementation
*     of it in all respects, except for the final conversion of the
*     result to an ID value.

*  Parameters:
*     As for astStcsChan_.

*  Returned Value:
*     The ID value associated with the new StcsChan.
*/

/* Local Variables: */
   astDECLARE_GLOBALS;           /* Pointer to thread-specific global data */
   AstStcsChan *new;             /* Pointer to new StcsChan */
   va_list args;                 /* Variable argument list */

   int *status;                  /* Pointer to inherited status value */

/* Get a pointer to the inherited status value. */
   status = astGetStatusPtr;

/* Get a pointer to the thread specific global data structure. */
   astGET_GLOBALS(NULL);

/* Check the global status. */
   if ( !astOK ) return NULL;

/* Initialise the StcsChan, allocating memory and initialising the
   virtual function table as well if necessary. This interface is for
   use by external C functions and uses the standard "wrapper"
   functions included in this class. */
   new = astInitStcsChan( NULL, sizeof( AstStcsChan ), !class_init, 
                          &class_vtab, "StcsChan", source, SourceWrap, 
                          sink, SinkWrap );

/* If successful, note that the virtual function table has been
   initialised. */
   if ( astOK ) {
      class_init = 1;

/* Obtain the variable argument list and pass it along with the
   options string to the astVSet method to initialise the new
   StcsChan's attributes. */
      va_start( args, options );
      astVSet( new, options, NULL, args );
      va_end( args );

/* If an error occurred, clean up by deleting the new object. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return an ID value for the new StcsChan. */
   return astMakeId( new );
}

AstStcsChan *astStcsChanForId_( const char *(* source)( void ),
                                char *(* source_wrap)( const char *(*)( void ), int * ),
                                void (* sink)( const char * ),
                                void (* sink_wrap)( void (*)( const char * ),
                                                    const char *, int * ),
                                const char *options, ... ) {
/*
*+
*  Name:
*     astStcsChanFor

*  Purpose:
*     Initialise an StcsChan from a foreign language interface.

*  Type:
*     Public function.

*  Synopsis:
*     #include "stcschan.h"
*     AstStcsChan *astStcsChanFor( const char *(* source)( void ),
*                                  char *(* source_wrap)( const char *(*)
*                                                         ( void ), int * ),
*                                  void (* sink)( const char * ),
*                                  void (* sink_wrap)( void (*)( const char * ),
*                                                      const char *, int * ),
*                                  const char *options, ... )

*  Class Membership:
*     StcsChan constructor.

*  Description:
*     This function creates a new StcsChan from a foreign language
*     interface and optionally initialises its attributes.
*
*     A StcsChan is a specialised form of Channel which supports STC-S
*     I/O operations. Writing an Object to an StcsChan (using
c     astWrite) will, if the Object is suitable, generate an
f     AST_WRITE) will, if the Object is suitable, generate an
*     STC-S description of that Object, and reading from an StcsChan will 
*     create a new Object from its STC-S description.
*
*     Normally, when you use an StcsChan, you should provide "source"
c     and "sink" functions which connect it to an external data store
c     by reading and writing the resulting text. These functions
f     and "sink" routines which connect it to an external data store
f     by reading and writing the resulting text. These routines
*     should perform any conversions needed between external character 
c     encodings and the internal ASCII encoding. If no such functions 
f     encodings and the internal ASCII encoding. If no such routines
*     are supplied, a Channel will read from standard input and write 
*     to standard output.

*  Parameters:
*     source
*        Pointer to a "source" function which will be used to obtain
*        lines of input text. Generally, this will be obtained by
*        casting a pointer to a source function which is compatible
*        with the "source_wrap" wrapper function (below). The pointer
*        should later be cast back to its original type by the
*        "source_wrap" function before the function is invoked.
*
*        If "source" is NULL, the StcsChan will read from standard
*        input instead.
*     source_wrap
*        Pointer to a function which can be used to invoke the
*        "source" function supplied (above). This wrapper function is
*        necessary in order to hide variations in the nature of the
*        source function, such as may arise when it is supplied by a
*        foreign (non-C) language interface.
*
*        The single parameter of the "source_wrap" function is a
*        pointer to the "source" function, and it should cast this
*        function pointer (as necessary) and invoke the function with
*        appropriate arguments to obtain the next line of input
*        text. The "source_wrap" function should then return a pointer
*        to a dynamically allocated, null terminated string containing
*        the text that was read. The string will be freed (using
*        astFree) when no longer required and the "source_wrap"
*        function need not concern itself with this. A NULL pointer
*        should be returned if there is no more input to read.
*
*        If "source_wrap" is NULL, the StcsChan will read from standard
*        input instead.
*     sink
*        Pointer to a "sink" function which will be used to deliver
*        lines of output text. Generally, this will be obtained by
*        casting a pointer to a sink function which is compatible with
*        the "sink_wrap" wrapper function (below). The pointer should
*        later be cast back to its original type by the "sink_wrap"
*        function before the function is invoked.
*
*        If "sink" is NULL, the StcsChan will write to standard output
*        instead.
*     sink_wrap
*        Pointer to a function which can be used to invoke the "sink"
*        function supplied (above). This wrapper function is necessary
*        in order to hide variations in the nature of the sink
*        function, such as may arise when it is supplied by a foreign
*        (non-C) language interface.
*
*        The first parameter of the "sink_wrap" function is a pointer
*        to the "sink" function, and the second parameter is a pointer
*        to a const, null-terminated character string containing the
*        text to be written.  The "sink_wrap" function should cast the
*        "sink" function pointer (as necessary) and invoke the
*        function with appropriate arguments to deliver the line of
*        output text. The "sink_wrap" function then returns void.
*
*        If "sink_wrap" is NULL, the Channel will write to standard
*        output instead.
*     options
*        Pointer to a null-terminated string containing an optional
*        comma-separated list of attribute assignments to be used for
*        initialising the new StcsChan. The syntax used is identical to
*        that for the astSet function and may include "printf" format
*        specifiers identified by "%" symbols in the normal way.
*     ...
*        If the "options" string contains "%" format specifiers, then
*        an optional list of additional arguments may follow it in
*        order to supply values to be substituted for these
*        specifiers. The rules for supplying these are identical to
*        those for the astSet function (and for the C "printf"
*        function).

*  Returned Value:
*     astStcsChanFor()
*        A pointer to the new StcsChan.

*  Notes:
*     - A null Object pointer (AST__NULL) will be returned if this
*     function is invoked with the global error status set, or if it
*     should fail for any reason.
*     - This function is only available through the public interface
*     to the StcsChan class (not the protected interface) and is
*     intended solely for use in implementing foreign language
*     interfaces to this class.
*-

*  Implememtation Notes:
*     - This function behaves exactly like astStcsChanId_, in that it
*     returns ID values and not true C pointers, but it has two
*     additional arguments. These are pointers to the "wrapper
*     functions" which are needed to accommodate foreign language
*     interfaces.
*/

/* Local Variables: */
   astDECLARE_GLOBALS;           /* Pointer to thread-specific global data */
   AstStcsChan *new;              /* Pointer to new StcsChan */
   va_list args;                 /* Variable argument list */
   int *status;                  /* Pointer to inherited status value */

/* Get a pointer to the inherited status value. */
   status = astGetStatusPtr;

/* Check the global status. */
   if ( !astOK ) return NULL;

/* Get a pointer to the thread specific global data structure. */
   astGET_GLOBALS(NULL);

/* Initialise the StcsChan, allocating memory and initialising the
   virtual function table as well if necessary. */
   new = astInitStcsChan( NULL, sizeof( AstStcsChan ), !class_init, 
                          &class_vtab, "StcsChan", source, source_wrap,
                          sink, sink_wrap );

/* If successful, note that the virtual function table has been
   initialised. */
   if ( astOK ) {
      class_init = 1;

/* Obtain the variable argument list and pass it along with the
   options string to the astVSet method to initialise the new
   StcsChan's attributes. */
      va_start( args, options );
      astVSet( new, options, NULL, args );
      va_end( args );

/* If an error occurred, clean up by deleting the new object. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return an ID value for the new StcsChan. */
   return astMakeId( new );
}

AstStcsChan *astInitStcsChan_( void *mem, size_t size, int init,
                               AstStcsChanVtab *vtab, const char *name,
                               const char *(* source)( void ),
                               char *(* source_wrap)( const char *(*)( void ), int * ),
                               void (* sink)( const char * ),
                               void (* sink_wrap)( void (*)( const char * ),
                                                   const char *, int * ), int *status ) {
/*
*+
*  Name:
*     astInitStcsChan

*  Purpose:
*     Initialise an StcsChan.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "stcschan.h"
*     AstStcsChan *astInitStcsChan( void *mem, size_t size, int init,
*                                   AstStcsChanVtab *vtab, const char *name,
*                                   const char *(* source)( void ),
*                                   char *(* source_wrap)( const char *(*)( void ), int * ),
*                                   void (* sink)( const char * ),
*                                   void (* sink_wrap)( void (*)( const char * ),
*                                                     const char *, int * ) )

*  Class Membership:
*     StcsChan initialiser.

*  Description:
*     This function is provided for use by class implementations to
*     initialise a new StcsChan object. It allocates memory (if
*     necessary) to accommodate the StcsChan plus any additional data
*     associated with the derived class.  It then initialises a
*     StcsChan structure at the start of this memory. If the "init"
*     flag is set, it also initialises the contents of a virtual
*     function table for an StcsChan at the start of the memory passed
*     via the "vtab" parameter.

*  Parameters:
*     mem
*        A pointer to the memory in which the StcsChan is to be
*        initialised.  This must be of sufficient size to accommodate
*        the StcsChan data (sizeof(StcsChan)) plus any data used by the
*        derived class. If a value of NULL is given, this function
*        will allocate the memory itself using the "size" parameter to
*        determine its size.
*     size
*        The amount of memory used by the StcsChan (plus derived class
*        data).  This will be used to allocate memory if a value of
*        NULL is given for the "mem" parameter. This value is also
*        stored in the StcsChan structure, so a valid value must be
*        supplied even if not required for allocating memory.
*     init
*        A boolean flag indicating if the StcsChan's virtual function
*        table is to be initialised. If this value is non-zero, the
*        virtual function table will be initialised by this function.
*     vtab
*        Pointer to the start of the virtual function table to be
*        associated with the new StcsChan.
*     name
*        Pointer to a constant null-terminated character string which
*        contains the name of the class to which the new object
*        belongs (it is this pointer value that will subsequently be
*        returned by the astGetClass method).
*     source
*        Pointer to a "source" function which will be used to obtain
*        lines of text. Generally, this will be obtained by
*        casting a pointer to a source function which is compatible
*        with the "source_wrap" wrapper function (below). The pointer
*        should later be cast back to its original type by the
*        "source_wrap" function before the function is invoked.
*
*        If "source" is NULL, the Channel will read from standard
*        input instead.
*     source_wrap
*        Pointer to a function which can be used to invoke the
*        "source" function supplied (above). This wrapper function is
*        necessary in order to hide variations in the nature of the
*        source function, such as may arise when it is supplied by a
*        foreign (non-C) language interface.
*
*        The single parameter of the "source_wrap" function is a
*        pointer to the "source" function, and it should cast this
*        function pointer (as necessary) and invoke the function with
*        appropriate arguments to obtain the next line of input
*        text. The "source_wrap" function should then return a pointer
*        to a dynamically allocated, null terminated string containing
*        the text that was read. The string will be freed (using
*        astFree) when no longer required and the "source_wrap"
*        function need not concern itself with this. A NULL pointer
*        should be returned if there is no more input to read.
*
*        If "source_wrap" is NULL, the Channel will read from standard
*        input instead.
*     sink
*        Pointer to a "sink" function which will be used to deliver
*        lines of text. Generally, this will be obtained by
*        casting a pointer to a sink function which is compatible with
*        the "sink_wrap" wrapper function (below). The pointer should
*        later be cast back to its original type by the "sink_wrap"
*        function before the function is invoked.
*
*        If "sink" is NULL, the contents of the StcsChan will not be
*        written out before being deleted.
*     sink_wrap
*        Pointer to a function which can be used to invoke the "sink"
*        function supplied (above). This wrapper function is necessary
*        in order to hide variations in the nature of the sink
*        function, such as may arise when it is supplied by a foreign
*        (non-C) language interface.
*
*        The first parameter of the "sink_wrap" function is a pointer
*        to the "sink" function, and the second parameter is a pointer
*        to a const, null-terminated character string containing the
*        text to be written.  The "sink_wrap" function should cast the
*        "sink" function pointer (as necessary) and invoke the
*        function with appropriate arguments to deliver the line of
*        output text. The "sink_wrap" function then returns void.
*
*        If "sink_wrap" is NULL, the Channel will write to standard
*        output instead.

*  Returned Value:
*     A pointer to the new StcsChan.

*  Notes:
*     - A null pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   AstStcsChan *new;              /* Pointer to new StcsChan */

/* Check the global status. */
   if ( !astOK ) return NULL;

/* If necessary, initialise the virtual function table. */
   if ( init ) astInitStcsChanVtab( vtab, name );

/* Initialise a Channel structure (the parent class) as the first
   component within the StcsChan structure, allocating memory if
   necessary. */
   new = (AstStcsChan *) astInitChannel( mem, size, 0,
                                         (AstChannelVtab *) vtab, name,
                                         source, source_wrap, sink,
                                         sink_wrap );

   if ( astOK ) {

/* Initialise the StcsChan data. */
/* ---------------------------- */
      new->stcsarea = -INT_MAX;
      new->stcscoords = -INT_MAX;
      new->stcsprops = -INT_MAX;

/* If an error occurred, clean up by deleting the new object. */
      if ( !astOK ) new = astDelete( new );
   }

/* Return a pointer to the new object. */
   return new;
}

AstStcsChan *astLoadStcsChan_( void *mem, size_t size,
                               AstStcsChanVtab *vtab, const char *name,
                               AstChannel *channel, int *status ) {
/*
*+
*  Name:
*     astLoadStcsChan

*  Purpose:
*     Load an StcsChan.

*  Type:
*     Protected function.

*  Synopsis:
*     #include "stcschan.h"
*     AstStcsChan *astLoadStcsChan( void *mem, size_t size,
*                                   AstStcsChanVtab *vtab, const char *name,
*                                   AstChannel *channel )

*  Class Membership:
*     StcsChan loader.

*  Description:
*     This function is provided to load a new StcsChan using data read
*     from a Channel. It first loads the data used by the parent class
*     (which allocates memory if necessary) and then initialises a
*     StcsChan structure in this memory, using data read from the input
*     Channel.
*
*     If the "init" flag is set, it also initialises the contents of a
*     virtual function table for an StcsChan at the start of the memory
*     passed via the "vtab" parameter.

*  Parameters:
*     mem
*        A pointer to the memory into which the StcsChan is to be
*        loaded.  This must be of sufficient size to accommodate the
*        StcsChan data (sizeof(StcsChan)) plus any data used by derived
*        classes. If a value of NULL is given, this function will
*        allocate the memory itself using the "size" parameter to
*        determine its size.
*     size
*        The amount of memory used by the StcsChan (plus derived class
*        data).  This will be used to allocate memory if a value of
*        NULL is given for the "mem" parameter. This value is also
*        stored in the StcsChan structure, so a valid value must be
*        supplied even if not required for allocating memory.
*
*        If the "vtab" parameter is NULL, the "size" value is ignored
*        and sizeof(AstStcsChan) is used instead.
*     vtab
*        Pointer to the start of the virtual function table to be
*        associated with the new StcsChan. If this is NULL, a pointer
*        to the (static) virtual function table for the StcsChan class
*        is used instead.
*     name
*        Pointer to a constant null-terminated character string which
*        contains the name of the class to which the new object
*        belongs (it is this pointer value that will subsequently be
*        returned by the astGetClass method).
*
*        If the "vtab" parameter is NULL, the "name" value is ignored
*        and a pointer to the string "StcsChan" is used instead.

*  Returned Value:
*     A pointer to the new StcsChan.

*  Notes:
*     - A null pointer will be returned if this function is invoked
*     with the global error status set, or if it should fail for any
*     reason.
*-
*/

/* Local Variables: */
   astDECLARE_GLOBALS;          /* Pointer to thread-specific global data */
   AstStcsChan *new;            /* Pointer to the new StcsChan */

/* Initialise. */
   new = NULL;

/* Check the global error status. */
   if( !astOK ) return new;

/* Get a pointer to the thread specific global data structure. */
   astGET_GLOBALS(channel);

/* If a NULL virtual function table has been supplied, then this is
   the first loader to be invoked for this StcsChan. In this case the
   StcsChan belongs to this class, so supply appropriate values to be
   passed to the parent class loader (and its parent, etc.). */
   if ( !vtab ) {
      size = sizeof( AstStcsChan );
      vtab = &class_vtab;
      name = "StcsChan";

/* If required, initialise the virtual function table for this class. */
      if ( !class_init ) {
         astInitStcsChanVtab( vtab, name );
         class_init = 1;
      }
   }

/* Invoke the parent class loader to load data for all the ancestral
   classes of the current one, returning a pointer to the resulting
   partly-built StcsChan. */
   new = astLoadChannel( mem, size, (AstChannelVtab *) vtab, name,
                         channel );

   if ( astOK ) {

/* Read input data. */
/* ================ */
/* Request the input Channel to read all the input data appropriate to
   this class into the internal "values list". */
      astReadClassData( channel, "StcsChan" );

/* Now read each individual data item from this list and use it to
   initialise the appropriate instance variable(s) for this class. */

/* In the case of attributes, we first read the "raw" input value,
   supplying the "unset" value as the default. If a "set" value is
   obtained, we then use the appropriate (private) Set... member
   function to validate and set the value properly. */

/* StcsArea. */
/* --------- */
      new->stcsarea = astReadInt( channel, "stcsarea", -INT_MAX );
      if ( TestStcsArea( new, status ) ) SetStcsArea( new, new->stcsarea, status );

/* StcsCoords. */
/* ----------- */
      new->stcscoords = astReadInt( channel, "stcscoords", -INT_MAX );
      if ( TestStcsCoords( new, status ) ) SetStcsCoords( new, new->stcscoords, status );

/* StcsProps. */
/* ---------- */
      new->stcsprops = astReadInt( channel, "stcsprops", -INT_MAX );
      if ( TestStcsProps( new, status ) ) SetStcsProps( new, new->stcsprops, status );

   }

/* If an error occurred, clean up by deleting the new StcsChan. */
   if ( !astOK ) new = astDelete( new );

/* Return the new StcsChan pointer. */
   return new;
}

/* Virtual function interfaces. */
/* ============================ */
/* These provide the external interface to the virtual functions defined by
   this class. Each simply checks the global error status and then locates and
   executes the appropriate member function, using the function pointer stored
   in the object's virtual function table (this pointer is located using the
   astMEMBER macro defined in "object.h").

   Note that the member function may not be the one defined here, as it may
   have been over-ridden by a derived class. However, it should still have the
   same interface. */


















