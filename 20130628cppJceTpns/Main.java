import TPNS_PUSH_PROTOCOL.*;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.File;
import com.qq.taf.jce.*;
public class Main
{
public static void main(String[]agrs){
    try{
        DataInputStream dis = new DataInputStream(new FileInputStream(new File("tpns.txt")));
        System.out.println("version:" + dis.readByte());
        System.out.println("cmd:" + dis.readByte());
        System.out.println("length:" + dis.readInt());
        System.out.println("seq:" + dis.readInt());
        System.out.println("rspCode:" + dis.readByte());

        byte[] data = new byte[1024];
        dis.read(data);
        JceInputStream jis = new JceInputStream(data);
        TpnsPushReq trh = new TpnsPushReq();
        trh.readFrom(jis);
        System.out.println(trh);
        
    } catch(Exception e) {
        e.printStackTrace();
    }
}

}
