# ----------------------------------------------------------------------------
#
# GNU Make Standard Library (GMSL)
#
# A library of functions to be used with GNU Make's $(call) that
# provides functionality not available in standard GNU Make.
#
# Copyright (c) 2005-2022 John Graham-Cumming
#
# This file is part of GMSL
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# Neither the name of the John Graham-Cumming nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# ----------------------------------------------------------------------------

# This is the GNU Make Standard Library version number as a list with
# three items: major, minor, revision

gmsl_version := 1 2 0

__gmsl_name := GNU Make Standard Library

# Used to output warnings and error from the library, it's possible to
# disable any warnings or errors by overriding these definitions
# manually or by setting GMSL_NO_WARNINGS or GMSL_NO_ERRORS

ifdef GMSL_NO_WARNINGS
__gmsl_warning :=
else
__gmsl_warning = $(if $1,$(warning $(__gmsl_name): $1))
endif

ifdef GMSL_NO_ERRORS
__gmsl_error :=
else
 __gmsl_error = $(if $1,$(error $(__gmsl_name): $1))
endif

# If GMSL_TRACE is enabled then calls to the library functions are
# traced to stdout using warning messages with their arguments

ifdef GMSL_TRACE
__gmsl_tr1 = $(warning $0('$1'))
__gmsl_tr2 = $(warning $0('$1','$2'))
__gmsl_tr3 = $(warning $0('$1','$2','$3'))
else
__gmsl_tr1 :=
__gmsl_tr2 :=
__gmsl_tr3 :=
endif

# See if spaces are valid in variable names (this was the case until
# GNU Make 3.82)
ifeq ($(MAKE_VERSION),3.82)
__gmsl_spaced_vars := $(false)
else
__gmsl_spaced_vars := $(true)
endif

# Figure out whether we have $(eval) or not (GNU Make 3.80 and above)
# if we do not then output a warning message, if we do then some
# functions will be enabled.

__gmsl_have_eval := $(false)
__gmsl_ignore := $(eval __gmsl_have_eval := $(true))

# If this is being run with Electric Cloud's emake then warn that
# their $(eval) support is incomplete in 1.x, 2.x, 3.x, 4.x and 5.0,
# 5.1, 5.2 and 5.3

