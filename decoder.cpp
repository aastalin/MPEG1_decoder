#include <iostream>
#include <fstream>
#include <math.h>
#include "assert.h"
#include "string.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "decoder.h"
#include "vlc.h"

#define PI 3.14159265359

cv::Mat image;
cv::Mat estim;
cv::Mat refer;

/* Data handler */
DataHandler::DataHandler(char* filename)
        : _ptr(7)
{
    _obj.open(filename, std::ios::binary);

    if( !_obj.is_open() )
    {
        std::cerr << "[ERROR] cannot open file " << filename << std::endl;
        exit(-1);
    }
    _buf.clear();
}

void DataHandler::next_start_code()
{   
    _ptr = 7;
    while( !nextbits("000000000000000000000001") )
    {
        READBYTE();
    }
}

bool DataHandler::nextbits(std::string A)
{
    u8 byte = _byte;
    size_t ptr = _ptr;
    std::string B = "";

    size_t idx=0;
    while( B.size()<A.size() )
    {
        if( ptr>6 )
        {
            if( idx>=_buf.size() )
            {
                if( _obj.eof() ) exit(-1);
                _obj.read( (char*)&byte, 1);
                _buf.push_back(byte);
                idx += 1;
            }
            else
            {
                byte = _buf[idx];
                idx += 1;
            }
            ptr = 0;
        }
        else
        {
            ptr += 1;
        }
        B.append(((1<<(7-ptr))&byte)==0 ? "0":"1");
    }
    if( B.compare(A)==0 )
    {
        READUINT(A.size());
        return true;
    }
    else return false;
}
/* read from obj */
void DataHandler::NEXTDATA()
{
    if( _buf.size()==0 )
    {
        _obj.read( (char*)&_byte, 1);
    }
    else
    {
        _byte = _buf[0];
        _buf.erase(_buf.begin()); //pop front
    }
}
/* read one byte */
u8 DataHandler::READBYTE()
{
    assert(_ptr==7);

    NEXTDATA();
    _ptr = 7;
    return _byte;
}
/* read one bit */
bool DataHandler::READBIT()
{
    if( _ptr>6 )   // get a new byte
    {
        NEXTDATA();
        _ptr = 0;
    }
    else
    {
        _ptr += 1;
    }
    return ((1<<(7-_ptr))&_byte)==0 ? 0 : 1;
}
/* read unsigned int */
size_t DataHandler::READUINT(size_t num)
{
    size_t data = 0;
    for( size_t i=0; i<num; i++ )
    {
        if( READBIT() ) data = 2*data+1;
        else data = 2*data;
    }
    return data;
}
/* read start code */
STARTCODE DataHandler::READCODE()
{
    u8 byte = READBYTE();

    STARTCODE code;
    switch( byte )
    {
        case 0x00:
            code = PictureStart;
            break;
        case 0xb2:
            code = UserData;
            break;
        case 0xb3:
            code = SequenceHeader;
            break;
        case 0xb4:
            code = SequenceError;
            break;
        case 0xb5:
            code = Extension;
            break;
        case 0xb7:
            code = SequenceEnd;
            break;
        case 0xb8:
            code = GroupStart;
            break;
        default:
            std::cerr << "[ERROR] incorrect start code" << std::endl;
            exit(-1);
    }

    return code;
}
/* helping function */
void DataHandler::PUSHDATA(u8 data)
{
    _buf.push_back(data);
}



/* Initialize */
void build_IDCT_table()
{
    // IDCT coefficient
    for( size_t x=0; x<8; x++ ) {
    for( size_t y=0; y<8; y++ ) {
        for( size_t u=0; u<8; u++ ) {
        for( size_t v=0; v<8; v++ ) {
            float CuCv = 1;
            if( u==0 && v==0 ) CuCv = 0.5;
            else if( u==0 || v==0 ) CuCv = 0.70710678118;

            _IDCT[x*8+y][u*8+v] = CuCv * cos(double(2*x+1)*u*PI/16) * cos(double(2*y+1)*v*PI/16);
        }}
    }}
}



