// Interfaz remota Master
// NO MODIFICAR

package interfaces;
import java.rmi.*;

public interface Master extends Remote {
    // obtiene acceso a la metainformación de un fichero
    public File lookup(String fname) throws RemoteException;
}
