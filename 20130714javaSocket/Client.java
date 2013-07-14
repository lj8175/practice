import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;


public class Client {
	
	public static void main(String args[]){
		Socket socket = null;
		DataInputStream dis = null;
		DataOutputStream dos = null;
		try {
			socket = new Socket("192.168.1.104",8175);
			dis = new DataInputStream(socket.getInputStream());
			dos = new DataOutputStream(socket.getOutputStream());
			//String str = (new BufferedReader(new InputStreamReader(System.in))).readLine();
			///*
			byte b[] = new byte[65535];
			for (int i = 0; i<b.length; ++i)
			{
				b[i] = 'a';
			}
			String str = new String(b);
			//*/
			dos.writeUTF(str);
			str = dis.readUTF();
			System.out.println(str.length());
			
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (socket != null){
				try{
					dos.close();
					dis.close();
					socket.close();
				}catch(IOException e){
					e.printStackTrace();
				}
			}
		}
	}

}
