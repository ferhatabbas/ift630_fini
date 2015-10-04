import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.IOException;
import java.lang.reflect.Array;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.*;

/**
 * Created by clement on 4/2/15.
 * Classe principale du serveur elle garde en mémoire toutes les informations sur les clients
 * ainsi que les informations sur les fichiers que les clients ont.
 */
public class Server{

    public static void main(String args[]){
        try {
            new Server().run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private ServerSocket srv;
    HashMap<String, Set<String>> clientList;
    HashMap<String, JSONObject> fichiersSplit;
    HashMap<String, JSONArray> fileList;

    /**
     * Constructeur il demarre le socket et initialise les listes synchronisées
     * @throws IOException
     */
    public Server() throws IOException {
        srv = new ServerSocket(11111);
        clientList = new HashMap<>();
        fileList = new HashMap<>();
        fichiersSplit = new HashMap<>();
    }

    /**
     * Permet d'ajouter ou de modifier les fichiers que possède un utilisateur
     * @param ipport
     * @param arr
     */
    public synchronized void ajouterUtilisateur(String ipport, JSONArray arr){
        Set<String> res = new HashSet<>();
        for(int i=0;i<arr.size();++i) {
            JSONObject obj = (JSONObject)arr.get(i);
            setFichierSplit(obj);
            res.add((String)obj.get("nom"));
        }
        clientList.put(ipport, res);
    }

    /**
     * Méthode pour avoir la liste des utilisateurs qui possède un fichier
     * @param filename
     * @return La liste des utilisateurs qui possède le fichier
     */
    public synchronized ArrayList<String> utilisateurFichiers(String filename){
        ArrayList<String> res = new ArrayList<>();
        for(String ip:clientList.keySet()){
            if(clientList.get(ip).contains(filename))
                res.add(ip);
        }
        return res;
    }

    public synchronized void setFichierSplit(JSONObject jsobj){
        fichiersSplit.put((String)jsobj.get("nom"), (JSONObject)jsobj.get("split"));
    }

    /**
     * Méthode pour avoir le split d'un fichier disponible sur le réseaux
     * @return
     */
    public synchronized JSONObject getFichierSplit(String fname){
        return fichiersSplit.get(fname);
    }

    public synchronized Set<String> listeFichiers(){
        Set<String> res = new HashSet<>();
        for(Set<String> fichiers: clientList.values()){
            res.addAll(fichiers);
        }
        return res;
    }

    /**
     * Boucle principale de l'attente serveur, cette méthode s'occupe de
     * demarrer un thread par requête de client
     * elle permet aussi d'handle le control+C côté serveur et de join sur
     * tout les threads pour qu'il termine.
     * @throws IOException
     */
    public void run() throws IOException {
        final ArrayList<Thread> threads = new ArrayList<>();
        Runtime.getRuntime().addShutdownHook(new Thread(){
            public void run(){
                for(Thread t:threads){
                    try {
                        t.join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        while(true){
            Socket skt = srv.accept();
            ServerRunner t = new ServerRunner(this, skt);
            t.start();
            threads.add(t);
        }
    }

    public void close() throws IOException {
        srv.close();
    }
}
