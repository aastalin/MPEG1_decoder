#ifndef __VLC_H__
#define __VLC_H__

#include "decoder.h"


int macroblock_address_increment(DataHandler& handler)
{
    int value = 0;

    if( handler.nextbits("1") ) value = 1;
    else if( handler.nextbits("011") ) value = 2;
    else if( handler.nextbits("010") ) value = 3;
    else if( handler.nextbits("0011") ) value = 4;
    else if( handler.nextbits("0010") ) value = 5;
    else if( handler.nextbits("00011") ) value = 6;
    else if( handler.nextbits("00010") ) value = 7;
    else if( handler.nextbits("0000111") ) value = 8;
    else if( handler.nextbits("0000110") ) value = 9;
    else if( handler.nextbits("00001011") ) value = 10;
    else if( handler.nextbits("00001010") ) value = 11;
    else if( handler.nextbits("00001001") ) value = 12;
    else if( handler.nextbits("00001000") ) value = 13;
    else if( handler.nextbits("00000111") ) value = 14;
    else if( handler.nextbits("00000110") ) value = 15;
    else if( handler.nextbits("0000010111") ) value = 16;
    else if( handler.nextbits("0000010110") ) value = 17;
    else if( handler.nextbits("0000010101") ) value = 18;
    else if( handler.nextbits("0000010100") ) value = 19;
    else if( handler.nextbits("0000010011") ) value = 20;
    else if( handler.nextbits("0000010010") ) value = 21;
    else if( handler.nextbits("00000100011") ) value = 22;
    else if( handler.nextbits("00000100010") ) value = 23;
    else if( handler.nextbits("00000100001") ) value = 24;
    else if( handler.nextbits("00000100000") ) value = 25;
    else if( handler.nextbits("00000011111") ) value = 26;
    else if( handler.nextbits("00000011110") ) value = 27;
    else if( handler.nextbits("00000011101") ) value = 28;
    else if( handler.nextbits("00000011100") ) value = 29;
    else if( handler.nextbits("00000011011") ) value = 30;
    else if( handler.nextbits("00000011010") ) value = 31;
    else if( handler.nextbits("00000011001") ) value = 32;
    else if( handler.nextbits("00000011000") ) value = 33;

    if( DEBUG ) std::cout << "macroblock_address_increment: " << value << std::endl;
    return value;
}


