// Implementación de la interfaz remota Master

package master;
import java.rmi.*;
import java.rmi.server.*;
import manager.*;
import interfaces.*;

public class MasterImpl extends UnicastRemoteObject implements Master {
    public static final long  serialVersionUID=1234567891;

    public MasterImpl(ManagerImpl m, int replica) throws RemoteException {
    }
    // obtiene acceso a la metainformación de un fichero
    public synchronized File lookup(String fname) throws RemoteException {
        return null;
    }
}
