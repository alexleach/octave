/*

Copyright (C) 2004 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "str-vec.h"
#include "quit.h"

#include "defun.h"
#include "error.h"
#include "ov.h"
#include "ov-uint64.h"
#include "ov-uint32.h"
#include "ov-uint16.h"
#include "ov-uint8.h"
#include "ov-int64.h"
#include "ov-int32.h"
#include "ov-int16.h"
#include "ov-int8.h"
#include "ov-scalar.h"
#include "ov-re-mat.h"

// XXX FIXME XXX -- could probably eliminate some code duplication by
// clever use of templates.

#define BITOPX(OP, FNAME, RET) \
      { \
	int nelx = x.numel (); \
	int nely = y.numel (); \
 \
	bool is_scalar_op = (nelx == 1 || nely == 1); \
 \
	dim_vector dvx = x.dims (); \
	dim_vector dvy = y.dims (); \
 \
	bool is_array_op = (dvx == dvy); \
 \
	if (is_array_op || is_scalar_op) \
	  { \
	    RET result; \
 \
	    if (nelx != 1) \
	      result.resize (dvx); \
	    else \
	      result.resize (dvy); \
 \
	    for (int i = 0; i < nelx; i++) \
	      if (is_scalar_op) \
		for (int k = 0; k < nely; k++) \
		  result(i+k) = x(i) OP y(k); \
	      else \
		result(i) = x(i) OP y(i); \
 \
	      retval = result; \
	  } \
	else \
	  error ("%s: size of x and y must match, or one operand must be a scalar", FNAME); \
      }

#define BITOP(OP, FNAME) \
 \
  octave_value retval; \
 \
  int nargin = args.length (); \
 \
  if (nargin == 2) \
    { \
      if (args(0).type_id () == octave_matrix::static_type_id () || \
	  args(0).type_id () == octave_scalar::static_type_id () || \
	  args(1).type_id () == octave_matrix::static_type_id () || \
	  args(1).type_id () == octave_scalar::static_type_id ()) \
	{ \
	  bool arg0_is_int = true; \
	  bool arg1_is_int = true; \
 \
	  if (args(0).type_id () == octave_matrix::static_type_id () || \
	      args(0).type_id () == octave_scalar::static_type_id ())	\
	    arg0_is_int = false; \
 \
	  if (args(1).type_id () == octave_matrix::static_type_id () || \
	      args(1).type_id () == octave_scalar::static_type_id ())	\
	    arg1_is_int = false; \
 \
	  if (!arg0_is_int && !arg1_is_int) \
	    { \
	      uint64NDArray x (args(0).array_value ());		\
	      uint64NDArray y (args(1).array_value ());	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, uint64NDArray);		\
	      retval = retval.array_value ();			\
	    } \
	  else \
	    { \
	      int p = (arg0_is_int ? 1 : 0); \
	      int q = (arg0_is_int ? 0 : 1); \
	      NDArray dx = args(p).array_value (); \
 \
	      if (args(q).type_id () == octave_uint64_matrix::static_type_id () || \
		  args(q).type_id () == octave_uint64_scalar::static_type_id ()) \
		{							\
		  uint64NDArray x (dx);					\
		  uint64NDArray y = args(q).uint64_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, uint64NDArray);			\
		 } \
	      else if (args(q).type_id () == octave_uint32_matrix::static_type_id () || \
		       args(q).type_id () == octave_uint32_scalar::static_type_id ()) \
		{							\
		  uint32NDArray x (dx);					\
		  uint32NDArray y = args(q).uint32_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, uint32NDArray);			\
		} \
	      else if (args(q).type_id () == octave_uint16_matrix::static_type_id () || \
		       args(q).type_id () == octave_uint16_scalar::static_type_id ()) \
		{							\
		  uint16NDArray x (dx);					\
		  uint16NDArray y = args(q).uint16_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, uint16NDArray);			\
		} \
	      else if (args(q).type_id () == octave_uint8_matrix::static_type_id () || \
		       args(q).type_id () == octave_uint8_scalar::static_type_id ()) \
		{							\
		  uint8NDArray x (dx);					\
		  uint8NDArray y = args(q).uint8_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, uint8NDArray);			\
		}							\
	      else if (args(q).type_id () == octave_int64_matrix::static_type_id () || \
		       args(q).type_id () == octave_int64_scalar::static_type_id ()) \
		{							\
		  int64NDArray x (dx);					\
		  int64NDArray y = args(q).int64_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, int64NDArray);			\
		}							\
	      else if (args(q).type_id () == octave_int32_matrix::static_type_id () || \
		       args(q).type_id () == octave_int32_scalar::static_type_id ()) \
		{							\
		  int32NDArray x (dx);					\
		  int32NDArray y = args(q).int32_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, int32NDArray);			\
		}							\
	      else if (args(q).type_id () == octave_int16_matrix::static_type_id () || \
	      args(q).type_id () == octave_int16_scalar::static_type_id ()) \
		{							\
		  int16NDArray x (dx);					\
		  int16NDArray y = args(q).int16_array_value ();	\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, int16NDArray);			\
		}							\
	      else if (args(q).type_id () == octave_int8_matrix::static_type_id () || \
		       args(q).type_id () == octave_int8_scalar::static_type_id ()) \
		{							\
		  int8NDArray x (dx);					\
		  int8NDArray y = args(q).int8_array_value ();		\
		  if (! error_state)					\
		    BITOPX (OP, FNAME, int8NDArray);			\
		}							\
	      else							\
		error ("%s: illegal operand type", FNAME);		\
	    }								\
	}								\
      else if (args(0).type_id () == args(1).type_id ())		\
	{ \
	  if (args(0).type_id () == octave_uint64_matrix::static_type_id () || \
	      args(0).type_id () == octave_uint64_scalar::static_type_id ()) \
	    { \
	      uint64NDArray x = args(0).uint64_array_value ();	\
	      uint64NDArray y = args(1).uint64_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, uint64NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_uint32_matrix::static_type_id () || \
	      args(0).type_id () == octave_uint32_scalar::static_type_id ()) \
	    { \
	      uint32NDArray x = args(0).uint32_array_value ();	\
	      uint32NDArray y = args(1).uint32_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, uint32NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_uint16_matrix::static_type_id () || \
	      args(0).type_id () == octave_uint16_scalar::static_type_id ()) \
	    { \
	      uint16NDArray x = args(0).uint16_array_value ();	\
	      uint16NDArray y = args(1).uint16_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, uint16NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_uint8_matrix::static_type_id () || \
	      args(0).type_id () == octave_uint8_scalar::static_type_id ()) \
	    { \
	      uint8NDArray x = args(0).uint8_array_value ();	\
	      uint8NDArray y = args(1).uint8_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, uint8NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_int64_matrix::static_type_id () || \
	      args(0).type_id () == octave_int64_scalar::static_type_id ()) \
	    { \
	      int64NDArray x = args(0).int64_array_value ();	\
	      int64NDArray y = args(1).int64_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, int64NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_int32_matrix::static_type_id () || \
	      args(0).type_id () == octave_int32_scalar::static_type_id ()) \
	    { \
	      int32NDArray x = args(0).int32_array_value ();	\
	      int32NDArray y = args(1).int32_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, int32NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_int16_matrix::static_type_id () || \
	      args(0).type_id () == octave_int16_scalar::static_type_id ()) \
	    { \
	      int16NDArray x = args(0).int16_array_value ();	\
	      int16NDArray y = args(1).int16_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, int16NDArray);		\
	    } \
	  else if (args(0).type_id () == octave_int8_matrix::static_type_id () || \
	      args(0).type_id () == octave_int8_scalar::static_type_id ()) \
	    { \
	      int8NDArray x = args(0).int8_array_value ();	\
	      int8NDArray y = args(1).int8_array_value ();	\
	      if (! error_state)				\
		BITOPX (OP, FNAME, int8NDArray);		\
	    } \
	  else \
	    error ("%s: illegal operand type", FNAME); \
	} \
      else \
	error ("%s: must have matching operand types", FNAME); \
    } \
  else \
    print_usage (FNAME); \
 \
  return retval

DEFUN (bitand, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} bitand (@var{x}, @var{y})\n\
calculates the bitwise AND of nonnegative integers.\n\
@var{x}, @var{y} must be in range [0..bitmax]\n\
@end deftypefn\n\
@seealso{bitor, bitxor, bitset, bitget, bitcmp, bitshift, bitmax}")
{
  BITOP (&, "bitand");
}

DEFUN (bitor, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} bitor (@var{x}, @var{y})\n\
calculates the bitwise OR of nonnegative integers.\n\
@var{x}, @var{y} must be in range [0..bitmax]\n\
@end deftypefn\n\
@seealso{bitor, bitxor, bitset, bitget, bitcmp, bitshift, bitmax}")
{
  BITOP (|, "bitor");
}

DEFUN (bitxor, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} bitxor (@var{x}, @var{y})\n\
calculates the bitwise XOR of nonnegative integers.\n\
@var{x}, @var{y} must be in range [0..bitmax]\n\
@end deftypefn\n\
@seealso{bitand, bitor, bitset, bitget, bitcmp, bitshift, bitmax}")
{
  BITOP (^, "bitxor");
}

static EIGHT_BYTE_INT
bitshift (const double& a, int n)
{
  if (n > 0)
    return static_cast<EIGHT_BYTE_INT> (a) << n;
  else if (n < 0)
    return static_cast<EIGHT_BYTE_INT> (a) >> -n;
  else
    return static_cast<EIGHT_BYTE_INT> (a);
}

// Note that the bitshift operators are undefined if shifted by more bits than
// in the type. Therefore need to test for the size of the shift
#define DO_BITSHIFT(T) \
      if (! error_state) \
	{ \
          double d1, d2; \
 \
          if (n.all_integers (d1, d2)) \
            { \
	      int m_nel = m.numel (); \
	      int n_nel = n.numel (); \
 \
	      bool is_scalar_op = (m_nel == 1 || n_nel == 1); \
 \
	      dim_vector m_dv = m.dims (); \
	      dim_vector n_dv = n.dims (); \
 \
	      bool is_array_op = (m_dv == n_dv); \
 \
	      if (is_array_op || is_scalar_op) \
		{ \
		  T ## NDArray result; \
 \
		  if (m_nel != 1) \
		    result.resize (m_dv); \
		  else \
		    result.resize (n_dv); \
 \
		  for (int i = 0; i < m_nel; i++) \
		    if (is_scalar_op) \
		      for (int k = 0; k < n_nel; k++) \
			if (static_cast<int> (n(k)) >= bits_in_type) \
			  result(i+k) = 0; \
			else \
			  result(i+k) = bitshift (m(i), static_cast<int> (n(k))) & mask; \
		    else \
		      if (static_cast<int> (n(i)) >= bits_in_type) \
			result(i) = 0;					\
		      else 						\
			result(i) = bitshift (m(i), static_cast<int> (n(i))) & mask; \
 \
		  retval = result; \
		} \
	      else \
		error ("bitshift: size of A and N must match, or one operand must be a scalar"); \
	    } \
          else \
            error ("bitshift: expecting second argument to be integer"); \
        }

#define DO_UBITSHIFT(T, N)					\
  do								\
    {								\
      int bits_in_type = sizeof (octave_ ## T) << 3;		\
      T ## NDArray m = m_arg.T ## _array_value ();		\
	octave_ ## T mask = ~0ULL;				\
      if ((N) < static_cast<int>(sizeof (octave_ ## T) << 3))	\
	mask = mask >> ((sizeof (octave_ ## T) << 3) - (N));	\
      else if ((N) < 1)						\
	mask = 0;						\
      DO_BITSHIFT (T);						\
    }								\
  while (0)

#define DO_SBITSHIFT(T, N)					\
  do								\
    {								\
      int bits_in_type = sizeof (octave_ ## T) << 3;		\
      T ## NDArray m = m_arg.T ## _array_value ();		\
	octave_ ## T mask = -1;					\
      if ((N) < static_cast<int>(sizeof (octave_ ## T) << 3))	\
	mask = mask >> ((sizeof (octave_ ## T) << 3) - (N));	\
      else if ((N) < 1)						\
	mask = 0;						\
      DO_BITSHIFT (T);						\
    }								\
  while (0)

DEFUN (bitshift, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Function File} {} bitshift (@var{a}, @var{k})\n\
@deftypefnx {Function File} {} bitshift (@var{a}, @var{k}, @var{n})\n\
return a @var{k} bit shift of @var{n}- digit unsigned\n\
integers in @var{a}. A positive @var{k} leads to a left shift.\n\
A negative value to a right shift. If @var{n} is omitted it defaults\n\
to log2(bitmax)+1. \n\
@var{n} must be in range [1,log2(bitmax)+1] usually [1,33]\n\
\n\
@example\n\
bitshift (eye (3), 1))\n\
@result{}\n\
@group\n\
2 0 0\n\
0 2 0\n\
0 0 2\n\
@end group\n\
\n\
bitshift (10, [-2, -1, 0, 1, 2])\n\
@result{} 2   5  10  20  40\n\
\n\
bitshift ([1, 10], 2, [3,4])\n\
@result{} 4  8\n\
@end example\n\
@end deftypefn\n\
@seealso{bitand, bitor, bitxor, bitset, bitget, bitcmp, bitmax}")
{
  octave_value retval;

  int nargin = args.length ();

  if (nargin == 2 || nargin == 3)
    {
      NDArray n = args(1).array_value ();
      int nbits = 64;
      
      if (nargin == 3)
	{
	  nbits = args(2).nint_value ();
	  
	  if (nbits < 0)
	    error ("bitshift: number of bits to mask must be positive");
	}

      if (error_state)
	return retval;

      octave_value m_arg = args(0);
      std::string cname = m_arg.class_name ();

      if (cname == "uint8")
	DO_UBITSHIFT (uint8, nbits < 8 ? nbits : 8);
      else if (cname == "uint16")
	DO_UBITSHIFT (uint16, nbits < 16 ? nbits : 16);
      else if (cname == "uint32")
	DO_UBITSHIFT (uint32, nbits < 32 ? nbits : 32);
      else if (cname == "uint64")
	DO_UBITSHIFT (uint64, nbits < 64 ? nbits : 64);
      else if (cname == "int8")
	DO_SBITSHIFT (int8, nbits < 8 ? nbits : 8);
      else if (cname == "int16")
	DO_SBITSHIFT (int16, nbits < 16 ? nbits : 16);
      else if (cname == "int32")
	DO_SBITSHIFT (int32, nbits < 32 ? nbits : 32);
      else if (cname == "int64")
	DO_SBITSHIFT (int64, nbits < 64 ? nbits : 64);
      else if (cname == "double")
	{
	  nbits = (nbits < 53 ? nbits : 53);
	  EIGHT_BYTE_INT mask = 0x1FFFFFFFFFFFFFLL;
	  if (nbits < 53)
	    mask = mask >> (53 - nbits);
	  else if (nbits < 1)
	    mask = 0;
	  int bits_in_type = 64;
	  NDArray m = m_arg.array_value ();
	  DO_BITSHIFT ( );
	}
      else
	error ("bitshift: not defined for %s objects", cname.c_str ());
    }
  else
    print_usage ("bitshift");

  return retval;
}

DEFUN (bitmax, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} bitmax ()\n\
Returns the largest integer that can be represented as a floating point\n\
value. That is for IEEE-754 compatiable systems with @code{2^53 - 1}.\n\
@end deftypefn")
{
  octave_value retval;
  if (args.length() != 0)
    print_usage ("bitmax");
  else
    retval = ((double)0x1FFFFFFFFFFFFFLL);
  return retval;
}

DEFUN (intmax, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} intmax (@var{type})\n\
@end deftypefn")
{
  octave_value retval;
  std::string cname = "int32";
  int nargin = args.length ();

  if (nargin == 1 && args(0).is_string())
    cname = args(0).string_value ();
  else if (nargin != 0)
    {
      print_usage ("intmax");
      return retval;
    }

  if (cname == "uint8")
    retval = octave_uint8 (std::numeric_limits<octave_uint8_t>::max());
  else if (cname == "uint16")
    retval = octave_uint16 (std::numeric_limits<octave_uint16_t>::max());
  else if (cname == "uint32")
    retval = octave_uint32 (std::numeric_limits<octave_uint32_t>::max());
  else if (cname == "uint64")
    retval = octave_uint64 (std::numeric_limits<octave_uint64_t>::max());
  else if (cname == "int8")
    retval = octave_int8 (std::numeric_limits<octave_int8_t>::max());
  else if (cname == "int16")
    retval = octave_int16 (std::numeric_limits<octave_int16_t>::max());
  else if (cname == "int32")
    retval = octave_int32 (std::numeric_limits<octave_int32_t>::max());
  else if (cname == "int64")
    retval = octave_int64 (std::numeric_limits<octave_int64_t>::max());
  else
    error ("intmax: not defined for '%s' objects", cname.c_str ());

  return retval;
}

DEFUN (intmin, args, ,
  "-*- texinfo -*-\n\
@deftypefn {Built-in Function} {} intmin (@var{type})\n\
@end deftypefn")
{
  octave_value retval;
  std::string cname = "int32";
  int nargin = args.length ();

  if (nargin == 1 && args(0).is_string())
    cname = args(0).string_value ();
  else if (nargin != 0)
    {
      print_usage ("intmin");
      return retval;
    }

  if (cname == "uint8")
    retval = octave_uint8 (std::numeric_limits<octave_uint8_t>::min());
  else if (cname == "uint16")
    retval = octave_uint16 (std::numeric_limits<octave_uint16_t>::min());
  else if (cname == "uint32")
    retval = octave_uint32 (std::numeric_limits<octave_uint32_t>::min());
  else if (cname == "uint64")
    retval = octave_uint64 (std::numeric_limits<octave_uint64_t>::min());
  else if (cname == "int8")
    retval = octave_int8 (std::numeric_limits<octave_int8_t>::min());
  else if (cname == "int16")
    retval = octave_int16 (std::numeric_limits<octave_int16_t>::min());
  else if (cname == "int32")
    retval = octave_int32 (std::numeric_limits<octave_int32_t>::min());
  else if (cname == "int64")
    retval = octave_int64 (std::numeric_limits<octave_int64_t>::min());
  else
    error ("intmin: not defined for '%s' objects", cname.c_str ());

  return retval;
}

/*
;;; Local Variables: ***
;;; mode: C++ ***
;;; End: ***
*/
