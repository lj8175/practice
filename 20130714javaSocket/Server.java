import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;


public class Server {
	
	public static void main(String[] args)
	{
        ServerSocket server = null;
        try{
			server = new ServerSocket(8175);
        } catch (IOException e) {
            e.printStackTrace();
            return ;
        }


		Socket socket = null;
        DataInputStream dis = null;
        DataOutputStream dos = null;
		while(true){
		    try{
				socket = server.accept();
				dis = new DataInputStream(socket.getInputStream());
				dos = new DataOutputStream(socket.getOutputStream());
				String msg = dis.readUTF();
				System.out.println(msg.length());
				dos.writeUTF(msg);
			} catch(IOException e) {
			    e.printStackTrace();
		    } finally {
                if (socket != null) {
                    try {
                        String s;
                        //socket.close(); //java can delete it
                        //dis.close();
                        //dos.close();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }

	}

}