ifdef ECLOUD_BUILD_ID
__gmsl_emake_major := $(word 1,$(subst ., ,$(EMAKE_VERSION)))
__gmsl_emake_minor := $(word 2,$(subst ., ,$(EMAKE_VERSION)))
ifneq ("$(findstring $(__gmsl_emake_major),1 2 3 4)$(findstring $(__gmsl_emake_major)$(__gmsl_emake_minor),50 51 52 53)","")
$(warning You are using a version of Electric Cloud's emake which has incomplete $$(eval) support)
__gmsl_have_eval := $(false)
endif
endif

# See if we have $(lastword) (GNU Make 3.81 and above)

__gmsl_have_lastword := $(lastword $(false) $(true))

# See if we have native or and and (GNU Make 3.81 and above)

__or_tt := /$(or $(true),$(true))/$(or $(true),$(false))/$(or $(false),$(true))/$(or $(false),$(false))/
__and_tt := /$(and $(true),$(true))/$(and $(true),$(false))/$(and $(false),$(true))/$(and $(false),$(false))/
__gmsl_have_or := $(if $(filter /T/T/T//,$(__or_tt)),$(true),$(false))
__gmsl_have_and := $(if $(filter /T////,$(__and_tt)),$(true),$(false))

ifneq ($(__gmsl_have_eval),$(true))
$(call __gmsl_warning,Your make version $(MAKE_VERSION) does not support $$$$(eval): some functions disabled)
endif

__gmsl_dollar := $$
__gmsl_hash := \#

# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
# Function:  gmsl_compatible
# Arguments: List containing the desired library version number (maj min rev)
# Returns:   $(true) if this version of the library is compatible
#            with the requested version number, otherwise $(false)
# ----------------------------------------------------------------------------
gmsl_compatible = $(strip                                                 \
    $(if $(call gt,$(word 1,$1),$(word 1,$(gmsl_version))),               \
        $(false),                                                         \
        $(if $(call lt,$(word 1,$1),$(word 1,$(gmsl_version))),           \
            $(true),                                                      \
            $(if $(call gt,$(word 2,$1),$(word 2,$(gmsl_version))),       \
                $(false),                                                 \
                $(if $(call lt,$(word 2,$1),$(word 2,$(gmsl_version))),   \
                    $(true),                                              \
                    $(call lte,$(word 3,$1),$(word 3,$(gmsl_version))))))))

# ###########################################################################
# LOGICAL OPERATORS
# ###########################################################################

# not is defined in gmsl

# ----------------------------------------------------------------------------
# Function:  and
# Arguments: Two boolean values
# Returns:   Returns $(true) if both of the booleans are true
# ----------------------------------------------------------------------------
ifneq ($(__gmsl_have_and),$(true))
and = $(__gmsl_tr2)$(if $1,$(if $2,$(true),$(false)),$(false))
endif

# ----------------------------------------------------------------------------
# Function:  or
# Arguments: Two boolean values
# Returns:   Returns $(true) if either of the booleans is true
# ----------------------------------------------------------------------------
ifneq ($(__gmsl_have_or),$(true))
or = $(__gmsl_tr2)$(if $1$2,$(true),$(false))
endif

# ----------------------------------------------------------------------------
# Function:  xor
# Arguments: Two boolean values
# Returns:   Returns $(true) if exactly one of the booleans is true
# ----------------------------------------------------------------------------
xor = $(__gmsl_tr2)$(if $1,$(if $2,$(false),$(true)),$(if $2,$(true),$(false)))

# ----------------------------------------------------------------------------
# Function:  nand
# Arguments: Two boolean values
# Returns:   Returns value of 'not and'
# ----------------------------------------------------------------------------
nand = $(__gmsl_tr2)$(if $1,$(if $2,$(false),$(true)),$(true))

# ----------------------------------------------------------------------------
# Function:  nor
# Arguments: Two boolean values
# Returns:   Returns value of 'not or'
# ----------------------------------------------------------------------------
nor = $(__gmsl_tr2)$(if $1$2,$(false),$(true))

# ----------------------------------------------------------------------------
# Function:  xnor
# Arguments: Two boolean values
# Returns:   Returns value of 'not xor'
# ----------------------------------------------------------------------------
xnor =$(__gmsl_tr2)$(if $1,$(if $2,$(true),$(false)),$(if $2,$(false),$(true)))

# ###########################################################################
# LIST MANIPULATION FUNCTIONS
# ###########################################################################

# ----------------------------------------------------------------------------
# Function:  first (same as LISP's car, or head)
# Arguments: 1: A list
# Returns:   Returns the first element of a list
# ----------------------------------------------------------------------------
first = $(__gmsl_tr1)$(firstword $1)

# ----------------------------------------------------------------------------
# Function:  last
# Arguments: 1: A list
# Returns:   Returns the last element of a list
# ----------------------------------------------------------------------------
ifeq ($(__gmsl_have_lastword),$(true))
last = $(__gmsl_tr1)$(lastword $1)
else
last = $(__gmsl_tr1)$(if $1,$(word $(words $1),$1))
endif

# ----------------------------------------------------------------------------
# Function:  rest (same as LISP's cdr, or tail)
# Arguments: 1: A list
# Returns:   Returns the list with the first element removed
# ----------------------------------------------------------------------------
rest = $(__gmsl_tr1)$(wordlist 2,$(words $1),$1)

# ----------------------------------------------------------------------------
# Function:  chop
# Arguments: 1: A list
# Returns:   Returns the list with the last element removed
# ----------------------------------------------------------------------------
chop = $(__gmsl_tr1)$(wordlist 2,$(words $1),x $1)

# ----------------------------------------------------------------------------
# Function:  map
# Arguments: 1: Name of function to $(call) for each element of list
#            2: List to iterate over calling the function in 1
# Returns:   The list after calling the function on each element
# ----------------------------------------------------------------------------
map = $(__gmsl_tr2)$(strip $(foreach a,$2,$(call $1,$a)))

# ----------------------------------------------------------------------------
# Function:  pairmap
# Arguments: 1: Name of function to $(call) for each pair of elements
#            2: List to iterate over calling the function in 1
#            3: Second list to iterate over calling the function in 1
# Returns:   The list after calling the function on each pair of elements
# ----------------------------------------------------------------------------
pairmap = $(strip $(__gmsl_tr3)\
          $(if $2$3,$(call $1,$(call first,$2),$(call first,$3))     \
                        $(call pairmap,$1,$(call rest,$2),$(call rest,$3))))

# ----------------------------------------------------------------------------
# Function:  leq
# Arguments: 1: A list to compare against...
#            2: ...this list
# Returns:   Returns $(true) if the two lists are identical
# ----------------------------------------------------------------------------
leq = $(__gmsl_tr2)$(strip $(if $(call seq,$(words $1),$(words $2)),     \
          $(call __gmsl_list_equal,$1,$2),$(false)))

__gmsl_list_equal = $(if $(strip $1),                                       \
                        $(if $(call seq,$(call first,$1),$(call first,$2)), \
                            $(call __gmsl_list_equal,                       \
                                $(call rest,$1),                            \
                                $(call rest,$2)),                           \
                            $(false)),                                      \
                     $(true))

# ----------------------------------------------------------------------------
# Function:  lne
# Arguments: 1: A list to compare against...
#            2: ...this list
# Returns:   Returns $(true) if the two lists are different
# ----------------------------------------------------------------------------
lne = $(__gmsl_tr2)$(call not,$(call leq,$1,$2))

# ----------------------------------------------------------------------------
# Function:  reverse
# Arguments: 1: A list to reverse
# Returns:   The list with its elements in reverse order
# ----------------------------------------------------------------------------
reverse =$(__gmsl_tr1)$(strip $(if $1,$(call reverse,$(call rest,$1)) \
                        $(call first,$1)))

# ----------------------------------------------------------------------------
# Function:  uniq
# Arguments: 1: A list from which to remove repeated elements
# Returns:   The list with duplicate elements removed without reordering
# ----------------------------------------------------------------------------
uniq = $(strip $(__gmsl_tr1) $(if $1,$(firstword $1) \
                               $(call uniq,$(filter-out $(firstword $1),$1))))

# ----------------------------------------------------------------------------
# Function:  length
# Arguments: 1: A list
# Returns:   The number of elements in the list
# ----------------------------------------------------------------------------
length = $(__gmsl_tr1)$(words $1)

# ###########################################################################
# STRING MANIPULATION FUNCTIONS
# ###########################################################################

# Helper function that translates any GNU Make 'true' value (i.e. a
# non-empty string) to our $(true)

__gmsl_make_bool = $(if $(strip $1),$(true),$(false))

# ----------------------------------------------------------------------------
# Function:  seq
# Arguments: 1: A string to compare against...
#            2: ...this string
# Returns:   Returns $(true) if the two strings are identical
# ----------------------------------------------------------------------------
seq = $(__gmsl_tr2)$(if $(subst x$1,,x$2)$(subst x$2,,x$1),$(false),$(true))

# ----------------------------------------------------------------------------
# Function:  sne
# Arguments: 1: A string to compare against...
#            2: ...this string
# Returns:   Returns $(true) if the two strings are not the same
# ----------------------------------------------------------------------------
sne = $(__gmsl_tr2)$(call not,$(call seq,$1,$2))

# ----------------------------------------------------------------------------
# Function:  split
# Arguments: 1: The character to split on
#            2: A string to split
# Returns:   Splits a string into a list separated by spaces at the split
#            character in the first argument
# ----------------------------------------------------------------------------
split = $(__gmsl_tr2)$(strip $(subst $1, ,$2))

# ----------------------------------------------------------------------------
# Function:  merge
# Arguments: 1: The character to put between fields
#            2: A list to merge into a string
# Returns:   Merges a list into a single string, list elements are separated
#            by the character in the first argument
# ----------------------------------------------------------------------------
merge = $(__gmsl_tr2)$(strip $(if $2,                                     \
            $(if $(call seq,1,$(words $2)),                               \
                $2,$(call first,$2)$1$(call merge,$1,$(call rest,$2)))))

ifdef __gmsl_have_eval
# ----------------------------------------------------------------------------
# Function:  tr
# Arguments: 1: The list of characters to translate from
#            2: The list of characters to translate to
#            3: The text to translate
# Returns:   Returns the text after translating characters
# ----------------------------------------------------------------------------
tr = $(strip $(__gmsl_tr3)$(call assert_no_dollar,$0,$1$2$3)              \
     $(eval __gmsl_t := $3)                                               \
     $(foreach c,                                                         \
         $(join $(addsuffix :,$1),$2),                                    \
         $(eval __gmsl_t :=                                               \
             $(subst $(word 1,$(subst :, ,$c)),$(word 2,$(subst :, ,$c)), \
                 $(__gmsl_t))))$(__gmsl_t))

# Common character classes for use with the tr function.  Each of
# these is actually a variable declaration and must be wrapped with
# $() or ${} to be used.

[A-Z] := A B C D E F G H I J K L M N O P Q R S T U V W X Y Z #
[a-z] := a b c d e f g h i j k l m n o p q r s t u v w x y z #
[0-9] := 0 1 2 3 4 5 6 7 8 9 #
[A-F] := A B C D E F #

# ----------------------------------------------------------------------------
# Function:  uc
# Arguments: 1: Text to upper case
# Returns:   Returns the text in upper case
# ----------------------------------------------------------------------------
uc = $(__gmsl_tr1)$(call assert_no_dollar,$0,$1)$(call tr,$([a-z]),$([A-Z]),$1)

# ----------------------------------------------------------------------------
# Function:  lc
# Arguments: 1: Text to lower case
# Returns:   Returns the text in lower case
# ----------------------------------------------------------------------------
lc = $(__gmsl_tr1)$(call assert_no_dollar,$0,$1)$(call tr,$([A-Z]),$([a-z]),$1)

# ----------------------------------------------------------------------------
# Function:  strlen
# Arguments: 1: A string
# Returns:   Returns the length of the string
# ----------------------------------------------------------------------------

# This results in __gmsl_tab containing a tab

__gmsl_tab :=	#

__gmsl_characters := A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
__gmsl_characters += a b c d e f g h i j k l m n o p q r s t u v w x y z
__gmsl_characters += 0 1 2 3 4 5 6 7 8 9
__gmsl_characters += ` ~ ! @ \# $$ % ^ & * ( ) - _ = +
__gmsl_characters += { } [ ] \ : ; ' " < > , . / ? |

# This results in __gmsl_space containing just a space

__gmsl_empty :=
__gmsl_space := $(__gmsl_empty) $(__gmsl_empty)

strlen = $(__gmsl_tr1)$(call assert_no_dollar,$0,$1)$(strip $(eval __temp := $(subst $(__gmsl_space),x,$1))$(foreach a,$(__gmsl_characters),$(eval __temp := $$(subst $$a,x,$(__temp))))$(eval __temp := $(subst x,x ,$(__temp)))$(words $(__temp)))

# This results in __gmsl_newline containing just a newline

define __gmsl_newline


endef

# ----------------------------------------------------------------------------
# Function:  substr
# Arguments: 1: A string
#            2: Start position (first character is 1)
#            3: End position (inclusive)
# Returns:   A substring.
# Note:      The string in $1 must not contain a �
# ----------------------------------------------------------------------------

substr = $(if $2,$(__gmsl_tr3)$(call assert_no_dollar,$0,$1$2$3)$(strip $(eval __temp := $$(subst $$(__gmsl_space),� ,$$1))$(foreach a,$(__gmsl_characters),$(eval __temp := $$(subst $$a,$$a$$(__gmsl_space),$(__temp))))$(eval __temp := $(wordlist $2,$3,$(__temp))))$(subst �,$(__gmsl_space),$(subst $(__gmsl_space),,$(__temp))))

endif # __gmsl_have_eval

# ###########################################################################
# SET MANIPULATION FUNCTIONS
# ###########################################################################

# Sets are represented by sorted, deduplicated lists.  To create a set
# from a list use set_create, or start with the empty_set and
# set_insert individual elements

# This is the empty set
empty_set :=

# ----------------------------------------------------------------------------
# Function:  set_create
# Arguments: 1: A list of set elements
# Returns:   Returns the newly created set
# ----------------------------------------------------------------------------
set_create = $(__gmsl_tr1)$(sort $1)

# ----------------------------------------------------------------------------
# Function:  set_insert
# Arguments: 1: A single element to add to a set
#            2: A set
# Returns:   Returns the set with the element added
# ----------------------------------------------------------------------------
set_insert = $(__gmsl_tr2)$(sort $1 $2)

# ----------------------------------------------------------------------------
# Function:  set_remove
# Arguments: 1: A single element to remove from a set
#            2: A set
# Returns:   Returns the set with the element removed
# ----------------------------------------------------------------------------
set_remove = $(__gmsl_tr2)$(filter-out $1,$2)

# ----------------------------------------------------------------------------
# Function:  set_is_member, set_is_not_member
# Arguments: 1: A single element
#            2: A set
# Returns:   (set_is_member) Returns $(true) if the element is in the set
#            (set_is_not_member) Returns $(false) if the element is in the set
# ----------------------------------------------------------------------------
set_is_member = $(__gmsl_tr2)$(if $(filter $1,$2),$(true),$(false))
set_is_not_member = $(__gmsl_tr2)$(if $(filter $1,$2),$(false),$(true))

# ----------------------------------------------------------------------------
# Function:  set_union
# Arguments: 1: A set
#            2: Another set
# Returns:   Returns the union of the two sets
# ----------------------------------------------------------------------------
set_union = $(__gmsl_tr2)$(sort $1 $2)

# ----------------------------------------------------------------------------
# Function:  set_intersection
# Arguments: 1: A set
#            2: Another set
# Returns:   Returns the intersection of the two sets
# ----------------------------------------------------------------------------
set_intersection = $(__gmsl_tr2)$(filter $1,$2)

# ----------------------------------------------------------------------------
# Function:  set_is_subset
# Arguments: 1: A set
#            2: Another set
# Returns:   Returns $(true) if the first set is a subset of the second
# ----------------------------------------------------------------------------
set_is_subset = $(__gmsl_tr2)$(call set_equal,$(call set_intersection,$1,$2),$1)

# ----------------------------------------------------------------------------
# Function:  set_equal
# Arguments: 1: A set
#            2: Another set
# Returns:   Returns $(true) if the two sets are identical
# ----------------------------------------------------------------------------
set_equal = $(__gmsl_tr2)$(call seq,$1,$2)

# ###########################################################################
# ARITHMETIC LIBRARY
# ###########################################################################

# Integers a represented by lists with the equivalent number of x's.
# For example the number 4 is x x x x.

# ----------------------------------------------------------------------------
# Function:  int_decode
# Arguments: 1: A number of x's representation
# Returns:   Returns the integer for human consumption that is represented
#            by the string of x's
# ----------------------------------------------------------------------------
int_decode = $(__gmsl_tr1)$(if $1,$(if $(call seq,$(word 1,$1),x),$(words $1),$1),0)

# ----------------------------------------------------------------------------
# Function:  int_encode
# Arguments: 1: A number in human-readable integer form
# Returns:   Returns the integer encoded as a string of x's
# ----------------------------------------------------------------------------
__int_encode = $(if $1,$(if $(call seq,$(words $(wordlist 1,$1,$2)),$1),$(wordlist 1,$1,$2),$(call __int_encode,$1,$(if $2,$2 $2,x))))
__strip_leading_zero = $(if $1,$(if $(call seq,$(patsubst 0%,%,$1),$1),$1,$(call __strip_leading_zero,$(patsubst 0%,%,$1))),0)
int_encode = $(__gmsl_tr1)$(call __int_encode,$(call __strip_leading_zero,$1))

# The arithmetic library functions come in two forms: one form of each
# function takes integers as arguments and the other form takes the
# encoded form (x's created by a call to int_encode).  For example,
# there are two plus functions:
#
# plus        Called with integer arguments and returns an integer
# int_plus    Called with encoded arguments and returns an encoded result
#
# plus will be slower than int_plus because its arguments and result
# have to be translated between the x's format and integers.  If doing
# a complex calculation use the int_* forms with a single encoding of
# inputs and single decoding of the output.  For simple calculations
# the direct forms can be used.

# Helper function used to wrap an int_* function into a function that
# takes a pair of integers, perhaps a function and returns an integer
# result
__gmsl_int_wrap = $(call int_decode,$(call $1,$(call int_encode,$2),$(call int_encode,$3)))
__gmsl_int_wrap1 = $(call int_decode,$(call $1,$(call int_encode,$2)))
__gmsl_int_wrap2 = $(call $1,$(call int_encode,$2),$(call int_encode,$3))

# ----------------------------------------------------------------------------
# Function:  int_plus
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the sum of the two numbers in x's representation
# ----------------------------------------------------------------------------
int_plus = $(strip $(__gmsl_tr2)$1 $2)

# ----------------------------------------------------------------------------
# Function:  plus (wrapped version of int_plus)
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the sum of the two integers
# ----------------------------------------------------------------------------
plus = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_plus,$1,$2)

# ----------------------------------------------------------------------------
# Function:  int_subtract
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the difference of the two numbers in x's representation,
#            or outputs an error on a numeric underflow
# ----------------------------------------------------------------------------
int_subtract = $(strip $(__gmsl_tr2)$(if $(call int_gte,$1,$2), \
                $(filter-out xx,$(join $1,$2)),                 \
                $(call __gmsl_warning,Subtraction underflow)))

# ----------------------------------------------------------------------------
# Function:  subtract (wrapped version of int_subtract)
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the difference of the two integers,
#            or outputs an error on a numeric underflow
# ----------------------------------------------------------------------------
subtract = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_subtract,$1,$2)

# ----------------------------------------------------------------------------
# Function:  int_multiply
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the product of the two numbers in x's representation
# ----------------------------------------------------------------------------
int_multiply = $(strip $(__gmsl_tr2)$(foreach a,$1,$2))

# ----------------------------------------------------------------------------
# Function:  multiply (wrapped version of int_multiply)
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the product of the two integers
# ----------------------------------------------------------------------------
multiply = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_multiply,$1,$2)

# ----------------------------------------------------------------------------
# Function:  int_divide
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the result of integer division of argument 1 divided
#            by argument 2 in x's representation
# ----------------------------------------------------------------------------
int_divide = $(__gmsl_tr2)$(strip $(if $1,$(if $2,                         \
                 $(subst M,x,$(filter-out x,$(subst $2,M,$1))),            \
                 $(call __gmsl_error,Division by zero))))

# ----------------------------------------------------------------------------
# Function:  divide (wrapped version of int_divide)
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the integer division of the first argument by the second
# ----------------------------------------------------------------------------
divide = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_divide,$1,$2)

# ----------------------------------------------------------------------------
# Function:  int_modulo
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the remainder of integer division of argument 1 divided
#            by argument 2 in x's representation
# ----------------------------------------------------------------------------
int_modulo = $(__gmsl_tr2)$(strip $(if $1,$(if $2,                         \
                 $(filter-out M,$(subst $2,M,$1)),                         \
                 $(call __gmsl_error,Division by zero))))

# ----------------------------------------------------------------------------
# Function:  modulo (wrapped version of int_modulo)
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the remainder of integer division of the first argument
#            by the second
# ----------------------------------------------------------------------------
modulo = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_modulo,$1,$2)

# ----------------------------------------------------------------------------
# Function:  int_max, int_min
# Arguments: 1: A number in x's representation
#            2: Another number in x's represntation
# Returns:   Returns the maximum or minimum of its arguments in x's
#            representation
# ----------------------------------------------------------------------------
int_max = $(__gmsl_tr2)$(subst xx,x,$(join $1,$2))
int_min = $(__gmsl_tr2)$(subst xx,x,$(filter xx,$(join $1,$2)))

# ----------------------------------------------------------------------------
# Function:  max, min
# Arguments: 1: An integer
#            2: Another integer
# Returns:   Returns the maximum or minimum of its integer arguments
# ----------------------------------------------------------------------------
max = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_max,$1,$2)
min = $(__gmsl_tr2)$(call __gmsl_int_wrap,int_min,$1,$2)

# ----------------------------------------------------------------------------
# Function: int_gt, int_gte, int_lt, int_lte, int_eq, int_ne
# Arguments: Two x's representation numbers to be compared
# Returns:   $(true) or $(false)
#
# int_gt    First argument greater than second argument
# int_gte   First argument greater than or equal to second argument
# int_lt    First argument less than second argument
# int_lte   First argument less than or equal to second argument
# int_eq    First argument is numerically equal to the second argument
# int_ne    First argument is not numerically equal to the second argument
# ----------------------------------------------------------------------------
int_gt = $(__gmsl_tr2)$(call __gmsl_make_bool,      \
                          $(filter-out $(words $2), \
                              $(words $(call int_max,$1,$2))))
int_gte = $(__gmsl_tr2)$(call __gmsl_make_bool,     \
                           $(call int_gt,$1,$2)$(call int_eq,$1,$2))
int_lt = $(__gmsl_tr2)$(call __gmsl_make_bool,      \
                          $(filter-out $(words $1), \
                              $(words $(call int_max,$1,$2))))
int_lte = $(__gmsl_tr2)$(call __gmsl_make_bool,     \
                           $(call int_lt,$1,$2)$(call int_eq,$1,$2))
int_eq = $(__gmsl_tr2)$(call __gmsl_make_bool,      \
                          $(filter $(words $1),$(words $2)))
int_ne = $(__gmsl_tr2)$(call __gmsl_make_bool,      \
                          $(filter-out $(words $1),$(words $2)))

# ----------------------------------------------------------------------------
# Function: gt, gte, lt, lte, eq, ne
# Arguments: Two integers to be compared
# Returns:   $(true) or $(false)
#
# gt    First argument greater than second argument
# gte   First argument greater than or equal to second argument
# lt    First argument less than second argument
# lte   First argument less than or equal to second argument
# eq    First argument is numerically equal to the second argument
# ne    First argument is not numerically equal to the second argument
# ----------------------------------------------------------------------------
gt = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_gt,$1,$2)
gte = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_gte,$1,$2)
lt = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_lt,$1,$2)
lte = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_lte,$1,$2)
eq = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_eq,$1,$2)
ne = $(__gmsl_tr2)$(call __gmsl_int_wrap2,int_ne,$1,$2)

# increment adds 1 to its argument, decrement subtracts 1.  Note that
# decrement does not range check and hence will not underflow, but
# will incorrectly say that 0 - 1 = 0

# ----------------------------------------------------------------------------
# Function:  int_inc
# Arguments: 1: A number in x's representation
# Returns:   The number incremented by 1 in x's representation
# ----------------------------------------------------------------------------
int_inc = $(strip $(__gmsl_tr1)$1 x)

# ----------------------------------------------------------------------------
# Function:  inc
# Arguments: 1: An integer
# Returns:   The argument incremented by 1
# ----------------------------------------------------------------------------
inc = $(__gmsl_tr1)$(call __gmsl_int_wrap1,int_inc,$1)

# ----------------------------------------------------------------------------
# Function:  int_dec
# Arguments: 1: A number in x's representation
# Returns:   The number decremented by 1 in x's representation
# ----------------------------------------------------------------------------
int_dec = $(__gmsl_tr1)$(strip                        \
          $(if $(call sne,0,$(words $1)),             \
          $(wordlist 2,$(words $1),$1)))

# ----------------------------------------------------------------------------
# Function:  dec
# Arguments: 1: An integer
# Returns:   The argument decremented by 1
# ----------------------------------------------------------------------------
dec = $(__gmsl_tr1)$(call __gmsl_int_wrap1,int_dec,$1)

# double doubles its argument, and halve halves it

# ----------------------------------------------------------------------------
# Function:  int_double
# Arguments: 1: A number in x's representation
# Returns:   The number doubled (i.e. * 2) and returned in x's representation
# ----------------------------------------------------------------------------
int_double = $(strip $(__gmsl_tr1)$1 $1)

# ----------------------------------------------------------------------------
# Function:  double
# Arguments: 1: An integer
# Returns:   The integer times 2
# ----------------------------------------------------------------------------
double = $(__gmsl_tr1)$(call __gmsl_int_wrap1,int_double,$1)

# ----------------------------------------------------------------------------
# Function:  int_halve
# Arguments: 1: A number in x's representation
# Returns:   The number halved (i.e. / 2) and returned in x's representation
# ----------------------------------------------------------------------------
int_halve = $(__gmsl_tr1)$(strip $(subst xx,x,$(filter-out xy x y, \
                             $(join $1,$(foreach a,$1,y x)))))

# ----------------------------------------------------------------------------
# Function:  halve
# Arguments: 1: An integer
# Returns:   The integer divided by 2
# ----------------------------------------------------------------------------
halve = $(__gmsl_tr1)$(call __gmsl_int_wrap1,int_halve,$1)

# ----------------------------------------------------------------------------
# Function:  sequence
# Arguments: 1: An integer
#            2: An integer
# Returns:   The sequence [arg1, arg2] of integers if arg1 < arg2 or
#            [arg2, arg1] if arg2 > arg1. If arg1 == arg1 return [arg1]
# ----------------------------------------------------------------------------
sequence = $(__gmsl_tr2)$(strip $(if $(call lte,$1,$2),          \
         $(call __gmsl_sequence_up,$1,$2),                       \
         $(call __gmsl_sequence_dn,$2,$1)))

__gmsl_sequence_up = $(if $(call seq,$1,$2),$1,$1 $(call __gmsl_sequence_up,$(call inc,$1),$2))
__gmsl_sequence_dn = $(if $(call seq,$1,$2),$1,$2 $(call __gmsl_sequence_dn,$1,$(call dec,$2)))

# ----------------------------------------------------------------------------
# Function:  dec2hex, dec2bin, dec2oct
# Arguments: 1: An integer
# Returns:   The decimal argument converted to hexadecimal, binary or
#            octal
# ----------------------------------------------------------------------------

__gmsl_digit = $(subst 15,f,$(subst 14,e,$(subst 13,d,$(subst 12,c,$(subst 11,b,$(subst 10,a,$1))))))

dec2hex = $(call __gmsl_dec2base,$(call int_encode,$1),$(call int_encode,16))
dec2bin = $(call __gmsl_dec2base,$(call int_encode,$1),$(call int_encode,2))
dec2oct = $(call __gmsl_dec2base,$(call int_encode,$1),$(call int_encode,8))

__gmsl_base_divide = $(subst $2,X ,$1)
__gmsl_q = $(strip $(filter X,$1))
__gmsl_r = $(words $(filter x,$1))

__gmsl_dec2base = $(eval __gmsl_temp := $(call __gmsl_base_divide,$1,$2))$(call __gmsl_dec2base_,$(call __gmsl_q,$(__gmsl_temp)),$(call __gmsl_r,$(__gmsl_temp)),$2)
__gmsl_dec2base_ = $(if $1,$(call __gmsl_dec2base,$(subst X,x,$1),$3))$(call __gmsl_digit,$2)

ifdef __gmsl_have_eval
# ###########################################################################
# ASSOCIATIVE ARRAYS
# ###########################################################################

# Magic string that is very unlikely to appear in a key or value

__gmsl_aa_magic := faf192c8efbc25c27992c5bc5add390393d583c6

# ----------------------------------------------------------------------------
# Function:  set
# Arguments: 1: Name of associative array
#            2: The key value to associate
#            3: The value associated with the key
# Returns:   Nothing
# ----------------------------------------------------------------------------
set = $(__gmsl_tr3)$(call assert_no_space,$0,$1$2)$(call assert_no_dollar,$0,$1$2$3)$(eval __gmsl_aa_$1_$(__gmsl_aa_magic)_$2_gmsl_aa_$1 := $3)

# Only used internally by memoize function

__gmsl_set = $(call set,$1,$2,$3)$3

# ----------------------------------------------------------------------------
# Function:  get
# Arguments: 1: Name of associative array
#            2: The key to retrieve
# Returns:   The value stored in the array for that key
# ----------------------------------------------------------------------------
get = $(strip $(__gmsl_tr2)$(call assert_no_space,$0,$1$2)$(call assert_no_dollar,$0,$1$2)$(__gmsl_aa_$1_$(__gmsl_aa_magic)_$2_gmsl_aa_$1))

# ----------------------------------------------------------------------------
# Function:  keys
# Arguments: 1: Name of associative array
# Returns:   Returns a list of all defined keys in the array
# ----------------------------------------------------------------------------
keys = $(__gmsl_tr1)$(call assert_no_space,$0,$1)$(call assert_no_dollar,$0,$1)$(sort $(patsubst __gmsl_aa_$1_$(__gmsl_aa_magic)_%_gmsl_aa_$1,%, \
                  $(filter __gmsl_aa_$1_$(__gmsl_aa_magic)_%_gmsl_aa_$1,$(.VARIABLES))))

# ----------------------------------------------------------------------------
# Function:  defined
# Arguments: 1: Name of associative array
#            2: The key to test
# Returns:   Returns true if the key is defined (i.e. not empty)
# ----------------------------------------------------------------------------
defined = $(__gmsl_tr2)$(call assert_no_space,$0,$1$2)$(call assert_no_dollar,$0,$1$2)$(call sne,$(call get,$1,$2),)

endif # __gmsl_have_eval

ifdef __gmsl_have_eval
# ###########################################################################
# NAMED STACKS
# ###########################################################################

# ----------------------------------------------------------------------------
# Function:  push
# Arguments: 1: Name of stack
#            2: Value to push onto the top of the stack (must not contain
#               a space)
# Returns:   None
# ----------------------------------------------------------------------------
push = $(__gmsl_tr2)$(call assert_no_space,$0,$1$2)$(call assert_no_dollar,$0,$1$2)$(eval __gmsl_stack_$1 := $2 $(if $(filter-out undefined,\
    $(origin __gmsl_stack_$1)),$(__gmsl_stack_$1)))

# ----------------------------------------------------------------------------
# Function:  pop
# Arguments: 1: Name of stack
# Returns:   Top element from the stack after removing it
# ----------------------------------------------------------------------------
pop = $(__gmsl_tr1)$(call assert_no_space,$0,$1)$(call assert_no_dollar,$0,$1)$(strip $(if $(filter-out undefined,$(origin __gmsl_stack_$1)), \
    $(call first,$(__gmsl_stack_$1))                                       \
    $(eval __gmsl_stack_$1 := $(call rest,$(__gmsl_stack_$1)))))

# ----------------------------------------------------------------------------
# Function:  peek
# Arguments: 1: Name of stack
# Returns:   Top element from the stack without removing it
# ----------------------------------------------------------------------------
peek = $(__gmsl_tr1)$(call assert_no_space,$0,$1)$(call assert_no_dollar,$0,$1)$(call first,$(__gmsl_stack_$1))

# ----------------------------------------------------------------------------
# Function:  depth
# Arguments: 1: Name of stack
# Returns:   Number of items on the stack
# ----------------------------------------------------------------------------
depth = $(__gmsl_tr1)$(call assert_no_space,$0,$1)$(call assert_no_dollar,$0,$1)$(words $(__gmsl_stack_$1))

endif # __gmsl_have_eval

ifdef __gmsl_have_eval
# ###########################################################################
# STRING CACHE
# ###########################################################################

# ----------------------------------------------------------------------------
# Function:  memoize
# Arguments: 1. Name of the function to be called if the string
#               has not been previously seen
#            2. A string
# Returns:   Returns the result of a memo function (which the user must
#            define) on the passed in string and remembers the result.
#
# Example:   Set memo = $(shell echo "$1" | md5sum) to make a cache
#            of MD5 hashes of strings. $(call memoize,memo,foo bar baz)
# ----------------------------------------------------------------------------
__gmsl_memoize = $(subst $(__gmsl_space),�,$1)cc2af1bb7c4482f2ba75e338b963d3e7$(subst $(__gmsl_space),�,$2)
memoize = $(__gmsl_tr2)$(strip $(if $(call defined,__gmsl_m,$(__gmsl_memoize)),\
                  $(call get,__gmsl_m,$(__gmsl_memoize)),                      \
                  $(call __gmsl_set,__gmsl_m,$(__gmsl_memoize),$(call $1,$2))))

endif # __gmsl_have_eval

# ###########################################################################
# DEBUGGING FACILITIES
# ###########################################################################

# ----------------------------------------------------------------------------
# Target:    gmsl-echo-%
# Arguments: The % should be replaced by the name of a variable that you
#            wish to print out.
# Action:    Echos the value of the variable that matches the %.
#            For example, 'make gmsl-echo-SHELL' will output the value of
#            the SHELL variable.
# ----------------------------------------------------------------------------
gmsl-echo-%: ; @echo $($*)

# ----------------------------------------------------------------------------
# Target:    gmsl-print-%
# Arguments: The % should be replaced by the name of a variable that you
#            wish to print out.
# Action:    Echos the name of the variable that matches the % and its value.
#            For example, 'make gmsl-print-SHELL' will output the value of
#            the SHELL variable
# ----------------------------------------------------------------------------
gmsl-print-%: ; @echo $* = $($*)

# ----------------------------------------------------------------------------
# Function:  assert
# Arguments: 1: A boolean that must be true or the assertion will fail
#            2: The message to print with the assertion
# Returns:   None
# ----------------------------------------------------------------------------
assert = $(if $2,$(if $1,,$(call __gmsl_error,Assertion failure: $2)))

# ----------------------------------------------------------------------------
# Function:  assert_exists
# Arguments: 1: Name of file that must exist, if it is missing an assertion
#               will be generated
# Returns:   None
# ----------------------------------------------------------------------------
assert_exists = $(if $0,$(call assert,$(wildcard $1),file '$1' missing))

# ----------------------------------------------------------------------------
# Function:  assert_no_dollar
# Arguments: 1: Name of a function being executd
#            2: Arguments to check
# Returns:   None
# ----------------------------------------------------------------------------
assert_no_dollar = $(call __gmsl_tr2)$(call assert,$(call not,$(findstring $(__gmsl_dollar),$2)),$1 called with a dollar sign in argument)

# ----------------------------------------------------------------------------
# Function:  assert_no_space
# Arguments: 1: Name of a function being executd
#            2: Arguments to check
# Returns:   None
# ----------------------------------------------------------------------------
ifeq ($(__gmsl_spaced_vars),$(false))
assert_no_space = $(call assert,$(call not,$(findstring $(__gmsl_aa_magic),$(subst $(__gmsl_space),$(__gmsl_aa_magic),$2))),$1 called with a space in argument)
else
assert_no_space =
endif
