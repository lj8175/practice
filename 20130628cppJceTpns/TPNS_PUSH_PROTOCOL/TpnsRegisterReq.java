// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 2.1.5.7 by WSRD Tencent.
// Generated from `/usr/local/resin_system.mqq.com/webapps/communication/taf/upload/joelliu/tpns.jce'
// **********************************************************************

package TPNS_PUSH_PROTOCOL;

public final class TpnsRegisterReq extends com.qq.taf.jce.JceStruct implements java.lang.Cloneable
{
    public String className()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsRegisterReq";
    }

    public String fullClassName()
    {
        return "TPNS_PUSH_PROTOCOL.TpnsRegisterReq";
    }

    public long accessId = 0;

    public String accessKey = "";

    public String deviceId = "";

    public String appCert = "";

    public String account = "";

    public String ticket = "";

    public short deviceType = 0;

    public TPNS_PUSH_PROTOCOL.DeviceInfo deviceInfo = null;

    public long getAccessId()
    {
        return accessId;
    }

    public void  setAccessId(long accessId)
    {
        this.accessId = accessId;
    }

    public String getAccessKey()
    {
        return accessKey;
    }

    public void  setAccessKey(String accessKey)
    {
        this.accessKey = accessKey;
    }

    public String getDeviceId()
    {
        return deviceId;
    }

    public void  setDeviceId(String deviceId)
    {
        this.deviceId = deviceId;
    }

    public String getAppCert()
    {
        return appCert;
    }

    public void  setAppCert(String appCert)
    {
        this.appCert = appCert;
    }

    public String getAccount()
    {
        return account;
    }

    public void  setAccount(String account)
    {
        this.account = account;
    }

    public String getTicket()
    {
        return ticket;
    }

    public void  setTicket(String ticket)
    {
        this.ticket = ticket;
    }

    public short getDeviceType()
    {
        return deviceType;
    }

    public void  setDeviceType(short deviceType)
    {
        this.deviceType = deviceType;
    }

    public TPNS_PUSH_PROTOCOL.DeviceInfo getDeviceInfo()
    {
        return deviceInfo;
    }

    public void  setDeviceInfo(TPNS_PUSH_PROTOCOL.DeviceInfo deviceInfo)
    {
        this.deviceInfo = deviceInfo;
    }

    public TpnsRegisterReq()
    {
    }

    public TpnsRegisterReq(long accessId, String accessKey, String deviceId, String appCert, String account, String ticket, short deviceType, TPNS_PUSH_PROTOCOL.DeviceInfo deviceInfo)
    {
        this.accessId = accessId;
        this.accessKey = accessKey;
        this.deviceId = deviceId;
        this.appCert = appCert;
        this.account = account;
        this.ticket = ticket;
        this.deviceType = deviceType;
        this.deviceInfo = deviceInfo;
    }

    public boolean equals(Object o)
    {
        if(o == null)
        {
            return false;
        }

        TpnsRegisterReq t = (TpnsRegisterReq) o;
        return (
            com.qq.taf.jce.JceUtil.equals(accessId, t.accessId) && 
            com.qq.taf.jce.JceUtil.equals(accessKey, t.accessKey) && 
            com.qq.taf.jce.JceUtil.equals(deviceId, t.deviceId) && 
            com.qq.taf.jce.JceUtil.equals(appCert, t.appCert) && 
            com.qq.taf.jce.JceUtil.equals(account, t.account) && 
            com.qq.taf.jce.JceUtil.equals(ticket, t.ticket) && 
            com.qq.taf.jce.JceUtil.equals(deviceType, t.deviceType) && 
            com.qq.taf.jce.JceUtil.equals(deviceInfo, t.deviceInfo) );
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
        _os.write(accessId, 0);
        _os.write(accessKey, 1);
        _os.write(deviceId, 2);
        _os.write(appCert, 3);
        if (null != account)
        {
            _os.write(account, 4);
        }
        if (null != ticket)
        {
            _os.write(ticket, 5);
        }
        _os.write(deviceType, 6);
        if (null != deviceInfo)
        {
            _os.write(deviceInfo, 7);
        }
    }

    static TPNS_PUSH_PROTOCOL.DeviceInfo cache_deviceInfo;

    public void readFrom(com.qq.taf.jce.JceInputStream _is)
    {
        this.accessId = (long) _is.read(accessId, 0, true);
        this.accessKey =  _is.readString(1, true);
        this.deviceId =  _is.readString(2, true);
        this.appCert =  _is.readString(3, true);
        this.account =  _is.readString(4, false);
        this.ticket =  _is.readString(5, false);
        this.deviceType = (short) _is.read(deviceType, 6, false);
        if(null == cache_deviceInfo)
        {
            cache_deviceInfo = new TPNS_PUSH_PROTOCOL.DeviceInfo();
        }
        this.deviceInfo = (TPNS_PUSH_PROTOCOL.DeviceInfo) _is.read(cache_deviceInfo, 7, false);
    }

    public void display(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.display(accessId, "accessId");
        _ds.display(accessKey, "accessKey");
        _ds.display(deviceId, "deviceId");
        _ds.display(appCert, "appCert");
        _ds.display(account, "account");
        _ds.display(ticket, "ticket");
        _ds.display(deviceType, "deviceType");
        _ds.display(deviceInfo, "deviceInfo");
    }

    public void displaySimple(java.lang.StringBuilder _os, int _level)
    {
        com.qq.taf.jce.JceDisplayer _ds = new com.qq.taf.jce.JceDisplayer(_os, _level);
        _ds.displaySimple(accessId, true);
        _ds.displaySimple(accessKey, true);
        _ds.displaySimple(deviceId, true);
        _ds.displaySimple(appCert, true);
        _ds.displaySimple(account, true);
        _ds.displaySimple(ticket, true);
        _ds.displaySimple(deviceType, true);
        _ds.displaySimple(deviceInfo, false);
    }

}

