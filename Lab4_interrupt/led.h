#ifndef LED_H
#define LED_H
#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
void PORT1_IRQHandler(void);
void ledInitialize(void);
void switchInitialize(void);
void p1InterruptInitialize(void);
void p2InterruptInitialize(void);
void b1ActionPerformed(void);
void b2ActionPerformed(void);
void doStuff(void);
#endif
