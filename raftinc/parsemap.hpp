/**
 * parsemap.hpp - 
 * @author: Jonathan Beard
 * @version: Sun Mar 19 05:04:05 2017
 * 
 * Copyright 2017 Jonathan Beard
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
#ifndef _PARSEMAP_HPP_
#define _PARSEMAP_HPP_  1
#include <cstdint>
#include "parsetreedefs.hpp"
#include "parsedefs.hpp"
#include "parsetree.hpp"
#include "submap.hpp"

namespace raft
{

/** just pre-declaration **/
class kernel;
class kernel_wrapper;

class parsemap : public submap
{
public:
    
    parsemap();
    virtual ~parsemap();
   
   /** 
    * parse_link - this one is meant to be used
    * by the parser for template overloads. The
    * state struct is defined in parsedefs.hpp
    *
    * @param   a - raft::kernel*, src kernel
    * @param   b - raft::kernel*, dst kernel
    */
    void parse_link( raft::kernel *src, 
                     raft::kernel *dst );
  
    /**
     * parse_link_split - for situations like
     * kernel <= kernel. For parsemap <= kernel
     * use the other overload of split, similar
     * for kernel <= parsemap and parsemap <= parsemap.
     * NOTE: we could have built this into the 
     * simpler parse_link, however the logic
     * gets a bit convoluted and we might as well
     * do it this way since the information is 
     * there statically..produces slightly better
     * code.
     * @param src - raft::kernel*
     * @param dst - raft::kernel*
     */
    void parse_link_split( raft::kernel *src,
                           raft::kernel *dst );
    
    
    void parse_link_split_prepend( raft::kernel   *src );

    /**
     * parse_link_join - very similar to the
     * above, however this is for joins.
     */
    void parse_link_join( raft::kernel *src,
                          raft::kernel *dst );
    
    /**
     * parse_link_continue - designed for map >> x situations
     * where we want to link all the kernels in the parse head
     * to dst. if needed this method will duplicate dst the
     * number of times so that the number of kernels in the 
     * parse head before calling this function is equal to
     * the number after.
     * @param dst - raft::kernel, to be added
     */
    void parse_link_continue(   /** source is the parse head **/
                                raft::kernel *dst );

    
    /** 
     * start_ group - start a new parse head group, this 
     * is typically the destination side, the head groups
     * from LHS -> RHS.
     */
    void new_rhs_group();


    /**
     * new_lhs_group - add a new group to the LHS with
     * no kernels, basically adds an empty container
     * to the left most side of the parse tree. This is 
     * quite useful for dealing with operator precedence
     * issues. 
     */
    void new_lhs_group();

    /**
     * add_to_rhs_group - add a new kernel to the RHS of
     * the parse tree. Will add head to the most
     * recent group added. Call new_rhs_group if a fresh 
     * group is desired.
     *
     * @param k - raft::kernel*, kernel to add to rhs
     */
    void add_to_rhs_group( raft::kernel * const k );
    
    /**
     * add_to_lhs_group - add a new kernel to the LHS of parse. 
     * Will add head to the most recent group added. Call 
     * new_rhs_group if a fresh group is desired.
     *
     * @param k - raft::kernel*, kernel to add to head 
     */
    void add_to_lhs_group( raft::kernel * const k );
   
    /**
     * get_group_size - returns the total number of groups
     * at the parse head (which is the RHS of the parse tree 
     * as it's expanded along the DAG from left to right (picture
     * the stream operators and how they connect/reduce), 
     * not the number of kernels (this could vary by group).
     * @return std::size_t - total number of groups
     */
    std::size_t  get_group_size();

    /**
     * pop_tree_frontier - pops the head frontier (all 
     * groups at the RHS of the parse tree, e.g., last one to be 
     * added). If we're visualizing the graph as a DAG
     * LHS to RHS and top to bottom. This is the furthest
     * RHS set of groups that has been parsed, all the way to the 
     * bottom. Returns all groups at the current RHS frontier.
     * 
     * @return - group_ptr_t, aka, std::unique_ptr< std::vector< kernel > >
     */
    group_ptr_t pop_tree_frontier();

    

private:

    void parse_link_helper( raft::kernel *src, 
                            raft::kernel *dst );

    raft::parsetree tree; 
    

    /** override default **/
    virtual void updateKernels( raft::kernel * const a, raft::kernel * const b );
};

}
#endif /* END _PARSEMAP_HPP_ */