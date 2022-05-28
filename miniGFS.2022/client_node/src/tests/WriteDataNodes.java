// Programa que prueba la funcionalidad de la escritura en una lista
// de nodo de datos escribiendo en el primero que lo propagará al resto

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class WriteDataNodes {
    static public void main (String args[]) {
        if (args.length!=4) {
            System.err.println("Usage: WriteDataNodes RegistryHostname RegistryHostname File Data");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Manager m = (Manager) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_manager");
            List <DataNode> ldn = m.getDataNodes();
            DataNode dn = ldn.get(0);
            ldn.remove(0);
	    byte [] datos = args[3].getBytes();
            dn.writeChunk(ldn, args[2], datos);
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en WriteDataNodes:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
