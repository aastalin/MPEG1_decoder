#ifndef __DECODER_H__
#define __DECODER_H__

#include <vector>
#define DEBUG 0

typedef unsigned char u8;

enum STARTCODE
{
    PictureStart,
    UserData,
    SequenceHeader,
    SequenceError,
    Extension,
    SequenceEnd,
    GroupStart
};


class DataHandler
{

public:
    DataHandler(char* filename);
    void next_start_code();
    bool nextbits(std::string A);

    u8        READBYTE();
    bool      READBIT();
    size_t    READUINT(size_t num);
    STARTCODE READCODE();

    void PUSHDATA(u8 data);

private:
    void NEXTDATA();

private:
    std::ifstream _obj;
    std::vector<u8> _buf;

    size_t _ptr;
    u8     _byte;
};

double _IDCT[64][64] = {0};

size_t _zigzag[64] = {  0,  1,  8, 16,  9,  2,  3, 10,
                       17, 24, 32, 25, 18, 11,  4,  5,
                       12, 19, 26, 33, 40, 48, 41, 34,
                       27, 20, 13,  6,  7, 14, 21, 28,
                       35, 42, 49, 56, 57, 50, 43, 36,
                       29, 22, 15, 23, 30, 37, 44, 51,
                       58, 59, 52, 45, 38, 31, 39, 46,
                       53, 60, 61, 54, 47, 55, 62, 63};

size_t _intraQ[8][8] = {{  8, 16, 19, 22, 26, 27, 29, 34},
                        { 16, 16, 22, 24, 27, 29, 34, 37},
                        { 19, 22, 26, 27, 29, 34, 34, 38},
                        { 22, 22, 26, 27, 29, 34, 37, 40},
                        { 22, 26, 27, 29, 32, 35, 40, 48},
                        { 26, 27, 29, 32, 35, 40, 48, 58},
                        { 26, 27, 29, 34, 38, 46, 56, 69},
                        { 27, 29, 35, 38, 46, 56, 69, 83}};

size_t _non_intraQ[8][8] = {{ 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16},
                            { 16, 16, 16, 16, 16, 16, 16, 16}};

bool _closed_gop;

size_t _x_ptr;
size_t _y_ptr;

size_t _horiz;
size_t _verti;
size_t _h_block;
size_t _v_block;

u8 _pos;
size_t _vbv_size;
size_t _vbv_delay;
size_t _cbp;
size_t _type;
size_t _qscale;

int _forward_horiz;
int _forward_verti;
int _forward_horiz_r;
int _forward_verti_r;

int complement_horizontal_forward_r;
int complement_vertical_forward_r;

int recon_right_for;
int recon_down_for;
int recon_right_for_prev;
int recon_down_for_prev;

bool full_pel_forward_vector;
int  forward_f;
int  forward_r_size;

int _dct_dc_y_past;
int _dct_dc_cb_past;
int _dct_dc_cr_past;

int _y[16][16];
int _cb[8][8];
int _cr[8][8];

int _num;
int _macroblock_address;
int _past_macroblock_address;

#endif // define __DECODER_H__
