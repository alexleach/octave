## Copyright (C) 2013 Vytautas Jančauskas
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or (at
## your option) any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, see
## <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn {Function File} {@var{value} =} get (@var{recorder}, @var{name})
## @deftypefnx {Function File} {@var{values} =} get (@var{recorder})
## Return the @var{value} of the property identified by @var{name}.
## If @var{name} is a cell array, return the values of the properties
## corresponding to the elements of the cell array.  Given only the
## recorder object, return a scalar structure with values of all
## properties of @var{recorder}.  The field names of the structure
## correspond to property names.
## @end deftypefn

function result = get (varargin)
  recorder = varargin{1};
  properties = __get_properties__ (recorder);
  if nargin == 1
    result = properties;
  elseif nargin == 2
    if ischar (varargin{2})
      result = getfield (properties, varargin{2});
    else
      result = {};
      index = 1;
      for property = varargin{2}
        result{index} = getfield (properties, char (property));
        index = index + 1;
      endfor
    endif
  else
    error ("audiorecorder: wrong number of arguments to the get method");
  endif
endfunction
