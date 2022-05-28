// Clase que instancia un nodo de datos
// NO MODIFICAR

package datanode;
import java.io.File;
import java.rmi.*;
import java.rmi.server.*;
import interfaces.*;

class DataNodeSrv  {
    static public void main (String args[])  {
        if (args.length!=3) {
            System.err.println("Usage: DataNodeSrv RegistryHostname RegistryPortNumber Name");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            String name=args[2];
            new File(name).mkdir();
            Manager m = (Manager) Naming.lookup("//" + args[0] + ":" + args[1] + "/GFS_manager");
            DataNode srv = new DataNodeImpl(m, name);
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en DataNode:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