/* Parsing */
void readExtensionAndUserData(DataHandler& handler)
{
    if( handler.nextbits("10110101") )  // extension_start_code
    {
        std::cout << "[TODO] add the handler here" << std::endl;
    }

    if( handler.nextbits("10110010") )  // user_data_start_code
    {
        std::cout << "[TODO] add the handler here" << std::endl;
    }
}
/* find ME */
void calculateME()
{
    int right_for = recon_right_for >> 1;
    int down_for  = recon_down_for  >> 1;
    int right_half_for = recon_right_for-2*right_for;
    int down_half_for  = recon_down_for-2*down_for;

    // pe = pe_past
    int x_ptr = _x_ptr*16;
    int y_ptr = _y_ptr*16;

    for( int m=0; m<16; m++ ) {
    for( int n=0; n<16; n++ ) {

        int x = std::max( std::min( x_ptr + m + right_for, int(_horiz)-1 ), 0);
        int y = std::max( std::min( y_ptr + n + down_for,  int(_verti)-1 ), 0);

        if( !right_half_for &&  down_half_for && (y+1)<_verti )
        {
            cv::Vec3b d1 = refer.at<cv::Vec3b>(y  ,x);
            cv::Vec3b d2 = refer.at<cv::Vec3b>(y+1,x);
            cv::Vec3b d = {u8((int(d1[0])+int(d2[0]))/2), u8((int(d1[1])+int(d2[1]))/2), u8((int(d1[2])+int(d2[2]))/2)};
            estim.at<cv::Vec3b>(n,m) = d;
        }
        else if(  right_half_for && !down_half_for && (x+1)<_horiz )
        {
            cv::Vec3b d1 = refer.at<cv::Vec3b>(y,x);
            cv::Vec3b d2 = refer.at<cv::Vec3b>(y,x+1);
            cv::Vec3b d = {u8((int(d1[0])+int(d2[0]))/2), u8((int(d1[1])+int(d2[1]))/2), u8((int(d1[2])+int(d2[2]))/2)};
            estim.at<cv::Vec3b>(n,m) = d;
        }
        else if(  right_half_for &&  down_half_for && (y+1)<_verti && (x+1)<_horiz )
        {
            cv::Vec3b d1 = refer.at<cv::Vec3b>(y  ,x);
            cv::Vec3b d2 = refer.at<cv::Vec3b>(y+1,x);
            cv::Vec3b d3 = refer.at<cv::Vec3b>(y  ,x+1);
            cv::Vec3b d4 = refer.at<cv::Vec3b>(y+1,x+1);
            cv::Vec3b d = {u8((int(d1[0])+int(d2[0])+int(d3[0])+int(d4[0]))/4),
                           u8((int(d1[1])+int(d2[1])+int(d3[1])+int(d4[1]))/4),
                           u8((int(d1[2])+int(d2[2])+int(d3[2])+int(d4[2]))/4)};
            estim.at<cv::Vec3b>(n,m) = d;
        }
        else
        {
            estim.at<cv::Vec3b>(n,m) = refer.at<cv::Vec3b>(y,x);
        }
    }}
}
/* calcuate dct_recon and IDCT */
void reconstruction(size_t idx, bool pattern, bool intra)
{
    // dct_recon
    if( pattern==0 ) // pattern=0
    {
        for( size_t m=0; m<8; m++ ) {
        for( size_t n=0; n<8; n++ ) {
            _dct_recon[m][n] = 0;
        }}
    }
    else if( intra ) // I-frame
    {
        for( size_t m=0; m<8; m++ ) {
        for( size_t n=0; n<8; n++ ) {
            _dct_recon[m][n] = ( 2*_dct_zz[m*8+n]*_qscale*_intraQ[m][n] )/16;
            if( (_dct_recon[m][n]&1)==0 )
            {
                if( _dct_recon[m][n]>0 ) _dct_recon[m][n] -= 1;
                else if( _dct_recon[m][n]<0 ) _dct_recon[m][n] += 1;
            }
            _dct_recon[m][n] = std::max( std::min( _dct_recon[m][n], 2047), -2048);
        }}
        // y DC value
        _dct_recon[0][0] = _dct_zz[0]*8;
        switch( idx )
        {
            case 0:
                if( _num-_past_macroblock_address > 1 ) _dct_recon[0][0] = 128*8+_dct_recon[0][0];
                else _dct_recon[0][0] = _dct_dc_y_past + _dct_recon[0][0];
                _dct_dc_y_past = _dct_recon[0][0];
                break;
            case 4:
                if( _num-_past_macroblock_address > 1 ) _dct_recon[0][0] = 128*8+_dct_recon[0][0];
                else _dct_recon[0][0] = _dct_dc_cb_past + _dct_recon[0][0];
                _dct_dc_cb_past = _dct_recon[0][0];
                break;
            case 5:
                if( _num-_past_macroblock_address > 1 ) _dct_recon[0][0] = 128*8+_dct_recon[0][0];
                else _dct_recon[0][0] = _dct_dc_cr_past + _dct_recon[0][0];
                _dct_dc_cr_past = _dct_recon[0][0];
                break;
            default: //case1,2,3
                _dct_recon[0][0] += _dct_dc_y_past;
                _dct_dc_y_past = _dct_recon[0][0];
                break;
        }
    }
    else if( _type==2 )  // P-frame
    {
        for( size_t m=0; m<8; m++ ) {
        for( size_t n=0; n<8; n++ ) {
            if( _dct_zz[m*8+n]>=0 )
            {
                _dct_recon[m][n] = ( (2*_dct_zz[m*8+n]+1)*_qscale*_non_intraQ[m][n] )/16;
            }
            else
            {
                _dct_recon[m][n] = ( (2*_dct_zz[m*8+n]-1)*_qscale*_non_intraQ[m][n] )/16;
            }
            if( (_dct_recon[m][n]&1)==0 )
            {
                if( _dct_recon[m][n]>0 ) _dct_recon[m][n] -= 1;
                else if( _dct_recon[m][n]<0 ) _dct_recon[m][n] += 1;
            }
            _dct_recon[m][n] = std::max( std::min( _dct_recon[m][n], 2047), -2048);
            if( _dct_zz[m*8+n]==0 ) _dct_recon[m][n] = 0;
        }}
    }

    // IDCT
    size_t x_offset = 0;
    size_t y_offset = 0;

    if( idx==1 || idx==3 ) y_offset += 8;
    if( idx==2 || idx==3 ) x_offset += 8;

    for( size_t m=0; m<8; m++ ) {
    for( size_t n=0; n<8; n++ ) {
        double val = 0;
        for( size_t u=0; u<8; u++ ) {
        for( size_t v=0; v<8; v++ ) {
            val += double(_dct_recon[u][v]) * _IDCT[m*8+n][u*8+v];
        }}
        int v = std::min(std::max(int(std::floor(0.25*val+0.49)),0),255);
        if( idx<4 ) _y[x_offset+m][y_offset+n] = v;
        else if( idx==4 ) _cb[m][n] = v;
        else if( idx==5 ) _cr[m][n] = v;
    }}
}
/* YCbCr to BGR */
void fillRGB(bool intraCoded, bool skip)
{
    for( size_t m=0; m<16; m++ ) {
    for( size_t n=0; n<16; n++ ) {
        float YY = float(_y[n][m]);
        float Cb = float(_cb[n/2][m/2]);
        float Cr = float(_cr[n/2][m/2]);

        if( intraCoded )    // I-frame
        {
            u8 b = std::min(std::max(int( YY +    1.772*(Cb-128) ), 0), 255) ;
            u8 g = std::min(std::max(int( YY - 0.344136*(Cb-128) - 0.714136*(Cr-128) ), 0),255);
            u8 r = std::min(std::max(int( YY +    1.402*(Cr-128) ), 0), 255) ;
            
            cv::Vec3b c = {b, g, r};
            image.at<cv::Vec3b>(_y_ptr*16+n,_x_ptr*16+m) = c;
        }
        else if( skip )    // skip
        {
            calculateME();
            cv::Vec3b me = estim.at<cv::Vec3b>(n,m);
            u8 b = me[0];
            u8 g = me[1];
            u8 r = me[2];

            cv::Vec3b c = {b, g, r};
            image.at<cv::Vec3b>(_y_ptr*16+n,_x_ptr*16+m) = c;
        }
        else               // motion
        {
            calculateME();
            cv::Vec3b me = estim.at<cv::Vec3b>(n,m);
            u8 b = std::min(std::max(int( YY +    1.772*Cb ), 0),255) + me[0];
            u8 g = std::min(std::max(int( YY - 0.344136*Cb - 0.714136*Cr ), 0),255) + me[1];
            u8 r = std::min(std::max(int( YY +    1.402*Cr ), 0), 255) +me[2];

            cv::Vec3b c = {b, g, r};
            image.at<cv::Vec3b>(_y_ptr*16+n,_x_ptr*16+m) = c;
        }
    }}
}
/* block layer */
void block(DataHandler& handler, bool pattern, bool intra, size_t idx)
{
    if( pattern )
    {
        for( size_t i=0; i<64; i++ ) _dct_zz[i] = 0;
        if( DEBUG ) std::cout << "    ==BLOCK(" << idx << ")==" << std::endl;
        if( intra )
        {
            size_t dc_size = 0;
            if( idx<4 )   // luminance
            {
                dc_size = dct_dc_size_luminance(handler);
                if( DEBUG ) std::cout << "        dct_dc_size_luminance:" << dc_size << std::endl;

                if( dc_size>0 )
                {
                    size_t dct_dc_differential = handler.READUINT(dc_size);

                    if ( dct_dc_differential & ( 1 << (dc_size-1)) ) _dct_zz[0] = dct_dc_differential;
                    else _dct_zz[0] = ( -1 << (dc_size) ) | (dct_dc_differential+1) ;

                    if( DEBUG ) std::cout << "        dct_dc_differential_luminance:" << dct_dc_differential << std::endl;
                }
            }
            else        // chrominance
            {
                dc_size = dct_dc_size_chrominance(handler);
                if( DEBUG ) std::cout << "        dct_dc_size_chrominance:" << dc_size << std::endl;

                if( dc_size>0 )
                {
                    size_t dct_dc_differential = handler.READUINT(dc_size);

                    if ( dct_dc_differential & ( 1 << (dc_size-1)) ) _dct_zz[0] = dct_dc_differential;
                    else _dct_zz[0] = ( -1 << (dc_size) ) | (dct_dc_differential+1) ;

                    if( DEBUG ) std::cout << "        dct_dc_differential_chrominance:" << dct_dc_differential << std::endl;
                }
            }
        }
        else
        {
            dct_coeff_next(handler, false);
        }

        if( _type!=4 )
        {
            _idx = 0;
            while( !handler.nextbits("10") )
            {
                dct_coeff_next(handler, true);  // vlc data
            }
         }
         if( DEBUG ) std::cout << "        EOB" << std::endl;
    }
    reconstruction(idx, pattern, intra);
}
/* motion vector */
void motion_estimation()
{
    if( forward_f==1 || _forward_horiz==0 )
    {
        complement_horizontal_forward_r = 0;
    }
    else
    {
        complement_horizontal_forward_r = forward_f-1-_forward_horiz_r;
    }

    if( forward_f==1 || _forward_verti==0 )
    {

        complement_vertical_forward_r = 0;
    }
    else
    {
        complement_vertical_forward_r = forward_f-1-_forward_verti_r;
    }

    int right_little, right_big;
    right_little = _forward_horiz * forward_f;
    if( right_little==0 )
    {
        right_big = 0;
    }
    else
    {
        if( right_little>0 )
        {
            right_little = right_little - complement_horizontal_forward_r ;
            right_big = right_little - 32 * forward_f;
        }
        else
        {
            right_little = right_little + complement_horizontal_forward_r ;
            right_big = right_little + 32 * forward_f;
        }
    }

    int down_little, down_big;
    down_little = _forward_verti * forward_f;
    if( down_little==0 )
    {
        down_big = 0;
    }
    else
    {
        if( down_little>0 )
        {
            down_little = down_little - complement_vertical_forward_r;
            down_big = down_little - 32 * forward_f;
        }
        else
        {
            down_little = down_little + complement_vertical_forward_r;
            down_big = down_little + 32 * forward_f;
        }
    }

    int maxV = ( 16 * forward_f ) -1;
    int minV = (-16 * forward_f );
    int new_vector = recon_right_for_prev + right_little;
    if( new_vector<=maxV && new_vector>= minV )
    {
        recon_right_for = recon_right_for_prev + right_little;
    }
    else
    {
        recon_right_for = recon_right_for_prev + right_big;
    }
    recon_right_for_prev = recon_right_for;
    if( full_pel_forward_vector ) recon_right_for = recon_right_for << 1;

    new_vector = recon_down_for_prev + down_little;
    if( new_vector<=maxV && new_vector>=minV )
    {
        recon_down_for = recon_down_for_prev + down_little;
    }
    else
    {
        recon_down_for = recon_down_for_prev + down_big;
    }
    recon_down_for_prev = recon_down_for;
    if( full_pel_forward_vector ) recon_down_for = recon_down_for << 1;
    if( DEBUG ) std::cout << "    ForMV " << recon_right_for << " " << recon_down_for << std::endl;
}
/* calcuate value for skip macro */
void skip(DataHandler& handler)
{
    for( size_t i=1; i<_macroblock_address; i++ )
    {
        _y_ptr = (_num+i) / _h_block;
        _x_ptr = (_num+i) % _h_block;

        size_t x_ptr = _x_ptr*16;
        size_t y_ptr = _y_ptr*16;

        for( size_t m=0; m<16; m++ ) {
        for( size_t n=0; n<16; n++ ) {
            estim.at<cv::Vec3b>(n,m) = image.at<cv::Vec3b>(y_ptr+n,x_ptr+m);
        }}
        fillRGB(false, true);
        _dct_dc_y_past = 1024;
        _dct_dc_cb_past = 1024;
        _dct_dc_cr_past = 1024;
        recon_right_for = 0;
        recon_down_for  = 0;
        recon_right_for_prev = 0;
        recon_down_for_prev  = 0;
    }
}