bool* macroblock_type(DataHandler& handler, size_t type)
{
    bool* param = new bool[5];    // quant, motion_forward, motion_backward, pattern, intra

    switch( type )
    {
        case 1:
            if( handler.nextbits("1") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
            else if( handler.nextbits("01") )
            {
                param[0] = 1;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
        case 2:
            if( handler.nextbits("1") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("01") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("001") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 0;
                param[3] = 0;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("00011") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
            else if( handler.nextbits("00010") )
            {
                param[0] = 1;
                param[1] = 1;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
           else if( handler.nextbits("00001") )
            {
                param[0] = 1;
                param[1] = 0;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("000001") )
            {
                param[0] = 1;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
        case 3:
            if( handler.nextbits("10") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 1;
                param[3] = 0;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("11") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 1;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("010") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 1;
                param[3] = 0;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("011") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 1;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("0010") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 0;
                param[3] = 0;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("0011") )
            {
                param[0] = 0;
                param[1] = 1;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("00011") )
            {
                param[0] = 0;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
            else if( handler.nextbits("00010") )
            {
                param[0] = 1;
                param[1] = 1;
                param[2] = 1;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("000011") )
            {
                param[0] = 1;
                param[1] = 1;
                param[2] = 0;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("000010") )
            {
                param[0] = 1;
                param[1] = 0;
                param[2] = 1;
                param[3] = 1;
                param[4] = 0;
                break;
            }
            else if( handler.nextbits("000001") )
            {
                param[0] = 1;
                param[1] = 0;
                param[2] = 0;
                param[3] = 0;
                param[4] = 1;
                break;
            }
        default:
        {
            std::cerr << "not supported picture type" << std::endl;
            exit(-1);
        }
    }

    if( DEBUG )
    {
        std::cout << "macroblock_type: " << type << std::endl;
        std::cout << "    macroblcok_quant: " << param[0] << std::endl;
        std::cout << "    macroblcok_motion_forward: " << param[1] << std::endl;
        std::cout << "    macroblcok_motion_backward: " << param[2] << std::endl;
        std::cout << "    macroblcok_pattern: " << param[3] << std::endl;
        std::cout << "    macroblcok_intra: " << param[4] << std::endl;
    }
    return param;
}


size_t dct_dc_size_luminance(DataHandler& handler)
{
    size_t size;
    if( handler.nextbits("100") ) size = 0;
    else if( handler.nextbits("00") ) size = 1;
    else if( handler.nextbits("01") ) size = 2;
    else if( handler.nextbits("101") ) size = 3;
    else if( handler.nextbits("110") ) size = 4;
    else if( handler.nextbits("1110") ) size = 5;
    else if( handler.nextbits("11110") ) size = 6;
    else if( handler.nextbits("111110") ) size = 7;
    else if( handler.nextbits("1111110") ) size = 8;

    return size;
}


size_t dct_dc_size_chrominance(DataHandler& handler)
{
    size_t size;
    if( handler.nextbits("00") ) size = 0;
    else if( handler.nextbits("01") ) size = 1;
    else if( handler.nextbits("10") ) size = 2;
    else if( handler.nextbits("110") ) size = 3;
    else if( handler.nextbits("1110") ) size = 4;
    else if( handler.nextbits("11110") ) size = 5;
    else if( handler.nextbits("111110") ) size = 6;
    else if( handler.nextbits("1111110") ) size = 7;
    else if( handler.nextbits("11111110") ) size = 8;

    return size;
}

size_t _idx;
int _dct_zz[64];
int _dct_recon[8][8];

void dct_coeff_next(DataHandler& handler, bool isNext)
{
    int level;
    size_t run;
    bool escape = false;
    if( !isNext && handler.nextbits("1") ){ run = 0; level = 1; }
    else if( isNext && handler.nextbits("11") ){ run = 0; level = 1; }
    else if( handler.nextbits("011") ){ run = 1; level = 1; }
    else if( handler.nextbits("0100") ){ run = 0; level = 2; }
    else if( handler.nextbits("0101") ){ run = 2; level = 1; }
    else if( handler.nextbits("00101") ){ run = 0; level = 3; }
    else if( handler.nextbits("00111") ){ run = 3; level = 1; }
    else if( handler.nextbits("00110") ){ run = 4; level = 1; }
    else if( handler.nextbits("000110") ){ run = 1; level = 2; }
    else if( handler.nextbits("000111") ){ run = 5; level = 1; }
    else if( handler.nextbits("000101") ){ run = 6; level = 1; }
    else if( handler.nextbits("000100") ){ run = 7; level = 1; }
    else if( handler.nextbits("0000110") ){ run = 0; level = 4; }
    else if( handler.nextbits("0000100") ){ run = 2; level = 2; }
    else if( handler.nextbits("0000111") ){ run = 8; level = 1; }
    else if( handler.nextbits("0000101") ){ run = 9; level = 1; }
    else if( handler.nextbits("000001") )
    {
        escape = true;
        run = handler.READUINT(6);
        if( handler.nextbits("00000000")) level = handler.READUINT(8);
        else if( handler.nextbits("10000000")) level = handler.READUINT(8)-256;
        else if( handler.nextbits("1")) level = handler.READUINT(7)-128;
        else level = handler.READUINT(8);
    }
    else if( handler.nextbits("00100110") ){ run = 0; level = 5; }
    else if( handler.nextbits("00100001") ){ run = 0; level = 6; }
    else if( handler.nextbits("00100101") ){ run = 1; level = 3; }
    else if( handler.nextbits("00100100") ){ run = 3; level = 2; }
    else if( handler.nextbits("00100111") ){ run = 10; level = 1; }
    else if( handler.nextbits("00100011") ){ run = 11; level = 1; }
    else if( handler.nextbits("00100010") ){ run = 12; level = 1; }
    else if( handler.nextbits("00100000") ){ run = 13; level = 1; }
    else if( handler.nextbits("0000001010") ){ run = 0; level = 7; }
    else if( handler.nextbits("0000001100") ){ run = 1; level = 4; }
    else if( handler.nextbits("0000001011") ){ run = 2; level = 3; }
    else if( handler.nextbits("0000001111") ){ run = 4; level = 2; }
    else if( handler.nextbits("0000001001") ){ run = 5; level = 2; }
    else if( handler.nextbits("0000001110") ){ run = 14; level = 1; }
    else if( handler.nextbits("0000001101") ){ run = 15; level = 1; }
    else if( handler.nextbits("0000001000") ){ run = 16; level = 1; }
    else if( handler.nextbits("000000011101") ){ run = 0; level = 8; }
    else if( handler.nextbits("000000011000") ){ run = 0; level = 9; }
    else if( handler.nextbits("000000010011") ){ run = 0; level = 10; }
    else if( handler.nextbits("000000010000") ){ run = 0; level = 11; }
    else if( handler.nextbits("000000011011") ){ run = 1; level = 5; }
    else if( handler.nextbits("000000010100") ){ run = 2; level = 4; }
    else if( handler.nextbits("000000011100") ){ run = 3; level = 3; }
    else if( handler.nextbits("000000010010") ){ run = 4; level = 3; }
    else if( handler.nextbits("000000011110") ){ run = 6; level = 2; }
    else if( handler.nextbits("000000010101") ){ run = 7; level = 2; }
    else if( handler.nextbits("000000010001") ){ run = 8; level = 2; }
    else if( handler.nextbits("000000011111") ){ run = 17; level = 1; }
    else if( handler.nextbits("000000011010") ){ run = 18; level = 1; }
    else if( handler.nextbits("000000011001") ){ run = 19; level = 1; }
    else if( handler.nextbits("000000010111") ){ run = 20; level = 1; }
    else if( handler.nextbits("000000010110") ){ run = 21; level = 1; }
    else if( handler.nextbits("0000000011010") ){ run = 0; level = 12; }
    else if( handler.nextbits("0000000011001") ){ run = 0; level = 13; }
    else if( handler.nextbits("0000000011000") ){ run = 0; level = 14; }
    else if( handler.nextbits("0000000010111") ){ run = 0; level = 15; }
    else if( handler.nextbits("0000000010110") ){ run = 1; level = 6; }
    else if( handler.nextbits("0000000010101") ){ run = 1; level = 7; }
    else if( handler.nextbits("0000000010100") ){ run = 2; level = 5; }
    else if( handler.nextbits("0000000010011") ){ run = 3; level = 4; }
    else if( handler.nextbits("0000000010010") ){ run = 5; level = 3; }
    else if( handler.nextbits("0000000010001") ){ run = 9; level = 2; }
    else if( handler.nextbits("0000000010000") ){ run = 10; level = 2; }
    else if( handler.nextbits("0000000011111") ){ run = 22; level = 1; }
    else if( handler.nextbits("0000000011110") ){ run = 23; level = 1; }
    else if( handler.nextbits("0000000011101") ){ run = 24; level = 1; }
    else if( handler.nextbits("0000000011100") ){ run = 25; level = 1; }
    else if( handler.nextbits("0000000011011") ){ run = 26; level = 1; }
    else if( handler.nextbits("00000000011111") ){ run = 0; level = 16; }
    else if( handler.nextbits("00000000011110") ){ run = 0; level = 17; }
    else if( handler.nextbits("00000000011101") ){ run = 0; level = 18; }
    else if( handler.nextbits("00000000011100") ){ run = 0; level = 19; }
    else if( handler.nextbits("00000000011011") ){ run = 0; level = 20; }
    else if( handler.nextbits("00000000011010") ){ run = 0; level = 21; }
    else if( handler.nextbits("00000000011001") ){ run = 0; level = 22; }
    else if( handler.nextbits("00000000011000") ){ run = 0; level = 23; }
    else if( handler.nextbits("00000000010111") ){ run = 0; level = 24; }
    else if( handler.nextbits("00000000010110") ){ run = 0; level = 25; }
    else if( handler.nextbits("00000000010101") ){ run = 0; level = 26; }
    else if( handler.nextbits("00000000010100") ){ run = 0; level = 27; }
    else if( handler.nextbits("00000000010011") ){ run = 0; level = 28; }
    else if( handler.nextbits("00000000010010") ){ run = 0; level = 29; }
    else if( handler.nextbits("00000000010001") ){ run = 0; level = 30; }
    else if( handler.nextbits("00000000010000") ){ run = 0; level = 31; }
    else if( handler.nextbits("000000000011000") ){ run = 0; level = 32; }
    else if( handler.nextbits("000000000010111") ){ run = 0; level = 33; }
    else if( handler.nextbits("000000000010110") ){ run = 0; level = 34; }
    else if( handler.nextbits("000000000010101") ){ run = 0; level = 35; }
    else if( handler.nextbits("000000000010100") ){ run = 0; level = 36; }
    else if( handler.nextbits("000000000010011") ){ run = 0; level = 37; }
    else if( handler.nextbits("000000000010010") ){ run = 0; level = 38; }
    else if( handler.nextbits("000000000010001") ){ run = 0; level = 39; }
    else if( handler.nextbits("000000000010000") ){ run = 0; level = 40; }
    else if( handler.nextbits("000000000011111") ){ run = 1; level = 8; }
    else if( handler.nextbits("000000000011110") ){ run = 1; level = 9; }
    else if( handler.nextbits("000000000011101") ){ run = 1; level = 10; }
    else if( handler.nextbits("000000000011100") ){ run = 1; level = 11; }
    else if( handler.nextbits("000000000011011") ){ run = 1; level = 12; }
    else if( handler.nextbits("000000000011010") ){ run = 1; level = 13; }
    else if( handler.nextbits("000000000011001") ){ run = 1; level = 14; }
    else if( handler.nextbits("0000000000010011") ){ run = 1; level = 15; }
    else if( handler.nextbits("0000000000010010") ){ run = 1; level = 16; }
    else if( handler.nextbits("0000000000010001") ){ run = 1; level = 17; }
    else if( handler.nextbits("0000000000010000") ){ run = 1; level = 18; }
    else if( handler.nextbits("0000000000010100") ){ run = 6; level = 3; }
    else if( handler.nextbits("0000000000011010") ){ run = 11; level = 2; }
    else if( handler.nextbits("0000000000011001") ){ run = 12; level = 2; }
    else if( handler.nextbits("0000000000011000") ){ run = 13; level = 2; }
    else if( handler.nextbits("0000000000010111") ){ run = 14; level = 2; }
    else if( handler.nextbits("0000000000010110") ){ run = 15; level = 2; }
    else if( handler.nextbits("0000000000010101") ){ run = 16; level = 2; }
    else if( handler.nextbits("0000000000011111") ){ run = 27; level = 1; }
    else if( handler.nextbits("0000000000011110") ){ run = 28; level = 1; }
    else if( handler.nextbits("0000000000011101") ){ run = 29; level = 1; }
    else if( handler.nextbits("0000000000011100") ){ run = 30; level = 1; }
    else if( handler.nextbits("0000000000011011") ){ run = 31; level = 1; }

    if( !escape && handler.READBIT() ) level = -level;
    if( isNext )
    {
        _idx = _idx + run + 1;
        _dct_zz[_zigzag[_idx]] = level;
    }
    else
    {
        _idx = run;
        _dct_zz[_zigzag[_idx]] = level;
    }
    if( DEBUG )
    {
        if( isNext) std::cout << "        dct_coeff_next: " << (escape?"ESCAPE ":"") << "Run: " << run <<" Level: " << level << std::endl;
        else std::cout << "        dct_coeff_first: " << (escape?"ESCAPE ":"") << "Run: " << run <<" Level: " << level << std::endl;
    }
}


int motion_code(DataHandler& handler)
{
    int code = 0;
    if( handler.nextbits("00000011001") ) code = -16;
    else if( handler.nextbits("00000011011") ) code = -15;
    else if( handler.nextbits("00000011101") ) code = -14;
    else if( handler.nextbits("00000011111") ) code = -13;
    else if( handler.nextbits("00000100001") ) code = -12;
    else if( handler.nextbits("00000100011") ) code = -11;
    else if( handler.nextbits("0000010011") ) code = -10;
    else if( handler.nextbits("0000010101") ) code = -9;
    else if( handler.nextbits("0000010111") ) code = -8;
    else if( handler.nextbits("00000111") ) code = -7;
    else if( handler.nextbits("00001001") ) code = -6;
    else if( handler.nextbits("00001011") ) code = -5;
    else if( handler.nextbits("0000111") ) code = -4;
    else if( handler.nextbits("00011") ) code = -3;
    else if( handler.nextbits("0011") ) code = -2;
    else if( handler.nextbits("011") ) code = -1;
    else if( handler.nextbits("1") ) code = 0;
    else if( handler.nextbits("010") ) code = 1;
    else if( handler.nextbits("0010") ) code = 2;
    else if( handler.nextbits("00010") ) code = 3;
    else if( handler.nextbits("0000110") ) code = 4;
    else if( handler.nextbits("00001010") ) code = 5;
    else if( handler.nextbits("00001000") ) code = 6;
    else if( handler.nextbits("00000110") ) code = 7;
    else if( handler.nextbits("0000010110") ) code = 8;
    else if( handler.nextbits("0000010100") ) code = 9;
    else if( handler.nextbits("0000010010") ) code = 10;
    else if( handler.nextbits("00000100010") ) code = 11;
    else if( handler.nextbits("00000100000") ) code = 12;
    else if( handler.nextbits("00000011110") ) code = 13;
    else if( handler.nextbits("00000011100") ) code = 14;
    else if( handler.nextbits("00000011010") ) code = 15;
    else if( handler.nextbits("00000011000") ) code = 16;
    return code;
}


int coded_block_pattern(DataHandler& handler)
{
    int cdp = 0;
    if( handler.nextbits("111") ) cdp = 60;
    else if( handler.nextbits("1101") ) cdp = 4;
    else if( handler.nextbits("1100") ) cdp = 8;
    else if( handler.nextbits("1011") ) cdp = 16;
    else if( handler.nextbits("1010") ) cdp = 32;
    else if( handler.nextbits("10011") ) cdp = 12;
    else if( handler.nextbits("10010") ) cdp = 48;
    else if( handler.nextbits("10001") ) cdp = 20;
    else if( handler.nextbits("10000") ) cdp = 40;
    else if( handler.nextbits("01111") ) cdp = 28;
    else if( handler.nextbits("01110") ) cdp = 44;
    else if( handler.nextbits("01101") ) cdp = 52;
    else if( handler.nextbits("01100") ) cdp = 56;
    else if( handler.nextbits("01011") ) cdp = 1;
    else if( handler.nextbits("01010") ) cdp = 61;
    else if( handler.nextbits("01001") ) cdp = 2;
    else if( handler.nextbits("01000") ) cdp = 62;
    else if( handler.nextbits("001111") ) cdp = 24;
    else if( handler.nextbits("001110") ) cdp = 36;
    else if( handler.nextbits("001101") ) cdp = 3;
    else if( handler.nextbits("001100") ) cdp = 63;
    else if( handler.nextbits("0010111") ) cdp = 5;
    else if( handler.nextbits("0010110") ) cdp = 9;
    else if( handler.nextbits("0010101") ) cdp = 17;
    else if( handler.nextbits("0010100") ) cdp = 33;
    else if( handler.nextbits("0010011") ) cdp = 6;
    else if( handler.nextbits("0010010") ) cdp = 10;
    else if( handler.nextbits("0010001") ) cdp = 18;
    else if( handler.nextbits("0010000") ) cdp = 34;
    else if( handler.nextbits("00011111") ) cdp = 7;
    else if( handler.nextbits("00011110") ) cdp = 11;
    else if( handler.nextbits("00011101") ) cdp = 19;
    else if( handler.nextbits("00011100") ) cdp = 35;
    else if( handler.nextbits("00011011") ) cdp = 13;
    else if( handler.nextbits("00011010") ) cdp = 49;
    else if( handler.nextbits("00011001") ) cdp = 21;
    else if( handler.nextbits("00011000") ) cdp = 41;
    else if( handler.nextbits("00010111") ) cdp = 14;
    else if( handler.nextbits("00010110") ) cdp = 50;
    else if( handler.nextbits("00010101") ) cdp = 22;
    else if( handler.nextbits("00010100") ) cdp = 42;
    else if( handler.nextbits("00010011") ) cdp = 15;
    else if( handler.nextbits("00010010") ) cdp = 51;
    else if( handler.nextbits("00010001") ) cdp = 23;
    else if( handler.nextbits("00010000") ) cdp = 43;
    else if( handler.nextbits("00001111") ) cdp = 25;
    else if( handler.nextbits("00001110") ) cdp = 37;
    else if( handler.nextbits("00001101") ) cdp = 26;
    else if( handler.nextbits("00001100") ) cdp = 38;
    else if( handler.nextbits("00001011") ) cdp = 29;
    else if( handler.nextbits("00001010") ) cdp = 45;
    else if( handler.nextbits("00001001") ) cdp = 53;
    else if( handler.nextbits("00001000") ) cdp = 57;
    else if( handler.nextbits("00000111") ) cdp = 30;
    else if( handler.nextbits("00000110") ) cdp = 46;
    else if( handler.nextbits("00000101") ) cdp = 54;
    else if( handler.nextbits("00000100") ) cdp = 58;
    else if( handler.nextbits("000000111") ) cdp = 31;
    else if( handler.nextbits("000000110") ) cdp = 47;
    else if( handler.nextbits("000000101") ) cdp = 55;
    else if( handler.nextbits("000000100") ) cdp = 59;
    else if( handler.nextbits("000000011") ) cdp = 27;
    else if( handler.nextbits("000000010") ) cdp = 39;
    return cdp;
}

#endif // define __VLC_H__
