import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.net.Socket;

/**
 * Created by clement on 4/12/15.
 * Cette classe va répondre aux demande d'un client
 */
public class ServerRunner extends Thread {
    private Socket socket;
    private JSONParser parser;
    private Server parent;

    public ServerRunner(Server parent, Socket skt) {
        this.parent = parent;
        parser = new JSONParser();
        socket = skt;
    }

    private String convertData(char[] data){
        StringBuilder s = new StringBuilder().append(data);
        if(s.charAt(0)!='{')
            s.insert(0, '{');
        return s.substring(0,s.lastIndexOf("}")+1);
    }

    private char[] receiveData() throws InterruptedException, IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        int timeout = 100;
        while(!in.ready()) {
            Thread.sleep(100,100);
            if(--timeout==0)
                return null;
        }
        char[] data = new char[65535];
        in.read(data);
        return data;
    }

    @Override
    public void run() {
        try {
            char [] data = receiveData();
            if(data==null)
                return;
            String req = convertData(data);
            ServerCommand cmd = dispatchRequest(req);
            sendRequest(cmd.execute());
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

    private void sendRequest(String req) throws IOException {
        if(req.contentEquals(""))
            return;
        BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        out.write(req);
        out.flush();
    }

    public ServerCommand dispatchRequest(String reqs) throws ParseException {
        System.out.println(reqs);
        JSONObject jsobj = (JSONObject)parser.parse(reqs);
        if(!jsobj.containsKey("status"))
            return new NullCommand();

        ServerCommand cmd = new NullCommand();
        String scmd = jsobj.get("status").toString();
        switch(scmd){
            case "clientSendInfo":
                String addr = jsobj.get("address")+":"+jsobj.get("port");
                parent.ajouterUtilisateur(addr, (JSONArray)jsobj.get("fichiers"));
                break;
            case "listeFichier":
                cmd = new ListFile(parent.listeFichiers());
                break;

            case "envoieListeFichiers":
                break;
            case "downloadFile":
                cmd = new UtilisateurListe(
                        parent.utilisateurFichiers((String)jsobj.get("fichier")),
                        parent.getFichierSplit((String)jsobj.get("fichier")),
                        (String)jsobj.get("fichier"));
                break;
        }
        return cmd;
    }
}
