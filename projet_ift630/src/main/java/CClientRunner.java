import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.net.Socket;

/**
 * Created by clement on 4/18/15.
 * Petit daemon qui va télécharger un split chez un client
 */
public class CClientRunner extends Thread {

    private Socket srvCon;
    private BufferedReader reader;
    private InputStream datareader;
    private BufferedWriter writer;
    private JSONParser parser;
    private CClient parent;
    private JSONObject split;
    private String fname;

    public CClientRunner(String cur_addr, String port, String fname, JSONObject jssplit, CClient parent) throws IOException {
        this.parent = parent;
        split = jssplit;
        this.fname = fname;
        parser = new JSONParser();
        srvCon = new Socket(cur_addr, Integer.parseInt(port));
        reader = new BufferedReader(new InputStreamReader(srvCon.getInputStream()));
        datareader = srvCon.getInputStream();
        writer = new BufferedWriter(new OutputStreamWriter(srvCon.getOutputStream()));
    }

    private void sendSplit() throws IOException {
        JSONObject jsobj = new JSONObject();
        jsobj.put("fichier", fname);
        jsobj.put("split", split);
        String req = jsobj.toJSONString();
        writer.write(req);
        writer.flush();
    }

    @Override
    public void run() {
        try {
            sendSplit();
            int timeout = 100;
            while (!reader.ready()) {
                try {
                    Thread.sleep(100, 100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                if (--timeout == 0) {
                    System.out.println("thread: "+this+" timeout.");
                    srvCon.close();
                    return;
                }
            }
            int begin = (int)split.get("begin");
            int end = (int)split.get("end");
            byte[] data = new byte[end-begin];
            datareader.read(data);
            parent.appendData(data, begin, end);
        } catch (Exception e) {
            e.printStackTrace();
            parent.missingSplit(split);
        }
    }
}
