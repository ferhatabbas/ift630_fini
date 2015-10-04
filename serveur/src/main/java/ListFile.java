import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import java.util.Set;

/**
 * Created by clement on 4/2/15.
 */
public class ListFile implements ServerCommand {
    private Set<String> arr_;
    public ListFile(Set<String> arr) {
        arr_ = arr;
    }

    public String execute(){
        JSONArray arr = new JSONArray();
        JSONObject jsobj = new JSONObject();
        for(String s: arr_){
            arr.add(s);
        }
        jsobj.put("status", "listeFichier");
        jsobj.put("fichiers", arr);
        String rep = jsobj.toJSONString();
        return rep;
    }
}
