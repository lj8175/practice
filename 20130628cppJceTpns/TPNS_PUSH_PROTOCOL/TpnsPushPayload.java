// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 2.1.5.7 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tpns.jce'
// **********************************************************************

package TPNS_PUSH_PROTOCOL;

public final class TpnsPushPayload extends com.qq.taf.jce.JceStruct implements java.lang.Cloneable
{
    public String className()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsPushPayload";
    }

    public String fullClassName()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsPushPayload";
    }

    public int type = 0;

    public TPNS_PUSH_PROTOCOL.NotifyMsg notifyMsg = null;

    public TPNS_PUSH_PROTOCOL.TransparentMsg transparentMsg = null;

    public int getType()
    {
        return type;
    }

    public void  setType(int type)
    {
        this.type = type;
    }

    public TPNS_PUSH_PROTOCOL.NotifyMsg getNotifyMsg()
    {
        return notifyMsg;
    }

    public void  setNotifyMsg(TPNS_PUSH_PROTOCOL.NotifyMsg notifyMsg)
    {
        this.notifyMsg = notifyMsg;
    }

    public TPNS_PUSH_PROTOCOL.TransparentMsg getTransparentMsg()
    {
        return transparentMsg;
    }

    public void  setTransparentMsg(TPNS_PUSH_PROTOCOL.TransparentMsg transparentMsg)
    {
        this.transparentMsg = transparentMsg;
    }

    public TpnsPushPayload()
    {
    }

    public TpnsPushPayload(int type, TPNS_PUSH_PROTOCOL.NotifyMsg notifyMsg, TPNS_PUSH_PROTOCOL.TransparentMsg transparentMsg)
    {
        this.type = type;
        this.notifyMsg = notifyMsg;
        this.transparentMsg = transparentMsg;
    }

    public boolean equals(Object o)
    {
        if(o == null)
        {
            return false;
        }

        TpnsPushPayload t = (TpnsPushPayload) o;
        return (
            com.qq.taf.jce.JceUtil.equals(type, t.type) && 
            com.qq.taf.jce.JceUtil.equals(notifyMsg, t.notifyMsg) && 
            com.qq.taf.jce.JceUtil.equals(transparentMsg, t.transparentMsg) );
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
        _os.write(type, 0);
        if (null != notifyMsg)
        {
            _os.write(notifyMsg, 1);
        }
        if (null != transparentMsg)
        {
            _os.write(transparentMsg, 2);
        }
    }

    static int cache_type;
    static TPNS_PUSH_PROTOCOL.NotifyMsg cache_notifyMsg;
    static TPNS_PUSH_PROTOCOL.TransparentMsg cache_transparentMsg;

    public void readFrom(com.qq.taf.jce.JceInputStream _is)
    {
        this.type = (int) _is.read(type, 0, true);
        if(null == cache_notifyMsg)
        {
            cache_notifyMsg = new TPNS_PUSH_PROTOCOL.NotifyMsg();
        }
        this.notifyMsg = (TPNS_PUSH_PROTOCOL.NotifyMsg) _is.read(cache_notifyMsg, 1, false);
        if(null == cache_transparentMsg)
        {
            cache_transparentMsg = new TPNS_PUSH_PROTOCOL.TransparentMsg();
        }
        this.transparentMsg = (TPNS_PUSH_PROTOCOL.TransparentMsg) _is.read(cache_transparentMsg, 2, false);
    }

    public void display(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.display(type, "type");
        _ds.display(notifyMsg, "notifyMsg");
        _ds.display(transparentMsg, "transparentMsg");
    }

    public void displaySimple(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.displaySimple(type, true);
        _ds.displaySimple(notifyMsg, true);
        _ds.displaySimple(transparentMsg, false);
    }

}

