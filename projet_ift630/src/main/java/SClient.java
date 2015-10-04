import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.*;
import java.net.Socket;

/**
 * Created by Jason on 2015-04-02.
 */

public class SClient {
    private Socket srvCon;
    private BufferedWriter writer;
    private BufferedReader reader;

    private String addr;
    private int port;

    private String curAddr;
    private int curPort;

    private JSONParser parser;
    private JSONObject requestFile;
    private JSONObject sendInformation;

    public SClient(String addr, int port){
        this.addr = addr;
        this.port = port;

        parser = new JSONParser();
        InitJSONObject();
    }

    public void register(String addr, int p){
        curAddr = addr;
        curPort = p;
    }

    public void begin() throws IOException {
        srvCon = new Socket(addr, port);
        writer = new BufferedWriter(new OutputStreamWriter(srvCon.getOutputStream()));
        reader = new BufferedReader(new InputStreamReader(srvCon.getInputStream()));
    }

    public void close() throws IOException {
        srvCon.close();
    }

    public void requestFileFromServer(Writer out) throws IOException, InterruptedException {
        sendRequest(requestFile);
        JSONObject jsobj = receiveRequest();
        if(jsobj == null || !jsobj.containsKey("status")) {
            out.write("Impossible de retrouver les données du serveur.\n");
            return;
        }
        JSONArray arr = (JSONArray)jsobj.get("fichiers");
        out.write("-- Liste des fichiers disponibles --\n");
        for(int i = 0; i< arr.size(); ++i) {
            out.write(arr.get(i)+"\n");
        }
    }

    /**
     * envoie les informations du client au server, ces informations contiennent
     * le nom du fichier et le split associé
     * {
     *  status:clientSendInfo,
     *  fichiers:[
     *    {
     *        nom:f_nom,
     *        split:[{max:xxxxxx, num:yyyy}, ...]
     *    },
     *    ...
     *  ]
     * }
     * max : correspond à la taille totale du fichier
     * num : correspond aux nombres de split du fichier
     * @param fichiers
     * @throws IOException
     */
    public void sendClientInformation(File[] fichiers) throws IOException {
        if(fichiers == null) {
            System.out.println("Créer le répertoire fichier pour votre application.");
            return;
        }
        JSONObject req = new JSONObject(sendInformation);
        JSONArray arr = new JSONArray();
        for(File f: fichiers){
            JSONObject js = new JSONObject();
            js.put("nom", f.getName());
            js.put("split", Utilities.getFileSplit(f.getName()));
            arr.add(js);
        }
        req.put("fichiers", arr);
        req.put("address", curAddr);
        req.put("port", curPort);
        sendRequest(req);
    }

    public JSONObject downloadFile(Writer out, String fichier) throws IOException, InterruptedException {
        JSONObject jsobj = new JSONObject();
        jsobj.put("status", "downloadFile");
        jsobj.put("fichier", fichier);
        sendRequest(jsobj);
        jsobj = receiveRequest();
        out.write(jsobj.toJSONString() + "\n");
        return jsobj;
    }

    /**
     * Permet d'initialiser les objets JSON que l'on va utiliser
     * pour le transfert à travers le socket
     */
    private void InitJSONObject(){
        requestFile = new JSONObject();
        requestFile.put("status", "listeFichier");
        sendInformation = new JSONObject();
        sendInformation.put("status", "clientSendInfo");
    }

    private JSONObject receiveRequest() throws InterruptedException, IOException {
        JSONObject obj;
        while(true) {
            int timeout = 1000;
            while (!reader.ready()) {
                Thread.sleep(100, 100);
                if(--timeout == 0) {
                    srvCon.close();
                    return null;
                }
            }
            char[] data = new char[65535];
            reader.read(data);
            String rep = Utilities.convertData(data);
            try {
                obj = (JSONObject)parser.parse(rep);
                break;
            } catch (ParseException e) {
                e.printStackTrace();
            }
        }
        return obj;
    }

    /**
     * Méthodes d'aide pour envoyer des informations sur un socket
     * @param req
     */
    private void sendRequest(String req) throws IOException {
        if(srvCon.isClosed()) {
            srvCon = new Socket(addr, port);
        }
        writer.write(req);
        writer.flush();
    }

    private void sendRequest(JSONObject req) throws IOException {
        sendRequest(req.toJSONString());
    }
}