/*
 *  pth_context.h
 *  
 *
 *  Created by lorenzo galeotti on 23/07/08.
 *  Copyright 2008 Utrix. All rights reserved.
 *
 */




#include "pth_struct.h"


context_t* pth_init(void (*f) (void*),void* arg);

int pth_start(context_t* old, context_t* next);


int addpar(partition_t new);

int relasepart(partition_t part);