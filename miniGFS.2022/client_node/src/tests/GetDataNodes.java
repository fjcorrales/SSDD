// Programa que prueba la funcionalidad del manager solicitando
// la lista de nodos de datos

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class GetDataNodes {
    static public void main (String args[]) {
        if (args.length!=2) {
            System.err.println("Uso: GetDataNodes RegistryHostname RegistryHostname");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Manager m = (Manager) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_manager");
            List <DataNode> l = m.getDataNodes();
            System.out.println("Lista de nodos de datos");
            for (DataNode dn: l)
                System.out.println(dn.getName());
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en GetDataNodes:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
