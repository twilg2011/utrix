/*
 *  pth_stack.s
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */


.text
.globl _setsp
_setsp:  movl %eax,%esp
         ret
		 
.globl _setbp
_setbp: movl %eax,%ebp
        ret