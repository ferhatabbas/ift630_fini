import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.*;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;


/**
 * @author ferhat
 */
public class ClientApp implements Runnable {
    private SClient sclient;
    private CServer csrv;

    public static void main(String args[]){
        Thread t = new Thread(new ClientApp());
        t.start();
        try {
            t.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }


    public ClientApp(){
        try {
            sclient = new SClient("127.0.0.1", 11111);
            Random dice = new Random();
            int pmin = 20000;
            int pmax = 30000;
            csrv = new CServer(dice.nextInt(pmax-pmin)+pmin);
            csrv.start();
            sclient.register("127.0.0.1", csrv.getCServeurPort());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    public int clientPort(){
        return csrv.getCServeurPort();
    }

    @Override
    public void run(){
        // Trouver les fichiers partagés
        Scanner input = new Scanner(System.in);
        Writer out = new OutputStreamWriter(System.out);

        while (true){
            try {
                out.write("cmd> ");
                out.flush();
                if(command(input.nextLine().toString(), out)==0)
                    break;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public int command(String str, Writer out) throws IOException {
        String[] args = str.split(" ");
        try {
            sclient.begin();
            switch (args[0]) {
                case "close":
                    return 0;
                case "list":
                    listFile(out);
                    break;
                case "dl":
                    downloadFile(out, args[1]);
                    break;
                case "update":
                    sendInformation();
                    break;
                default:
                case "help":
                    out.write("Commandes disponible: \n" +
                            "list -- Liste les fichiers disponibles.\n" +
                            "dl <filename> -- télécharge un fichier.\n" +
                            "update -- envoie la liste des fichiers au serveur.\n" +
                            "close -- ferme le client.\n");
                    break;
            }
            sclient.close();
            return 1;
        }catch(InterruptedException e) {
            e.printStackTrace();
            sclient.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return -1;
    }

    private void sendInformation() {
        File dir = new File("fichiers");
        try {
            sclient.sendClientInformation(dir.listFiles());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void listFile(Writer out) throws IOException, InterruptedException {
        sclient.requestFileFromServer(out);
    }

    private void downloadFile(Writer out, String fichier) throws Exception {
        File f = new File("fichiers"+File.separator+fichier);
        if(f.exists()) {
            out.write("Fichiers déjà présent.\n");
            return;
        }
        JSONObject jsobj = sclient.downloadFile(out, fichier);
        JSONObject split = (JSONObject)jsobj.get("split");
        if(split==null){
            out.write("Mauvais nom de fichier.\n");
            return;
        }

        // TODO: Création du fichier vide pour le téléchargement.
        // OK
        f = new File("fichiers"+File.separator+fichier);
        f.createNewFile();
        RandomAccessFile raf = new RandomAccessFile(f, "rw");
        long max = (long) split.get("max");
        raf.setLength(max);
        raf.close();

        // TODO: Demarre le CClient pour télécharger le fichier.
        JSONArray arr = new JSONArray();
        int len = (int)max/Utilities.SSIZE;
        int reste = (int)max%Utilities.SSIZE;
        int old=0;
        //Création du JSON array finito
        for(int i=0; i<len; ++i){
            JSONObject obj = new JSONObject();
            obj.put("begin", old);
            obj.put("end", (old+Utilities.SSIZE));
            old = (old+Utilities.SSIZE);
            arr.add(obj);
        }
        if(reste!=0){
            JSONObject obj = new JSONObject();
            obj.put("begin", old);
            obj.put("end", (old+(reste)));
            arr.add(obj);
        }
        jsobj.put("split", arr);
        CClient cc = new CClient(jsobj);
        cc.start();

        /**
         * Utilisé pour terminer les threads correctement
         */
//        final ArrayList<CClient> threads = new ArrayList<>();
//        Runtime.getRuntime().addShutdownHook(new Thread(){
//            public void run(){
//                for(CClient t:threads){
//                    try {
//
//                        t.join();
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                    }
//                }
//            }
//        });
    }
}
