#ifndef _RENDEZVUE_CHECKDATA_H_
#define _RENDEZVUE_CHECKDATA_H_

#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdint.h>

#define HEAD_LENGTH		4
#define TAIL_LENGTH		4

#ifndef IMAGE_WIDTH_VIA_ETH
#define IMAGE_WIDTH_VIA_ETH 640
#endif

#ifndef IMAGE_HEIGHT_VIA_ETH
#define IMAGE_HEIGHT_VIA_ETH 480
#endif

class  CRendezvueCheckData
{
public:
	CRendezvueCheckData(void);
	~CRendezvueCheckData(void);

public:
	unsigned char* FindData(unsigned char* input_buf, int input_size, int* out_size);
    void init_variable(void) ;

private:
	int m_tmp_size;
    unsigned char* m_tmp_buf;
	int m_tmp_index;

	int m_head_find_count;
    unsigned char m_head_string[HEAD_LENGTH];
	int m_tail_find_count ;
    unsigned char m_tail_string[TAIL_LENGTH] ;
};

#endif
