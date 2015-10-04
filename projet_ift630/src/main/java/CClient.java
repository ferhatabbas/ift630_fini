import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.*;
/**
 * Created by clement on 4/8/15.
 * Classe qui va faire le téléchargement entre les clients
 * elle s'occupe seulement d'un seul fichier
 */
class CClient extends Thread {
    private JSONParser parser;
    private String nom;
    private JSONArray fsplit;
    private ArrayList<String> users;
    private Queue<JSONObject> queue;
    private int done = 0;

    private ArrayList<CClientRunner> runners;

    public CClient(JSONObject json) throws Exception {
        parser = new JSONParser();
        nom = (String) json.get("nom");
        users = new ArrayList<>();
        JSONArray arr = (JSONArray) json.get("utilisateurs");
        for (int i = 0; i < arr.size(); ++i) {
            JSONObject jsobj = (JSONObject) arr.get(i);
            users.add(jsobj.get("address") + ":" + jsobj.get("port"));
        }
        fsplit = (JSONArray) json.get("split");
        queue = new LinkedList<>();
        for (int i = 0; i < fsplit.size(); ++i) {
            queue.add((JSONObject) fsplit.get(i));
        }
        runners = new ArrayList<>();
    }

    @Override
    public void run() {
        JSONObject jssplit;
        Iterator it = users.iterator();
        String cur_addr = "";
        while ((jssplit = getNext()) != null) {
            if (done == 1)
                break;
            if (!it.hasNext()) {
                it = users.iterator();
                cur_addr = it.next().toString();
            } else {
                cur_addr = it.next().toString();
            }

            String addr = cur_addr.substring(0, cur_addr.indexOf(":"));
            String port = cur_addr.substring(cur_addr.indexOf(":") + 1);
            try {
                CClientRunner cc = new CClientRunner(addr, port, nom, jssplit, this);
                runners.add(cc);
                cc.start();
                if (runners.size() >= Utilities.MAX_DL) {
                    // wait a least one subprocess done;
                    while (true) {
                        for (int i = 0; i < runners.size(); ++i) {
                            if (!runners.get(i).isAlive())
                                runners.remove(i);
                        }
                        if (runners.size() != Utilities.MAX_DL)
                            break;
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        for (CClientRunner ccr : runners) {
            try {
                ccr.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void stopThread() {
        //kill al child stop dispatch
        done = 1;
    }

    public synchronized JSONObject getNext() {
        if (queue.size() == 0)
            return null;
        return queue.remove();
    }

    public synchronized void missingSplit(JSONObject obj) {
        queue.add(obj);
    }

    public synchronized void appendData(byte[] data, int begin, int end) {
        try {
            RandomAccessFile raf = new RandomAccessFile("fichiers" + File.separator + nom, "rw");
            raf.seek(begin);
            raf.seek(begin);
            raf.write(data);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
