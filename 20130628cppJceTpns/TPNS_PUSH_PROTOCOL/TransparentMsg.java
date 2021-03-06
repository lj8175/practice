// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 2.1.5.7 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tpns.jce'
// **********************************************************************

package TPNS_PUSH_PROTOCOL;

public final class TransparentMsg extends com.qq.taf.jce.JceStruct implements java.lang.Cloneable
{
    public String className()
    {
        return "TPNS_PUSH_PROTOCOL.TransparentMsg";
    }

    public String fullClassName()
    {
        return "TPNS_PUSH_PROTOCOL.TransparentMsg";
    }

    public String msg = "";

    public String getMsg()
    {
        return msg;
    }

    public void  setMsg(String msg)
    {
        this.msg = msg;
    }

    public TransparentMsg()
    {
    }

    public TransparentMsg(String msg)
    {
        this.msg = msg;
    }

    public boolean equals(Object o)
    {
        if(o == null)
        {
            return false;
        }

        TransparentMsg t = (TransparentMsg) o;
        return (
            com.qq.taf.jce.JceUtil.equals(msg, t.msg) );
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
        _os.write(msg, 0);
    }


    public void readFrom(com.qq.taf.jce.JceInputStream _is)
    {
        this.msg =  _is.readString(0, true);
    }

    public void display(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.display(msg, "msg");
    }

    public void displaySimple(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.displaySimple(msg, false);
    }

}

