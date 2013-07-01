// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 2.1.5.7 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tpns.jce'
// **********************************************************************

package TPNS_PUSH_PROTOCOL;

public final class TpnsReconnectReq extends com.qq.taf.jce.JceStruct implements java.lang.Cloneable
{
    public String className()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsReconnectReq";
    }

    public String fullClassName()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsReconnectReq";
    }

    public String token = "";

    public String deviceId = "";

    public short networkType = 0;

    public java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo> unregisterAppList = null;

    public String getToken()
    {
        return token;
    }

    public void  setToken(String token)
    {
        this.token = token;
    }

    public String getDeviceId()
    {
        return deviceId;
    }

    public void  setDeviceId(String deviceId)
    {
        this.deviceId = deviceId;
    }

    public short getNetworkType()
    {
        return networkType;
    }

    public void  setNetworkType(short networkType)
    {
        this.networkType = networkType;
    }

    public java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo> getUnregisterAppList()
    {
        return unregisterAppList;
    }

    public void  setUnregisterAppList(java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo> unregisterAppList)
    {
        this.unregisterAppList = unregisterAppList;
    }

    public TpnsReconnectReq()
    {
    }

    public TpnsReconnectReq(String token, String deviceId, short networkType, java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo> unregisterAppList)
    {
        this.token = token;
        this.deviceId = deviceId;
        this.networkType = networkType;
        this.unregisterAppList = unregisterAppList;
    }

    public boolean equals(Object o)
    {
        if(o == null)
        {
            return false;
        }

        TpnsReconnectReq t = (TpnsReconnectReq) o;
        return (
            com.qq.taf.jce.JceUtil.equals(token, t.token) && 
            com.qq.taf.jce.JceUtil.equals(deviceId, t.deviceId) && 
            com.qq.taf.jce.JceUtil.equals(networkType, t.networkType) && 
            com.qq.taf.jce.JceUtil.equals(unregisterAppList, t.unregisterAppList) );
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
        _os.write(token, 0);
        _os.write(deviceId, 1);
        _os.write(networkType, 2);
        if (null != unregisterAppList)
        {
            _os.write(unregisterAppList, 3);
        }
    }

    static java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo> cache_unregisterAppList;

    public void readFrom(com.qq.taf.jce.JceInputStream _is)
    {
        this.token =  _is.readString(0, true);
        this.deviceId =  _is.readString(1, true);
        this.networkType = (short) _is.read(networkType, 2, true);
        if(null == cache_unregisterAppList)
        {
            cache_unregisterAppList = new java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo>();
            TPNS_PUSH_PROTOCOL.AppInfo __var_1 = new TPNS_PUSH_PROTOCOL.AppInfo();
            ((java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo>)cache_unregisterAppList).add(__var_1);
        }
        this.unregisterAppList = (java.util.ArrayList<TPNS_PUSH_PROTOCOL.AppInfo>) _is.read(cache_unregisterAppList, 3, false);
    }

    public void display(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.display(token, "token");
        _ds.display(deviceId, "deviceId");
        _ds.display(networkType, "networkType");
        _ds.display(unregisterAppList, "unregisterAppList");
    }

    public void displaySimple(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.displaySimple(token, true);
        _ds.displaySimple(deviceId, true);
        _ds.displaySimple(networkType, true);
        _ds.displaySimple(unregisterAppList, false);
    }

}

