/**
 * pointer.hpp - 
 * @author: Jonathan Beard
 * @version: Thu May 15 09:58:51 2014
 * 
 * Copyright 2014 Jonathan Beard
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _POINTER_HPP_
#define _POINTER_HPP_  1

#include <cstdlib>
#include <cstdint>
#include <cstddef>

#include "defs.hpp"

class Pointer{
   using wrap_t = std::size_t;

public:
   /**
    * Pointer - used to synchronize read and write
    * pointers for the ring buffer.  This class encapsulates
    * wrapping.
    */
   Pointer( const std::size_t cap ) : max_cap( cap ){};
   
   Pointer( const std::size_t cap, 
            const wrap_t wrap_set );
   /**
    * Pointer - used to snchronize read and write pointers for the
    * ring buffer, this constructer is a copy constructor that
    * copies an old Pointer object and sets a new max_capacity
    * @param   other, const Pointer&, the other pointer to be cpied
    * @param   new_cap, the new max cap
    */
   Pointer( Pointer * const other, 
            const std::size_t new_cap );

   /**
    * val - returns the current value of val.
    * @return std::size_t, current 'true' value of the pointer
    */
   static std::size_t val( Pointer * const ptr ) ;

   /**
    * inc - increments the pointer, takes care of wrapping
    * the pointers as well so you don't run off the page
    * @return  std::size_t, current value of pointer after increment
    */
   static void inc( Pointer * const ptr ) ;
   
   /**
    * incBy - increments the current pointer poisition
    * by 'in' increments.  To be used for range insertion
    * and removal
    * @param  ptr - Pointer * const
    * @param  in - const std::size_t
    * @return void
    */
   static void incBy( Pointer * const ptr,
                      const std::size_t in );

   
   /**
    * wrapIndiciator - returns the current wrap position, 
    * the read should never be ahead of the write, and 
    * at best they should be equal.  This is used when
    * determining to return max_cap or zero for the current
    * queue size.  
    * @return  std::size_t
    */
   static std::size_t wrapIndicator( Pointer * const ptr ) ;
   
private:
   volatile std::uint64_t           a  = 0;
   volatile std::uint64_t           b  = 0;
   /**
    * size of wrap pointer might become an issue
    * if GHz increase drastically or if this runs
    * for a really really long time....@ 10GHz and
    * assuming 1 wrap per cycle that works out to 
    * around 54 years, @ 2GHz we have ~250 years.
    * TODO, get these set correctly if we do eventually
    * wrap an unsigned 64 int.
    */
   volatile wrap_t    wrap_a  = 0;
   volatile wrap_t    wrap_b  = 0;
   const    std::size_t      max_cap;
};
#endif /* END _POINTER_HPP_ */
