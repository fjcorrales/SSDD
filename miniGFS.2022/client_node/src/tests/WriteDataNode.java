// Programa que prueba la funcionalidad de la escritura en un nodo de datos
// escribiendo datos en el primer nodo de dato

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class WriteDataNode {
    static public void main (String args[]) {
        if (args.length!=4) {
            System.err.println("Uso: WriteDataNode RegistryHostname RegistryHostname File Data");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Manager m = (Manager) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_manager");
            DataNode dn = m.getDataNodes().get(0);
	    byte [] datos = args[3].getBytes();
            dn.writeChunk(null, args[2], datos);
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en WriteDataNode:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
