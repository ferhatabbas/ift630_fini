import java.io.IOException;
import java.net.*;

/**
 * Created by Jason on 2015-04-02.
 */

public class CServer extends Thread {
    private ServerSocket srv;
    private int port;
    private String addr;
    public CServer(int port) throws IOException {
        System.out.println("port:"+port);
        this.port = port;
        srv = new ServerSocket(port);
        addr = srv.getInetAddress().toString();
    }

    public int getCServeurPort(){
        return port;
    }

    public String getCAddr(){
        return addr;
    }

    @Override
    public void run() {
        while(true){
            try {
                Socket skt = srv.accept();
                System.out.println("Receive connexion.");
                new Thread(new CServerRunner(skt)).start();
                System.out.println("Thread started.");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
