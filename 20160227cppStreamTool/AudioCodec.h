#ifndef __m3u8_monitor__AudioCodec__
#define __m3u8_monitor__AudioCodec__

#include <stdint.h>
#include <stdio.h>
#include <iostream>


struct AudioCodecHeader {
    /*
     Format of SoundData. The following values are defined:
     0 = Linear PCM, platform endian
     1 = ADPCM
     2 = MP3
     3 = Linear PCM, little endian
     4 = Nellymoser 16 kHz mono
     5 = Nellymoser 8 kHz mono
     6 = Nellymoser
     7 = G.711 A-law logarithmic PCM
     8 = G.711 mu-law logarithmic PCM
     9 = reserved
     10 = AAC
     11 = Speex
     14 = MP3 8 kHz
     15 = Device-specific sound
     Formats 7, 8, 14, and 15 are reserved.
     AAC is supported in Flash Player 9,0,115,0 and higher.
     Speex is supported in Flash Player 10 and higher.
     */
    uint8_t soundFormat;
    
    /*
     Sampling rate. The following values are defined:
     0 = 5.5 kHz
     1 = 11 kHz
     2 = 22 kHz
     3 = 44 kHz
     */
    uint8_t soundRate;
    
    /*
     Size of each audio sample. This parameter only pertains to
     uncompressed formats. Compressed formats always decode
     to 16 bits internally.
     0 = 8-bit samples
     1 = 16-bit samples
     */
    uint8_t soundSize;
    
    /*
     Mono or stereo sound
     0 = Mono sound
     1 = Stereo sound
     */
    uint8_t soundType;
    
    /*
     The following values are defined:
     0 = AAC sequence header
     1 = AAC raw
     
     当soundFormat=10时该字段有效
     */
    uint8_t AACPacketType;
};


class AudioCodec {
public:
    
    static int parse_audio_header(AudioCodecHeader *header, uint8_t *data, uint32_t len)
    {
        uint32_t leftLen = len;
        
        if(leftLen < 1) { return -1; }
        header->soundFormat = (data[0] & 0xF0) >> 4;
        header->soundRate = (data[0] & 0x0C) >> 2;
        header->soundSize = (data[0] & 0x02) >> 1;
        header->soundType = data[0] & 0x01;
        leftLen--;
        
//        for(int i = 0; i < len; i++) {
//            printf("%02x", data[i]);
//        }
//        
//        printf("\n");
        const char *audioCodec = "未知";
        switch (header->soundFormat) {
            case 0:
                audioCodec = "Linear PCM, platform endian";
                break;
            case 1:
                audioCodec = "ADPCM";
                break;
            case 2:
                audioCodec = "MP3";
                break;
            
            case 3:
                audioCodec = "Linear PCM, little endian";
                break;
                
            case 4:
                audioCodec = "Nellymoser 16 kHz mono";
                break;
                
            case 5:
                audioCodec = "Nellymoser 8 kHz mono";
                break;
                
            case 6:
                audioCodec = "Nellymoser";
                break;
                
            case 7:
                audioCodec = "G.711 A-law logarithmic PCM";
                break;
                
            case 8:
                audioCodec = "G.711 mu-law logarithmic PCM";
                break;
                
            case 10:
                audioCodec = "AAC";
                break;
                
            case 11:
                audioCodec = "Speex";
                break;
                
            case 14:
                audioCodec = "MP3 8 kHz";
                break;
                
            case 15:
                audioCodec = "Device-specific sound";
                break;
                
            default:
                break;
        }
        
        if(10 == header->soundFormat) {
            if(leftLen < 1) { return -1; }
            header->AACPacketType = data[1];
            leftLen--;

            if(0 == header->AACPacketType) { //该音频帧为AAC Sequence Header
                /*
                 AAC Sequence Header结构
                 http://blog.csdn.net/jwybobo2007/article/details/9221657
                 */
                struct AACSequenceHeader {
                    uint8_t audioObjectType; //编码结构类型
                    uint8_t samplingFrequencyIndex; //音频采样率索引0->96000, 1->88200, 2->64000, 3->48000, 4->44100, 5->32000, 6->24000, 7->22050, 8->16000
                    uint8_t channelConfiguration; //音频输出声道数
                    uint8_t frameLengthFlag;
                    uint8_t dependsOnCoreCoder;
                    uint8_t extensionFlag;
                }aacHeader;
                
                if(leftLen < 2) { return -1; }
                leftLen -= 2;
                
                aacHeader.audioObjectType = (data[2] >> 3) & 0x1F;
                aacHeader.samplingFrequencyIndex = ((data[2] & 0x07) << 1) | ((data[3] >> 7) & 0x01);
                aacHeader.channelConfiguration = (data[3] >> 3) & 0x0F;
                aacHeader.frameLengthFlag = (data[3] >> 2) & 0x01;
                aacHeader.dependsOnCoreCoder = (data[3] >> 1) & 0x01;
                aacHeader.extensionFlag = data[3] & 0x01;
                
                
                uint32_t samplingFrequency[] = {96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050, 16000};
                
                printf("\n/*---音频AAC Sequence Header---\n"
                       "编码结构类型: %u\n"
                       "音频采样率: %u\n"
                       "声道数: %u\n"
                       "------------------------------*/\n",
                    aacHeader.audioObjectType,
                    samplingFrequency[aacHeader.samplingFrequencyIndex],
                    aacHeader.channelConfiguration);
            } else {
                
            }
        }
        
        static int showAudioCodec = 0;
        
        if(!showAudioCodec) {
            showAudioCodec = 1;
            printf("音频编码类型%s.\n", audioCodec);
        }
        
        return 0;
    }

};

#endif /* defined(__m3u8_monitor__AudioCodec__) */
