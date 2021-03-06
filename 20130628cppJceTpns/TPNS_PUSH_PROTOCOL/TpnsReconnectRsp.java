// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 2.1.5.7 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tpns.jce'
// **********************************************************************

package TPNS_PUSH_PROTOCOL;

public final class TpnsReconnectRsp extends com.qq.taf.jce.JceStruct implements java.lang.Cloneable
{
    public String className()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsReconnectRsp";
    }

    public String fullClassName()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsReconnectRsp";
    }

    public short sdkVersion = 0;

    public short getSdkVersion()
    {
        return sdkVersion;
    }

    public void  setSdkVersion(short sdkVersion)
    {
        this.sdkVersion = sdkVersion;
    }

    public TpnsReconnectRsp()
    {
    }

    public TpnsReconnectRsp(short sdkVersion)
    {
        this.sdkVersion = sdkVersion;
    }

    public boolean equals(Object o)
    {
        if(o == null)
        {
            return false;
        }

        TpnsReconnectRsp t = (TpnsReconnectRsp) o;
        return (
            com.qq.taf.jce.JceUtil.equals(sdkVersion, t.sdkVersion) );
    }

    public int hashCode()
    {
        try
        {
            throw new Exception("Need define key first!");
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
        }
        return 0;
    }
    public java.lang.Object clone()
    {
        java.lang.Object o = null;
        try
        {
            o = super.clone();
        }
        catch(CloneNotSupportedException ex)
        {
            assert false; // impossible
        }
        return o;
    }

    public void writeTo(com.qq.taf.jce.JceOutputStream _os)
    {
        _os.write(sdkVersion, 0);
    }


    public void readFrom(com.qq.taf.jce.JceInputStream _is)
    {
        this.sdkVersion = (short) _is.read(sdkVersion, 0, true);
    }

    public void display(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.display(sdkVersion, "sdkVersion");
    }

    public void displaySimple(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.displaySimple(sdkVersion, false);
    }

}

