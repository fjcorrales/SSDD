// Programa que prueba la funcionalidad de la lectura en un nodo de datos
// leyendo datos del primer nodo de datos

package tests;
import java.util.List;
import java.rmi.*;
import interfaces.*;

public class ReadDataNode {
    static public void main (String args[]) {
        if (args.length!=3) {
            System.err.println("Uso: ReadDataNode RegistryHostname RegistryHostname File");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            Manager m = (Manager) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_manager");
            DataNode dn = m.getDataNodes().get(0);
	    byte [] datos;
            if ((datos = dn.readChunk(args[2])) == null)
		System.err.println("Fichero no existe");
            else
		System.out.println("Leído: " + new String(datos));
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en ReadDataNode:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
