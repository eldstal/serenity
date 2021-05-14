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
#include <stdlib.h>
#            define VERIFY(cond) if(!(cond)){exit(0);}
#            define VERIFY_NOT_REACHED() exit(0)
#            define TODO VERIFY_NOT_REACHED
#        else
#            define VERIFY assert
#            define VERIFY_NOT_REACHED() assert(false)
#            define TODO VERIFY_NOT_REACHED
#        endif
#    endif
#endif
