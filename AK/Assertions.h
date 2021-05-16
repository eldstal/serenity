/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#if defined(KERNEL)
#    include <Kernel/Assertions.h>
#else
#    include <assert.h>
#    ifndef __serenity__
#        ifdef ASSERT_IS_EXIT
             extern "C" {
               extern int puts(const char*);
             }
#            define __stringify_helper(x) #    x
#            define __stringify(x) __stringify_helper(x)
#            define VERIFY(cond) if(!(cond)){puts("Assertion " __FILE__ ":" __stringify(__LINE__) " failed: " #cond ". This is not an eligible crash.\n");}
//#            define VERIFY(cond) if(!(cond)){puts("Assertion " __FILE__ ":" __stringify(__LINE__) " failed.\n");}
#            define VERIFY(cond) if(!(cond)){;}
#            define VERIFY_NOT_REACHED() VERIFY(false)
#            define TODO VERIFY_NOT_REACHED
#        else
#            define VERIFY assert
#            define VERIFY_NOT_REACHED() assert(false)
#            define TODO VERIFY_NOT_REACHED
#        endif
#    endif
#endif
