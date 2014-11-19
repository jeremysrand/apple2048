;
; File: textpage.s
; Author: Jeremy Rand
; Date: July 23, 2014
;
; This file contains some assembly routines to more efficiently flip text pages.
;

    .export _switchToPage2, _switchToPage1, _gsSwitchToPage2

    .include "apple2.inc"

    .A8
    .I8
    .PC02

_switchToPage2:
    ldx #0
@L1:
    lda $400,x      ; Line 0
    sta $800,x
    lda $480,x      ; Line 1
    sta $880,x
    lda $500,x      ; Line 2
    sta $900,x
    lda $580,x      ; Line 3
    sta $980,x
    lda $600,x      ; Line 4
    sta $a00,x
    lda $680,x      ; Line 5
    sta $a80,x
    lda $700,x      ; Line 6
    sta $b00,x
    lda $780,x      ; Line 7
    sta $b80,x

    lda $428,x      ; Line 8
    sta $828,x
    lda $4a8,x      ; Line 9
    sta $8a8,x
    lda $528,x      ; Line 10
    sta $928,x
    lda $5a8,x      ; Line 11
    sta $9a8,x
    lda $628,x      ; Line 12
    sta $a28,x
    lda $6a8,x      ; Line 13
    sta $aa8,x
    lda $728,x      ; Line 14
    sta $b28,x
    lda $7a8,x      ; Line 15
    sta $ba8,x

    lda $450,x      ; Line 16
    sta $850,x
    lda $4d0,x      ; Line 17
    sta $8d0,x
    lda $550,x      ; Line 18
    sta $950,x
    lda $5d0,x      ; Line 19
    sta $9d0,x
    lda $650,x      ; Line 20
    sta $a50,x
    lda $6d0,x      ; Line 21
    sta $ad0,x
    lda $750,x      ; Line 22
    sta $b50,x
    lda $7d0,x      ; Line 23
    sta $bd0,x

    inx
    cpx #40
    beq @L2
    jmp @L1
@L2:
    stz HISCR
    rts


_switchToPage1:
    stz LOWSCR
    rts


_gsSwitchToPage2:
    
    .P816
    clc
    xce
    rep #$30

    .A16
    .I16
    ldx #0
@L3:
    lda $400,x      ; Line 0
    sta $e00800,x
    lda $480,x      ; Line 1
    sta $e00880,x
    lda $500,x      ; Line 2
    sta $e00900,x
    lda $580,x      ; Line 3
    sta $e00980,x
    lda $600,x      ; Line 4
    sta $e00a00,x
    lda $680,x      ; Line 5
    sta $e00a80,x
    lda $700,x      ; Line 6
    sta $e00b00,x
    lda $780,x      ; Line 7
    sta $e00b80,x

    lda $428,x      ; Line 8
    sta $e00828,x
    lda $4a8,x      ; Line 9
    sta $e008a8,x
    lda $528,x      ; Line 10
    sta $e00928,x
    lda $5a8,x      ; Line 11
    sta $e009a8,x
    lda $628,x      ; Line 12
    sta $e00a28,x
    lda $6a8,x      ; Line 13
    sta $e00aa8,x
    lda $728,x      ; Line 14
    sta $e00b28,x
    lda $7a8,x      ; Line 15
    sta $e00ba8,x

    lda $450,x      ; Line 16
    sta $e00850,x
    lda $4d0,x      ; Line 17
    sta $e008d0,x
    lda $550,x      ; Line 18
    sta $e00950,x
    lda $5d0,x      ; Line 19
    sta $e009d0,x
    lda $650,x      ; Line 20
    sta $e00a50,x
    lda $6d0,x      ; Line 21
    sta $e00ad0,x
    lda $750,x      ; Line 22
    sta $e00b50,x
    lda $7d0,x      ; Line 23
    sta $e00bd0,x

    inx
    inx
    cpx #40
    beq @L4
    jmp @L3

@L4:
    sep #$30
    .A8
    .I8
    sec
    xce
    .PC02
    stz HISCR
    rts