/* macroblock layer */
void macroblock(DataHandler& handler)
{
    while( handler.nextbits("00000001111") )   // macroblock_stuffing
    {
        handler.READUINT(11);
        std::cout << "[TODO] macroblock_stuffing" << std::endl;
    }

    while( handler.nextbits("00000001000") )   // macroblock_escape
    {
        handler.READUINT(11);
        std::cout << "[TODO] macroblock_escape" << std::endl;
    }

    _macroblock_address = macroblock_address_increment(handler);
    skip(handler);

    _num += _macroblock_address;
    if( DEBUG ) std::cout << "==MACROBLOCK(" << _num+1 << ")==" << std::endl;

    _y_ptr = _num / _h_block;
    _x_ptr = _num % _h_block;

    bool* param = macroblock_type(handler,_type);
    if( param[0] )   // macroblock_quant
    {
        _qscale = handler.READUINT(5);
        if( DEBUG ) std::cout << "    quantier_scale: " << _qscale << std::endl;
    }

    if( param[1] )   // macroblock_motion_forward
    {
        if( DEBUG ) std::cout << "    forward_f_code: "<< forward_r_size+1 << std::endl;

        _forward_horiz = motion_code(handler);
        if( DEBUG ) std::cout << "    motion_horizontal_forward_code: " << _forward_horiz << std::endl;

        if( (forward_f!=1)&&(_forward_horiz!=0) )
        {
            _forward_horiz_r = handler.READUINT(forward_r_size);
            if( DEBUG ) std::cout << "    motion_horizontal_forward_r: " << _forward_horiz_r << std::endl;
        }

        _forward_verti = motion_code(handler);
        if( DEBUG ) std::cout << "    motion_vertical_forward_code: " << _forward_verti << std::endl;

        if( (forward_f!=1)&&(_forward_verti!=0) )
        {
            _forward_verti_r = handler.READUINT(forward_r_size);
            if( DEBUG ) std::cout << "    motion_vertical_forward_r: " << _forward_verti_r << std::endl;
        }
        motion_estimation();
    }
    else
    {
        recon_right_for_prev = recon_down_for_prev = 0;
        recon_right_for = recon_down_for = 0;
    }

    if( param[2] )   // macroblock_motion_backward
    {   
        std::cout << "[TODO] add macroblock_motion_backward support" << std::endl;
    }

    _cbp = 0;
    if( param[3] )   // macroblock_pattern
    {
        _cbp = coded_block_pattern(handler);
        if( DEBUG ) std::cout << "    coded_block_pattern: " << _cbp << std::endl;
    }

    if( param[4] )   // macroblock_intra
    {
        recon_right_for_prev = recon_down_for_prev = 0;
        recon_right_for = recon_down_for = 0;
    }
    else
    {
        _dct_dc_y_past = 1024;
        _dct_dc_cb_past = 1024;
        _dct_dc_cr_past = 1024;
    }

    bool pattern_code[6];
    for( size_t i=0; i<6; i++ )
    {
        pattern_code[i] = 0;
        if( _cbp & (1<<(5-i)) ) pattern_code[i] = 1;
        if( param[4] ) pattern_code[i] = 1;   //macroblock_intra
    }
 
    for( size_t i=0; i<6; i++ )
    {
        block(handler, pattern_code[i], param[4], i);
    }

    fillRGB(param[4], false);
    if( param[4] ) _past_macroblock_address = _num;
}
/* slice layer */
void slice(DataHandler& handler)
{
    _qscale = handler.READUINT(5);   // quantizer_scale
    if( DEBUG ) std::cout << "quantizer_scale: " << _qscale << std::endl;

    while( handler.READBIT() ) // extra_bit_slice
    {
        std::cout << "[LOAD] extra information slice" << std::endl;
        handler.READUINT(8);
    }

    _num = -1;
    recon_right_for = 0;
    recon_down_for  = 0;
    recon_right_for_prev = 0;
    recon_down_for_prev  = 0;
    _past_macroblock_address = -2;

    do {

        macroblock(handler);
    } while( !handler.nextbits("00000000000000000000000") );
    handler.next_start_code();
}
/* picture layer */
void picture(DataHandler& handler)
{
    handler.READUINT(10);  // temporal_reference
    _type = handler.READUINT(3);

    _vbv_delay = handler.READUINT(16);   // vbv_delay
    if( DEBUG ) std::cout << "vbv_delay: " << _vbv_delay << std::endl;

    size_t forward_f_code = 3;
    switch( _type )
    {
        case 1:
            if( DEBUG ) std::cout << "[INFO] intra-coded (I)" << std::endl;
            forward_r_size = forward_f_code-1;
            forward_f = 1 << forward_r_size;
            full_pel_forward_vector = false;
            break;
        case 2:
            if( DEBUG ) std::cout << "[INFO] predictive-coded (P)" << std::endl;
            full_pel_forward_vector = handler.READBIT();
            forward_f_code = handler.READUINT(3);
            forward_r_size = forward_f_code-1;
            forward_f = 1 << forward_r_size;
            if( DEBUG ) std::cout << "    forward_f_code: " << forward_f_code << std::endl;
            break;
        case 3:
            if( DEBUG ) std::cout << "[INFO] bidirectionally-predictive-coded (B)" << std::endl;
            full_pel_forward_vector = handler.READBIT();
            forward_f_code = handler.READUINT(3);
            forward_r_size = forward_f_code-1;
            forward_f = 1 << forward_r_size;
            if( DEBUG ) std::cout << "    forward_f_code: " << forward_f_code << std::endl;
            break;
        default:
            std::cerr << "[ERROR] non supported type " << _type << std::endl;
            exit(-1);
    }

    while( handler.READBIT() ) // extra_bit_picture
    {
        std::cout << "[LOAD] extra information picture" << std::endl;
        handler.READUINT(8);
    }

    handler.next_start_code();
    readExtensionAndUserData(handler);

    _pos = handler.READBYTE();             // slice_vertical_positions
    assert(_pos>=0x01 && _pos<=0xAF);
    do {

        slice(handler);
        image.copyTo(refer);

        // display result
        cv::imshow("Video", image);
        cv::waitKey(30);

        _type = 2;
        _pos = handler.READBYTE();        // slice_vertical_positions
    } while( _pos>=0x01 && _pos<=0xAF );
    handler.PUSHDATA(_pos);
}
/* group of picture layer */
void group_of_pictures(DataHandler& handler)
{
    handler.READUINT(25);  // time_code
    _closed_gop = handler.READBIT();
    handler.READBIT();     // broken link

    handler.next_start_code();
    readExtensionAndUserData(handler);
    assert(handler.READCODE()==PictureStart);

    do {
        // reset pointer
        _x_ptr = 0;
        _y_ptr = 0;
        _dct_dc_y_past  = 0;
        _dct_dc_cb_past = 0;
        _dct_dc_cr_past = 0;
        _macroblock_address = 0;

        picture(handler);
    } while(handler.nextbits("00000000"));
}
/* sequence header */
void sequence_header(DataHandler& handler)
{
    _horiz = handler.READUINT(12);
    _verti = handler.READUINT(12);
    _h_block = (_horiz+15) / 16;
    _v_block = (_verti+15) / 16;

    image = cv::Mat(_verti, _horiz, CV_8UC3);    
    estim = cv::Mat(16, 16, CV_8UC3);

    handler.READUINT(27);   // pel_aspect_ratio, picture_rate, bit_rate, marker_bit
    _vbv_size = handler.READUINT(10);
    handler.READUINT(1);    // constrained_parameter_flag

    if( handler.READBIT() ) // build intra quantizer matrix
    {
        std::cout << "[LOAD] intra quantizer matrix" << std::endl;
        for( size_t m=0; m<8; m++ ) {
        for( size_t n=0; n<8; n++ ) {
            _intraQ[m][n] = handler.READUINT(8);
        }}
    }

    if( handler.READBIT() ) // build nonintra quantizer matrix
    {
        std::cout << "[LOAD] nonintra quantizer matrix" << std::endl;
        for( size_t m=0; m<8; m++ ) {
        for( size_t n=0; n<8; n++ ) {
            _non_intraQ[m][n] = handler.READUINT(8);
        }}
    }

    handler.next_start_code();
    readExtensionAndUserData(handler);
}
/* video sequence layer */
void video_sequence(DataHandler& handler)
{
    handler.next_start_code();
    assert(handler.READCODE()==SequenceHeader);
    do {
        sequence_header(handler);
        assert(handler.READCODE()==GroupStart);
        do {
            group_of_pictures(handler);
        } while( handler.nextbits("10111000") );
    } while( handler.nextbits("10110011") );
    assert(handler.READCODE()==SequenceEnd);
}



/* MAIN */
int main(int argc, char *argv[])
{
    if( argc<2 )  // check argument
    {
        std::cout << "Usage: " << argv[0] << " [input]" << std::endl;
        exit(-1);
    }
    std::cout << "MPEG-1 decoder written by D0792202 with file " << argv[1] << std::endl;

    // create window and prepare IDCT coefficient
    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
    build_IDCT_table();

    // start parsing
    DataHandler handler(argv[1]);
    video_sequence(handler);

    std::cout << "All done. Happy summer vacation!" << std::endl;
    return 0;
}
