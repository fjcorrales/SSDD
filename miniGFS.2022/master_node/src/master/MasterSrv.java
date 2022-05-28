// Clase que instancia y registra los servicios del maestro
// NO MODIFICAR

package master;
import java.rmi.*;
import java.rmi.server.*;
import manager.*;
import interfaces.*;

class MasterSrv  {

    static public void main (String args[])  {
        if (args.length!=2) {
            System.err.println("Usage: MasterSrv RegistryPortNumber ReplicationFactor");
            return;
        }
        if (System.getSecurityManager() == null)
            System.setSecurityManager(new SecurityManager());

        try {
            ManagerImpl manager = new ManagerImpl();
            Naming.rebind("rmi://localhost:" + args[0] + "/GFS_manager", manager);

            MasterImpl master = new MasterImpl(manager, Integer.parseInt(args[1]));
            Naming.rebind("rmi://localhost:" + args[0] + "/GFS_master", master);
        }
        catch (RemoteException e) {
            System.err.println("Error de comunicacion: " + e.toString());
            System.exit(1);
        }
        catch (Exception e) {
            System.err.println("Excepcion en Master:");
            e.printStackTrace();
            System.exit(1);
        }
    }
}
