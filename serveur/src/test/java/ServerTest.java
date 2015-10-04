import junit.framework.TestCase;
import org.json.simple.JSONArray;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;

/**
 * Created by clement on 4/12/15.
 */
public class ServerTest extends TestCase {
    JSONArray arr1;
    JSONArray arr2;
    JSONArray arrm;
    public void setUp(){
        arr1 = new JSONArray();
        arr1.add("f11");
        arr1.add("f12");
        arr1.add("f13");
        arr1.add("f14");
        arr1.add("f15");

        arr2 = new JSONArray();
        arr2.add("f21");
        arr2.add("f22");
        arr2.add("f23");
        arr2.add("f24");
        arr2.add("f25");

        arrm = new JSONArray();
        arrm.add("f11");
        arrm.add("f22");
        arrm.add("f13");
        arrm.add("f24");
        arrm.add("f15");
        arrm.add("mix_1970_guardians_of_the_galaxy");

    }
    public void testFichierListe() throws IOException {

        Server s = new Server();
        s.ajouterUtilisateur("ip1", arr1);
        s.ajouterUtilisateur("ip2", arr2);
        s.ajouterUtilisateur("ip3", arrm);

        Set<String> resatt = new HashSet<>();
        int i = 0;
        for(; i<arr1.size(); ++i){
            resatt.add((String)arr1.get(i));
            resatt.add((String)arr2.get(i));
        }
        resatt.add((String)arrm.get(i));
        assertTrue("File list must be the same.", resatt.containsAll(s.listeFichiers()));
        s.close();
    }

    public void testTrouveUtilisateur() throws IOException {
        Server s = new Server();
        s.ajouterUtilisateur("ip1", arr1);
        s.ajouterUtilisateur("ip2", arr2);
        s.ajouterUtilisateur("ip3", arrm);

        ArrayList<String> resatt1 = new ArrayList<>();
        resatt1.add("arr1");
        resatt1.add("arrm");
        assertTrue("User must have f11 file", resatt1.containsAll(s.utilisateurFichiers("f11")));
        assertTrue("User must have f15 file", resatt1.containsAll(s.utilisateurFichiers("f15")));

        ArrayList<String> resatt2 = new ArrayList<>();
        resatt2.add("arrm");
        assertTrue("User must have mix_1970_guardians_of_the_galaxy file",
                resatt1.containsAll(s.utilisateurFichiers("mix_1970_guardians_of_the_galaxy")));

        assertTrue("User list must be emtpy", s.utilisateurFichiers("Bob_dylan").isEmpty());

        s.close();
    }
}
