import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.io.File;

/**
 * Created by clement on 4/8/15.
 */
public class Utilities {

    final static int SSIZE = 1000;

    final static int MAX_DL = 5;

    /**
     * Convertie les données reçu du serveur pour les donner au parser json
     * @param data
     * @return
     */
    public static String convertData(char[] data) {
        String s = new StringBuilder().append(data).toString();

        if (s.charAt(0) != '{')
            s = "{" + s;
        return s.substring(0, s.lastIndexOf("}") + 1);
    }

    /**
     * Méthode qui permet de remplir les requêtes du serveur graçe à une ellipse
     * @param strbuf
     * @param args
     * @return
     */
    public static String changeArgs(StringBuffer strbuf, String ... args){
        StringBuffer res = new StringBuffer(strbuf);
        for(String str: args){
            int offset = res.indexOf("??");
            if(offset == -1)
                break;
            res.replace(offset, offset+2, str);
        }
        return res.toString();
    }

    /**
     * Convertion des valeurs de split en entier
     * @param split
     * @param value
     * @return
     */
    public static int retrieveLongSplit(JSONObject split, String value){
        long lv = (Long) split.get(value);
        return (int)lv;
    }

    /**
     * Génère un split pour un fichier
     * @param fname
     * @return
     */
    public static JSONObject getFileSplit(String fname){
        File f = new File("fichiers"+File.separator+fname);
        int len = (int) (f.length() / SSIZE);
        JSONObject obj = new JSONObject();
        obj.put("max", f.length());
        obj.put("num", len);
        return obj;
    }
}
