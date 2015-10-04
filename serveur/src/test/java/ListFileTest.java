import junit.framework.TestCase;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.ParseException;

import java.util.ArrayList;
import java.util.HashSet;

/**
 * Created by clement on 4/2/15.
 */
public class ListFileTest extends TestCase {
    public void testExecute() throws ParseException{
        HashSet<String> el = new HashSet<>();
        ArrayList<String> selements = new ArrayList<>();
        JSONArray arr = new JSONArray();
        selements.add("test");
        selements.add("toto");
        selements.add("dish");
        for(String s: selements) {
            el.add(s);
        }
        for(String s:el){
            arr.add(s);
        }

        JSONObject jsobj = new JSONObject();
        jsobj.put("status", "listeFichier");
        jsobj.put("fichiers", arr);

        ListFile lf = new ListFile(el);
        String res = lf.execute();
        assertTrue("JSON Must be the same", jsobj.toJSONString().contentEquals(res));
    }
}
