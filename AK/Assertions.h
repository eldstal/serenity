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
   void _exit(int);
}
#            define VERIFY(cond) if(!(cond)){ _exit(0);}
#            define VERIFY_NOT_REACHED() VERIFY(false)
#            define TODO VERIFY_NOT_REACHED
#        else
#            define VERIFY assert
#            define VERIFY_NOT_REACHED() assert(false)
#            define TODO VERIFY_NOT_REACHED
#        endif
#    endif
#endif
