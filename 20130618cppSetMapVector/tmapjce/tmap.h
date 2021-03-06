// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.0.0.20 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tmap.jce'
// **********************************************************************

#ifndef __TMAP_H_
#define __TMAP_H_

#include <map>
#include <string>
#include <vector>
#include "../jce/Jce.h"
#include "../pdu_header.h"
using namespace std;


struct pdu_protocol_header;

namespace TEST_JEC_MAP
{
    struct tmap : public taf::JceStructBase
    {
    public:
        static string className()
        {
            return "TEST_JEC_MAP.tmap";
        }
        static string MD5()
        {
            return "0e0fd6a0aa16c7b8f7dacb635d4be203";
        }
        tmap()
        {
        }
        void resetDefautlt()
        {
        }
        template<typename WriterT>
        void writeTo(taf::JceOutputStream<WriterT>& _os) const
        {
            _os.write(imap, 1);
        }
        template<typename ReaderT>
        void readFrom(taf::JceInputStream<ReaderT>& _is)
        {
            resetDefautlt();
            _is.read(imap, 1, true);
        }
        ostream& display(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.display(imap,"imap");
            return _os;
        }
        ostream& displaySimple(ostream& _os, int _level=0) const
        {
            taf::JceDisplayer _ds(_os, _level);
            _ds.displaySimple(imap, false);
            return _os;
        }
        int Encode(uint8_t * pui_buff, int32_t * pi_buff_len, pdu_protocol_header * pdu_header)
        {
            try
            {
                int i_len = pdu_header?*pi_buff_len:0;
                int i_ret = 0;
                if (pdu_header && (i_ret = ::pdu_header_pack1(pdu_header, pui_buff, (uint32_t *)&i_len)) != 0) { return i_ret; }

                taf::JceOutputStream<taf::BufferWriterBuff> os;
                os.setBuffer(reinterpret_cast<char *>(pui_buff + i_len), *pi_buff_len - i_len);
                this->writeTo(os);
                i_len = i_len + static_cast<int>(os.getLength());
                if (pdu_header && (i_ret = pdu_header_pack2(pui_buff, (uint32_t *)pi_buff_len, (uint32_t *)&i_len)) != 0) { return i_ret; }
                *pi_buff_len = i_len;
            }
            catch (taf::JceNotEnoughBuff & e)
            {
            	 return -100;
            }
            catch (taf::JceDecodeInvalidValue & e)
            {
            	 return -104;
            }
            catch (...)
            {
            	 return -1;
            }
            return 0;
        }

        int Decode(uint8_t * pui_buff, int32_t * pi_buff_len, pdu_protocol_header * pdu_header)
        {
            try
            {
                int i_ret = 0;
                int i_wup_len = pdu_header?*pi_buff_len:0;
                int i_end_len = 0;
                if (pdu_header && (i_ret = pdu_header_unpack(pui_buff, (uint32_t *)&i_wup_len, pdu_header, (uint32_t *)&i_end_len)) != 0) return i_ret;

                taf::JceInputStream<taf::MapBufferReader> is;
                is.setBuffer(reinterpret_cast<const char*>(pui_buff + i_wup_len), static_cast<size_t>(*pi_buff_len - i_wup_len - i_end_len));

                this->readFrom(is);
            }
            catch (taf::JceDecodeMismatch & e)
            {
            	return -101;
            }
            catch (taf::JceDecodeInvalidValue & e)
            {
            	return -104;
            }
            catch (taf::JceDecodeRequireNotExist & e)
            {
            	return -103;
            }
            catch (...)
            {
            	return -1;
            }
            return 0;
        }
    public:
        map<taf::Int32, taf::Int32> imap;
    };
    inline bool operator==(const tmap&l, const tmap&r)
    {
        return l.imap == r.imap;
    }
    inline bool operator!=(const tmap&l, const tmap&r)
    {
        return !(l == r);
    }
    int struct_tmap_pack(tmap * pst_struct, uint8_t * pui_buff, int32_t * pi_buff_len, pdu_protocol_header * pdu_header);
    int struct_tmap_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, tmap * pst_struct, pdu_protocol_header * pdu_header);
    int struct_tmap_unpack(uint8_t * pui_buff, int32_t * pi_buff_len, tmap * pst_struct, pdu_protocol_header * pdu_header, uint8_t * pui_mapbuff, int32_t * pi_mapbuff_len);


}



#endif
