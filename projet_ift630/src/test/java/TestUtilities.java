import junit.framework.TestCase;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

/**
 * Created by clement on 4/17/15.
 */
public class TestUtilities extends TestCase {
    public void testSplit(){
        String[] fnames = new String[]{"f1.txt", "plotte.txt", "test2mb.txt", "test4mb.txt", "toto.txt"};

        for(String n: fnames) {
            JSONObject val = Utilities.getFileSplit(n);
            System.out.println(val.toJSONString());
        }
    }
}
