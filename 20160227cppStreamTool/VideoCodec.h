#ifndef __m3u8_monitor__VideoCodec__
#define __m3u8_monitor__VideoCodec__


#include "Utils.h"
#include <stdint.h>
#include <stdio.h>
#include <iostream>


/*
 Type of video frame. The following values are defined:
 1 = key frame (for AVC, a seekable frame)
 2 = inter frame (for AVC, a non-seekable frame)
 3 = disposable inter frame (H.263 only)
 4 = generated key frame (reserved for server use only)
 5 = video info/command frame
 */
enum VideoCodecFrameType {
    KEY_FRAME = 0x01,
    INTER_FRAME,
    DISPOSABLE_INTER_FRAME,
    GENERATED_KEY_FRAME,
    COMMAND_FRAME
};

/*
 Codec Identifier. The following values are defined:
 2 = Sorenson H.263
 3 = Screen video
 4 = On2 VP6
 5 = On2 VP6 with alpha channel
 6 = Screen video version 2
 7 = AVC
 */

/*
 If CodecID == 2 H263VIDEOPACKET
 If CodecID == 3 SCREENVIDEOPACKET
 If CodecID == 4 VP6FLVVIDEOPACKET
 If CodecID == 5 VP6FLVALPHAVIDEOPACKET
 If CodecID == 6 SCREENV2VIDEOPACKET
 if CodecID == 7 AVCVIDEOPACKET
 */
enum VideoCodecID {
    H263VIDEOPACKET = 0x02,
    SCREENVIDEOPACKET,
    VP6FLVVIDEOPACKET,
    VP6FLVALPHAVIDEOPACKET,
    SCREENV2VIDEOPACKET,
    AVCVIDEOPACKET
};


/*
 The following values are defined:
 0 = AVC sequence header
 1 = AVC NALU
 2 = AVC end of sequence (lower level NALU sequence ender is not required or supported)
 */
enum VideoCodecAVCType {
    AVC_SEQUENCE_HEADER,
    AVC_NALU,
    AVC_SEQUENCE_END
};


struct VideoCodecHeader {
    
    VideoCodecFrameType frameType;
    VideoCodecID codecID;
    
    struct { //本字段在codecID=7(AVCVIDEOPACKET)时有效
        
        VideoCodecAVCType AVCPacketType;
        /*
         IF AVCPacketType == 1
         Composition time offset
         ELSE
         0
         See ISO 14496-12, 8.15.3 for an explanation of composition
         times. The offset in an FLV file is always in milliseconds.
         */
        int compositionTime;
        
    }avcVideoPacket;
};


class VideoCodec {
public:
    
    static int parse_video_header(VideoCodecHeader *header, uint8_t *data, uint32_t len)
    {
        uint8_t *p = data;
        uint32_t leftLen = len;
        
        if(leftLen < 1) { return -1; }
        
        header->frameType = (VideoCodecFrameType)((*p & 0xF0) >> 4);
        header->codecID = (VideoCodecID)(*p & 0x0F);
        p++;
        leftLen--;
        
        if(AVCVIDEOPACKET == header->codecID) {
            
            if(leftLen < 1) { return -1; }
            
            header->avcVideoPacket.AVCPacketType = (VideoCodecAVCType)*p++;
            leftLen--;
            
            switch (header->avcVideoPacket.AVCPacketType) {
                
                case AVC_SEQUENCE_HEADER: { //0 = AVC sequence header
                    header->avcVideoPacket.compositionTime = 0;
                    
                    struct AVCDecoderConfigurationRecord {
                        uint8_t configurationVersion;
                        uint8_t AVCProfileIndication;
                        uint8_t profileCompatilbility;
                        uint8_t AVCLevelIndication;
                        uint8_t reserved6; //111111
                        uint8_t lengthSizeMinusOne;
                        uint8_t reserved3; //111
                        uint8_t numOfSequenceParameterSets;
                        uint8_t numOfPictureParameterSets;
                    }avcHeader;
                    
                    if(leftLen < 9) { return -1; }
                    
                    p += 3;
                    leftLen -= 9;
                    
                    avcHeader.configurationVersion = *p++;
                    avcHeader.AVCLevelIndication = *p++;
                    avcHeader.profileCompatilbility = *p++;
                    avcHeader.AVCLevelIndication = *p++;
                    avcHeader.reserved6 = *p >> 2;
                    if(avcHeader.reserved6 != 0x3F) {
                        printf("avc sequence header error.\n");
                    }
                    avcHeader.lengthSizeMinusOne = (*p++ & 0x03) + 1;
                    avcHeader.reserved3 = *p >> 5;
                    
                    if(avcHeader.reserved3 != 0x07) {
                        printf("avc sequence header error.\n");
                    }
                    avcHeader.numOfSequenceParameterSets = *p++ & 0x1F;
                    
                    for(int i = 0; i < avcHeader.numOfSequenceParameterSets; i++) {
                        uint16_t spsSize;
                        
                        if(leftLen < 2) { return -1; }
                        Utils::big_endian_to_local((uint8_t*)&spsSize, p, 2, 2);
                        p += 2;
                        leftLen -= 2;
                        
                        if(leftLen < spsSize) { return -1; }
                        p += spsSize;
                        leftLen -= spsSize;
                    }
                    
                    if(leftLen < 1) { return -1; }
                    avcHeader.numOfPictureParameterSets = *p++;
                    leftLen--;
                    
                    for(int i = 0; i < avcHeader.numOfPictureParameterSets; i++) {
                        uint16_t ppsSize;
                        
                        if(leftLen < 2) { return -1; }
                        Utils::big_endian_to_local((uint8_t*)&ppsSize, p, 2, 2);
                        p += 2;
                        leftLen -= 2;
                        
                        if(leftLen < ppsSize) { return -1; }
                        p += ppsSize;
                        leftLen -= ppsSize;
                    }
                    
                    printf("\n/*---视频AVC Sequence Header---\n"
                           "-----------------------------*/\n");

                }break;
                    
                case AVC_NALU: //1 = AVC NALU
                    break;
                    
                case AVC_SEQUENCE_END: //2 = AVC end of sequence
                    header->avcVideoPacket.compositionTime = 0;
                    break;
            }
        }
        
        return 0;
    }
};

#endif /* defined(__m3u8_monitor__VideoCodec__) */
