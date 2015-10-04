import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.net.Socket;
import java.nio.file.Files;

/**
 * Created by clement on 4/15/15.
 */
public class CServerRunner implements Runnable{

    final private static String path = "fichiers";
    private Socket srvCon;
    private BufferedReader reader;
    private BufferedWriter writer;
    private OutputStream datawriter;
    private JSONParser parser;

    public CServerRunner(Socket skt) throws IOException {
        srvCon = skt;
        parser = new JSONParser();
        reader = new BufferedReader(new InputStreamReader(srvCon.getInputStream()));
        writer = new BufferedWriter(new OutputStreamWriter(srvCon.getOutputStream()));
        datawriter = srvCon.getOutputStream();
    }

    private byte[] readFile(String filename, JSONObject split) throws IOException {
        int min = Utilities.retrieveLongSplit(split, "begin");
        int max = Utilities.retrieveLongSplit(split, "end");
        File f = new File(path+File.separator+filename);
        RandomAccessFile facc = new RandomAccessFile(f, "r");
        byte[] data = new byte[max-min];
        facc.seek(min);
        facc.read(data);
        return data;
    }

    private JSONObject receiveData() throws IOException, InterruptedException, ParseException {
        int timeout = 100;
        while(!reader.ready()) {
            Thread.sleep(100,100);
            if(--timeout==0)
                return null;
        }
        char[] data = new char[65535];
        reader.read(data);
        return (JSONObject) parser.parse(Utilities.convertData(data));
    }

    @Override
    public void run() {
        try {
            System.out.println("Running method.");
            JSONObject jsobj = receiveData();
            System.out.println("Data received: "+jsobj.toJSONString());
            byte[] data = readFile((String)jsobj.get("fichier"), (JSONObject)jsobj.get("split"));
            datawriter.write(data);
            datawriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
}
