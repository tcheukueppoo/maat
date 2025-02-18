#!/usr/bin/env perl

use strict;
use warnings;

# View the progress made in the maat project


my %tasks = (
   doc => {
      dev_doc => {
      },
      user_doc => {
      }
   }
   parser => {
      unimpl  => 1,
      package => { nonblock_package_stat => 1, block_package_stat => 1, },
      use     => 1,
      load    => 1,
      vardef  => {
         let_var   => 1,
         our_var   => 1,
         temp_var  => 1,
         state_var => 1,
         const_var => 1,
      },
      exp     => {
         nil  => 1,
         bool => 1,
         num  => {
            integer => {
               bin => 1,
               oct => 1,
               dec => 1,
               hex => 1,
            },
            float => { hex => 1, dec => 1, },
         },
         str  => {
            str_default    => {
               single_quote   => 1,
               double_quote   => 1,
               backtick_quote => 1,
            },
            str_with_pairs => {
               single_quote_pair   => 1,
               double_quote_pair   => 1,
               backtick_quote_pair => 1,
               regexp_quote_pair   => 1,
            },
            here_doc       => { },
         },
         vardef => 0,
         anonymous_fn => { 1 },
         array => 1,
         map => 1,
         statement_expression => {
         }
      }
      once    => { nonblock_once_stat => 1, block_once_stat => 1 },
      fn => 1,
      class => 1,
      role => 1
      try => 1,
      phasers => [],
      defer => { nonblock_defer => 1, block_defer => 1 },
      return => 1,
      statement_modifier => {},
      flow => { when => 1, unless => 1, if => 1, with => 1 },
      loop => {},
      loop_control => {},
      vardef => {},
      label => 1,
   },
   compiler => {
   },
   virtual_machine => {
   },
   features => {
   },

);



